//
// Created by Laurent Voisard on 12/20/2024.
//

// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppHiddenFunction
#ifndef MODULE_H
#define MODULE_H
#include <format>
#include <iostream>
#include <ostream>

#include "flecs.h"

template <typename T>
class BaseModule {
public:
    BaseModule(flecs::world &world) {
        std::cout << "Creating Module " << typeid(T).name() << std::endl;
        world.module<T>();
        static_cast<T*>(this)->register_components(world);
        static_cast<T*>(this)->register_pipeline(world);
        static_cast<T*>(this)->register_systems(world);
    }
    virtual ~BaseModule() = default;

    void print() {
        std::cout << "Base" << std::endl;
    }
private:

    void register_components(flecs::world &world) {
        std::cout << "Base class register component" << std::endl;
        throw std::runtime_error("Undefined Component Registration: Module does not define \"register_components\"");
    }

    void register_systems(flecs::world &world) {
        std::cout << "Base systems" << std::endl;
        throw std::runtime_error("Undefined System Registration: Module does not define \"register_systems\"");
    }

    void register_pipeline(flecs::world &world) {
        std::cout << "No pipeline registration implemented" << std::endl;
    }
};


#endif //MODULE_H
