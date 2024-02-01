#include "mesh.h"

#include "material.h"
#include "shader.h"

std::shared_ptr<mesh> mesh::deserialize(const tinygltf::Model& model, const tinygltf::Mesh& mesh_to_load) {
    auto deserialized_mesh = std::make_shared<mesh>();
    deserialized_mesh->setup_gltf_mesh(model, mesh_to_load);
    return deserialized_mesh;
}

void mesh::setup_gltf_mesh(const tinygltf::Model &model, const tinygltf::Mesh& mesh_to_load) {
    for(const auto& gltf_primitive : mesh_to_load.primitives) {
        primitive mesh_primitive;
        mesh_primitive.mode = gltf_primitive.mode;
        mesh_primitive.attributes = gltf_primitive.attributes;
        glGenVertexArrays(1, &mesh_primitive.VAO);
        glBindVertexArray(mesh_primitive.VAO);

        for(const auto& attribute : mesh_primitive.attributes) {
            const auto& accessor = model.accessors[attribute.second];
            const auto& buffer_view = model.bufferViews[accessor.bufferView];
            const auto& buffer = model.buffers[buffer_view.buffer];

            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(buffer_view.target, VBO);

            glBufferData(buffer_view.target, buffer_view.byteLength, &buffer.data.at(0) + buffer_view.byteOffset, GL_STATIC_DRAW);

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

            mesh_primitive.indices = gltf_primitive.indices;
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

        mesh_primitive.material_index = gltf_primitive.material;

        primitives.push_back(mesh_primitive);
    }
}

tinygltf::Mesh mesh::serialize() {
    tinygltf::Mesh serialized_mesh;
    for(const auto& primitive : primitives) {
        tinygltf::Primitive serialized_primitive;
        serialized_primitive.mode = primitive.mode;
        serialized_primitive.attributes = primitive.attributes;
        serialized_primitive.indices = primitive.indices;
        serialized_primitive.material = primitive.material_index;
    }

    return serialized_mesh;
}
