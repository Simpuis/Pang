#pragma once

#include <tiny_gltf.h>
#include <entt/entt.hpp>
#include <utility>

#include "material.h"

struct deserialization_data {
    deserialization_data(const tinygltf::Model &model, const tinygltf::Node &node, entt::registry &registry,
                         const entt::entity &entity, std::map<unsigned int, std::shared_ptr<material>> &materialLookup)
            : model(model), node(node), registry(registry), entity(entity), material_lookup(materialLookup) {}

    const tinygltf::Model& model;
    const tinygltf::Node& node;
    entt::registry& registry;
    const entt::entity& entity;
    std::map<unsigned int, std::shared_ptr<material>>& material_lookup;
};

class scene_deserializer {
public:
    enum gltf_file_type { ascii, binary };

public:
    explicit scene_deserializer(gltf_file_type file_type, std::string&& filename) : scene_path(std::move(filename)), input_type(file_type) {}

public:
    template<typename T>
    scene_deserializer& register_core_type();
    template<typename T>
    scene_deserializer& register_extension_type();

    void load_scene_into_registry(entt::registry& registry);

private:
    static bool load_scene_file(tinygltf::Model& model, const std::string& path, gltf_file_type file_type);

    std::vector<std::function<void(deserialization_data& data)>> core_deserializers;
    std::map<std::string, std::function<void(deserialization_data& data)>> extension_deserializers;
    std::string scene_path;
    gltf_file_type input_type;
};

template<typename T>
scene_deserializer &scene_deserializer::register_core_type() {
    core_deserializers.push_back(&T::deserialize);

    return *this;
}

template<typename T>
scene_deserializer &scene_deserializer::register_extension_type() {
    extension_deserializers.insert(T::serialization_label(), &T::deserialize);

    return *this;
}
