#pragma once

#include <string>
#include <memory>

class game;

class context_builder {
public:
    context_builder() = default;

public:
    context_builder& set_window_size(unsigned int width, unsigned int height);
    context_builder& set_window_title(const std::string& title);
    context_builder& set_editor(bool is_editor);

    std::unique_ptr<game> build();

private:
    unsigned int window_width_;
    unsigned int window_height_;
    std::string window_title_;
    bool is_editor_ = false;
};
