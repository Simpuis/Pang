module;

#include <string>
#include <memory>

export module context_builder;

import game;

export class context_builder {
public:
    context_builder() = default;

public:
    context_builder& set_window_size(unsigned int width, unsigned int height);
    context_builder& set_window_title(const std::string& title);
    context_builder& set_editor(bool is_editor);

    context_builder& launch();

private:
    unsigned int window_width_;
    unsigned int window_height_;
    std::string window_title_;
    bool is_editor_ = false;
};

context_builder& context_builder::set_window_size(unsigned int width, unsigned int height) {
    window_width_ = width;
    window_height_ = height;

    return *this;
}

context_builder& context_builder::set_window_title(const std::string &title) {
    window_title_ = title;

    return *this;
}

context_builder &context_builder::set_editor(bool is_editor) {
    is_editor_ = is_editor;

    return *this;
}

context_builder &context_builder::launch() {
    auto context = std::make_unique<game>(window_width_, window_height_, window_title_);
    context->loop();

    return *this;
}
