#pragma once

#include <tiny_gltf.h>
#include <entt/entt.hpp>
#include <utility>
#include <flecs.h>
#include <optional>

#include "src/render/material.h"
#include "src/render/mesh.h"
#include "src/modules/transformation/transformation.h"

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

    template<typename... Serializable_Ts>
    void load_scene_into_registry(flecs::world& world, const std::string& filename, gltf_file_type file_type);

    template<typename T, typename... Serializable_Ts>
    void serialize_type(flecs::entity& entity, tinygltf::Node& node);

private:
    static bool load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type);
    bool write_mode = false;

    std::map<unsigned int, std::shared_ptr<material>> material_lookup;
};

template<typename... Serializable_Ts>
void scene_deserializer::load_scene_into_registry(flecs::world &world, const std::string &filename,
                                                  scene_deserializer::gltf_file_type file_type) {
    write_mode = false;
    tinygltf::Model model;
    if(!load_scene_file(model, filename, file_type)) return;

    for(auto& node : model.nodes) {
        auto entity = world.entity(node.name.c_str());

        serialize_type<Serializable_Ts...>(entity, node);

        if(node.mesh >= 0) {
            mesh::deserialize(model, node, material_lookup, entity);
        }
    }
}

template<typename T, typename... Serializable_Ts>
void scene_deserializer::serialize_type(flecs::entity& entity, tinygltf::Node& node) {
    T component_serializable;

    serialize<scene_deserializer>(*this, node, component_serializable);

    entity.set<T>(component_serializable);

    if constexpr(sizeof...(Serializable_Ts) > 0) {
        serialize_type<Serializable_Ts...>(entity, node);
    }
}
