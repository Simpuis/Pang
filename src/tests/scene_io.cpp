#pragma once

#include <tiny_gltf.h>
#include <flecs.h>
#include <gtest/gtest.h>

#include "src/serialization/scene_serializer.h"

TEST(Serialization, SerializeAndUnserializeRegistry) {
    flecs::world source = flecs::world();
    flecs::world destination = flecs::world();

    source.import<transformation>();
    source.import<rendering>();

    mesh_lookup meshes;

}