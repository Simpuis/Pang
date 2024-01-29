#include "rendering.h"

rendering::rendering(flecs::world& world) {
    world.component<mesh_component>()
            .member<unsigned int>("mesh");
}
