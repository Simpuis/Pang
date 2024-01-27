#include "shader_loader.h"

#include <fstream>
#include <iostream>

#include "src/render/shader.h"

std::unique_ptr<shader> shader_loader::load_shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	const std::string vertex_shader_source = read_shader_file(vertex_shader_path);
	const std::string fragment_shader_source = read_shader_file(fragment_shader_path);

	return std::make_unique<shader>(shader(vertex_shader_source.c_str(), fragment_shader_source.c_str()));
}

std::string shader_loader::read_shader_file(const std::string& shader_path)
{
	std::ifstream file(shader_path);

	if(!file.is_open())
	{
		throw std::runtime_error("Unable to open the shader file at " + shader_path);
	}

	std::string source;
	{
		std::string line;
		while(std::getline(file, line))
		{
			source += line + "\n";
		}
	}

	return source;
}
