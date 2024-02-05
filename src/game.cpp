#include "game.h"

#include <nlohmann/json.hpp>

#include "gl_debug.h"
#include "src/render/material.h"
#include "src/render/texture.h"
#include "src/render/shader.h"
#include "src/flecs_modules/freefly_camera/freefly.h"

game::game(int width, int height, const std::string& title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    init_glfw_window(width, height, title);
    input_ = std::make_unique<input_handler>(input_handler());
}

game::~game()
{
    glfwTerminate();
}

void game::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

void game::loop(unsigned int startup_scene)
{
    if(scene_manager_) {
        scene_manager_->load_scene(startup_scene);

        scene_manager_->get_world().import<transformation>();
        scene_manager_->get_world().import<freefly>();
        scene_manager_->get_world().import<rendering>();
        scene_manager_->get_world().set<freefly_camera>({});
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF("InterVariable.ttf", 18);

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init();

    double lastFrameTime = glfwGetTime();
    while (!glfwWindowShouldClose(window_)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        input_->process_input(scene_manager_->get_world(), window_);

        if(editor_) {
            editor_->update(io, scene_manager_->get_world());
        }

        const double currentFrameTime = glfwGetTime();
        const double delta = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        scene_manager_->get_world().progress(delta);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        scene_manager_->update();

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

void game::setup_scenes(std::initializer_list<std::pair<unsigned int, std::string>> scene_filenames) {
    scene_manager_ = std::make_unique<scene_manager>(scene_filenames);
}

void game::setup_editor() {
    editor_ = editor();
}


