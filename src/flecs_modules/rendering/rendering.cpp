#include "rendering.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/render/shader.h"
#include "src/flecs_modules/freefly_camera/freefly.h"
#include "src/serialization/scene_manager.h"
#include "src/editor/editor.h"

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
        .kind(scene_manager::play_and_editor_onstore)
        .write<transform_matrix, local_space>()
        .write<transform_matrix, world_space>()
        .term_at(4).singleton()
        .each([](flecs::entity e, const position& pos, const rotation& rot, const scale& local_scale, const scene_root& root) {
            auto parent = e.parent();
            transform_matrix* parent_transform = nullptr;
            if(parent != root.root_entity && parent.is_valid()) {
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
        .kind(scene_manager::play_and_editor_onstore)
        .iter([](flecs::iter& it) {
            glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        });

    world.system<const transform_matrix,
                 const mesh_component,
                 const mesh_table,
                 const material_table,
                 const texture_table,
                 const main_camera,
                 const render_debug_camera>("Render")
                 .term_at(1).second<world_space>()
                 .term_at(3).singleton()
                 .term_at(4).singleton()
                 .term_at(5).singleton()
                 .term_at(6).singleton()
                 .term_at(6).optional()
                 .term_at(7).singleton()
                 .kind(scene_manager::play_and_editor_onstore)
                 .each([&](
                 const transform_matrix& transform,
                 const mesh_component& mesh_comp,
                 const mesh_table& mesh_lookup,
                 const material_table& material_lookup,
                 const texture_table& texture_lookup,
                 const main_camera& main,
                 const render_debug_camera& use_debug_camera) {
        if(mesh_comp.mesh >= 0 && mesh_comp.mesh < mesh_lookup.table.size()) {
            for (auto &primitive: mesh_lookup.table.at(mesh_comp.mesh)->primitives) {
                const material *mat = material_lookup.table.at(primitive.material_index).get();

                for (auto &[key, value]: mat->textures) {
                    texture_lookup.table.at(value.index)->bind(mat->uniforms.at(key).int_value);
                }
                mat->material_shader->use();

                mat->material_shader->set_matrix("model", transform.transform);

                glm::mat4x4 view;
                camera cam;
                if(use_debug_camera.debug_camera) {
                    const auto* debug_camera_trans = use_debug_camera.debug_camera->get<transform_matrix, world_space>();
                    const auto* debug_camera_freefly = use_debug_camera.debug_camera->get<freefly_controller>();
                    view = glm::inverse(debug_camera_trans->transform * debug_camera_freefly->local_trans);
                    cam = *use_debug_camera.debug_camera->get<camera>();
                }
                else {
                    view = glm::inverse(main.camera_entity.get<transform_matrix, world_space>()->transform);
                    cam = *main.camera_entity.get<camera>();
                }
                mat->material_shader->set_matrix("view", view);

                glm::mat4 projection_matrix;
                projection_matrix = glm::perspective(glm::radians(cam.field_of_view_degrees),
                                                     2560.0f / 1440.0f, cam.near_plane_z, cam.far_plane_z);
                mat->material_shader->set_matrix("projection", projection_matrix);

                glBindVertexArray(primitive.VAO);
                glDrawElements(primitive.mode, primitive.count, primitive.indices_component_type, nullptr);

                glBindVertexArray(0);
                glUseProgram(0);
            }
        }
    });
}

