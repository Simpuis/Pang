#pragma once

#include <string>
#include <memory>
#include <entt/entity/registry.hpp>
#include <flecs.h>

#include "editor/editor.h"
#include "input_handler.h"
#include "render/renderer.h"
#include "render/camera.h"
#include "render/freefly_camera.h"
#include "src/serialization/scene_deserializer.h"

class scene_deserializer;

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

    template<typename T, typename Deserializer_T>
    void import_deserializable_module(Deserializer_T& deserializer);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	std::unique_ptr<input_handler> input_;

    flecs::world world_;
	renderer renderer_;
	editor editor_;
};

template<typename T, typename... Args>
scene_deserializer<Args...> game::import_deserializable_module() {
    world_.import<T>();
    T::template register_deserializers<Deserializer_T>(deserializer);
}
