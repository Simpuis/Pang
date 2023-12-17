#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <entt/entity/registry.hpp>

class editor
{
public:
	void update(ImGuiIO& imgui_io, entt::registry& registry);
};

