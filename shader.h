#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class shader
{
public:

	shader(const char* vertex_shader_source, const char* fragment_shader_source);

	void use() const;

	void set_int(const std::string& name, int value) const;
	void set_vector(const std::string& name, glm::vec2 vector) const;
	void set_vector(const std::string& name, glm::vec3 vector) const;
	void set_vector(const std::string& name, glm::vec4 vector) const;
	void set_matrix(const std::string& name, glm::mat4 matrix) const;

private:
	static unsigned int compile_shader(const char* shader_source, GLenum shader_type);
	void link(unsigned int vertex_shader, unsigned int fragment_shader);

	static bool check_shader_compile_success(unsigned int shader);
	static bool check_program_compile_success(unsigned int program);

	unsigned int id_;
};

