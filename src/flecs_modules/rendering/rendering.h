#pragma once

#include <flecs.h>

#include "src/serialization/serializable.h"

struct mesh_component {
    mesh_component() = default;
    explicit mesh_component(unsigned int mesh) : mesh(mesh) {}

    unsigned int mesh;
};

REFL_TYPE(mesh_component, bases<>)
        REFL_FIELD(mesh, serializable())
REFL_END

struct rendering {
    explicit rendering(flecs::world& world);
};