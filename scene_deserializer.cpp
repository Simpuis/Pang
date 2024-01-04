#include <iostream>
#include "scene_deserializer.h"

void scene_deserializer::load_scene_into_registry(entt::registry &registry) {
    tinygltf::Model model = load_scene_file(scene_path, input_type);

    for(auto node : model.nodes) {
        auto entity = registry.create();

        if(!node.name.empty()) {
            name_deserializer(node, registry, entity);
        }

        if(!node.matrix.empty()) {
            transform_deserializer(node, registry, entity);
        }
    }
}

tinygltf::Model
scene_deserializer::load_scene_file(const std::string &path, const scene_deserializer::gltf_file_type file_type) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string error_message;
    std::string warning_message;

    if(file_type == gltf_file_type::ascii) {
        loader.LoadASCIIFromFile(&model, &error_message, &warning_message, path);
    }
    else if(file_type == gltf_file_type::binary) {
        loader.LoadBinaryFromFile(&model, &error_message, &warning_message, path);
    }

    if(!error_message.empty()) {
        std::cout << "TinyGLTF Error loading scene file " << path << std::endl << "Error Message: " << error_message;
    }

    if(!warning_message.empty()) {
        std::cout << "TinyGLTF Warning loading scene file " << path << std::endl << "Warning Message: " << warning_message;
    }

    return model;
}
