/*******************************************************************************************
*
*   raylib-extras [ImGui] example - Docking example
*
*	This is an example of using the ImGui docking features that are part of docking branch
*	You must replace the default imgui with the code from the docking branch for this to work
*	https://github.com/ocornut/imgui/tree/docking
*
*   Copyright (c) 2024 Jeffery Myers
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"

#include <flecs.h>

#include "core/core_module.h"
#include "editor/editor_module.h"
#include "gui/gui_module.h"
#include "input/input_module.h"
#include "physics/physics_module.h"
#include "rendering/rendering_module.h"
#include "tilemap/tilemap_module.h"


// DPI scaling functions
float ScaleToDPIF(float value)
{
	return GetWindowScaleDPI().x * value;
}

int ScaleToDPII(int value)
{
    return int(GetWindowScaleDPI().x * value);
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1280;
	int screenHeight = 800;

    flecs::world m_world;
    m_world.import<core::CoreModule>();
    m_world.import<input::InputModule>();
    m_world.import<rendering::RenderingModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<tilemap::TilemapModule>();
    m_world.import<gui::GUIModule>();
    m_world.import<editor::EditorModule>();

	// do not set the FLAG_WINDOW_HIGHDPI flag, that scales a low res framebuffer up to the native resolution.
	// use the native resolution and scale your geometry.
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - Docking");
	SetTargetFPS(144);

	bool run = true;

	bool showDemoWindow = true;

	// if the linked ImGui has docking, enable it.
	// this will only be true if you use the docking branch of ImGui.
#ifdef IMGUI_HAS_DOCK
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

    RenderTexture2D tex = LoadRenderTexture(800, 600);
	// Main game loop
	while (!WindowShouldClose() && run)    // Detect window close button or ESC key, or a quit from the menu
	{
	    BeginTextureMode(tex);
		DrawCircle(tex.texture.width / 2, tex.texture.height / 2, tex.texture.height * 0.45f, DARKGREEN);
	    EndTextureMode();

		BeginDrawing();
		ClearBackground(DARKGRAY);

		// draw something to the raylib window below the GUI.


		// start ImGui content
		rlImGuiBegin();

		// if you want windows to dock to the viewport, call this.
#ifdef IMGUI_HAS_DOCK
		ImGui::DockSpaceOverViewport(0,  NULL, ImGuiDockNodeFlags_PassthruCentralNode); // set ImGuiDockNodeFlags_PassthruCentralNode so that we can see the raylib contents behind the dockspace
#endif

	    if (ImGui::Begin("Game View")) {
	        rlImGuiImageRenderTextureFit(&tex, true);
	    }
	    ImGui::End();
		// show a simple menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit"))
					run = false;

				ImGui::EndMenu();
            }

			if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("Demo Window", nullptr, showDemoWindow))
					showDemoWindow = !showDemoWindow;

                ImGui::EndMenu();
            }
			ImGui::EndMainMenuBar();
		}

		// show some windows

		if (showDemoWindow)
			ImGui::ShowDemoWindow(&showDemoWindow);

		if (ImGui::Begin("Test Window"))
		{
			ImGui::TextUnformatted("Another window");
		}
		ImGui::End();

		// end ImGui Content
		rlImGuiEnd();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	rlImGuiShutdown();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}