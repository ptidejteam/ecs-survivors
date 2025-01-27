//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include "modules/base_module.h"
#include "flecs.h"
namespace rendering::gui {
  class GUIModule: public BaseModule<GUIModule> {
    friend class BaseModule<GUIModule>;
  public:
    // do not add implementation to the constructor
    GUIModule(flecs::world& world): BaseModule(world) {};
  private:

    void register_components(flecs::world& world);
    //void register_pipeline(flecs::world& world) = delete;
    void register_systems(flecs::world& world);

  };
}

#endif //PHYSICS_MODULE_H
