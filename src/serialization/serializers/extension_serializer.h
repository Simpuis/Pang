#pragma once

#include <magic_enum.hpp>

#include "extension_serialization_functions.h"

template<typename... Extension_Ts>
class extension_serializer : public serializer {
    void save(serializer_save_data& save_data) override {
        save_types<Extension_Ts...>(save_data);
    }
    void load(serializer_load_data& load_data) override {
        load_types<Extension_Ts...>(load_data);
    }

    template<typename Head_T, typename... Tail_Ts>
    void save_types(serializer_save_data& save_data) {
        constexpr auto type = refl::reflect<Head_T>();
        save_data.world.each<Head_T>([&](flecs::entity entity, Head_T& component) {
            tinygltf::Value::Object object = serialize_component(save_data, component);

            tinygltf::Node& gltf_node = save_data.model.nodes[save_data.entity_node_map[entity]];
            gltf_node.extensions[type.name.c_str()] = tinygltf::Value(object);
        });

        if constexpr(sizeof...(Tail_Ts) > 0) {
            save_types<Tail_Ts...>(save_data);
        }
    }

    template<typename Head_T>
    tinygltf::Value::Object serialize_component(serializer_save_data &save_data, const Head_T &component) const {
        constexpr auto type = refl::reflect<Head_T>();
        std::map<std::string, tinygltf::Value> object;

        for_each(type.members, [&](auto member) {
            if constexpr(is_readable(member) && refl::descriptor::has_attribute<serializable>(member)) {
                std::pair<std::string, tinygltf::Value> pair;
                pair.first = get_display_name(member);
                assign_component(pair.second, member(component), save_data);
                object.insert(pair);
            }
        });

        return object;
    }

    template<typename Head_T, typename... Tail_Ts>
    void load_types(serializer_load_data& load_data) {
        constexpr auto type = refl::reflect<Head_T>();
        for(gsl::index i = 0; i < load_data.model.nodes.size(); i++) {
            if(load_data.model.nodes[i].extensions.find(type.name.c_str()) != load_data.model.nodes[i].extensions.end()) {
                const tinygltf::Value& object = load_data.model.nodes[i].extensions.at(type.name.c_str());

                Head_T component_instance = deserialize_component<Head_T>(load_data, object);

                load_data.node_entity_map[i].set<Head_T>(component_instance);
            }
        }

        if constexpr(sizeof...(Tail_Ts) > 0) {
            load_types<Tail_Ts...>(load_data);
        }
    }

    template<typename Head_T>
    Head_T deserialize_component(serializer_load_data &load_data, const tinygltf::Value &object) const {
        constexpr auto type = refl::reflect<Head_T>();
        Head_T component_instance;

        for_each(type.members, [&](auto member) {
            if constexpr(is_readable(member) && refl::descriptor::has_attribute<serializable>(member)) {
                assign_component(member(component_instance), object.Get(get_display_name(member)), load_data);
            }
        });

        return component_instance;
    }
};
