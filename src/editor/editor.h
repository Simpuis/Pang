#pragma once

#include <imgui.h>
#include <flecs.h>
#include <memory>
#include <vector>

#include "editor_element.h"
#include "inspector.h"
#include "scene_hierarchy.h"
#include "file_menu.h"
#include "transformation_gizmo.h"

class editor
{
public:
    editor();

public:
    template<typename... Serializable_Ts>
    void register_serializables() {
        editor_elements.push_back(std::make_unique<inspector<Serializable_Ts...>>());
    }

	void update(ImGuiIO& imgui_io, flecs::world& world);

private:
    std::vector<std::unique_ptr<editor_element>> editor_elements;

    shared_editor_state shared_state;
};
