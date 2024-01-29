#include "renderer.h"

#include "camera.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/flecs_modules/rendering/rendering.h"

void renderer::render_scene(const camera& main_cam, const flecs::world& world,
                            GLFWwindow* window) //const
{
    glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    world.each([&main_cam, this](const position& pos, const rotation& rot, const scale& local_scale, const mesh_component& mesh_comp) {
        for(auto& primitive : meshes.map().at(mesh_comp.mesh)->primitives) {
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
