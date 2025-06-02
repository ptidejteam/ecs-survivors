//
// Created by laurent on 30/05/25.
//

#ifndef CORE_QUERIES_H
#define CORE_QUERIES_H
#include <flecs.h>

#include "components.h"

namespace core::queries {
    inline flecs::query<Position2D, Tag> position_and_tag_query;
}
#endif //CORE_QUERIES_H
