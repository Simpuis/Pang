#include "transform.h"

#include <tiny_gltf.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <gsl/gsl_narrow>
#include "src/serialization/scene_deserializer.h"
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

void transform::serialize(scene_serializer &serializer, tinygltf::Model &model, tinygltf::Node *node) const {
    std::vector<double> vectorized_matrix = std::vector<double>(16);

    for(int j = 0; j < 4; j++) {
        for(int i = 0; i < 4; i++) {
            vectorized_matrix[j * 4 + i] = transform_matrix[i][j];
        }
    }
    node->translation = vectorized_matrix;
}

void transform::deserialize(deserialization_data& data) {
    auto& trans = data.registry.emplace<transform>(data.entity);
    trans.transform_matrix = glm::mat4x4(1.0f);

    if(!data.node.translation.empty()) {
        const auto &translation = data.node.translation;
        trans.transform_matrix = glm::translate(trans.transform_matrix,
                                                glm::vec3(translation[0], translation[1], translation[2]));
    }

    if(!data.node.rotation.empty()) {
        const auto& rotation = data.node.rotation;
        glm::quat rot = glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]);
        trans.transform_matrix *= glm::mat4_cast(rot);
    }

    if(!data.node.scale.empty()) {
        const auto& scale = data.node.scale;
        trans.transform_matrix = glm::scale(trans.transform_matrix, glm::vec3(scale[0], scale[1], scale[2]));
    }
}

