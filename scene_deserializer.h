#pragma once

#include <tiny_gltf.h>
#include <entt/entt.hpp>
#include <utility>

class scene_deserializer {
public:
    enum gltf_file_type { ascii, binary };

public:
    explicit scene_deserializer(gltf_file_type file_type, std::string filename) : scene_path(std::move(filename)), input_type(file_type) {}

public:
    template<typename T>
    scene_deserializer& register_extension_type();
    template<typename T>
    scene_deserializer& register_transform_type();
    template<typename T>
    scene_deserializer& register_name_type();

    void load_scene_into_registry(entt::registry& registry);

private:
    static tinygltf::Model load_scene_file(const std::string& path, const gltf_file_type file_type);

    std::map<std::string, std::function<void(const tinygltf::Node&, entt::registry&, const entt::entity&)>> extension_deserializers;
    std::function<void(const tinygltf::Node&, entt::registry&, const entt::entity&)> transform_deserializer;
    std::function<void(const tinygltf::Node&, entt::registry&, const entt::entity&)> name_deserializer;
    std::string scene_path;
    gltf_file_type input_type;
};

template<typename T>
scene_deserializer &scene_deserializer::register_extension_type() {
    extension_deserializers.insert(T::serialization_label(), &T::deserialize);

    return *this;
}

template<typename T>
scene_deserializer &scene_deserializer::register_transform_type() {
    transform_deserializer = &T::deserialize;

    return *this;
}
template<typename T>
scene_deserializer &scene_deserializer::register_name_type() {
    name_deserializer = &T::deserialize;

    return *this;
}
