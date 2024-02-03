#include "scene_manager.h"

#include "scene_serializer.h"
#include "src/serialization/serializers/gltf_core_node_serializer.h"
#include "src/serialization/serializers/buffer_serializer.h"
#include "src/serialization/serializers/mesh_serializer.h"
#include "src/serialization/serializers/image_serializer.h"
#include "src/serialization/serializers/material_serializer.h"
#include "src/serialization/serializers/sampler_serializer.h"
#include "src/serialization/serializers/texture_serializer.h"

scene_manager::scene_manager(std::initializer_list<std::pair<unsigned int, std::string>> scene_filenames) {
    for(const auto& scene : scene_filenames) {
        scenes[scene.first] = scene.second;
    }
}


int scene_manager::get_current_scene_index() const {
    return loaded_scene;
}

void scene_manager::unload_scene(flecs::world& world) {
    world.reset();
}


