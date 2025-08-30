//
// Created by laurent on 8/30/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

namespace core {
    class Application {
    public:
        virtual ~Application()= default;

        virtual void init() = 0;
        virtual void run() = 0;
    };
}
#endif // APPLICATION_H
