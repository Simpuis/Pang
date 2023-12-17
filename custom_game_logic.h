#pragma once
#include <entt/entity/registry.hpp>

class custom_game_logic
{
public:
    static void init(entt::registry& registry);
	static void update(entt::registry& registry, double delta);
};

