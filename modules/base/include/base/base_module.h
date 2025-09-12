//
// Created by Laurent Voisard on 12/20/2024.
//

// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppHiddenFunction
#ifndef BASE_MODULE_H
#define BASE_MODULE_H
#include <format>
#include <iostream>

#include "core/logger.h"
#include "flecs.h"


namespace base {
    template<typename T>
    class BaseModule {
    public:
        BaseModule(flecs::world &world) {
            LOG_INFO( core::Base, "Creating Module " + std::string(typeid(T).name()));
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

    private:
        BaseModule() = delete;

        void register_components(flecs::world &world) {
            LOG_ERROR(core::LogLocation::Base, "No component registration defined");
            throw std::runtime_error("Undefined Component Registration: Module does not define \"register_components\"");
        }

        void register_systems(flecs::world &world) {
            LOG_ERROR(core::LogLocation::Base, "No system registration defined");
            throw std::runtime_error("Undefined System Registration: Module does not define \"register_systems\"");
        }

        void register_queries(flecs::world &world) {
            LOG_INFO(core::LogLocation::Base, "No query registration implemented");
        }

        void register_pipeline(flecs::world &world) {
            LOG_INFO(core::LogLocation::Base, "No pipeline registration implemented");
        }

        void register_submodules(flecs::world &world) {
            LOG_INFO(core::LogLocation::Base, "No sub module registration implemented");
        }

        void register_entities(flecs::world &world) {
            LOG_INFO(core::LogLocation::Base, "No entity registration implemented");
        }
    };


}
#endif //BASE_MODULE_H
