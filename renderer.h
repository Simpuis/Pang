#pragma once
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

class renderer
{
public:
	void render_scene(const entt::registry& registry , GLFWwindow* window) const;
};

