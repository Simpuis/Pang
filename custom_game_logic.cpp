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

void custom_game_logic::init(entt::registry& registry)
{
	auto entity = registry.create();
	registry.emplace<transform>(entity, transform(glm::mat4(1)));
	registry.emplace<sprite>(entity, sprite());
	material_component& mat = registry.emplace<material_component>(entity, material_component(0));
	mat.material = std::make_shared<material>();
	mat.material->material_shader = std::unique_ptr<shader>(shader_loader::load_shader("vertex_shader.glsl", "fragment_shader.glsl"));
	mat.material->set_texture("texture1", texture_loader::load_texture("wall.jpg", GL_RGBA));

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
    input.register_transform_type<transform>()
         .register_name_type<name>()
         .load_scene_into_registry(destination);

    assert(destination.valid(e0));
    assert(destination.all_of<transform>(e0));
    assert(destination.all_of<name>(e0));
    assert(destination.get<name>(e0).name_string == "Poop");
    assert(destination.valid(e1));
    assert(destination.all_of<name>(e1));
    assert(destination.get<name>(e1).name_string == "Dinner");
}

void custom_game_logic::update(entt::registry& registry, double delta)
{
	auto transforms = registry.view<transform>();
	for(auto entity : transforms)
	{
		auto& trans = registry.get<transform>(entity);
		trans.transform_matrix = glm::translate(trans.transform_matrix, glm::vec3(0.1 * delta, 0.0f, 0.0f));
	}
}
