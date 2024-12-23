//
// Created by Laurent Voisard on 12/20/2024.
//

#ifndef GAME_H
#define GAME_H

#include <string>

#include "flecs.h"

class Game {
public:
    Game(const char* windowName, int windowWidth, int windowHeight);
    void run();

    static void UpdateDrawFrame();

private:
    flecs::world m_world;
    std::string m_windowName;
    int m_windowHeight;
    int m_windowWidth;
};



#endif //GAME_H
