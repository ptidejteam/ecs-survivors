//
// Created by laurent on 8/30/25.
//

#ifndef CORE_SCENE_H
#define CORE_SCENE_H

#include <flecs.h>
namespace core {
    class Scene {
    public:
        virtual ~Scene() = default;
        virtual flecs::entity load(flecs::world& world) = 0;
    };
} // namespace core
#endif // CORE_SCENE_H
