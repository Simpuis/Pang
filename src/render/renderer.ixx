module;

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <flecs.h>

export module renderer;

import camera;
import transformation;
import mesh;
import material;

/**
 * @brief The renderer class is responsible for rendering the scene.
 *
 */
export class renderer
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

void renderer::render_scene(const camera& main_cam, const flecs::world& world,
                            GLFWwindow* window) //const
{
    glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    world.each([&main_cam](const position& pos, const rotation& rot, const scale& local_scale, const mesh& mesh_comp) {
        for(auto& primitive : mesh_comp.primitives) {
            const material *mat = primitive.mat.get();

            for (auto &[key, value]: mat->get_texture()) {
                value->bind(key);
            }
            mat->material_shader->use();

            mat->material_shader->set_matrix("model", transformation::model(pos, rot, local_scale));

            mat->material_shader->set_matrix("view", main_cam.get_view_matrix());

            glm::mat4 projection_matrix;
            projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
            mat->material_shader->set_matrix("projection", projection_matrix);

            glBindVertexArray(primitive.VAO);
            glDrawElements(primitive.mode, primitive.count, primitive.indices_component_type, nullptr);

            glBindVertexArray(0);
            glUseProgram(0);
        }
    });
}
