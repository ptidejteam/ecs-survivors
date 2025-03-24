#include <format>

#include "game.h"


int main() {
#ifdef EMSCRIPTEN
    const int screenWidth = 1280;
    const int screenHeight = 607;
#else
    const int screenWidth = 1920;
    const int screenHeight = 1080;
#endif


    std::string titles[6] =  {
        "baseline-no-physics",
        "collision-relationship",
        "collision-entity",
        "spatial-hash-rebuilding",
        "box2d"
    };
    for (int strategy = 0; strategy < 5; strategy++) {
        for (int i = 0; i < 1; i++) {
            auto g = Game(std::format("{}-{}",titles[strategy], i).c_str(), screenWidth, screenHeight);
            g.set_collision_strategy(static_cast<PHYSICS_COLLISION_STRATEGY>(strategy));
            g.run();
        }
    }

    return 0;
}
