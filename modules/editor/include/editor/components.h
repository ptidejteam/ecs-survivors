//
// Created by laurent on 8/30/25.
//

#ifndef EDITOR_COMPONENTS_H
#define EDITOR_COMPONENTS_H
#include <string>
#include <vector>

namespace editor {
    struct Window {
        std::string name;
    };

    struct Hierarchy {};
    struct Inspector {};
    struct Console {
        std::vector<const char*> logs;
    };
}
#endif // EDITOR_COMPONENTS_H
