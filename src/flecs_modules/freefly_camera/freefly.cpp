#include "freefly.h"

#include "src/flecs_modules/transformation/transformation.h"
#include "src/input_handler.h"
#include "src/editor/editor.h"

freefly::freefly(flecs::world& world) {
    world.component<freefly_camera>();

    world.system<freefly_camera>("freefly_camera_init")
            .term_at(1).singleton()
            .kind(scene_manager::play_and_editor_onstart)
            .each([](flecs::iter& it, size_t, freefly_camera& camera) {
                camera.transform_matrix = glm::translate(camera.transform_matrix, glm::vec3(0.0f, 0.0f, 5.0f));
            });

    world.system<freefly_camera, frame_input_table>("freefly_camera_control")
        .term_at(1).singleton()
        .term_at(2).singleton()
        .kind(scene_manager::play_and_editor_onupdate)
        .each([](flecs::iter &it, size_t,
                 freefly_camera &camera,
                 frame_input_table &input) {
        double delta_x = camera.cam_x - input.mouse_x;
        double delta_y = camera.cam_y - input.mouse_y;

        if (input.mouse_right_click == GLFW_PRESS) {
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (abs(delta_x) > 0.00001 || abs(delta_y) > 0.00001) {
                constexpr float rot_speed = 0.1f;
                glm::quat rot1 = glm::angleAxis((float) (delta_x * rot_speed * it.delta_time()),
                                                glm::vec3(0.0, 1.0, 0.0));
                camera.transform_matrix *= glm::mat4_cast(rot1);
                glm::quat rot2 = glm::angleAxis((float) (delta_y * rot_speed * it.delta_time()),
                                                glm::vec3(1.0, 0.0, 0.0));
                camera.local_trans *= glm::mat4_cast(rot2);
            }
        }

        camera.cam_x = input.mouse_x;
        camera.cam_y = input.mouse_y;

        constexpr float camera_speed = 7.5f;
        if (input.key_w == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            camera.transform_matrix = glm::translate(camera.transform_matrix,
                                                     camera_speed * (float) it.delta_time() *
                                                     transformation::global_forward());
        }
        if (input.key_s == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            camera.transform_matrix = glm::translate(camera.transform_matrix,
                                                     camera_speed * (float) it.delta_time() *
                                                     -transformation::global_forward());
        }
        if (input.key_d == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            camera.transform_matrix = glm::translate(camera.transform_matrix,
                                                     camera_speed * (float) it.delta_time() *
                                                     transformation::global_right());
        }
        if (input.key_a == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            camera.transform_matrix = glm::translate(camera.transform_matrix,
                                                     camera_speed * (float) it.delta_time() *
                                                     -transformation::global_right());
        }
        if (input.key_space_bar == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            camera.transform_matrix = glm::translate(camera.transform_matrix,
                                                     camera_speed * (float) it.delta_time() *
                                                     transformation::global_up());
        }
        if (input.key_left_shift == GLFW_PRESS && input.mouse_right_click == GLFW_PRESS) {
            camera.transform_matrix = glm::translate(camera.transform_matrix,
                                                     camera_speed * (float) it.delta_time() *
                                                     -transformation::global_up());
        }
    });
}