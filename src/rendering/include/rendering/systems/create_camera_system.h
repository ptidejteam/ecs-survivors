//
// Created by laurent on 20/06/25.
//

#ifndef CREATE_CAMERA_SYSTEM_H
#define CREATE_CAMERA_SYSTEM_H
#include "rendering/components.h"

namespace rendering::systems {
    inline void create_camera_system(TrackingCamera &camera, Viewport &vp) {
        if (!camera.target.is_alive()) return;
        camera.camera.target = camera.target.get<core::Position2D>().value;
        camera.camera.offset = {vp.rect.width / 2.0f, vp.rect.height / 2.0f};
        camera.camera.zoom = 1.0f;
        camera.camera.rotation = 0;
    }
}
#endif //CREATE_CAMERA_SYSTEM_H
