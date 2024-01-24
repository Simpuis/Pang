#include <memory>

import game;

int main() {
    auto game_context = std::make_unique<game>(2560, 1440, "Pong");
	game_context->loop();

	return 0;
}
