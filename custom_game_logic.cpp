#include "custom_game_logic.h"

#include <glm/fwd.hpp>

#include <glm/ext/matrix_transform.hpp>

#include "material_component.h"
#include "sprite.h"
#include "transform.h"
#include "material.h"
#include "texture_loader.h"
#include "shader_loader.h"
#include "scene_serializer.h"
#include "name.h"
#include "scene_deserializer.h"
#include "mesh.h"

void custom_game_logic::init(entt::registry& registry)
{
    scene_deserializer input = scene_deserializer(scene_deserializer::gltf_file_type::ascii, "untitled.gltf");
    input.register_core_type<transform>()
            .register_core_type<name>()
            .register_core_type<mesh>()
            .load_scene_into_registry(registry);

}

void custom_game_logic::update(entt::registry& registry, double delta)
{
    auto view = registry.view<transform>();
    for(auto entity : view) {
        auto& trans = registry.get<transform>(entity);
        trans.transform_matrix = glm::rotate(trans.transform_matrix, glm::radians(20.0f) * (float)delta, glm::vec3(1.0f, 0.7f, 0.2f));
    }
}
