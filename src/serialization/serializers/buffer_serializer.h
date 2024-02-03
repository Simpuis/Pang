#pragma once

#include <vector>
#include <map>
#include <memory>
#include <tiny_gltf.h>

#include "serializer.h"

struct scene_buffer_data {
    std::vector<tinygltf::Accessor> accessors;
    std::vector<tinygltf::BufferView> buffer_views;
    std::vector<tinygltf::Buffer> buffers;
};

class buffer_serializer : public serializer{
public:
    void save(serializer_save_data& save_data) override;
    void load(serializer_load_data& load_data) override;
};
