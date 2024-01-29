#pragma once

#include <flecs.h>

struct mesh_component {
    mesh_component() = default;
    explicit mesh_component(unsigned int mesh) : mesh(mesh) {}

    unsigned int mesh;
};

struct rendering {
    explicit rendering(flecs::world& world);
};