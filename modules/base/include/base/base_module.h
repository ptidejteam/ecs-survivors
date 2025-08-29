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

template<typename T>
class BaseModule {
public:
    BaseModule(flecs::world &world): m_world(world) {
        std::cout << "Creating Module " << typeid(T).name() << std::endl;
        // Register the instance
        world.module<T>();
        static_cast<T *>(this)->register_components(world);
        static_cast<T *>(this)->register_queries(world);
        static_cast<T *>(this)->register_pipeline(world);
        static_cast<T *>(this)->register_systems(world);
        static_cast<T *>(this)->register_submodules(world);
        static_cast<T *>(this)->register_entities(world);
    }

    ~BaseModule() = default;

    void print() {
        std::cout << "Base" << std::endl;
    }
    virtual flecs::world get_world() const {return m_world;}
protected:
    flecs::world m_world;
private:
    BaseModule() = delete;

    void register_components(flecs::world &world) {
        std::cout << "Base class register component" << std::endl;
        throw std::runtime_error("Undefined Component Registration: Module does not define \"register_components\"");
    }

    void register_systems(flecs::world &world) {
        std::cout << "Base systems" << std::endl;
        throw std::runtime_error("Undefined System Registration: Module does not define \"register_systems\"");
    }

    void register_queries(flecs::world &world) {
        std::cout << "No query registration implemented" << std::endl;
    }

    void register_pipeline(flecs::world &world) {
        std::cout << "No pipeline registration implemented" << std::endl;
    }

    void register_submodules(flecs::world &world) {
        std::cout << "No sub module registration implemented" << std::endl;
    }

    void register_entities(flecs::world &world) {
        std::cout << "No entity registration implemented" << std::endl;
    }
};

#endif //MODULE_H
