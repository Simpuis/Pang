#include "game.h"

#include <nlohmann/json.hpp>

#include "gl_debug.h"

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
    world_.import<rendering>();

    scene_loader.load_scene_into_registry<>
            (world_, "untitled1.gltf", scene_deserializer::gltf_file_type::ascii, renderer_.meshes);
    world_.set<freefly_camera>(main_camera);

    scene_loader.save_scene_from_world<>(world_, "test_save.gltf", scene_deserializer::gltf_file_type::ascii);
}
