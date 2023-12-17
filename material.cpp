#include "material.h"

material::material()
= default;

const std::map<unsigned, std::string>& material::get_texture() const
{
	return material_texture_units_;
}

void material::set_texture(std::string uniform_name, const std::string& texture_unit, int unit)
{
    material_texture_units_[unit] = texture_unit;

    material_shader->use();
    material_shader->set_int(uniform_name, unit);
}

