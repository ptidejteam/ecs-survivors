#include "game.h"


int main() {
#ifdef EMSCRIPTEN
    const int screenWidth = 1280;
    const int screenHeight = 607;
#else
    const int screenWidth = 1920;
    const int screenHeight = 1080;
#endif



#ifdef TRACY_ENABLE
    std::printf("JOE");
#endif

    // TracyNoop;
    Game("ECS-Survivors", screenWidth, screenHeight).run();

    return 0;
}
