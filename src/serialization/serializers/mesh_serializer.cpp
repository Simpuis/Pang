#include "mesh_serializer.h"

#include "src/render/mesh.h"
#include "src/flecs_modules/rendering/rendering.h"

class material;

void mesh_serializer::save(const flecs::world &world, tinygltf::Model &model,
                           std::map<const flecs::entity, gsl::index> &entity_node_map) {

}

void mesh_serializer::load(flecs::world& world, const tinygltf::Model& model,
                           std::map<gsl::index, flecs::entity>& node_entity_map) {
    if(!world.has<mesh_table>()) {
        auto meshes = mesh_table();
        world.set<mesh_table>(meshes);
    }
    auto* meshes = world.get_mut<mesh_table>();

    std::map<unsigned int, std::shared_ptr<material>> material_lookup;
    unsigned int previous_table_size = meshes->table.size();
    for(auto pair : node_entity_map) {
        unsigned int relative_mesh_index = previous_table_size + model.nodes[pair.first].mesh;
        if(model.nodes[pair.first].mesh <= 0) continue;

        if(meshes->table.find(relative_mesh_index) == meshes->table.end()) {
            meshes->table.insert({relative_mesh_index,
                                 mesh::deserialize(model, model.meshes[model.nodes[pair.first].mesh], material_lookup)});
        }

        pair.second.set<mesh_component>(mesh_component(relative_mesh_index));
    }
}
