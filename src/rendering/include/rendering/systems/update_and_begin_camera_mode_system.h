//
// Created by laurent on 20/06/25.
//

#ifndef UPDATE_AND_BEGIN_CAMERA_MODE_SYSTEM_H
#define UPDATE_AND_BEGIN_CAMERA_MODE_SYSTEM_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include "rendering/components.h"

namespace rendering::systems {
    inline void update_and_begin_camera_mode_system(flecs::iter& it, size_t, TrackingCamera &camera, Viewport &vp) {
        Vector2 pos = camera.camera.target;
        if (camera.target.is_alive()) {
            pos = camera.target.get<core::Position2D>().value;
        };
        camera.camera.target = Vector2Lerp(camera.camera.target ,pos , it.delta_time() * 2.0f);
        camera.camera.offset.x = vp.rect.width / 2.0f;
        camera.camera.offset.y = vp.rect.height / 2.0f;
        BeginMode2D(camera.camera);
    }

    inline void end_camera_mode_system(flecs::iter &it) {
        EndMode2D();
    }
}
#endif //UPDATE_AND_BEGIN_CAMERA_MODE_SYSTEM_H
