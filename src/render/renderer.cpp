#include "renderer.h"

#include "camera.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/flecs_modules/rendering/rendering.h"
#include "src/serialization/serializers/mesh_serializer.h"
#include "src/serialization/serializers/material_serializer.h"
#include "src/serialization/serializers/texture_serializer.h"

void renderer::render_scene(const camera& main_cam, flecs::world& world,
                            GLFWwindow* window) //const
{
    glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if(render_query.world() != world) {
        rebuild_query(world);
    }

    render_query.each([&](
            const position& pos,
            const rotation& rot,
            const scale& local_scale,
            const mesh_component& mesh_comp,
            const mesh_table& mesh_lookup,
            const material_table& material_lookup,
            const texture_table& texture_lookup) {
        if(mesh_comp.mesh >= 0 && mesh_comp.mesh < mesh_lookup.table.size()) {
            for (auto &primitive: mesh_lookup.table.at(mesh_comp.mesh)->primitives) {
                const material *mat = material_lookup.table.at(primitive.material_index).get();

                for (auto &[key, value]: mat->textures) {
                    texture_lookup.table.at(value.index)->bind(mat->uniforms.at(key).int_value);
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
        }
    });
}

void renderer::rebuild_query(flecs::world& world) {
    render_query = world.query_builder<const position,
                                       const rotation,
                                       const scale,
                                       const mesh_component,
                                       const mesh_table,
                                       const material_table,
                                       const texture_table>()
                                       .term_at(5).singleton()
                                       .term_at(6).singleton()
                                       .term_at(7).singleton()
                                       .build();
}
