#pragma once

#include <glm/glm.hpp>

struct transform
{
	explicit transform(const glm::mat4& trans) : transform_matrix(trans) {}

	glm::mat4 transform_matrix;
};

