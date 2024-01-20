#pragma once

#include <tiny_gltf.h>
#include <entt/entt.hpp>
#include <utility>
#include <flecs.h>
#include <optional>

#include "src/render/material.h"

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
    void operator()(std::function<bool(const tinygltf::Node&)> predicate,
                    std::function<T(const tinygltf::Node&)> deserializer) {
        core_deserializers.emplace_back(std::move(predicate), [deserializer](const tinygltf::Node& node, flecs::entity& entity) {
            entity.set<T>(deserializer(node));
        });
    }

    template<typename T>
    void operator()(const std::string& extension_label,
                    std::function<T(const tinygltf::Value&)> deserializer) {
        extension_deserializers.insert({extension_label, [deserializer](const tinygltf::Value& value, flecs::entity& entity) {
            entity.set<T>(deserializer(value));
        }});
    }


    /**
     * @brief Load the scene into the registry. Should be called after all component types have been registered.
     *
     * @param registry The registry to load the scene into
     */
    void load_scene_into_registry(flecs::world& world, const std::string& filename, gltf_file_type file_type);

private:
    static bool load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type);

    std::vector<std::pair<std::function<bool(const tinygltf::Node&)>, std::function<void(const tinygltf::Node&, flecs::entity&)>>> core_deserializers;
    std::map<std::string, std::function<void(const tinygltf::Value&, flecs::entity&)>> extension_deserializers;

    std::map<unsigned int, std::shared_ptr<material>> material_lookup;
};


