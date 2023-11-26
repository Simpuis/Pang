#pragma once

#include <vector>
#include <glm/vec2.hpp>

class sprite
{
public:
	sprite();
	explicit sprite(const glm::vec2 size);

	void init();

public:
	unsigned int VBO_;
	unsigned int VAO_;
	unsigned int EBO_;

	glm::vec2 size_;
};

