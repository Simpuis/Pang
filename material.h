#pragma once
#include <map>
#include <memory>

#include "shader.h"
#include "texture.h"

namespace tinygltf {
    class Material;
}

class material
{
public:
	material();

	std::unique_ptr<shader> material_shader;

	[[nodiscard]] const std::map<unsigned int, std::shared_ptr<texture>>& get_texture() const;
	void set_texture(const std::string& uniform_name, std::shared_ptr<texture> texture_unit, int unit = 0);

    [[nodiscard]] tinygltf::Material& serialize() const;
    static std::shared_ptr<material> deserialize(const tinygltf::Material& gltf_material);
private:
	std::map<unsigned int, std::shared_ptr<texture>> material_texture_units_;
};

/*
template<typename Archive>
void serialize(Archive& )
*/
