//
// Created by Laurent on 12/29/2024.
//

#include "input_module.h"

#include "components.h"
#include "raylib.h"

namespace input {
    void InputModule::register_components(flecs::world &world) {
        world.component<InputHorizontal>();
        world.component<InputVertical>();
    }

    void InputModule::register_systems(flecs::world &world) {
        world.system<InputHorizontal>("Get horizontal input")
                .kind(flecs::PreUpdate)
                .each([](InputHorizontal &horizontal) {
                    if (IsKeyDown(KEY_A)) horizontal.value += -1;
                    if (IsKeyDown(KEY_D)) horizontal.value += 1;
                });

        world.system<InputVertical>("Get vertical input")
                .kind(flecs::PreUpdate)
                .each([](InputVertical &vertical) {
                    if (IsKeyDown(KEY_W)) vertical.value += -1;
                    if (IsKeyDown(KEY_S)) vertical.value += 1;
                });

        world.system<InputHorizontal, InputVertical>("Reset Input")
                .kind(flecs::PostUpdate)
                .each([](InputHorizontal &horizontal, InputVertical &vertical) {
                    horizontal.value = 0;
                    vertical.value = 0;
                });
    }
}
