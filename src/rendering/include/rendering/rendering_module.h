//
// Created by laure on 1/27/2025.
//

#ifndef RENDERING_MODULE_H
#define RENDERING_MODULE_H

#include <flecs.h>
#include "base/base_module.h"
namespace rendering {
    class RenderingModule: public base::BaseModule<RenderingModule> {
    public:
        RenderingModule(flecs::world world): BaseModule(world) {
        }
        inline static flecs::entity main_viewport;
    private:
        void register_components(flecs::world world);
        void register_entities(flecs::world world);
        void register_queries(flecs::world world);
        void register_systems(flecs::world world);
        void register_pipeline(flecs::world world);
        void register_submodules(flecs::world world);

        friend class BaseModule<RenderingModule>;
    };
}



#endif //RENDERING_MODULE_H
