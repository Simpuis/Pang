#include "material.h"

material::material()
= default;

const std::map<unsigned, std::unique_ptr<texture>>& material::get_texture() const
{
	return material_texture_units;
}

void material::set_texture(std::string uniform_name, texture* texture_unit, int unit)
{
    material_texture_units[unit] = std::unique_ptr<texture>(texture_unit);

    material_shader->use();
    material_shader->set_int(uniform_name, unit);
}
