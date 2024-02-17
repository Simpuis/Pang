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

REFL_TYPE(mesh_table, bases<>, singleton_component())
    REFL_FIELD(table, serializable())
REFL_END

struct material_table {
    std::vector<std::shared_ptr<material>> table;
};

REFL_TYPE(material_table, bases<>, singleton_component())
        REFL_FIELD(table, serializable())
REFL_END

struct texture_table {
    std::vector<std::shared_ptr<texture>> table;
};

REFL_TYPE(texture_table, bases<>, singleton_component())
        REFL_FIELD(table, serializable())
REFL_END

struct camera {
    enum class projection { perspective, orthographic };

    projection projection_type = projection::perspective;
    float field_of_view_degrees = 90.0f;
    float near_plane_z = 0.1f;
    float far_plane_z = 100.0f;
};

REFL_TYPE(camera, bases<>)
    REFL_FIELD(projection_type, serializable())
    REFL_FIELD(field_of_view_degrees, serializable())
    REFL_FIELD(near_plane_z, serializable())
    REFL_FIELD(far_plane_z, serializable())
REFL_END

struct main_camera {
    flecs::entity camera_entity;
};

REFL_TYPE(main_camera, bases<>, singleton_component())
    REFL_FIELD(camera_entity, serializable())
REFL_END

struct render_debug_camera {
    std::optional<flecs::entity> debug_camera;
};

struct rendering {
    explicit rendering(flecs::world& world);
};