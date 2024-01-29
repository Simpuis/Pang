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

class mesh_lookup {
public:
    unsigned int add(std::shared_ptr<mesh> new_mesh) {
        unsigned int count = mesh_map.size();
        mesh_map.insert({count, std::move(new_mesh)});
        return count;
    }
    const std::map<unsigned int, std::shared_ptr<mesh>>& map() { return mesh_map; }

private:
    std::map<unsigned int, std::shared_ptr<mesh>> mesh_map;
};

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

    mesh_lookup meshes;
};
