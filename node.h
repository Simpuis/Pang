#pragma once

#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "material.h"
#include "sprite.h"


class node
{
public:
	node();
	explicit node(const glm::vec3 pos);

public:
	void add_child(node* node_target);
	node* release_child(node* node);

	sprite* get_sprite() const;
	material* get_material() const;

private:
	node* parent_;
	std::vector<std::unique_ptr<node>> children_;

	std::unique_ptr<sprite> sprite_;
	std::unique_ptr<material> material_;

	glm::vec3 position_;
};

