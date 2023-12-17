#pragma once

#include <glm/glm.hpp>
#include <cereal/cereal.hpp>

struct transform
{
	transform() : transform_matrix(glm::mat4(0.0f)) {}
	explicit transform(const glm::mat4& trans) : transform_matrix(trans) {}

	glm::mat4 transform_matrix;
};

template<typename Archive>
void serialize(Archive& archive, transform& transform)
{
	archive(cereal::make_nvp("transform_matrix", transform.transform_matrix));
}
