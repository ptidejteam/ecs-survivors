//
// Created by laure on 3/24/2025.
//

#include <catch2/catch_test_macros.hpp>

#include <game.h>

TEST_CASE( "Start Game ")
{
    Game game = Game("Test Window", 1920, 1080);

    game.run();
}