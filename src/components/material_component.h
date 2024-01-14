#pragma once
#include <memory>
#include <entt/entt.hpp>
#include <tiny_gltf.h>

class material;
class scene_serializer;

struct material_component
{
	material_component() {}

	std::shared_ptr<material> material;

    void serialize(scene_serializer& serializer, tinygltf::Model& model, tinygltf::Node* node) const;
    static void deserialize(const tinygltf::Node& node, entt::registry& registry, const entt::entity& entity);
};
