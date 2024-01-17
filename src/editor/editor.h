#pragma once

#include <imgui.h>
#include <entt/entity/registry.hpp>
#include <flecs.h>

class editor
{
public:
	void update(ImGuiIO& imgui_io, flecs::world& world);
};

