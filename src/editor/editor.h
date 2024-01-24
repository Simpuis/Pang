#pragma once

#include <imgui.h>
#include <flecs.h>

class editor
{
public:
	void update(ImGuiIO& imgui_io, flecs::world& world);
};

