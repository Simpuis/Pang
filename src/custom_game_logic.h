#pragma once
#include <entt/entity/registry.hpp>
#include <glm/mat4x4.hpp>
#include <flecs.h>

class game;

class custom_game_logic
{
public:
    static void init(game&, flecs::world& world);
	static void update(game&, flecs::world& world, double delta);

};