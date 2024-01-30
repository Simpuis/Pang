#pragma once

#include <tiny_gltf.h>
#include <flecs.h>
#include <memory>
#include <iostream>
#include <map>

#include "src/flecs_modules/transformation/transformation.h"
#include "src/render/mesh.h"
#include "src/render/renderer.h"
#include "src/flecs_modules/rendering/rendering.h"

template<typename T>
concept serializer_type = requires
        (tinygltf::Model& model_save,
         std::map<const flecs::entity, tinygltf::Node*>& map_save,
         const flecs::world& world_save,
         const tinygltf::Model& model_load,
         std::map<const tinygltf::Node*, flecs::entity>& map_load,
         flecs::world& world_load)
{
    T::save(world_save, model_save, map_save);
    T::load(world_load, model_load, map_load);
};

class scene_serializer {
public:
    enum gltf_file_type { ascii, binary };

public:
    scene_serializer() = default;

public:
    template<typename T, typename U>
    requires std::convertible_to<T, U> && std::convertible_to<U, T>
    void operator()(T& node_serializable, U& component_serializable) const {
        write_mode ? node_serializable = (T)component_serializable : component_serializable = (U)node_serializable;
    }

    template<serializer_type... Serializer_Ts>
    void save_scene_to_file(flecs::world& world, const std::string& filename) {
        tinygltf::Model model;
        std::map<const flecs::entity, tinygltf::Node*> entity_node_map;
        // Position is here used to make it possible to iterate all "world objects", it should probably be replaced
        // by some sort of "serializable" component
        world.each<position>([&](flecs::entity entity, position& pos) {
            auto node = tinygltf::Node();
            node.name = entity.name().c_str();

            model.nodes.push_back(node);
            entity_node_map.insert({entity, &node});
        });

        save<Serializer_Ts...>(world, model, entity_node_map);

        tinygltf::TinyGLTF saver;
        saver.WriteGltfSceneToFile(&model, filename.c_str(), true, true, true, false);
    }

    template<serializer_type T, serializer_type... Serializer_Ts>
    void save(const flecs::world& world, tinygltf::Model& model, std::map<const flecs::entity, tinygltf::Node*>& entity_node_map) {
        T::save(world, model, entity_node_map);

        if constexpr(sizeof...(Serializer_Ts) > 0) {
            save<Serializer_Ts...>(world, model, entity_node_map);
        }
    }

    template<serializer_type... Serializer_Ts>
    void load_scene_from_file(flecs::world& world, const std::string& filename, gltf_file_type file_type) {
        tinygltf::Model model;
        if(!load_scene_file(model, filename, file_type)) return;
        std::map<const tinygltf::Node*, flecs::entity> node_entity_map;
        for(auto& node : model.nodes) {
            auto entity = world.entity(node.name.c_str());
            node_entity_map.insert({&node, entity});
        }

        load<Serializer_Ts...>(world, model, node_entity_map);
    }

    template<serializer_type T, serializer_type... Serializer_Ts>
    void load(flecs::world& world, tinygltf::Model& model, std::map<const tinygltf::Node*, flecs::entity>& node_entity_map) {
        T::load(world, model, node_entity_map);

        if constexpr(sizeof...(Serializer_Ts) > 0) {
            load<Serializer_Ts...>(world, model, node_entity_map);
        }
    }

private:
    static bool load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type);
    bool write_mode = false;
};
