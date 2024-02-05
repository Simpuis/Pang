#pragma once

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <flecs.h>
#include <map>
#include <memory>

struct position;
struct rotation;
struct scale;
struct mesh_component;
struct mesh_table;
struct material_table;
struct texture_table;

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
    void render_scene(const camera& main_camera, flecs::world& world, GLFWwindow* window);

private:
    void rebuild_query(flecs::world& world);

    flecs::query<const position,
                 const rotation,
                 const scale,
                 const mesh_component,
                 const mesh_table,
                 const material_table,
                 const texture_table> render_query;
};
