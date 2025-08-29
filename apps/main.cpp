#include "ecs-survivors/game.h"
int main() {
#ifdef EMSCRIPTEN
    const int screenWidth = 1920;
    const int screenHeight = 1080;
#else
    const int screenWidth = 1920;
    const int screenHeight = 1080;
#endif
    Game("ECS-Survivors", screenWidth, screenHeight).run();

 return 0;
}
