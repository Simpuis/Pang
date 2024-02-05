#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <flecs.h>

struct frame_input_table {
    double mouse_x;
    double mouse_y;
    int mouse_right_click;

    int key_w;
    int key_s;
    int key_a;
    int key_d;
    int key_left_shift;
    int key_space_bar;
};

class input_handler
{
public:
	void process_input(flecs::world& world, GLFWwindow* window);
};

