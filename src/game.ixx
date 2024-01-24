module;

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
#include "gl_debug.h"

export module game;

import transformation;
import serializer;
import freefly_camera;
import renderer;

/**
 * @brief The game class is the main class of the engine.
 * It handles setting up the glfw window and contains the main loop.
 * It also owns and manages the ECS registry.
 */
export class game
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
    scene_deserializer scene_loader;
    flecs::world world_;
	renderer renderer_;
	editor editor_;
};

game::game(int width, int height, const std::string& title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    init_glfw_window(width, height, title);
    input_ = std::make_unique<input_handler>(input_handler());

    setup_world();
}

game::~game()
{
    glfwTerminate();
}

void game::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

void game::loop()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init();

    main_camera.init(window_);

    double lastFrameTime = glfwGetTime();
    while (!glfwWindowShouldClose(window_)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        editor_.update(io, world_);

        const double currentFrameTime = glfwGetTime();
        const double delta = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        input_->process_input(window_);

        main_camera.tick(window_, delta);

        glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer_.render_scene(main_camera, world_, window_);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void game::init_glfw_window(const int width, const int height, const std::string& title)
{
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window_ == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        exit();
    }
    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit();
    }

    int flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
}

void game::exit()
{
    glfwSetWindowShouldClose(window_, true);
}

void game::setup_world() {
    world_ = flecs::world();

    world_.import<transformation>();

    scene_loader.load_scene_into_registry<>
            (world_, "untitled.gltf", scene_deserializer::gltf_file_type::ascii);
}
