#include "material.h"

#include "texture.h"
#include "shader.h"

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
