#pragma once
#include <glad/glad.h>

class shader
{
public:

	shader(const char* vertex_shader_source, const char* fragment_shader_source);

	void use() const;

private:
	static unsigned int compile_shader(const char* shader_source, GLenum shader_type);
	void link(unsigned int vertex_shader, unsigned int fragment_shader);

	static bool check_shader_compile_success(unsigned int shader);
	static bool check_program_compile_success(unsigned int program);

	unsigned int id_;
};

