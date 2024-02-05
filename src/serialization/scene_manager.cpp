#include "scene_manager.h"

#include "scene_serializer.h"
#include "src/serialization/serializers/gltf_core_node_serializer.h"
#include "src/serialization/serializers/buffer_serializer.h"
#include "src/serialization/serializers/mesh_serializer.h"
#include "src/serialization/serializers/image_serializer.h"
#include "src/serialization/serializers/material_serializer.h"
#include "src/serialization/serializers/sampler_serializer.h"
#include "src/serialization/serializers/texture_serializer.h"
#include "src/flecs_modules/freefly_camera/freefly.h"

scene_manager::scene_manager(std::initializer_list<std::pair<unsigned int, std::string>> scene_filenames) {
    for(const auto& scene : scene_filenames) {
        scenes[scene.first] = scene.second;
    }
}

int scene_manager::get_current_scene_index() const {
    return loaded_scene;
}

void scene_manager::unload_scene() {
    main_world.reset();
}

void scene_manager::update() {
    if(main_world.has<scene_swap_command>()) {
        const auto* swap_command = main_world.get<scene_swap_command>();
        if(std::holds_alternative<unsigned int>(swap_command->scene)) {
            load_scene(std::get<unsigned int>(swap_command->scene));
        }
        else if(std::holds_alternative<std::string>(swap_command->scene)) {
            load_scene(std::get<std::string>(swap_command->scene));
        }
    }
    if(main_world.has<scene_save_command>()) {
        const auto* save_command = main_world.get<scene_save_command>();
        if(save_command->save_path) {
            serializer.save_scene_to_file(main_world, save_command->save_path.value());
        }
        else {
            serializer.save_scene_to_file(main_world, scenes[loaded_scene]);
        }
    }
    if(main_world.has<new_scene_command>()) {
        load_scene("default.gltf");
    }
}

void scene_manager::load_scene(unsigned int scene_index) {
    load_scene(scenes[scene_index]);
}

void scene_manager::load_scene(std::string scene_filename) {
    unload_scene();

    serializer.load_scene_from_file(main_world, scene_filename);
    main_world.set<freefly_camera>({});
}


