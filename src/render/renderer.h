#pragma once
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <flecs.h>

class camera;

/**
 * @brief The renderer class is responsible for rendering the scene.
 *
 */
class renderer
{
public:
    /**
     * @brief Render the scene
     *
     * @param main_camera The camera to use to render the scene
     * @param registry The registry containing the entities to render
     * @param window The glfw window to render to
     */
	void render_scene(const camera& main_camera, const flecs::world& world, GLFWwindow* window);
};

