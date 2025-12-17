#include "core/application.h"
#include "game.h"

int main() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    auto ecs_survivors = Game("ECS-Survivors", screenWidth, screenHeight);
    ecs_survivors.init();
    ecs_survivors.run();

    return 0;
}
