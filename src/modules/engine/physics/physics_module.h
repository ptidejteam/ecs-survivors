//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef PHYSICS_MODULE_H
#define PHYSICS_MODULE_H


#include <cmath>
#include <vector>

#include "modules/base_module.h"
#include "flecs.h"
#include "modules/engine/core/components.h"
#include "modules/engine/rendering/components.h"

#include <raymath.h>
#include <algorithm>

#include "components.h"
#include <utility>

namespace physics {
    constexpr float PHYSICS_TICK_LENGTH = 0.016f;

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
        std::size_t operator ()(const flecs::entity &e) const {
            return e.id(); // Bitwise XOR to combine hashes, shifting h2 by 1 for better distribution
        }
    };

    class PhysicsModule : public BaseModule<PhysicsModule> {
        friend class BaseModule<PhysicsModule>;

    public:
        // do not add implementation to the constructor
        PhysicsModule(flecs::world &world): BaseModule(world) {
            m_collision_cells = std::unordered_map<std::pair<int, int>, std::vector<flecs::entity>, pair_hash>();
            m_collision_cells_lookup = std::unordered_map<flecs::entity, std::pair<int, int>, entity_hash>();
            m_collision_pairs = std::unordered_map<flecs::entity, std::vector<flecs::entity>, entity_hash>();
            for (int x = 0; x < m_screenWidth / m_cellSize; x++) {
                for (int y = 0; y < m_screenHeight / m_cellSize; y++) {
                    m_collision_cells[hash(Vector2{(float) x * m_cellSize, (float) y * m_cellSize})] = std::vector<
                        flecs::entity>();
                }
            }
        };

        inline static flecs::entity s1, s2, s3, s4, s5, s6, s7, s8, s9;

    private:
        std::unordered_map<std::pair<int, int>, std::vector<flecs::entity>, pair_hash> m_collision_cells;
        std::unordered_map<flecs::entity, std::pair<int, int>, entity_hash> m_collision_cells_lookup;
        std::unordered_map<flecs::entity, std::vector<flecs::entity>, entity_hash> m_collision_pairs;

        flecs::entity m_physicsTick;
        int m_screenWidth = 1920;
        int m_screenHeight = 1080;
        int m_cellSize = 60;

        void register_components(flecs::world &world);

        //void register_pipeline(flecs::world& world) = delete;
        void register_systems(flecs::world &world);

        std::pair<int, int> hash(const Vector2 &pos) {
            return {
                static_cast<int>(std::floor(pos.x / m_cellSize)),
                static_cast<int>(std::floor(pos.y / m_cellSize))
            };
        }

        // Add an entity to the hash map
        void add_entity(flecs::entity &entity, std::pair<int, int> cell) {
            //if(m_collision_cells.find(cell) == m_collision_cells.end()) return;
            m_collision_cells[cell].push_back(entity);
            m_collision_cells_lookup[entity] = cell;
        }

        // Remove an entity from the hash map
        void remove_entity(flecs::entity &entity, std::pair<int, int> cell) {
            //if(m_collision_cells.find(cell) == m_collision_cells.end()) return;
            auto &cellEntities = m_collision_cells[cell];
            m_collision_cells_lookup.erase(entity);
            for (auto it = cellEntities.begin(); it != cellEntities.end(); ++it) {
                if (*it == entity) {
                    cellEntities.erase(it);
                    // std::printf("Removed;");
                    break;
                }
            }
        }

        void remove_entity_from_grid(flecs::entity &entity) {
            remove_entity(entity, m_collision_cells_lookup[entity]);
        }

        int added_count = 0;

        void add_entity_to_grid(flecs::entity &entity) {
            added_count++;
            add_entity(entity, hash(entity.get<core::Position2D>()->value));
            std::cout << "added count " << added_count << std::endl;
        }

        // Update the position of an entity and rehash if necessary
        void update_entity_position(flecs::entity &entity, core::Position2D curr) {
            // Remove the entity from its current cell

            //if (hash(prev.value) == hash(curr.value)) return;
            //std::printf("%llu cells\n", m_collision_cells.size());
            if (m_collision_cells_lookup.find(entity) == m_collision_cells_lookup.end()) {
                add_entity(entity, hash(curr.value));
                return;
            }

            auto previous_cell = m_collision_cells_lookup[entity];
            auto current_cell = hash(curr.value);
            if (previous_cell == current_cell) return;

            //std::printf("%llu before ", m_collision_cells[previous_cell].size());
            remove_entity(entity, previous_cell);
            //std::printf("%llu after\n", m_collision_cells[previous_cell].size());
            add_entity(entity, current_cell);

            //std::printf("%llu cells\n", m_collision_cells.size());


            // Move the entity

            // Re-add the entity to its new cell
        }

        void check_collisions() {
            int count = 0;
            //std::printf("%d collisions detected\n", count);
            int entityCount = 0;
            for (auto &[cell, entities]: m_collision_cells) {
                entityCount += entities.size();
                for (size_t i = 0; i < entities.size(); ++i) {
                    for (size_t j = i + 1; j < entities.size(); ++j) {
                        if (collide(entities[i], entities[j]))
                            m_collision_pairs[entities[i]].push_back(entities[j]);
                    }
                }

                // Check neighboring cells
                for (auto &neighborOffset: neighborOffsets) {
                    auto neighborCell = std::make_pair(cell.first + neighborOffset.first,
                                                       cell.second + neighborOffset.second);
                    if (m_collision_cells.find(neighborCell) != m_collision_cells.end()) {
                        for (auto self: m_collision_cells[neighborCell]) {
                            for (auto other: entities) {
                                if (collide(self, other))
                                    m_collision_pairs[self].push_back(other);
                            }
                        }
                    }
                }
            }
            //std::printf("%d collisions detected with %d entities in %llu cells\n", count, entityCount, m_collision_cells.size());
        }

        bool collide(flecs::entity &self, flecs::entity &other) {
            Vector2 mypos = self.get<core::Position2D>()->value;
            Vector2 otherPos = other.get<core::Position2D>()->value;
            float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->
                                   radius;

            // Find the distance and adjust to resolve the overlap
            Vector2 direction = otherPos - mypos;

            float distanceSqr = Vector2LengthSqr(direction);

            return (distanceSqr < std::pow(combinedRadius, 2));
        }

        void resolve_collisions(flecs::world world) {
            for (auto [self, others]: m_collision_pairs) {
                for (auto other: others) {
                    Vector2 mypos = self.get<core::Position2D>()->value;
                    Vector2 otherPos = other.get<core::Position2D>()->value;
                    float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->
                                           radius;

                    // Find the distance and adjust to resolve the overlap
                    Vector2 direction = otherPos - mypos;
                    Vector2 moveDirection = Vector2Normalize(direction);
                    float overlap = combinedRadius - Vector2Length(direction);

                    // Move the entities apart by the amount of overlap
                    Vector2 move = moveDirection * overlap * 0.75f;

                    self.set<Velocity2D>({self.get<Velocity2D>()->value - move * 2.f});
                    other.set<Velocity2D>({other.get<Velocity2D>()->value + move * 2.f});

                    // Resolve by adjusting positions
                    self.set<core::Position2D>({mypos - move / 2.f}); // Move the current entity
                    other.set<core::Position2D>({otherPos + move / 2.f}); // Move the other entity

                    // world.entity().child_of(world.lookup("collision_records_container")).set<CollisionRecord>({self, other});
                }
            }
            m_collision_pairs.clear();
        }

        // Neighboring cells (including the current cell itself)
        std::vector<std::pair<int, int> > neighborOffsets = {
            {0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1},
            {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
        };

        // A custom hash function for pair<int, int> key in the unordered_map
    };
}

#endif //PHYSICS_MODULE_H
