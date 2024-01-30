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

    scene_serializer scene_loader;
    scene_loader.load_scene_into_registry<>
            (source, "untitled.gltf", scene_serializer::gltf_file_type::ascii, meshes);
    scene_loader.save_scene_from_world<>(source, "test_save.gltf", scene_serializer::gltf_file_type::ascii);
}