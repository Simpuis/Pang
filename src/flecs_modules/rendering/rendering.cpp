#include "rendering.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/render/shader.h"
#include "src/flecs_modules/freefly_camera/freefly.h"
#include "src/serialization/scene_serializer.h"

#include <glad/glad.h>

rendering::rendering(flecs::world& world) {
    world.component<mesh_component>()
            .member<unsigned int>("mesh");
    world.component<mesh_table>();
    world.component<material_table>();
    world.component<texture_table>();

    world.import<transformation>();
    world.import<freefly>();

    world.system<const position, const rotation, const scale, const scene_root>("world_transform_conversion")
        .kind(flecs::PreStore)
        .term_at(4).singleton()
        .each([](flecs::entity e, const position& pos, const rotation& rot, const scale& local_scale, const scene_root& root) {
            auto parent = e.parent();
            transform_matrix* parent_transform = nullptr;
            if(parent != root.root_entity) {
                parent_transform = parent.get_mut<transform_matrix, world_space>();
            }

            auto* local_transform = e.get_mut<transform_matrix, local_space>();
            local_transform->transform = glm::mat4x4(1.0f);
            local_transform->transform = glm::translate(local_transform->transform, pos.pos);
            local_transform->transform *= glm::mat4_cast(rot.rot);
            local_transform->transform = glm::scale(local_transform->transform, local_scale.vec);

            auto* global_transform = e.get_mut<transform_matrix, world_space>();
            if(parent_transform) {
                global_transform->transform = parent_transform->transform * local_transform->transform;
            }
            else {
                global_transform->transform = local_transform->transform;
            }
        });

    world.system("render_clear")
        .kind(flecs::OnStore)
        .iter([](flecs::iter& it) {
            glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        });

    world.system<const transform_matrix,
                 const mesh_component,
                 const mesh_table,
                 const material_table,
                 const texture_table,
                 const freefly_camera>("Render")
                 .term_at(1).second<world_space>()
                 .term_at(3).singleton()
                 .term_at(4).singleton()
                 .term_at(5).singleton()
                 .term_at(6).singleton()
                 .kind(flecs::OnStore)
                 .each([&](
                 const transform_matrix& transform,
                 const mesh_component& mesh_comp,
                 const mesh_table& mesh_lookup,
                 const material_table& material_lookup,
                 const texture_table& texture_lookup,
                 const freefly_camera& main_cam) {
        if(mesh_comp.mesh >= 0 && mesh_comp.mesh < mesh_lookup.table.size()) {
            for (auto &primitive: mesh_lookup.table.at(mesh_comp.mesh)->primitives) {
                const material *mat = material_lookup.table.at(primitive.material_index).get();

                for (auto &[key, value]: mat->textures) {
                    texture_lookup.table.at(value.index)->bind(mat->uniforms.at(key).int_value);
                }
                mat->material_shader->use();

                mat->material_shader->set_matrix("model", transform.transform);

                mat->material_shader->set_matrix("view", glm::inverse(main_cam.transform_matrix * main_cam.local_trans));

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

