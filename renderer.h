#pragma once
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "material_lookup_registry.h"

class renderer
{
public:
	void render_scene(const material_lookup_registry& material_registry, const entt::registry& registry , GLFWwindow* window) const;
};

