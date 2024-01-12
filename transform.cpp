#include "transform.h"

#include <tiny_gltf.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <gsl/gsl_narrow>
#include <scene_deserializer.h>

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

    if(data.node.translation.empty()) {
        trans.transform_matrix = glm::mat4(1.0f);
        return;
    }
    const auto& translation = data.node.translation;
    trans.transform_matrix = glm::mat4(1.0f);
    trans.transform_matrix = glm::translate(trans.transform_matrix, glm::vec3(translation[0], translation[1], translation[2]));
}

