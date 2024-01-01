#include "material.h"

material::material()
= default;

const std::map<unsigned, texture*>& material::get_texture() const
{
	return material_texture_units_;
}

void material::set_texture(std::string uniform_name, texture* texture_unit, int unit)
{
    material_texture_units_[unit] = texture_unit;

    material_shader->use();
    material_shader->set_int(uniform_name, unit);
}

