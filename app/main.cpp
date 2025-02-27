#include <iostream>

#include "game.h"

int main() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    Game("ECS-Survivors", screenWidth, screenHeight).run();

    return 0;
}
