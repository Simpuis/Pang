#include "material_lookup_registry.h"

void material_lookup_registry::register_material(material* mat)
{
	material_registry_.insert(std::make_pair(material_registry_.size(), std::shared_ptr<material>(mat)));
}

material* material_lookup_registry::get_material(const unsigned id) const
{
	return material_registry_.at(id).get();
}
