#pragma once

#include <map>
#include <memory>

class material;

class material_lookup_registry
{
public:
	material_lookup_registry() = default;

	void register_material(material* mat);
	material* get_material(unsigned int id) const;

private:
	std::map<unsigned int, std::shared_ptr<material>> material_registry_;
};

