#include <iostream>

#include "scene_deserializer.h"
#include "src/render/material.h"
#include "src/render/mesh.h"

void scene_deserializer::load_scene_into_registry(flecs::world& world, const std::string& filename, gltf_file_type file_type) {
    tinygltf::Model model;
    if(!load_scene_file(model, filename, file_type)) return;

    for(const auto& node : model.nodes) {
        auto entity = world.entity(node.name.c_str());

        for(const auto& core_deserializer : core_deserializers) {
            if(core_deserializer.first(node))
                core_deserializer.second(node, entity);
        }

        for(const auto& extension_deserializer : extension_deserializers) {
            if(node.extensions.find(extension_deserializer.first) != node.extensions.end())
                extension_deserializer.second(node.extensions.at(extension_deserializer.first), entity);
        }

        if(node.mesh >= 0) {
            mesh::deserialize(model, node, material_lookup, entity);
        }
    }
}

bool scene_deserializer::load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type) {
    tinygltf::TinyGLTF loader;
    std::string error_message;
    std::string warning_message;

    bool result = false;
    if(file_type == gltf_file_type::ascii) {
        result = loader.LoadASCIIFromFile(&model, &error_message, &warning_message, filename);
    }
    else if(file_type == gltf_file_type::binary) {
        result = loader.LoadBinaryFromFile(&model, &error_message, &warning_message, filename);
    }

    if(!error_message.empty()) {
        std::cout << "TinyGLTF Error loading scene file " << std::endl << "Error Message: " << error_message;
    }

    if(!warning_message.empty()) {
        std::cout << "TinyGLTF Warning loading scene file " << std::endl << "Warning Message: " << warning_message;
    }

    return result;
}
