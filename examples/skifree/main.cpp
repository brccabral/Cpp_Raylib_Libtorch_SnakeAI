#include "SkiFree.h"


#include <raylib.h>

int main()
{
    InitWindow(800, 600, "SkiFree");

    {
        SkiFree game;
        while (!WindowShouldClose())
        {
            game.inputs();
            game.update();

            BeginDrawing();
            ClearBackground(BLACK);
            game.draw();
            EndDrawing();
        }
    }

    CloseWindow();
}
