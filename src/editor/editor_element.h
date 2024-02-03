#pragma once

namespace flecs {
    struct world;
}

class editor_element {
public:
    virtual void tick(flecs::world& world) = 0;
};
