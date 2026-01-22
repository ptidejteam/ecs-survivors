#include "core/application.h"
#include "game.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    auto ecs_survivors = Game("ECS-Survivors", screenWidth, screenHeight);
    ecs_survivors.init();
    ecs_survivors.run();

    return 0;
}
