#include "SkiFree.h"

#define MANUAL 1

int main()
{
    InitWindow(800, 600, "SkiFree");
#if MANUAL
    SetTargetFPS(60);
#endif

    {
        SkiFree game;
        while (!WindowShouldClose())
        {
            game.inputs();
            game.update();

            BeginDrawing();
            ClearBackground(WHITE);
            game.draw();
            EndDrawing();
        }
    }

    CloseWindow();
}
