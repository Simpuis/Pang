#include "scene_serializer.h"

#include <vector>

#include "src/serialization/serializers/buffer_serializer.h"
#include "src/serialization/serializers/gltf_core_node_serializer.h"
#include "src/serialization/serializers/image_serializer.h"
#include "src/serialization/serializers/material_serializer.h"
#include "src/serialization/serializers/mesh_serializer.h"
#include "src/serialization/serializers/sampler_serializer.h"
#include "src/serialization/serializers/texture_serializer.h"
#include "src/flecs_modules/freefly_camera/freefly.h"


void scene_serializer::save_scene_to_file(flecs::world &world, const std::string &filename) {
    tinygltf::Model model;
    std::map<const flecs::entity, gsl::index> entity_node_map;
    // Position is here used to make it possible to iterate all "world objects", it should probably be replaced
    // by some sort of "serializable" component
    world.each<position>([&](flecs::entity entity, position &pos) {
        model.nodes.emplace_back();
        gsl::index node_index = model.nodes.size() - 1;
        model.nodes[node_index].name = entity.name().c_str();

        entity_node_map.insert({entity, node_index});
    });

    serializer_save_data save_data(model, entity_node_map, world);
    for(auto& s : serializers) {
       s->save(save_data);
    }

    tinygltf::TinyGLTF saver;
    saver.WriteGltfSceneToFile(&model, filename.c_str(), true, true, true, false);
}

void scene_serializer::load_scene_from_file(flecs::world &world, const std::string &filename) {
    world.import<transformation>();
    world.import<rendering>();
    world.import<freefly>();
    tinygltf::Model model;
    if (!load_scene_file(model, filename)) return;
    std::map<gsl::index, flecs::entity> node_entity_map;
    for (gsl::index i = 0; i < model.nodes.size(); i++) {
        auto entity = world.entity(model.nodes[i].name.c_str());
        node_entity_map.insert({i, entity});
    }

    serializer_load_data load_data(model, node_entity_map, world);
    for(auto& s : serializers) {
        s->load(load_data);
    }
}

void scene_serializer::register_serializer(std::unique_ptr<serializer>&& serializer_instance) {
    serializers.push_back(std::move(serializer_instance));
}

bool scene_serializer::load_scene_file(tinygltf::Model& model, const std::string& filename) {
    tinygltf::TinyGLTF loader;
    std::string error_message;
    std::string warning_message;

    std::cout << "Loading scene from file " << filename << std::endl;

    bool result = false;
    //if(file_type == gltf_file_type::ascii) {
        result = loader.LoadASCIIFromFile(&model, &error_message, &warning_message, filename);
    //}
    //else if(file_type == gltf_file_type::binary) {
    //    result = loader.LoadBinaryFromFile(&model, &error_message, &warning_message, filename);
    //}

    if(!error_message.empty()) {
        std::cout << "TinyGLTF Error loading scene file " << std::endl << "Error Message: " << error_message;
    }

    if(!warning_message.empty()) {
        std::cout << "TinyGLTF Warning loading scene file " << std::endl << "Warning Message: " << warning_message;
    }

    return result;
}

scene_serializer::scene_serializer() {
    serializers.emplace_back(std::make_unique<buffer_serializer>());
    serializers.emplace_back(std::make_unique<gltf_core_node_serializer>());
    serializers.emplace_back(std::make_unique<image_serializer>());
    serializers.emplace_back(std::make_unique<material_serializer>());
    serializers.emplace_back(std::make_unique<mesh_serializer>());
    serializers.emplace_back(std::make_unique<sampler_serializer>());
    serializers.emplace_back(std::make_unique<texture_serializer>());
}
