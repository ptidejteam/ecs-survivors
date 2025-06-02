#include <iostream>

#include "game.h"

int main() {
#ifdef EMSCRIPTEN
    const int screenWidth = 1440;
    const int screenHeight = 810;
#else
    const int screenWidth = 1920;
    const int screenHeight = 1080;
#endif
    //tmx::Map map;
    // if(map.load("../resources/tiled/maps/sampleMap.tmx")) {
    //     std::cout << "Map loaded" << std::endl;
    // }
    Game("ECS-Survivors", screenWidth, screenHeight).run();

    return 0;
}
