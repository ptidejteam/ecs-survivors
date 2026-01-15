//
// Created by Laurent on 12/29/2024.
//

#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#include "base/base_module.h"

namespace input {
class InputModule: public base::BaseModule<InputModule> {
public:
    InputModule(flecs::world& world): BaseModule(world) {}
private:
    void register_components(flecs::world& world);
    void register_systems(flecs::world& world);

    friend class BaseModule<InputModule>;
};
}


#endif //INPUT_MODULE_H
