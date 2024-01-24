#include <memory>

import context_builder;

int main() {
    auto context = context_builder()
                    .set_window_size(2560, 1440)
                    .set_window_title("Pong")
                    .set_editor(true)
                    .launch();

	return 0;
}
