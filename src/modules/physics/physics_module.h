//
// Created by laure on 12/22/2024.
//

#ifndef PHYSICS_MODULE_H
#define PHYSICS_MODULE_H

#include "modules/base_module.h"
#include "flecs.h"
namespace physics {
  class PhysicsModule: public BaseModule<PhysicsModule> {
    friend class BaseModule<PhysicsModule>;
  public:
    // do not add implementation to the constructor
    PhysicsModule(flecs::world& world): BaseModule(world) {};
  private:

    void register_components(flecs::world& world);
    //void register_pipeline(flecs::world& world) = delete;
    void register_systems(flecs::world& world);

  };
}

#endif //PHYSICS_MODULE_H
