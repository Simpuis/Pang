#pragma once

#include <map>
#include <memory>
#include <string>
#include <tiny_gltf.h>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

#include "src/serialization/shader_loader.h"

class shader;
class texture;

/**
 * @brief A material is a collection of textures units and a shader.
 *
 */

struct uniform_value {
    union {
        bool bool_value;
        int int_value;
        float float_value;

        glm::vec2 vec2_value;
        glm::vec3 vec3_value;
        glm::vec4 vec4_value;
        glm::mat2 mat2_value;
        glm::mat3 mat3_value;
        glm::mat4 mat4_value;
    };
};

struct texture_info {
    texture_info() = default;
    texture_info(int index, int tex_coord) : index(index), tex_coord(tex_coord) {}
    explicit texture_info(const tinygltf::TextureInfo& gltf_info) : index(gltf_info.index), tex_coord(gltf_info.texCoord) {}

    explicit operator tinygltf::TextureInfo() const {
        tinygltf::TextureInfo info;
        info.index = index;
        info.texCoord = tex_coord;
        return info;
    }

    int index = -1;
    int tex_coord = -1;
};

class material
{
public:
    /**
     * @brief Construct a new material object
     * Loads the default shader
     */
    material();

    /**
     * @brief Set a texture unit for this material
     *
     * @param uniform_name The name of the uniform in the shader
     * @param texture_unit A pointer to the texture
     * @param unit The texture unit to set
     */
    tinygltf::Material serialize();
    static std::shared_ptr<material> deserialize(const tinygltf::Model& model, const tinygltf::Material& gltf_material);

    void set_bool(const std::string&, bool value);
    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);
    void set_vector(const std::string& name, glm::vec2 vector);
    void set_vector(const std::string& name, glm::vec3 vector);
    void set_vector(const std::string& name, glm::vec4 vector);
    void set_matrix(const std::string& name, glm::mat4 matrix);

    uniform_value& get_uniform(const std::string& name);

    std::map<std::string, texture_info> textures;
    std::unique_ptr<shader> material_shader;
    std::map<std::string, uniform_value> uniforms;
private:
    void apply_texture_uniform(const std::string &texture_uniform, const std::string &use_uniform,
                               const tinygltf::TextureInfo &tex);
};
