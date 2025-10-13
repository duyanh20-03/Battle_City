#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;
    if (game.isRunning()) {
        game.run();
    }
    return 0;
}
