#pragma once

#include <tiny_gltf.h>

#include "serializer.h"
#include "src/editor/inspector.h"

inline void assign_component(int &value, const tinygltf::Value &gltf_value, serializer_load_data &load_data) {
    value = gltf_value.Get<int>();
}

inline void assign_component(bool &value, const tinygltf::Value &gltf_value, serializer_load_data &load_data) {
    value = gltf_value.Get<bool>();
}

inline void assign_component(float &value, const tinygltf::Value &gltf_value, serializer_load_data &load_data) {
    value = gltf_value.Get<double>();
}

inline void assign_component(double &value, const tinygltf::Value &gltf_value, serializer_load_data &load_data) {
    value = gltf_value.Get<double>();
}

inline void assign_component(std::string &value, const tinygltf::Value &gltf_value, serializer_load_data &load_data) {
    value = gltf_value.Get<std::string>();
}

template<typename T>
requires is_enum<T>
inline void assign_component(T &value, const tinygltf::Value &gltf_value, serializer_load_data &load_data) {
    value = static_cast<T>(gltf_value.Get<int>());
}

inline void assign_component(flecs::entity &value, const tinygltf::Value &gltf_value, serializer_load_data &load_data) {
    value = load_data.node_entity_map[gltf_value.Get<int>()];
}

inline void assign_component(tinygltf::Value &gltf_value, const int &value, serializer_save_data &save_data) {
    gltf_value = tinygltf::Value(value);
}

inline void assign_component(tinygltf::Value &gltf_value, const bool &value, serializer_save_data &save_data) {
    gltf_value = tinygltf::Value(value);
}

inline void assign_component(tinygltf::Value &gltf_value, const float &value, serializer_save_data &save_data) {
    gltf_value = tinygltf::Value(value);
}

inline void assign_component(tinygltf::Value &gltf_value, const double &value, serializer_save_data &save_data) {
    gltf_value = tinygltf::Value(value);
}

inline void assign_component(tinygltf::Value &gltf_value, const std::string &value, serializer_save_data &save_data) {
    gltf_value = tinygltf::Value(value);
}

template<typename T>
requires is_enum<T>
inline void assign_component(tinygltf::Value &gltf_value, const T &value, serializer_save_data &save_data) {
    gltf_value = tinygltf::Value(static_cast<int>(value));
}

inline void assign_component(tinygltf::Value &gltf_value, const flecs::entity &value, serializer_save_data &save_data) {
    gltf_value = tinygltf::Value(static_cast<int>(save_data.entity_node_map[value]));
}