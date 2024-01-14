#include "mesh.h"

#include <glad/glad.h>
#include <tiny_gltf.h>
#include <scene_deserializer.h>

#include "material.h"

void mesh::deserialize(deserialization_data& data) {
    if(data.node.mesh < 0) return;

    auto& emplaced_mesh = data.registry.emplace<mesh>(data.entity);
    emplaced_mesh.setup_gltf_mesh(data.model, data.model.meshes[data.node.mesh], data.material_lookup);
}

void mesh::setup_gltf_mesh(const tinygltf::Model &model, const tinygltf::Mesh& mesh_to_load, std::map<unsigned int, std::shared_ptr<material>>& material_lookup) {
    for(const auto& gltf_primitive : mesh_to_load.primitives) {
        primitive mesh_primitive;
        glGenVertexArrays(1, &mesh_primitive.VAO);
        glBindVertexArray(mesh_primitive.VAO);

        for(const auto& attribute : gltf_primitive.attributes) {
            const auto& accessor = model.accessors[attribute.second];
            const auto& buffer_view = model.bufferViews[accessor.bufferView];
            const auto& buffer = model.buffers[buffer_view.buffer];

            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, buffer_view.byteLength, &buffer.data.at(0) + buffer_view.byteOffset, GL_STATIC_DRAW);

            int vertex_attribute_index = -1;
            if(attribute.first == "POSITION") vertex_attribute_index = 0;
            if(attribute.first == "NORMAL") vertex_attribute_index = 1;
            if(attribute.first == "TEXCOORD_0") vertex_attribute_index = 2;

            if(vertex_attribute_index < 0) continue;

            int byte_stride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            glEnableVertexAttribArray(vertex_attribute_index);
            glVertexAttribPointer(vertex_attribute_index, accessor.type, accessor.componentType,
                                  accessor.normalized ? GL_TRUE : GL_FALSE, byte_stride, (char*)NULL + accessor.byteOffset);
        }

        {
            glGenBuffers(1, &mesh_primitive.EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_primitive.EBO);

            const auto &accessor = model.accessors[gltf_primitive.indices];
            const auto &buffer_view = model.bufferViews[accessor.bufferView];
            const auto &buffer = model.buffers[buffer_view.buffer];

            mesh_primitive.count = accessor.count;
            mesh_primitive.byte_offset = buffer_view.byteOffset;
            mesh_primitive.indices_component_type = accessor.componentType;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_view.byteLength, &buffer.data.at(0) + buffer_view.byteOffset,
                         GL_STATIC_DRAW);
        }

        glBindVertexArray(0);

        if(gltf_primitive.material < 0) {
            mesh_primitive.mat = std::make_shared<material>();
        }
        else if(material_lookup.count(gltf_primitive.material) <= 0) {
            material_lookup.insert({gltf_primitive.material, material::deserialize(model, model.materials[gltf_primitive.material])});
            mesh_primitive.mat = material_lookup[gltf_primitive.material];
        }
        else {
            mesh_primitive.mat = material_lookup[gltf_primitive.material];
        }

        primitives.push_back(mesh_primitive);
    }
}

void mesh::serialize(scene_serializer &serializer, tinygltf::Model &model, tinygltf::Node *node) const {

}
