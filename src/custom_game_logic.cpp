#include "custom_game_logic.h"

#include <glm/fwd.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

#include "components/material_component.h"
#include "components/transform.h"
#include "render/material.h"
#include "serialization/shader_loader.h"
#include "serialization/scene_serializer.h"
#include "serialization/scene_deserializer.h"
#include "render/mesh.h"
#include "game.h"

void custom_game_logic::init(game& context, flecs::world& world)
{
    scene_deserializer input = scene_deserializer(scene_deserializer::gltf_file_type::ascii, "untitled.gltf");
    input.register_core_type<transform>()
            .register_core_type<mesh>()
            .load_scene_into_registry(world);

    glm::mat4x4 cam_matrix = glm::mat4x4(1.0f);
}

void custom_game_logic::update(game& context, flecs::world& world, double delta)
{
    /*
    auto view = registry.view<transform>();
    for(auto entity : view) {
        auto& trans = registry.get<transform>(entity);
        trans.transform_matrix = glm::rotate(trans.transform_matrix, glm::radians(20.0f) * (float)delta, glm::vec3(1.0f, 0.7f, 0.2f));
    }*/

}
