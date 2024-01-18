#pragma once

#include <tiny_gltf.h>
#include <entt/entt.hpp>
#include <utility>
#include <flecs.h>

#include "src/render/material.h"

struct deserialization_data {
    deserialization_data(const tinygltf::Model &model, const tinygltf::Node &node, flecs::world& world,
                         flecs::entity& entity, std::map<unsigned int, std::shared_ptr<material>> &materialLookup)
            : model(model), node(node), world(world), entity(entity), material_lookup(materialLookup) {}

    const tinygltf::Model& model;
    const tinygltf::Node& node;
    flecs::world& world;
    flecs::entity& entity;
    std::map<unsigned int, std::shared_ptr<material>>& material_lookup;
};

template<typename T>
concept has_deserialize = requires (T x) { x.deserialize(std::declval<deserialization_data&>()); };

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
    template<typename T>
        requires has_deserialize<T>
    scene_deserializer& register_core_type();

    template<typename T>
    scene_deserializer& register_core_deserializer(std::function<void(deserialization_data& data)> deserializer);
    /**
     * @brief Register an extension component type for deserialization. The component must have a static deserialize function
     * that takes a deserialization_data object as its only parameter and constructs its component on the entity.
     * This function is called for every entity in the scene, and should be used for extension types.
     *
     * @tparam T The type of component to register
     * @return scene_deserializer& A reference to this object, to chain expressions
     */
    template<typename T>
        requires has_deserialize<T>
    scene_deserializer& register_extension_type();

    /**
     * @brief Load the scene into the registry. Should be called after all component types have been registered.
     *
     * @param registry The registry to load the scene into
     */
    void load_scene_into_registry(flecs::world& world, const std::string& filename, gltf_file_type file_type);

private:
    static bool load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type);

    std::vector<std::function<void(deserialization_data& data)>> core_deserializers;
    std::map<std::string, std::function<void(deserialization_data& data)>> extension_deserializers;
};

template<typename T>
    requires has_deserialize<T>
scene_deserializer &scene_deserializer::register_core_type() {
    core_deserializers.push_back(&T::deserialize);

    return *this;
}

template<typename T>
scene_deserializer& scene_deserializer::register_core_deserializer(std::function<void(deserialization_data& data)> deserializer) {
    core_deserializers.push_back(deserializer);

    return *this;
}

template<typename T>
    requires has_deserialize<T>
scene_deserializer &scene_deserializer::register_extension_type() {
    extension_deserializers.insert(T::serialization_label(), &T::deserialize);

    return *this;
}
