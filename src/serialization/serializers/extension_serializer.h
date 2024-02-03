#pragma once

template<typename... Args>
class extension_serializer : public serializer {
    void save(serializer_save_data& save_data) override;
    void load(serializer_load_data& load_data) override;
};

template<typename... Args>
void extension_serializer<Args...>::save(serializer_save_data &save_data) {

}

template<typename... Args>
void extension_serializer<Args...>::load(serializer_load_data &load_data) {

}
