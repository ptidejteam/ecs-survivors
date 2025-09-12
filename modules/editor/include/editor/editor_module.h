//
// Created by laurent on 8/30/25.
//

#ifndef ECS_SURVIVORS_EDITOR_MODULE_H
#define ECS_SURVIVORS_EDITOR_MODULE_H
#include "base/base_module.h"

namespace editor {
    class EditorModule :public base::BaseModule<EditorModule> {
    public:
        EditorModule(flecs::world &world): BaseModule(world) {};
    private:
        void register_components(flecs::world &world);

        void register_systems(flecs::world &world);

        void register_entities(flecs::world &world);

        void register_pipeline(flecs::world &world);


        friend class BaseModule<EditorModule>;
    };
}
#endif // ECS_SURVIVORS_EDITOR_MODULE_H
