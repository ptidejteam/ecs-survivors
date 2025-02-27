//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include <unordered_map>

#include "modules/base_module.h"
#include "flecs.h"
namespace rendering::gui {
  class GUIModule: public BaseModule<GUIModule> {
    friend class BaseModule<GUIModule>;
  public:
    // do not add implementation to the constructor
    GUIModule(flecs::world& world): BaseModule(world) {};

	static void init_prefabs(flecs::world& world);
  private:

    void register_components(flecs::world& world);
    void register_systems(flecs::world &world);

  };

  static const char* DECORATOR_TEXT_ALIGN_CENTER = "decorator_text_align_left";
  static const char* DECORATOR_TEXT_ALIGN_RIGHT = "decorator_text_align_right";
  static const char* DECORATOR_OUTLINED = "decorator_outlined";
}

#endif //GUI_MODULE_H
