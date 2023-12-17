#include "game.h"

#include <fstream>
#include <iostream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <entt/entity/registry.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <cereal/archives/json.hpp>
#include "cereal_glm_serialization.h"
#include "transform.h"

#include "custom_game_logic.h"

game::game(int width, int height, const std::string& title)
{
	init_glfw_window(width, height, title);
	input_ = std::make_unique<input_handler>(input_handler());

	registry_ = entt::registry();
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

	custom_game_logic::init(registry_);

	double lastFrameTime = glfwGetTime();
	while (!glfwWindowShouldClose(window_)) {
	    entt::registry test_registry;

		auto ent = test_registry.create();
		test_registry.emplace<transform>(ent, transform(glm::mat4(1)));

		std::stringstream storage;

		{
			// output finishes flushing when it goes out of scope
			cereal::JSONOutputArchive output(storage);

			entt::snapshot{test_registry}.get<transform>(output);
		}

		/*std::ofstream out_file;
		out_file.open("test");
		out_file << storage.rdbuf();*/

		cereal::JSONInputArchive input(storage);

		entt::registry test_input_registry;
		entt::snapshot_loader{test_input_registry}.get<transform>(input);

		assert(test_input_registry.get<transform>(ent).transform_matrix == glm::mat4(1), "test1 is not 0");

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		editor_.update(io, registry_);

		const double currentFrameTime = glfwGetTime();
		const double delta = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		input_->process_input(window_);

		custom_game_logic::update(registry_, delta);

		glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer_.render_scene(material_lookup_registry_, registry_, window_);

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

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
}

void game::exit()
{
	glfwSetWindowShouldClose(window_, true);
}
