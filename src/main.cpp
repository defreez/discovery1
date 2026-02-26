#include "core/game.hpp"
#include "platform/renderer.hpp"
#include "platform/input.hpp"

int main() {
    platform::Renderer renderer;
    renderer.init(1280, 720, "Discovery One");

    core::GameState game = core::make_game("maps/test_corridor.json");

    while (!renderer.should_close()) {
        core::PlayerInput input = platform::read_input();
        float dt = GetFrameTime();

        game = core::update_game(game, input, dt);

        renderer.draw(game);
    }

    renderer.shutdown();
    return 0;
}
