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
#include <unordered_map>

#include "flecs.h"

template<typename T>
class BaseModule {
public:
    BaseModule(flecs::world &world) {
        std::cout << "Creating Module " << typeid(T).name() << std::endl;

        if (instances().count(typeid(T).name()) > 0) {
            throw std::runtime_error("Module already exists!");
        }

        // Register the instance
        instances()[typeid(T).name()] = this;
        world.module<T>();
        static_cast<T *>(this)->register_submodules(world);
        static_cast<T *>(this)->register_components(world);
        static_cast<T *>(this)->register_pipeline(world);
        static_cast<T *>(this)->register_systems(world);
    }

    ~BaseModule() = default;

    static T *getInstance() {
        auto it = instances().find(typeid(T).name());
        if (it != instances().end()) {
            return static_cast<T *>(it->second);
        }
        return nullptr;
    }

    void print() {
        std::cout << "Base" << std::endl;
    }

private:
    // A map to store instances of each module type (singleton instances)
    static std::unordered_map<std::string, BaseModule *> &instances() {
        static std::unordered_map<std::string, BaseModule *> instance_map;
        return instance_map;
    }

    BaseModule() = delete;

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

    void register_submodules(flecs::world &world) {
        std::cout << "No sub module registration implemented" << std::endl;
    }
};

#endif //MODULE_H
