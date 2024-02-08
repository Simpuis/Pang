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
#include <optional>

#include "editor/editor.h"
#include "input_handler.h"
#include "src/serialization/scene_serializer.h"
#include "src/serialization/scene_manager.h"
#include "src/serialization/serializers/gltf_core_node_serializer.h"
#include "src/serialization/serializers/buffer_serializer.h"
#include "src/serialization/serializers/mesh_serializer.h"
#include "src/serialization/serializers/image_serializer.h"
#include "src/serialization/serializers/material_serializer.h"
#include "src/serialization/serializers/sampler_serializer.h"
#include "src/serialization/serializers/texture_serializer.h"
#include "src/serialization/serializers/extension_serializer.h"

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
    void loop(unsigned int startup_scene = 0);
    void setup_scenes(std::initializer_list<std::pair<unsigned int, std::string>> scene_filenames);
    template<typename... Extension_Serializer_Ts>
    void register_extension_types();
    void setup_editor();

    GLFWwindow* window_;

private:
    void init_glfw_window(int width, int height, const std::string& title);
    void exit();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    std::unique_ptr<input_handler> input_;
    std::unique_ptr<scene_manager> scene_manager_;
    std::optional<editor> editor_;
};

template<typename... Extension_Serializer_Ts>
void game::register_extension_types() {
    if(editor_) {
        editor_->register_serializables<position, rotation, scale, mesh_component, camera, mesh_table, Extension_Serializer_Ts...>();
    }

    scene_manager_->get_serializer().register_serializer(std::make_unique<extension_serializer<Extension_Serializer_Ts...>>());
}
