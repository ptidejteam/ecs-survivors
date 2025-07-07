//
// Created by laurent on 20/06/25.
//

#ifndef CREATE_CAMERA_SYSTEM_H
#define CREATE_CAMERA_SYSTEM_H
#include "modules/engine/rendering/components.h"

namespace rendering::systems {
    inline void create_camera_system(TrackingCamera &camera) {
        if (!camera.target.is_alive()) return;
        camera.camera.target = camera.target.get<core::Position2D>().value;
        camera.camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.camera.zoom = 1.0f;
        camera.camera.rotation = 0;
    }
}
#endif //CREATE_CAMERA_SYSTEM_H
