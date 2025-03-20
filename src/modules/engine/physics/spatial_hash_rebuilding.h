//
// Created by Laurent on 3/14/2025.
//

#ifndef SPATIAL_HASH_REBUILDING_H
#define SPATIAL_HASH_REBUILDING_H
#include <mutex>
#include <unordered_map>
#include <vector>

#include "base_spatial_hash.h"

class SpatialHashRebuilding : public BaseSpatialHash {
public:
    SpatialHashRebuilding() {
        m_collision_cells.clear();
        m_collision_pairs.clear();
    }

    ~SpatialHashRebuilding() override {
        m_collision_cells.clear();
        m_collision_pairs.clear();
    }

    // Add an entity to the hash map
    void add_entity(const flecs::entity &entity) override {
        //if(m_collision_cells.find(cell) == m_collision_cells.end()) return;
        std::pair<int, int> cell = hash(entity.get<core::Position2D>()->value);
        m_collision_cells[cell].push_back(entity);
    }

    void remove_entity(const flecs::entity &entity) override {
        //if(m_collision_cells.find(cell) == m_collision_cells.end()) return;
        auto &cellEntities = m_collision_cells[hash(entity.get<core::Position2D>()->value)];
        for (auto it = cellEntities.begin(); it != cellEntities.end(); ++it) {
            if (*it == entity) {
                cellEntities.erase(it);
                break;
            }
        }
    }

    void update_entity_position(const flecs::entity &entity) override {
        add_entity(entity);
    };

    void detect_collisions(const physics::Cell &cell) override {
        std::vector<std::pair<flecs::entity, flecs::entity> > local_pairs;

        auto it_cell = m_collision_cells.find({cell.x, cell.y});
        if (it_cell == m_collision_cells.end())
            return;

        for (auto &neighbor_offset: m_neighbor_offsets) {
            auto neighborCell = std::make_pair(cell.x + neighbor_offset.first,
                                               cell.y + neighbor_offset.second);

            auto it_neighbor = m_collision_cells.find(neighborCell);
            if (it_neighbor == m_collision_cells.end())
                continue;

            for (auto self: it_cell->second) {
                for (auto other: it_neighbor->second) {
                    if (collide(self, other)) {
                        local_pairs.emplace_back(self, other);
                    }
                }
            }
        }

        if (!local_pairs.empty()) {
            std::lock_guard<std::mutex> lock(m_pairs_mutex);
            m_collision_pairs.insert(
                m_collision_pairs.end(),
                local_pairs.begin(),
                local_pairs.end()
            );
        }
    }

    void resolve_collisions() override {
        for (auto &[self, other]: m_collision_pairs) {
            Vector2 mypos = self.get<core::Position2D>()->value;
            Vector2 otherPos = other.get<core::Position2D>()->value;
            float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->radius;

            Vector2 direction = otherPos - mypos;
            Vector2 moveDirection = Vector2Normalize(direction);
            float overlap = combinedRadius - Vector2Length(direction);
            if (overlap <= 0.f) continue;

            Vector2 move = moveDirection * overlap * 0.5f;

            self.set<physics::Velocity2D>({self.get<physics::Velocity2D>()->value - move * 2.f});
            other.set<physics::Velocity2D>({other.get<physics::Velocity2D>()->value + move * 2.f});

            self.set<core::Position2D>({mypos - move / 2.f});
            other.set<core::Position2D>({otherPos + move / 2.f});
        }

        m_collision_cells.clear();
        m_collision_pairs.clear();
    }

private:
    std::unordered_map<std::pair<int, int>, std::vector<flecs::entity>, pair_hash> m_collision_cells;
    std::vector<std::pair<flecs::entity, flecs::entity> > m_collision_pairs;
    std::mutex m_pairs_mutex;
};
#endif //SPATIAL_HASH_REBUILDING_H
