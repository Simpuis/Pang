#include "shader.h"

#include <iostream>
#include <glad/glad.h>

shader::shader(const char* vertex_shader_source, const char* fragment_shader_source)
{
	id_ = 0;
	{
		const unsigned int vertex_id = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
		const unsigned int fragment_id = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

		link(vertex_id, fragment_id);
	}
}

void shader::use() const
{
	glUseProgram(id_);
}

unsigned int shader::compile_shader(const char* shader_source, const GLenum shader_type)
{
	const unsigned int shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &shader_source, nullptr);
	glCompileShader(shader_id);
	check_shader_compile_success(shader_id);

	return shader_id;
}

void shader::link(const unsigned vertex_shader, const unsigned fragment_shader)
{
	id_ = glCreateProgram();

	glAttachShader(id_, vertex_shader);
	glAttachShader(id_, fragment_shader);
	glLinkProgram(id_);

	check_program_compile_success(id_);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

bool shader::check_shader_compile_success(const unsigned shader)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		char info_log[512];
		glGetShaderInfoLog(shader, 512, nullptr, info_log);
		std::cout << "Error: Fragment shader compilation failed\n" << info_log << std::endl;
	}

	return success;
}

bool shader::check_program_compile_success(const unsigned program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetProgramInfoLog(program, 512, nullptr, info_log);
		std::cout << "Error: Shader program linking error\n" << info_log << std::endl;
	}

	return success;
}
