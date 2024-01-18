#include "scene_serializer.h"
#include "src/render/material.h"

#include <tiny_gltf.h>
#include <memory>

void scene_serializer::operator()(entt::entity e) {
    if(model.nodes.size() <= (std::uint64_t)e) {
        model.nodes.emplace_back();
    }

    active_node = &model.nodes[(std::uint64_t)e];
}

void scene_serializer::operator()(std::underlying_type_t<entt::entity> size) {

}

scene_serializer::~scene_serializer() {
    tinygltf::TinyGLTF saver;
    saver.WriteGltfSceneToFile(&model, file, false, false, false, false);
}

std::uint32_t scene_serializer::get_material_index(const material *mat) {
    if(material_indices.find(mat) == material_indices.end()) {
        model.materials.push_back(mat->serialize());
        material_indices[mat] = model.materials.size() - 1;
    }

    return material_indices[mat];
}

