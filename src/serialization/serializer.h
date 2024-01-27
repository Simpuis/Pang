#pragma once

#include <tiny_gltf.h>
#include <flecs.h>
#include <memory>
#include <iostream>
#include <map>

#include "src/flecs_modules/transformation/transformation.h"
#include "src/render/mesh.h"

/**
 * @brief The scene_deserializer class is responsible for deserializing the scene from a gltf file.
 * Entity components that can be deserialized must be registered using register_core_type or register_extension_type
 * before calling load_scene_into_registry.
 */

class scene_deserializer {
public:
    /**
     * @brief The gltf_file_type enum is used to specify the type of file to load
     * ascii: A gltf file in json format, .gltf
     * binary: A gltf file in binary format, .glb
     */
    enum gltf_file_type { ascii, binary };

public:
    /**
     * @brief Construct a new scene_deserializer object
     *
     * @param file_type The type of file to load
     * @param filename The filename to load the scene from
     */
    scene_deserializer() = default;

public:
    /**
     * @brief Register a core component type for deserialization. The component must have a static deserialize function
     * that takes a deserialization_data object as its only parameter and constructs its component on the entity.
     * This function is called for every entity in the scene, and should be used for non-extension types.
     *
     * @tparam T The type of component to register
     * @return scene_deserializer& A reference to this object, to chain expressions
     */

    template<typename T, typename U>
    requires std::convertible_to<T, U> && std::convertible_to<U, T>
    void operator()(T& node_serializable, U& component_serializable) const {
        write_mode ? node_serializable = (T)component_serializable : component_serializable = (U)node_serializable;
    }

    template<typename... Serializable_Extension_Ts>
    void save_scene_from_world(flecs::world& world, const std::string& filename, gltf_file_type file_type) {
        write_mode = true;
        tinygltf::Model model;

        std::map<flecs::entity, tinygltf::Node&> entity_node_map;
        world.each<flecs::entity>([&](flecs::entity entity) {
            auto node = tinygltf::Node();
            node.name = entity.name().c_str();
            model.nodes.push_back(node);
        });

        tinygltf::TinyGLTF saver;
        saver.WriteGltfSceneToFile(&model, filename.c_str(), true, true, true, false);
    }

    template<typename... Serializable_Extension_Ts>
    void load_scene_into_registry(flecs::world& world, const std::string& filename, gltf_file_type file_type) {
        write_mode = false;
        tinygltf::Model model;
        if(!load_scene_file(model, filename, file_type)) return;

        for(auto& node : model.nodes) {
            auto entity = world.entity(node.name.c_str());

            if constexpr(sizeof...(Serializable_Extension_Ts) > 0) {
                serialize_extension<Serializable_Extension_Ts...>(entity, node);
            }

            if(!node.translation.empty()) {
                serialize_type<position>(entity, node);
            }

            if(!node.rotation.empty()) {
                serialize_type<rotation>(entity, node);
            }

            if(!node.scale.empty()) {
                serialize_type<scale>(entity, node);
            }

            if(node.mesh >= 0) {
                mesh::deserialize(model, node, material_lookup, entity);
            }
        }
    }

    template<typename T>
    void serialize_type(flecs::entity& entity, tinygltf::Node& node) {
        T component_serializable;

        serialize<scene_deserializer>(*this, node, component_serializable);

        entity.set<T>(component_serializable);
    }

    template<typename T, typename... Serializable_Ts>
    void serialize_extension(flecs::entity& entity, tinygltf::Node& node) {
        T component_serializable;

        serialize<scene_deserializer>(*this, node, component_serializable);

        entity.set<T>(component_serializable);

        if constexpr(sizeof...(Serializable_Ts) > 0) {
            serialize_type<Serializable_Ts...>(entity, node);
        }
    }

private:
    static bool load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type);
    bool write_mode = false;

    std::map<unsigned int, std::shared_ptr<material>> material_lookup;
};
