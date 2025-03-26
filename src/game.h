//
// Created by Laurent Voisard on 12/20/2024.
//

#ifndef GAME_H
#define GAME_H

#include <string>

#include "flecs.h"
#include "modules/engine/physics/physics_module.h"

struct EventBus {
    std::string name;
};

class Game {
public:
    Game(const char* windowName, int windowWidth, int windowHeight);
    void run();
    void set_collision_strategy(PHYSICS_COLLISION_STRATEGY strategy) {m_strategy = strategy;}
    void set_rep(int rep) {this->rep = rep;}

private:
    flecs::world m_world;
    std::string m_windowName;
    int m_windowHeight;
    int m_windowWidth;
    int rep;
    PHYSICS_COLLISION_STRATEGY m_strategy;
};



#endif //GAME_H
