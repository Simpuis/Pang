#include "name.h"

#include <tiny_gltf.h>
#include "src/serialization/scene_deserializer.h"

void name::serialize(scene_serializer& serializer, tinygltf::Model &model, tinygltf::Node *node) const {
    node->name = name_string;
}

void name::deserialize(deserialization_data& data) {
    if(data.node.name.empty()) return;

    auto& name_comp = data.registry.emplace<name>(data.entity, data.node.name);
}
