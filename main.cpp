#include <iostream>
#include "game/engine.hpp"

int main(int argc, char *argv[]) {
    Engine eng;
    eng.setupInteractively();
    eng.gameLoop();
    std::cout << "Gracias por jugar.\n";
    return 0;
}
