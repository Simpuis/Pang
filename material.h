#pragma once
#include <memory>

#include "shader.h"

class material
{
public:
	material();

	std::unique_ptr<shader> material_shader;
private:
};

