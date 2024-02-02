#include <gsl/util>
#include "material.h"

#include "texture.h"
#include "shader.h"

material::material() {
    material_shader = shader_loader::load_shader("vertex_shader.glsl", "fragment_shader.glsl");
}

std::shared_ptr<material> material::deserialize(const tinygltf::Model& model, const tinygltf::Material& gltf_material) {
    auto mat = std::make_shared<material>();

    const auto& pbr_metallic_roughness = gltf_material.pbrMetallicRoughness;

    mat->material_shader->use();

    const auto& base_color = pbr_metallic_roughness.baseColorFactor;
    mat->set_vector("baseColor", glm::vec4(base_color[0], base_color[1], base_color[2], base_color[3]));
    mat->set_float("metallicFactor", gsl::narrow_cast<float>(pbr_metallic_roughness.metallicFactor));
    mat->set_float("roughnessFactor", gsl::narrow_cast<float>(pbr_metallic_roughness.roughnessFactor));

    mat->apply_texture_uniform("baseColorTexture", "useBaseColorTexture", pbr_metallic_roughness.baseColorTexture);
    mat->apply_texture_uniform("metallicRoughnessTexture", "useMetallicRoughnessTexture", pbr_metallic_roughness.metallicRoughnessTexture);

    return mat;
}

void material::apply_texture_uniform(const std::string &texture_uniform, const std::string &use_uniform,
                                     const tinygltf::TextureInfo &tex) {
    if(tex.index > 0) {
        textures[texture_uniform] = texture_info(tex);
        set_int(texture_uniform, 0);
        set_bool(use_uniform, true);
    }
    else {
        set_bool(use_uniform, false);
    }
}

tinygltf::Material material::serialize() {
    tinygltf::Material serialized_material;
    tinygltf::PbrMetallicRoughness pbr_metallic_roughness;

    const glm::vec4 base_color = get_uniform("baseColor").vec4_value;
    pbr_metallic_roughness.baseColorFactor = std::vector<double>({base_color.x, base_color.y, base_color.z, base_color.w});
    pbr_metallic_roughness.metallicFactor = get_uniform("metallicFactor").float_value;
    pbr_metallic_roughness.roughnessFactor = get_uniform("roughnessFactor").float_value;
    if(textures.find("baseColorTexture") != textures.end()) {
        pbr_metallic_roughness.baseColorTexture = (tinygltf::TextureInfo) textures["baseColorTexture"];
    }
    if(textures.find("metallicRoughnessTexture") != textures.end()) {
        pbr_metallic_roughness.metallicRoughnessTexture = (tinygltf::TextureInfo) textures["metallicRoughnessTexture"];
    }

    return serialized_material;
}

uniform_value &material::get_uniform(const std::string &name) {
    return uniforms[name];
}

void material::set_bool(const std::string &name, bool value) {
    auto uniform = uniform_value();
    uniform.bool_value = value;
    uniforms[name] = uniform;
    material_shader->use();
    material_shader->set_bool(name, value);
}

void material::set_int(const std::string &name, int value) {
    auto uniform = uniform_value();
    uniform.int_value = value;
    uniforms[name] = uniform;
    material_shader->use();
    material_shader->set_int(name, value);
}

void material::set_float(const std::string &name, float value) {
    auto uniform = uniform_value();
    uniform.float_value = value;
    uniforms[name] = uniform;
    material_shader->use();
    material_shader->set_float(name, value);
}

void material::set_vector(const std::string &name, glm::vec2 value) {
    auto uniform = uniform_value();
    uniform.vec2_value = value;
    uniforms[name] = uniform;
    material_shader->use();
    material_shader->set_vector(name, value);
}

void material::set_vector(const std::string &name, glm::vec3 value) {
    auto uniform = uniform_value();
    uniform.vec3_value = value;
    uniforms[name] = uniform;
    material_shader->use();
    material_shader->set_vector(name, value);
}

void material::set_vector(const std::string &name, glm::vec4 value) {
    auto uniform = uniform_value();
    uniform.vec4_value = value;
    uniforms[name] = uniform;
    material_shader->use();
    material_shader->set_vector(name, value);
}

void material::set_matrix(const std::string &name, glm::mat4 value) {
    auto uniform = uniform_value();
    uniform.mat4_value = value;
    uniforms[name] = uniform;
    material_shader->use();
    material_shader->set_matrix(name, value);
}

