#pragma once
#include <map>
#include <memory>

#include "shader.h"
#include "texture.h"

class material
{
public:
	material();

	std::unique_ptr<shader> material_shader;

	const std::map<unsigned int, std::string>& get_texture() const;
	void set_texture(std::string uniform_name, const std::string& texture_unit, int unit = 0);

private:
	std::map<unsigned int, std::string> material_texture_units_;
};

/*
template<typename Archive>
void serialize(Archive& )
*/
