#pragma once

#include <memory>
#include <flecs.h>

#include "src/render/mesh.h"
#include "src/render/material.h"
#include "src/render/texture.h"
#include "src/serialization/serializable.h"

struct mesh_component {
    mesh_component() = default;
    explicit mesh_component(unsigned int mesh) : mesh(mesh) {}

    unsigned int mesh;
};

REFL_TYPE(mesh_component, bases<>)
        REFL_FIELD(mesh, serializable())
REFL_END

struct mesh_table {
    std::vector<std::shared_ptr<mesh>> table;
};

REFL_TYPE(mesh_table, bases<>)
    REFL_FIELD(table, serializable())
REFL_END

struct material_table {
    std::vector<std::shared_ptr<material>> table;
};

struct texture_table {
    std::vector<std::shared_ptr<texture>> table;
};

struct rendering {
    explicit rendering(flecs::world& world);
};