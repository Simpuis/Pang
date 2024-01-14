#pragma once
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

class camera;

class renderer
{
public:
	void render_scene(const camera& main_camera, const entt::registry& registry , GLFWwindow* window);
};

