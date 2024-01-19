#include "transformation.h"

#include <flecs.h>

#include "src/serialization/scene_deserializer.h"

transformation::transformation(flecs::world &world) {
    world.component<position>("position");
    world.component<rotation>("rotation");
    world.component<scale>("scale");
}

