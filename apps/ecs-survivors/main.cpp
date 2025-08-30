#include "core/application.h"
#include "game.h"

int main() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    core::Application* ecs_survivors = new Game("ECS-Survivors", screenWidth, screenHeight);
    ecs_survivors->init();
    ecs_survivors->run();
    delete ecs_survivors;

    return 0;
}
