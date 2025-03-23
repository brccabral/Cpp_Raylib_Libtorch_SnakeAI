#include <raylib.h>

int main()
{
    InitWindow(640, 480, "SnakeAI");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}