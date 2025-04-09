#include <raylib.h>
#include "RaceTopDown.h"

#include <cstdio>


int main()
{
    constexpr int SCREEN_WIDTH = 1366;
    constexpr int SCREEN_HEIGHT = 768;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Race TopDown");

    {
        auto game = RaceTopDown(1);
        while (!WindowShouldClose())
        {
            int action = 0;
            if (IsKeyDown(KEY_LEFT))
            {
                action = 0b0001;
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                action = 0b0010;
            }
            if (IsKeyDown(KEY_UP))
            {
                action = action | 0b0100;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                action = action | 0b1000;
            }
            game.apply_action(0, action);

            game.update();
            game.draw();

            if (game.check_end_game())
            {
                game.reset();
            }
        }
    }
    CloseWindow();
}
