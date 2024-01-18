#include "transformation.h"

#include <flecs.h>

#include "src/serialization/scene_deserializer.h"

transformation::transformation(flecs::world &world) {
    auto deserializer = world.get_mut<scene_deserializer>();

    world.component<position>("position");
    deserializer->register_core_deserializer<position>(position_deserializer());

    world.component<rotation>("rotation");
    deserializer->register_core_deserializer<rotation>(rotation_deserializer());

    world.component<scale>("scale");
    deserializer->register_core_deserializer<scale>(scale_deserializer());
}

std::function<void(deserialization_data &data)> transformation::position_deserializer() {
    return [] (deserialization_data &data) {
        if(data.node.translation.empty()) return;

        position position_comp;
        position_comp.pos = glm::vec3(data.node.translation[0], data.node.translation[1], data.node.translation[2]);

        data.entity.set<position>(position_comp);
    };
}

std::function<void(deserialization_data &data)> transformation::rotation_deserializer() {
    return [] (deserialization_data &data) {
        if(data.node.rotation.empty()) return;

        rotation rotation_comp;
        rotation_comp.rot = glm::quat(data.node.rotation[3], data.node.rotation[0], data.node.rotation[1], data.node.rotation[2]);

        data.entity.set<rotation>(rotation_comp);
    };
}

std::function<void(deserialization_data &data)> transformation::scale_deserializer() {
    return [] (deserialization_data &data) {
        if(data.node.scale.empty()) return;

        scale scale_comp;
        scale_comp.local_scale = glm::vec3(data.node.scale[0], data.node.scale[1], data.node.scale[2]);

        data.entity.set<scale>(scale_comp);
    };
}
