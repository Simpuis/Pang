#include "transform.h"

#include <tiny_gltf.h>
#include <glm/mat4x4.hpp>

void transform::serialize(tinygltf::Model &model, tinygltf::Node *node) const {
    std::vector<double> vectorized_matrix = std::vector<double>(16);

    for(int j = 0; j < 4; j++) {
        for(int i = 0; i < 4; i++) {
            vectorized_matrix[j * 4 + i] = transform_matrix[i][j];
        }
    }
    node->matrix = vectorized_matrix;
}
