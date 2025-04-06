#include <raylib.h>
#include "RaceTopDown.h"


int main()
{
    constexpr int SCREEN_WIDTH = 1366;
    constexpr int SCREEN_HEIGHT = 768;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Race TopDown");
    {
        auto game = RaceTopDown(15);
        while (!WindowShouldClose())
        {
            game.update();
            game.draw();
        }
    }
    CloseWindow();
}
