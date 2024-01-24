module;

#include <map>
#include <memory>
#include <string>
#include <tiny_gltf.h>
#include <glm/vec4.hpp>

#include "src/serialization/shader_loader.h"

export module material;

import texture;
import shader;

/**
 * @brief A material is a collection of textures units and a shader.
 *
 */
export class material
{
public:
    /**
     * @brief Construct a new material object
     * Loads the default shader
     */
	material();

	std::unique_ptr<shader> material_shader;

	[[nodiscard]] const std::map<unsigned int, std::shared_ptr<texture>>& get_texture() const;
    /**
     * @brief Set a texture unit for this material
     *
     * @param uniform_name The name of the uniform in the shader
     * @param texture_unit A pointer to the texture
     * @param unit The texture unit to set
     */
	void set_texture(const std::string& uniform_name, std::shared_ptr<texture> texture_unit, int unit = 0);

    static std::shared_ptr<material> deserialize(const tinygltf::Model& model, const tinygltf::Material& gltf_material);
private:
	std::map<unsigned int, std::shared_ptr<texture>> material_texture_units_;
};

material::material() {
    material_shader = shader_loader::load_shader("vertex_shader.glsl", "fragment_shader.glsl");
}

const std::map<unsigned, std::shared_ptr<texture>>& material::get_texture() const
{
    return material_texture_units_;
}

void material::set_texture(const std::string& uniform_name, std::shared_ptr<texture> texture_unit, int unit) {
    material_texture_units_[unit] = std::move(texture_unit);

    material_shader->use();
    material_shader->set_int(uniform_name, unit);
}

std::shared_ptr<material> material::deserialize(const tinygltf::Model& model, const tinygltf::Material& gltf_material) {
    auto mat = std::make_shared<material>();

    const auto& pbr_metallic_roughness = gltf_material.pbrMetallicRoughness;

    mat->material_shader->use();

    const auto& base_color = pbr_metallic_roughness.baseColorFactor;
    mat->material_shader->set_vector("baseColor", glm::vec4(base_color[0], base_color[1], base_color[2], base_color[3]));
    if(pbr_metallic_roughness.baseColorTexture.index >= 0) {
        mat->set_texture("baseColorTexture" + std::to_string(pbr_metallic_roughness.baseColorTexture.texCoord),
                         texture::deserialize(model, model.textures[pbr_metallic_roughness.baseColorTexture.index]),
                         pbr_metallic_roughness.baseColorTexture.texCoord);
        mat->material_shader->set_bool("useBaseColorTexture", true);
    }
    else {
        mat->material_shader->set_bool("useBaseColorTexture", false);
    }


    return mat;
}
