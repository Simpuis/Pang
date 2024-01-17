#include "transform.h"

#include <tiny_gltf.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <gsl/gsl_narrow>
#include <flecs.h>
#include "src/serialization/scene_deserializer.h"
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

void transform::serialize(scene_serializer &serializer, tinygltf::Model &model, tinygltf::Node *node) const {
    std::vector<double> vectorized_position { position.x, position.y, position.z };
    node->translation = vectorized_position;

    std::vector<double> vectorized_rotation { rotation.x, rotation.y, rotation.z, rotation.w };
    node->rotation = vectorized_rotation;

    std::vector<double> vectorized_scale { local_scale.x, local_scale.y, local_scale.z };
    node->scale = vectorized_scale;
}

void transform::deserialize(deserialization_data& data) {
    transform trans;

    if(!data.node.translation.empty()) {
        const auto &translation = data.node.translation;
        trans.position = glm::vec3(translation[0], translation[1], translation[2]);
    }

    if(!data.node.rotation.empty()) {
        const auto& rotation = data.node.rotation;
        trans.rotation = glm::quat(gsl::narrow<float>(rotation[3]), gsl::narrow<float>(rotation[0]),
                                   gsl::narrow<float>(rotation[1]), gsl::narrow<float>(rotation[2]));
    }

    if(!data.node.scale.empty()) {
        const auto& scale = data.node.scale;
        trans.local_scale = glm::vec3(scale[0], scale[1], scale[2]);
    }

    data.entity.set<transform>(trans);
}

