#pragma once

#include <memory>

#include "node.h"

class scene_graph
{
	scene_graph();
	~scene_graph();

public:
	node* create_node() const;
	node* create_node(node* parent) const;

private:
	std::unique_ptr<node> root_node_;
};

