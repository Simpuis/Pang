#include "context_builder.h"

#include "game.h"

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

std::unique_ptr<game> context_builder::build() {
    auto context = std::make_unique<game>(window_width_, window_height_, window_title_);

    if(is_editor_) {
        context->setup_editor();
    }

    return context;
}
