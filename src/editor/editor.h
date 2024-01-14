#pragma once

#include <imgui.h>
#include <entt/entity/registry.hpp>

class editor
{
public:
	void update(ImGuiIO& imgui_io, entt::registry& registry);
};

