#include <iostream>
#include "scene_deserializer.h"
#include "src/render/material.h"

void scene_deserializer::load_scene_into_registry(flecs::world& world) {
    tinygltf::Model model;
    if(!load_scene_file(model, scene_path, input_type)) return;

    std::map<unsigned int, std::shared_ptr<material>> material_lookup;

    for(const auto& node : model.nodes) {
        auto entity = world.entity(node.name.c_str());
        deserialization_data data(model, node, world, entity, material_lookup);

        for(const auto& core_deserializer : core_deserializers) {
            core_deserializer(data);
        }
    }
}

bool
scene_deserializer::load_scene_file(tinygltf::Model& model, const std::string &path, const scene_deserializer::gltf_file_type file_type) {
    tinygltf::TinyGLTF loader;
    std::string error_message;
    std::string warning_message;

    bool result = false;
    if(file_type == gltf_file_type::ascii) {
        result = loader.LoadASCIIFromFile(&model, &error_message, &warning_message, path);
    }
    else if(file_type == gltf_file_type::binary) {
        result = loader.LoadBinaryFromFile(&model, &error_message, &warning_message, path);
    }

    if(!error_message.empty()) {
        std::cout << "TinyGLTF Error loading scene file " << path << std::endl << "Error Message: " << error_message;
    }

    if(!warning_message.empty()) {
        std::cout << "TinyGLTF Warning loading scene file " << path << std::endl << "Warning Message: " << warning_message;
    }

    return result;
}
