#include "sprite.h"

#include <glad/glad.h>

sprite::sprite(): VBO_(0), VAO_(0), EBO_(0), size_(glm::vec2(0.5f, 0.5f))
{
	init();
}

sprite::sprite(const glm::vec2 size): VBO_(0), VAO_(0), EBO_(0), size_(size)
{
	init();
}

void sprite::init()
{
	glGenVertexArrays(1, &VAO_);

	glBindVertexArray(VAO_);

	const float vertices[] = {
		 size_.x,  size_.y, 0.0f,	1.0f, 1.0f, // top right
		 size_.x, -size_.y, 0.0f,	1.0f, 0.0f, // bottom right
		-size_.x, -size_.y, 0.0f,	0.0f, 0.0f, // bottom left
		-size_.x,  size_.y, 0.0f,	0.0f, 1.0f  // top left
	};

	const unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenBuffers(1, &VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO_);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
