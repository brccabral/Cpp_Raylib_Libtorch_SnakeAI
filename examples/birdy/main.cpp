#include "BirdyGame.h"

int main()
{
    constexpr int screen_width = 1366;
    constexpr int screen_height = 768;
    InitWindow(screen_width, screen_height, "Birdy");

    {
        auto game = BirdyGame(200);
        while (!WindowShouldClose())
        {
            game.draw();
        }
    }

    CloseWindow();
}
