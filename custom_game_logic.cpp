#include "custom_game_logic.h"

#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "material_component.h"
#include "sprite.h"
#include "transform.h"
#include "material.h"
#include "texture_loader.h"
#include "shader_loader.h"

void custom_game_logic::init(entt::registry& registry)
{
	auto entity = registry.create();
	registry.emplace<transform>(entity, transform(glm::mat4(1)));
	registry.emplace<sprite>(entity, sprite());
	material_component& mat = registry.emplace<material_component>(entity, material_component(0));
	mat.material = std::make_shared<material>();
	mat.material->material_shader = std::unique_ptr<shader>(shader_loader::load_shader("vertex_shader.glsl", "fragment_shader.glsl"));
	mat.material->set_texture("texture1", "wall.jpg");
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
