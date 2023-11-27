#include <vector>

#include "node.h"

#include "sprite.h"

node::node() : parent_(nullptr)
{
	sprite_ = std::make_unique<sprite>();
	material_ = std::make_unique<material>();
}

node::node(const glm::vec3 pos): parent_(nullptr), position_(pos)
{
	sprite_ = std::make_unique<sprite>();
	material_ = std::make_unique<material>();
}

void node::add_child(node* node_target)
{
	if(node_target->parent_)
	{
		node_target->parent_->release_child(node_target);
	}

	node_target->parent_ = this;
	children_.push_back(std::unique_ptr<node>(node_target));
}

node* node::release_child(node* node_target)
{
	auto iterator = std::find_if(children_.begin(), children_.end(), [node_target](const std::unique_ptr<node>& child)
	{
		return child.get() == node_target;
	});

	if(iterator != children_.end())
	{
		node* released_node = iterator->release();
		children_.erase(iterator);
		return released_node;
	}

	return nullptr;
}

sprite* node::get_sprite() const
{
	return sprite_.get();
}

material* node::get_material() const
{
	return material_.get();
}

