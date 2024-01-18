#include "game.h"

#include <iostream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "custom_game_logic.h"
#include "gl_debug.h"
#include "src/serialization/scene_deserializer.h"
#include "src/modules/transformation/transformation.h"
#include "src/render/mesh.h"

game::game(int width, int height, const std::string& title)
{
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

    main_camera.init(*this);
	custom_game_logic::init(*this, world_);

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

		custom_game_logic::update(*this, world_, delta);
        main_camera.tick(*this, delta);

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

    world_.set<scene_deserializer>({});
    auto deserializer = world_.get_mut<scene_deserializer>();

    world_.import<transformation>();

    deserializer->register_core_type<mesh>()
                .load_scene_into_registry(world_, "untitled.gltf", scene_deserializer::gltf_file_type::ascii);
}

