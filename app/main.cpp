#include "game.h"

int main() {
#ifdef EMSCRIPTEN
    const int screenWidth = 1920;
    const int screenHeight = 1080;
#else
    const int screenWidth = 1920;
    const int screenHeight = 1080;
#endif
    Game("ECS-Survivors", screenWidth, screenHeight).run();

    // flecs::world ecs = flecs::world();
    //
    // flecs::entity timer = ecs.timer().interval(1);
    // ecs.system().run([] (flecs::iter& iter) {
    //     std::cout << "normal" << std::endl;
    //     std::cout << iter.delta_time() << std::endl;
    //     std::cout << iter.delta_system_time() << std::endl;
    // });
    // ecs.system().tick_source(timer).run([] (flecs::iter& iter) {
    //     std::cout << "tick source" << std::endl;
    //     std::cout << iter.delta_time() << std::endl;
    //     std::cout << iter.delta_system_time() << std::endl;
    // });
    // ecs.set_time_scale(0);
    // //std::cout << "PRESS ENTER" << std::endl;
    // int i = 0;
    // while(i < 13) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(250));
    //     ecs.progress();
    //     i++;
    // }

    return 0;
}
