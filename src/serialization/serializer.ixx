module;

#include <tiny_gltf.h>
#include <flecs.h>
#include <memory>
#include <iostream>
#include <map>

export module serializer;

import transformation;
import mesh;
import material;

/**
 * @brief The scene_deserializer class is responsible for deserializing the scene from a gltf file.
 * Entity components that can be deserialized must be registered using register_core_type or register_extension_type
 * before calling load_scene_into_registry.
 */

export class scene_deserializer {
public:
    /**
     * @brief The gltf_file_type enum is used to specify the type of file to load
     * ascii: A gltf file in json format, .gltf
     * binary: A gltf file in binary format, .glb
     */
    enum gltf_file_type { ascii, binary };

public:
    /**
     * @brief Construct a new scene_deserializer object
     *
     * @param file_type The type of file to load
     * @param filename The filename to load the scene from
     */
    scene_deserializer() = default;

public:
    /**
     * @brief Register a core component type for deserialization. The component must have a static deserialize function
     * that takes a deserialization_data object as its only parameter and constructs its component on the entity.
     * This function is called for every entity in the scene, and should be used for non-extension types.
     *
     * @tparam T The type of component to register
     * @return scene_deserializer& A reference to this object, to chain expressions
     */

    template<typename T, typename U>
        requires std::convertible_to<T, U> && std::convertible_to<U, T>
    void operator()(T& node_serializable, U& component_serializable) const {
        write_mode ? node_serializable = (T)component_serializable : component_serializable = (U)node_serializable;
    }

    template<typename... Serializable_Extension_Ts>
    void load_scene_into_registry(flecs::world& world, const std::string& filename, gltf_file_type file_type) {
        write_mode = false;
        tinygltf::Model model;
        if(!load_scene_file(model, filename, file_type)) return;

        for(auto& node : model.nodes) {
            auto entity = world.entity(node.name.c_str());

            if constexpr(sizeof...(Serializable_Extension_Ts) > 0) {
                serialize_extension<Serializable_Extension_Ts...>(entity, node);
            }

            if(!node.translation.empty()) {
                serialize_type<position>(entity, node);
            }

            if(!node.rotation.empty()) {
                serialize_type<rotation>(entity, node);
            }

            if(!node.scale.empty()) {
                serialize_type<scale>(entity, node);
            }

            if(node.mesh >= 0) {
                mesh::deserialize(model, node, material_lookup, entity);
            }
        }
    }

    template<typename T>
    void serialize_type(flecs::entity& entity, tinygltf::Node& node) {
        T component_serializable;

        serialize<scene_deserializer>(*this, node, component_serializable);

        entity.set<T>(component_serializable);
    }

    template<typename T, typename... Serializable_Ts>
    void serialize_extension(flecs::entity& entity, tinygltf::Node& node) {
        T component_serializable;

        serialize<scene_deserializer>(*this, node, component_serializable);

        entity.set<T>(component_serializable);

        if constexpr(sizeof...(Serializable_Ts) > 0) {
            serialize_type<Serializable_Ts...>(entity, node);
        }
    }

private:
    static bool load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type);
    bool write_mode = false;

    std::map<unsigned int, std::shared_ptr<material>> material_lookup;
};

bool scene_deserializer::load_scene_file(tinygltf::Model& model, const std::string& filename, gltf_file_type file_type) {
    tinygltf::TinyGLTF loader;
    std::string error_message;
    std::string warning_message;

    bool result = false;
    if(file_type == gltf_file_type::ascii) {
        result = loader.LoadASCIIFromFile(&model, &error_message, &warning_message, filename);
    }
    else if(file_type == gltf_file_type::binary) {
        result = loader.LoadBinaryFromFile(&model, &error_message, &warning_message, filename);
    }

    if(!error_message.empty()) {
        std::cout << "TinyGLTF Error loading scene file " << std::endl << "Error Message: " << error_message;
    }

    if(!warning_message.empty()) {
        std::cout << "TinyGLTF Warning loading scene file " << std::endl << "Warning Message: " << warning_message;
    }

    return result;
}
