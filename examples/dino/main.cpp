#include <raylib.h>

int main()
{
    InitWindow(1366, 768, "Dino");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        EndDrawing();
    }

    CloseWindow();
}
