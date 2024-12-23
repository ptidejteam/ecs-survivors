//
// Created by laure on 12/20/2024.
//

// ReSharper disable CppHidingFunction
#ifndef CORE_MODULE_H
#define CORE_MODULE_H

#include <memory>

#include "modules/module.h"
#include "components.h"

namespace core {

    class CoreModule : public Module<CoreModule> {
        friend class Module;
    public:
        // do not add anything to the constructor, instead change the base class
        CoreModule(flecs::world& world): Module(world) {}
    private:
        void register_components(flecs::world &world);
        void register_systems(flecs::world &world);
    };
}

#endif //CORE_MODULE_H
