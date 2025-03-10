//
// Created by Laurent Voisard on 12/22/2024.
//

#include "physics_module.h"

#include <raygui.h>

#include "components.h"
#include "modules/engine/core/components.h"
#include <raymath.h>

using namespace core;

namespace physics {
    void PhysicsModule::register_components(flecs::world &world) {
        world.component<Velocity2D>();
        world.component<AccelerationSpeed>();

    }



    void PhysicsModule::register_systems(flecs::world &world) {



        world.system<const Velocity2D, DesiredVelocity2D>("reset desired vel")
                .kind(flecs::PreUpdate)
                .each([](const Velocity2D &vel, DesiredVelocity2D &desiredVel) {
                    desiredVel.value = vel.value;
                });

        world.system<Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>("Lerp Current to Desired Velocity")
                .kind(flecs::PostUpdate)
                .each([](flecs::iter &it, size_t, Velocity2D &vel, const DesiredVelocity2D &desiredVel,
                         const AccelerationSpeed &acceleration_speed) {
                    // eventually I want to use spherical linear interpolation for a smooth transition
                    vel.value = Vector2Lerp(vel.value, desiredVel.value, it.delta_time() * acceleration_speed.value);
                });

        world.system<Position2D, const Velocity2D>("Update Position")
                .kind(flecs::PostUpdate)
                .each([](const flecs::iter &it, size_t, Position2D &pos, const Velocity2D &vel) {
                    pos.value = Vector2Add(pos.value, vel.value * it.delta_time());
                });
    }
}
