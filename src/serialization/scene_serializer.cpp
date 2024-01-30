#include "scene_serializer.h"

bool scene_serializer::load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type) {
    tinygltf::TinyGLTF loader;
    std::string error_message;
    std::string warning_message;

    std::cout << "Loading scene from file " << filename << std::endl;

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
