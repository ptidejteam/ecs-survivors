//
// Created by laurent on 1/20/26.
//

#ifndef ECS_SURVIVORS_HEADLESS_APP_H
#define ECS_SURVIVORS_HEADLESS_APP_H
#include "core/application.h"


class HeadlessApp: public core::Application {
    public:
    void init() override;
    void run() override;
};


#endif // ECS_SURVIVORS_HEADLESS_APP_H
