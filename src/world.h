#pragma once

#include <flecs.h>

class world {
public:
    template<typename Head_Module_T, typename... Tail_Module_Ts>
    world import_modules();

public:
    flecs::world& get_mut_ecs() { return ecs; };
    const flecs::world& get_ecs() { return ecs; }

private:
    flecs::world ecs;
};

template<typename Head_Module_T, typename... Tail_Module_Ts>
world world::import_modules() {
    ecs.import<Head_Module_T>();

    if constexpr(sizeof...(Tail_Module_Ts) > 0) {
        import_modules<Tail_Module_Ts...>();
    }
}
