#pragma once

#include <map>
#include <gsl/gsl>
#include <tiny_gltf.h>
#include <flecs.h>

#include "serializer.h"

class gltf_core_node_serializer : public serializer{
public:
    void save(serializer_save_data& save_data) override;
    void load(serializer_load_data& load_data) override;
};
