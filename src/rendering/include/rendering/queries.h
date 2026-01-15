//
// Created by laurent on 30/05/25.
//

#ifndef RENDERING_QUERIES_H
#define RENDERING_QUERIES_H

#include <flecs.h>
#include "rendering/components.h"

namespace rendering::queries {
    inline flecs::query<Renderable> entity_count_query;
    inline flecs::query<Renderable> entity_visible_count_query;
}
#endif //RENDERING_QUERIES_H
