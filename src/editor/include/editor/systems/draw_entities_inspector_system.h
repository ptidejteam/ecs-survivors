//
// Created by laurent on 9/11/25.
//

#ifndef EDITOR_DRAW_ENTITIES_INSPECTOR_SYSTEM_H
#define EDITOR_DRAW_ENTITIES_INSPECTOR_SYSTEM_H

#include <flecs.h>
#include <imgui.h>

#include "editor/components.h"

namespace editor::systems {
    inline void draw_entities_inspector_system(flecs::entity e) {
        const char *name = e.name() != "" ? e.name().c_str() : std::to_string(e.id()).c_str();
        if (ImGui::TreeNode(name)) {
            if (ImGui::IsItemFocused()) {
                e.world().get_mut<SelectedEntity>().entity = e;
            }

            e.children([](flecs::entity child) {
                if (child.has<flecs::Poly>(flecs::System))
                    return;
                if (child.has<flecs::Poly>(flecs::Observer))
                    return;
                if (child.has<flecs::Component>() && !child.has(flecs::Module))
                    return;
                if (child.has<flecs::Member>())
                    return;
                draw_entities_inspector_system(child);
            });


            ImGui::TreePop();
        }
        //}
    }
} // namespace editor::systems
#endif // EDITOR_DRAW_ENTITIES_INSPECTOR_SYSTEM_H
