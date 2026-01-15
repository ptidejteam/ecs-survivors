//
// Created by Laurent Voisard on 12/20/2024.
//

#ifndef GAME_H
#define GAME_H

#include <string>

#include "core/graphical_application.h"
#include "flecs.h"

class Game : public core::GraphicalApplication {
public:
    Game(const char* window_name, int window_width, int window_height) : GraphicalApplication(window_name, window_width, window_height){};
    ~Game() override = default;

    void run() override;
    void init() override;

private:
    void UpdateDrawFrameDesktop();
    static void UpdateDrawFrameWeb(void* world);

};



#endif //GAME_H
