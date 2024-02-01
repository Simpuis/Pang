#pragma once

#include <tiny_gltf.h>
#include <flecs.h>
#include <memory>
#include <iostream>
#include <map>
#include <gsl/gsl>

#include "src/flecs_modules/transformation/transformation.h"
#include "src/render/mesh.h"
#include "src/render/renderer.h"
#include "src/flecs_modules/rendering/rendering.h"

struct serializer_save_data {
    serializer_save_data(tinygltf::Model& model,
                         std::map<const flecs::entity, gsl::index>& entity_node_map,
                         const flecs::world& world)
                         : model(model), entity_node_map(entity_node_map), world(world) {}

    tinygltf::Model& model;
    std::map<const flecs::entity, gsl::index>& entity_node_map;
    const flecs::world& world;
};

struct serializer_load_data {
    serializer_load_data(const tinygltf::Model& model,
                         std::map<gsl::index, flecs::entity>& node_entity_map,
                         flecs::world& world)
                         : model(model), node_entity_map(node_entity_map), world(world) {}

    const tinygltf::Model& model;
    std::map<gsl::index, flecs::entity>& node_entity_map;
    flecs::world& world;
};

template<typename T>
concept serializer_type = requires(serializer_save_data& save_data, serializer_load_data& load_data)
{
    T::save(save_data);
    T::load(load_data);
};

class scene_serializer {
public:
    enum gltf_file_type {
        ascii, binary
    };

public:
    scene_serializer() = default;

public:
    template<typename T, typename U>
    requires std::convertible_to<T, U> && std::convertible_to<U, T>
    void operator()(T &node_serializable, U &component_serializable) const {
        write_mode ? node_serializable = (T) component_serializable : component_serializable = (U) node_serializable;
    }

    template<serializer_type... Serializer_Ts>
    void save_scene_to_file(flecs::world &world, const std::string &filename) {
        tinygltf::Model model;
        std::map<const flecs::entity, gsl::index> entity_node_map;
        // Position is here used to make it possible to iterate all "world objects", it should probably be replaced
        // by some sort of "serializable" component
        world.each<position>([&](flecs::entity entity, position &pos) {
            model.nodes.emplace_back();
            gsl::index node_index = model.nodes.size() - 1;
            model.nodes[node_index].name = entity.name().c_str();

            entity_node_map.insert({entity, node_index});
        });

        serializer_save_data save_data(model, entity_node_map, world);
        save<Serializer_Ts...>(save_data);

        tinygltf::TinyGLTF saver;
        saver.WriteGltfSceneToFile(&model, filename.c_str(), true, true, true, false);
    }

    template<serializer_type T, serializer_type... Serializer_Ts>
    void save(serializer_save_data& save_data) {
        T::save(save_data);

        if constexpr (sizeof...(Serializer_Ts) > 0) {
            save<Serializer_Ts...>(save_data);
        }
    }

    template<serializer_type... Serializer_Ts>
    void load_scene_from_file(flecs::world &world, const std::string &filename, gltf_file_type file_type) {
        tinygltf::Model model;
        if (!load_scene_file(model, filename, file_type)) return;
        std::map<gsl::index, flecs::entity> node_entity_map;
        for (gsl::index i = 0; i < model.nodes.size(); i++) {
            auto entity = world.entity(model.nodes[i].name.c_str());
            node_entity_map.insert({i, entity});
        }

        serializer_load_data load_data(model, node_entity_map, world);
        load<Serializer_Ts...>(load_data);
    }

    template<serializer_type T, serializer_type... Serializer_Ts>
    void load(serializer_load_data& load_data) {
        T::load(load_data);

        if constexpr (sizeof...(Serializer_Ts) > 0) {
            load<Serializer_Ts...>(load_data);
        }
    }

private:
    static bool load_scene_file(tinygltf::Model &model, const std::string &filename, gltf_file_type file_type);

    bool write_mode = false;
};
