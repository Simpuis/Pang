#pragma once

#include <optional>

namespace flecs {
    struct world;
}

struct shared_editor_state {
    std::optional<flecs::entity> selected_entity;
};

class editor_element {
public:
    virtual void tick(flecs::world& world, shared_editor_state& shared_state) = 0;
};
