#pragma once

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <flecs.h>
#include <map>
#include <memory>

class mesh;
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
     * @param main_camera The camera to use to rendering the scene
     * @param registry The registry containing the entities to rendering
     * @param window The glfw window to rendering to
     */
    void render_scene(const camera& main_camera, const flecs::world& world, GLFWwindow* window);
};
