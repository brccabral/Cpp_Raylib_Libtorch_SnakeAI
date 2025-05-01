#include <raylib.h>

int main()
{
    InitWindow(800, 600, "SkiFree");

    {
        while (!WindowShouldClose())
        {
            ClearBackground(WHITE);
            BeginDrawing();
            EndDrawing();
        }
    }

    CloseWindow();
}
