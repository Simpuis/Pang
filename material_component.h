#pragma once
#include <memory>

class material;

struct material_component
{
	material_component(const unsigned int material_id) : id(material_id) {}

	unsigned int id;
	std::shared_ptr<material> material; 
};
