#pragma once

#include "serializer.h"

class texture_serializer : public serializer {
public:
    void save(serializer_save_data& save_data) override;
    void load(serializer_load_data& load_data) override;
};
