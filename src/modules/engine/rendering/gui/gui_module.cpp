//
// Created by Laurent Voisard on 12/22/2024.
//

#include "gui_module.h"

#include "components.h"
#define RAYGUI_IMPLEMENTATION
#include <modules/engine/rendering/pipeline_steps.h>
#include <stack>
#include <unordered_set>

#include "raygui.h"

namespace rendering::gui {
	void GUIModule::register_components(flecs::world &world) {
		world.component<Button>();
		world.component<Text>();
		world.component<Outline>();
	}

	void GUIModule::register_systems(flecs::world &world) {
		world.system().kind(flecs::OnStart).run([](flecs::iter &iter) {
			GuiLoadStyle("../resources/styles/dark/style_dark.rgs");
		});

		world.system<const Button, const Rectangle>("Draw Button")
			.kind<RenderGUI>()
			.each([](const Button &button, const Rectangle &rect) {
				if (GuiButton(rect, button.text.c_str())) {
					button.onclick_system.run();
				}
			});

		world.system<const Rectangle, const Outline>()
		.each([](const Rectangle &rect, const Outline &outline) {
			GuiDrawRectangle(rect, outline.border_size, outline.border_color, outline.fill_color);
		});

		world.system<const Text, const Rectangle>("Draw Text")
			.kind<RenderGUI>()
			.each([](flecs::entity e, const Text &text, const Rectangle &rect) {
				e.children([](flecs::entity child) {
					if (child.has<Decorator>()) {
						child.get<Decorator>()->add_decoration_system.run();
					}
				});
				GuiLabel(rect, text.text.c_str());
				e.children([](flecs::entity child) {
					if (child.has<Decorator>()) {
						child.get<Decorator>()->remove_decoration_system.run();
					}
				});
			});

		world.system("Draw FPS").kind<RenderGUI>().run([](flecs::iter &iter) { DrawFPS(10, 10); });
	}

	void GUIModule::init_prefabs(flecs::world &world) {
		world.prefab(DECORATOR_TEXT_ALIGN_CENTER)
			.set<Decorator>({world.system().kind(0).run(
								 [](flecs::iter &iter) { GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER); }),
							 world.system().kind(0).run(
								 [](flecs::iter &iter) { GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT); })});


		world.prefab(DECORATOR_TEXT_ALIGN_RIGHT)
			.set<Decorator>({world.system().kind(0).run(
								 [](flecs::iter &iter) { GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT); }),
							 world.system().kind(0).run(
								 [](flecs::iter &iter) { GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT); })});
	}
} // namespace rendering::gui
