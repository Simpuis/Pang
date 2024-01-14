#pragma once
#include <entt/entity/registry.hpp>
#include <glm/mat4x4.hpp>

class game;

class custom_game_logic
{
public:
    static void init(game&, entt::registry& registry);
	static void update(game&, entt::registry& registry, double delta);

};