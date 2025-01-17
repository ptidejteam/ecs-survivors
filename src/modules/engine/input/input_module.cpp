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

        world.system<const KeyBinding, InputHorizontal>("set horizontal input")
                .term_at(1).cascade()
                .kind(flecs::PreUpdate)
                .each([](const KeyBinding &binding, InputHorizontal &horizontal) {
                    if (IsKeyDown(binding.key)) {
                        horizontal.value += binding.value;
                    }
                });

        world.system<const KeyBinding, InputVertical>("set vertical input")
                .term_at(1).cascade()
                .kind(flecs::PreUpdate)
                .each([](const KeyBinding &binding, InputVertical &vertical) {
                    if (IsKeyDown(binding.key)) {
                        vertical.value += binding.value;
                    }
                });

        world.system<InputHorizontal>("Reset Input Horizontal")
                .kind(flecs::PostUpdate)
                .each([](InputHorizontal &horizontal) {
                    horizontal.value = 0;
                });

        world.system<InputVertical>("Reset Input Vertical")
                .kind(flecs::PostUpdate)
                .each([](InputVertical &vertical) {
                    vertical.value = 0;
                });
    }
}
