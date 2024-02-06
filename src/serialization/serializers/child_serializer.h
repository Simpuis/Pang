#pragma once


#include "serializer.h"

class child_serializer : public serializer {
    void save(serializer_save_data& save_data) override;
    void load(serializer_load_data& load_data) override;
};
