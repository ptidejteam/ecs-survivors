//
// Created by Laurent Voisard on 12/20/2024.
//

#ifndef GAME_H
#define GAME_H

#include <string>

#include "core/application.h"
#include "core/scene.h"
#include "flecs.h"
#include "game_scene.h"

class Game : public core::Application {
public:
    Game(const char* window_name, int window_width, int window_height);
    ~Game() override {delete game_scene;}

    void run() override;

private:

    void init() override;

    void UpdateDrawFrameDesktop();
    static void UpdateDrawFrameWeb(void* world);
    flecs::world m_world;
    core::Scene* game_scene;
    std::string m_window_name;
    int m_window_width;
    int m_window_height;
};



#endif //GAME_H
