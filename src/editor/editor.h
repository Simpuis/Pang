#pragma once

#include <imgui.h>
#include <flecs.h>
#include <memory>
#include <vector>

#include "editor_element.h"
#include "inspector.h"

class editor
{
public:

    template<typename... Serializable_Ts>
    void register_serializables() {
        inspector_element = std::make_unique<inspector<Serializable_Ts...>>();
    }

	void update(ImGuiIO& imgui_io, flecs::world& world);

private:
    std::unique_ptr<editor_element> inspector_element;
};
