#pragma once

#include <tiny_gltf.h>
#include <flecs.h>
#include <gtest/gtest.h>

import transformation;
import serializer;

TEST(Serialization, SerializeAndUnserializeRegistry) {
    flecs::world source;
    flecs::world destination;

    scene_deserializer scene_loader;
    scene_loader.load_scene_into_registry<>
            (source, "untitled.gltf", scene_deserializer::gltf_file_type::ascii);

    scene_loader.save_scene_from_world<>(source, "test_save.gltf", scene_deserializer::gltf_file_type::ascii);
}