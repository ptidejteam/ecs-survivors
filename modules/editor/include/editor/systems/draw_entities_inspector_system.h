//
// Created by laurent on 9/11/25.
//

#ifndef EDITOR_DRAW_ENTITIES_INSPECTOR_SYSTEM_H
#define EDITOR_DRAW_ENTITIES_INSPECTOR_SYSTEM_H

#include <flecs.h>
#include <imgui.h>

namespace editor::systems {
    inline void draw_entities_inspector_system(flecs::entity e) {
        const char* name = e.name() != "" ? e.name().c_str() : std::to_string(e.id()).c_str();
        //std::cout << e.type().str() << std::endl;
        //if (e.has<flecs::Component>()) return;
        if (ImGui::TreeNode(name)) {
            std::cout << e.type().str() << std::endl;
            e.children([] (flecs::entity child) {
                if (child.has<flecs::Poly>(flecs::System)) return;
                if (child.has<flecs::Poly>(flecs::Observer)) return;
                if (child.has<flecs::Component>() && !child.has(flecs::Module)) return;
                draw_entities_inspector_system(child);
            });
            ImGui::TreePop();
        }

    }
}
#endif // EDITOR_DRAW_ENTITIES_INSPECTOR_SYSTEM_H
