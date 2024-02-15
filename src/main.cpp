#include "context_builder.h"
#include "game.h"

int main() {
    auto context = context_builder()
            .set_window_size(2560, 1440)
            .set_window_title("Pong")
            .set_editor(true)
            .build();

    context->setup_scenes({{0, "untitled.gltf"}, {1, "untitled1.gltf"}});
    context->register_extension_types<camera>();
    context->loop();

	return 0;
}
