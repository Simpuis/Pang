#pragma once
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "texture_lookup_registry.h"

class renderer
{
public:
	void render_scene(texture_lookup_registry& texture_registry, const entt::registry& registry , GLFWwindow* window) const;
};

