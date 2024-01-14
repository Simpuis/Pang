#pragma once

#include <map>
#include <memory>
#include <string>

#include "shader.h"

class texture;

namespace tinygltf {
    struct Material;
    class Model;
}

/**
 * @brief A material is a collection of textures units and a shader.
 *
 */
class material
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

    [[nodiscard]] tinygltf::Material& serialize() const;
    static std::shared_ptr<material> deserialize(const tinygltf::Model& model, const tinygltf::Material& gltf_material);
private:
	std::map<unsigned int, std::shared_ptr<texture>> material_texture_units_;
};
