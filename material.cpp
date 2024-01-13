#include "material.h"

#include <tiny_gltf.h>
#include <string>
#include <memory>

#include "texture.h"
#include "shader.h"
#include "shader_loader.h"

material::material() {
    material_shader = shader_loader::load_shader("vertex_shader.glsl", "fragment_shader.glsl");
}

const std::map<unsigned, std::shared_ptr<texture>>& material::get_texture() const
{
	return material_texture_units_;
}

tinygltf::Material &material::serialize() const {
    tinygltf::Material mat;
    return mat;
}

void material::set_texture(const std::string& uniform_name, std::shared_ptr<texture> texture_unit, int unit) {
    material_texture_units_[unit] = std::move(texture_unit);

    material_shader->use();
    material_shader->set_int(uniform_name, unit);
}

std::shared_ptr<material> material::deserialize(const tinygltf::Model& model, const tinygltf::Material& gltf_material) {
    auto mat = std::make_shared<material>();

    const auto& pbr_metallic_roughness = gltf_material.pbrMetallicRoughness;
    if(pbr_metallic_roughness.baseColorTexture.index >= 0) {
        mat->set_texture("TEXTURE_" + std::to_string(pbr_metallic_roughness.baseColorTexture.texCoord),
                         texture::deserialize(model, model.textures[pbr_metallic_roughness.baseColorTexture.index]),
                         pbr_metallic_roughness.baseColorTexture.texCoord);
    }

    return mat;
}

