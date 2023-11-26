#include "scene_graph.h"

scene_graph::scene_graph()
{
	root_node_ = std::make_unique<node>();
}

node* scene_graph::create_node() const
{
	return create_node(root_node_.get());
}

node* scene_graph::create_node(node* const parent) const 
{
	node* created_node = new node();
	parent->add_child(created_node);

	return created_node;
}
