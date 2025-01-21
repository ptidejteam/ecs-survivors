#include <iostream>

#include "game.h"

int main() {
    const int screenWidth = 960;
    const int screenHeight = 450;

    Game("ECS-Survivors", screenWidth, screenHeight).run();

    return 0;
}
