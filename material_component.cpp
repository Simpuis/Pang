#include "material_component.h"

#include "scene_serializer.h"

void material_component::serialize(scene_serializer &serializer, tinygltf::Model &model, tinygltf::Node *node) const {
    auto material_index = serializer.get_material_index(material.get());
}
