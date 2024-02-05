#pragma once

#include <map>
#include <gsl/gsl>
#include <memory>
#include <tiny_gltf.h>
#include <flecs.h>

#include "serializer.h"

class material_serializer : public serializer {
public:
    void save(serializer_save_data& save_data) override;
    void load(serializer_load_data& load_data) override;
};
