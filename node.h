#pragma once

#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include "sprite.h"


class node
{
public:
	node();
	explicit node(const glm::vec3 pos);

public:
	void add_child(node* node_target);
	node* release_child(node* node);

public:
	node* parent_;
	std::vector<std::unique_ptr<node>> children_;
	std::unique_ptr<sprite> sprite_;

	glm::vec3 position_;
};

