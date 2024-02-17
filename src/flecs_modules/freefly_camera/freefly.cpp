#include "freefly.h"

#include "src/flecs_modules/transformation/transformation.h"
#include "src/input_handler.h"
#include "src/editor/editor.h"

freefly::freefly(flecs::world& world) {
    world.component<freefly_controller>();

    world.system<freefly_controller, transform_matrix>("freefly_controller_init")
            .term_at(1).singleton()
            .term_at(2).second<world_space>()
            .kind(scene_manager::editor_only_onstart)
            .each([](flecs::iter& it, size_t, freefly_controller& controller, transform_matrix& transform) {
                transform.transform = glm::translate(transform.transform, glm::vec3(0.0f, 0.0f, 5.0f));
            });

    world.system<freefly_controller, frame_input_table, transform_matrix>("freefly_controller_control")
        .term_at(2).singleton()
        .term_at(3).second<world_space>()
        .kind(scene_manager::editor_only_onupdate)
        .each([](flecs::iter &it, size_t t,
                 freefly_controller &controller,
                 frame_input_table &input,
                 transform_matrix& transform) {
        double delta_x = controller.cam_x - input.mouse_x;
        double delta_y = controller.cam_y - input.mouse_y;

        if (input.mouse_right_click == GLFW_PRESS) {
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (abs(delta_x) > 0.00001 || abs(delta_y) > 0.00001) {
                constexpr float rot_speed = 0.1f;
                glm::quat rot1 = glm::angleAxis((float) (delta_x * rot_speed * it.delta_time()),
                                                glm::vec3(0.0, 1.0, 0.0));
                transform.transform *= glm::mat4_cast(rot1);
                glm::quat rot2 = glm::angleAxis((float) (delta_y * rot_speed * it.delta_time()),
                                                glm::vec3(1.0, 0.0, 0.0));
                controller.local_trans *= glm::mat4_cast(rot2);
            }
        }

        controller.cam_x = input.mouse_x;
        controller.cam_y = input.mouse_y;

        constexpr float controller_speed = 7.5f;
        if (input.key_w == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            transform.transform = glm::translate(transform.transform,
                                                     controller_speed * (float) it.delta_time() *
                                                     transformation::global_forward());
        }
        if (input.key_s == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            transform.transform = glm::translate(transform.transform,
                                                     controller_speed * (float) it.delta_time() *
                                                     -transformation::global_forward());
        }
        if (input.key_d == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            transform.transform = glm::translate(transform.transform,
                                                     controller_speed * (float) it.delta_time() *
                                                     transformation::global_right());
        }
        if (input.key_a == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            transform.transform = glm::translate(transform.transform,
                                                     controller_speed * (float) it.delta_time() *
                                                     -transformation::global_right());
        }
        if (input.key_space_bar == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            transform.transform = glm::translate(transform.transform,
                                                     controller_speed * (float) it.delta_time() *
                                                     transformation::global_up());
        }
        if (input.key_left_shift == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            transform.transform = glm::translate(transform.transform,
                                                     controller_speed * (float) it.delta_time() *
                                                     -transformation::global_up());
        }

        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform.transform, it.entity(t).get_mut<scale>()->vec, it.entity(t).get_mut<rotation>()->rot,
                       it.entity(t).get_mut<position>()->pos, skew, perspective);
    });
}