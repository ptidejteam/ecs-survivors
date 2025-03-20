//
// Created by Laurent on 3/15/2025.
//

#ifndef BASE_SPATIAL_HASH_H
#define BASE_SPATIAL_HASH_H
#include <cmath>
#include <utility>
#include <raylib.h>
#include <flecs.h>
#include <raymath.h>
#include <flecs/addons/cpp/entity.hpp>

#include "modules/engine/core/components.h"
#include "modules/engine/rendering/components.h"

struct pair_hash {
    template<typename T1, typename T2>
    std::size_t operator ()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first); // Hash the first element of the pair
        auto h2 = std::hash<T2>{}(p.second); // Hash the second element of the pair
        // Combine the two hash values (using XOR)
        return h1 ^ (h2 << 1); // Bitwise XOR to combine hashes, shifting h2 by 1 for better distribution
    }
};

struct entity_hash {
    std::size_t operator ()(const flecs::entity& e) const {
        return e.id();
    }
};


class BaseSpatialHash {
public:
    virtual ~BaseSpatialHash() = default;

    std::pair<int, int> hash(const Vector2 &pos) {
        return {
            static_cast<int>(std::floor(pos.x / m_cell_size)),
            static_cast<int>(std::floor(pos.y / m_cell_size))
        };
    }

    virtual void add_entity(const flecs::entity& entity) = 0;
    virtual void remove_entity(const flecs::entity& entity) = 0;
    virtual void update_entity_position(const flecs::entity& entity) = 0;
    virtual void detect_collisions(const physics::Cell& cell) = 0;
    virtual void resolve_collisions() = 0;

    int get_cell_size() const { return m_cell_size; }

protected:
    bool collide(flecs::entity &self, flecs::entity &other) {
        // if we are colliding with our selfs of the collision is already counted.
        // we only compute when self id < other id, that way we check the collision only once
        if(self.id() >= other.id()) return false;

        Vector2 mypos = self.get<core::Position2D>()->value;
        Vector2 otherPos = other.get<core::Position2D>()->value;
        float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->
                               radius;

        // Find the distance and adjust to resolve the overlap
        Vector2 direction = otherPos - mypos;

        float distanceSqr = Vector2LengthSqr(direction);

        return distanceSqr < combinedRadius * combinedRadius;
    }

    std::vector<std::pair<int, int> > m_neighbor_offsets = {
        {0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };



private:
    int m_screen_width = 1920;
    int m_screen_height = 1080;
    const int m_cell_size = 64;
};

#endif //BASE_SPATIAL_HASH_H
