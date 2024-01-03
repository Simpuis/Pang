#include "scene_serializer.h"
#include "transform.h"

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

