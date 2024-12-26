#include <iostream>

#include "game.h"

int main() {
    const int screenWidth = 960;
    const int screenHeight = 450;

    std::cout << "Hello" << std::endl;
    Game game = Game("ECS-Survivors", screenWidth, screenHeight);
    game.run();

    return 0;
}
