#pragma once

#include <tiny_gltf.h>

#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <transform.h>
#include <name.h>
#include <scene_serializer.h>
#include <scene_deserializer.h>

TEST(Serialization, SerializeAndUnserializeRegistry) {
    entt::registry source;
    entt::registry destination;

    auto e0 = source.create();
    source.emplace<transform>(e0);
    source.emplace<name>(e0, "Poop");

    auto e1 = source.create();
    source.emplace<name>(e1, "Dinner");

    {
        scene_serializer output = scene_serializer("test.gltf");

        entt::snapshot{source}
                .get<entt::entity>(output)
                .get<transform>(output)
                .get<name>(output);
    }

    scene_deserializer input = scene_deserializer(scene_deserializer::gltf_file_type::ascii, "test.gltf");
    input.register_core_type<transform>()
            .register_core_type<name>()
            .load_scene_into_registry(destination);

    ASSERT_TRUE(destination.valid(e0));
    ASSERT_TRUE(destination.all_of<transform>(e0));
    ASSERT_TRUE(destination.all_of<name>(e0));
    ASSERT_TRUE(destination.get<name>(e0).name_string == "Poop");
    ASSERT_TRUE(destination.valid(e1));
    ASSERT_TRUE(destination.all_of<name>(e1));
    ASSERT_TRUE(destination.get<name>(e1).name_string == "Dinner");
}