//
// Created by Laurent Voisard on 12/22/2024.
//

#include "physics_module.h"

#include "components.h"
#include "modules/core/components.h"
#include "raymath.h"

using namespace core;

namespace physics {
    void PhysicsModule::register_components(flecs::world &world) {
        world.component<Velocity2D>();
    }

    void PhysicsModule::register_systems(flecs::world &world) {
        world.system<Position2D, const Velocity2D>()
            .each([](flecs::iter& it, size_t, Position2D& pos, const Velocity2D& vel) {
                pos.value = Vector2Add(pos.value, vel.value * it.delta_time());
            });
    }
}
