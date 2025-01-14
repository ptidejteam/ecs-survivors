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


        world.system<InputHorizontal>("set horizontal input")
                .kind(flecs::PreUpdate)
                .each([](flecs::entity e, InputHorizontal &horizontal) {
                    e.each([&horizontal](flecs::id id) {
                        if (id.is_pair() && id.second().has<KeyBinding>()) {
                            const auto* binding = id.second().get<KeyBinding>();
                            if (IsKeyDown(binding->key)) {
                                horizontal.value += binding->value;
                            }
                        }
                    });
                });

        world.system<InputVertical>("set vertical input")
                .kind(flecs::PreUpdate)
                .each([](flecs::entity e, InputVertical &vertical) {
                    e.each([&vertical](flecs::id id) {
                        if (id.is_pair() && id.second().has<KeyBinding>()) {
                            const auto* binding = id.second().get<KeyBinding>();
                            if (IsKeyDown(binding->key)) {
                                vertical.value += binding->value;
                            }
                        }
                    });
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
