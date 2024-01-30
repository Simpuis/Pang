#pragma once

#include <string>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <flecs.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <tiny_gltf.h>

#include "editor/editor.h"
#include "input_handler.h"
#include "src/render/freefly_camera.h"
#include "src/serialization/scene_serializer.h"
#include "src/render/renderer.h"

/**
 * @brief The game class is the main class of the engine.
 * It handles setting up the glfw window and contains the main loop.
 * It also owns and manages the ECS registry.
 */
class game
{
public:
    /**
     * @brief Construct a new game object
     *
     * @param width The width of the window
     * @param height The height of the window
     * @param title The title of the window
     */
    game(int width, int height, const std::string& title);
    ~game();

public:
    /**
     * @brief The main loop of the game
     *
     */
    void loop();

    freefly_camera main_camera;
    GLFWwindow* window_;

private:
    void init_glfw_window(int width, int height, const std::string& title);
    void setup_world();
    void exit();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    std::unique_ptr<input_handler> input_;
    scene_serializer scene_loader;
    flecs::world world_;
    renderer renderer_;
    editor editor_;
};
