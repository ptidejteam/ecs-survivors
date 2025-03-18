//
// Created by laure on 3/13/2025.
//

#ifndef ANALYTICS_MODULE_H
#define ANALYTICS_MODULE_H
#include "modules/base_module.h"

namespace analytics {

class AnalyticsModule : public BaseModule<AnalyticsModule> {
    friend class BaseModule<AnalyticsModule>;
public:
    AnalyticsModule(flecs::world world) : BaseModule(world) {};

    void register_components(flecs::world world);
    void register_systems(flecs::world world);

private:

};

} // analytics

#endif //ANALYTICS_MODULE_H
