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

#include "player/player_module.h"
#include "ai/ai_module.h"
#include "core/core_module.h"
#include "debugging/components.h"
#include "debugging/debug_module.h"
#include "editor/components.h"
#include "editor/editor_module.h"
#include "game_scene.h"
#include "gui/gui_module.h"
#include "input/input_module.h"
#include "physics/physics_module.h"
#include "rendering/components.h"
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
	int screenWidth = 1920;
	int screenHeight = 1080;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "LaeVo - Editor");

    SetExitKey(KEY_NULL);
    flecs::world world;

#ifndef EMSCRIPTEN
    // use the flecs explorer when not on browser
    world.import <flecs::stats>();
	world.set<flecs::Rest>({});

    //world.set_threads(4);
#endif

	world.import<core::CoreModule>();
	world.import<rendering::RenderingModule>();
	world.import<gui::GUIModule>();
	world.import<physics::PhysicsModule>();
	world.import<tilemap::TilemapModule>();
	world.import<input::InputModule>();
	world.import<player::PlayerModule>();
	world.import<ai::AIModule>();
	world.import<gameplay::GameplayModule>();
	world.import<debugging::DebugModule>();
	world.import<editor::EditorModule>();


	// do not set the FLAG_WINDOW_HIGHDPI flag, that scales a low res framebuffer up to the native resolution.
	// use the native resolution and scale your geometry.

    world.set<rendering::Settings>({"LaeVo - Editor", screenWidth, screenHeight, screenWidth, screenHeight});
    world.set<physics::Settings>({(float) screenWidth, (float) screenHeight});
    world.add<physics::CollisionRecordList>();
    world.set<physics::SpatialHashingGrid>({48, {0, 0}});
    world.set<core::Paused>({false});
    world.set<core::EnabledMenus>({0});

	rendering::RenderingModule::main_viewport
        .remove<rendering::Viewport>();
	rendering::RenderingModule::main_viewport
		.set<rendering::VirtualViewport>({0,0, 1920, 1080})
		.set<editor::Window>({"Game View"});

    // load whatever you need
    GameScene game_scene;
    game_scene.load(world);


	world.entity().set<editor::Window>({"Hierarchy"}).add<editor::Inspector>();

    world.progress();

	// Main game loop
	while (!WindowShouldClose() && !world.has<core::ExitConfirmed>())    // Detect window close button or ESC key, or a quit from the menu
	{
	    world.progress(GetFrameTime());
	}
	rlImGuiShutdown();

	//delete game_scene;

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------


	return 0;
}