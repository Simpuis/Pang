#pragma once

#include "src/serialization/scene_serializer.h"

class texture;

struct texture_table {
    std::vector<std::shared_ptr<texture>> table;
};

class texture_serializer {
public:
    static void save(serializer_save_data& save_data);
    static void load(serializer_load_data& load_data);
};
