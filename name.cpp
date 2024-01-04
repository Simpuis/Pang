#include "name.h"

#include <tiny_gltf.h>

void name::serialize(tinygltf::Model &model, tinygltf::Node *node) const {
    node->name = name_string;
}

void name::deserialize(const tinygltf::Node &node, entt::registry &registry, const entt::entity &entity) {
    auto& name_comp = registry.emplace<name>(entity, node.name);
}
