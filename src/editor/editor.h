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
        inspector_element = std::make_unique<inspector<Serializable_Ts...>>();
    }

	void update(ImGuiIO& imgui_io, flecs::world& world);

private:
    std::unique_ptr<file_menu> file_menu_element;
    std::unique_ptr<editor_element> inspector_element;
    std::unique_ptr<editor_element> hierarchy;
    std::unique_ptr<transformation_gizmo> gizmo;

    shared_editor_state shared_state;
};
