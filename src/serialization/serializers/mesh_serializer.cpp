#include "mesh_serializer.h"

#include "src/render/mesh.h"
#include "src/flecs_modules/rendering/rendering.h"
#include "buffer_serializer.h"
#include "material_serializer.h"

class material;

void mesh_serializer::save(serializer_save_data& save_data) {
    if(!save_data.world.has<mesh_table>()) return;
    auto* meshes = save_data.world.get<mesh_table>();

    for(const auto& unserialized_mesh : meshes->table) {
        save_data.model.meshes.push_back(unserialized_mesh->serialize());
    }
}

void mesh_serializer::load(serializer_load_data& load_data) {
    load_data.world.set<mesh_table>(mesh_table());
    auto* meshes = load_data.world.get_mut<mesh_table>();

    for(const auto& mesh : load_data.model.meshes) {
        meshes->table.push_back(mesh::deserialize(load_data.model, mesh));
    }
}
