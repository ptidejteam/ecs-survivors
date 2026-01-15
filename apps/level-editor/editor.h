//
// Created by laurent on 9/12/25.
//

#ifndef ECS_SURVIVORS_EDITOR_H
#define ECS_SURVIVORS_EDITOR_H
#include <flecs.h>
#include "core/graphical_application.h"

class Editor: public core::GraphicalApplication {
public:
    Editor(const char* window_name, int window_width, int window_height) : GraphicalApplication(window_name, window_width, window_height){};
    ~Editor() override = default;

    void run() override;
    void init() override;

private:

};
#endif // ECS_SURVIVORS_EDITOR_H
