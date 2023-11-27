#pragma once
#include <string>

#include "shader.h"

class shader_loader
{
public:
	static shader* load_shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

private:
	static std::string read_shader_file(const std::string& shader_path);
};

