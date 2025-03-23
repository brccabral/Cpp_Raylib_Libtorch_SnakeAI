#include <cstdlib>
#include <ctime>
#include "SnakeGameAI.h"


int main()
{
    srand(time(NULL));
    SnakeGameAI game;
    constexpr int BLOCK_SIZE = 20;

    InitWindow(640, 480, "SnakeAI");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        for (auto [x, y]: game.snake)
        {
            DrawRectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLUE);
            DrawRectangle(
                    x * BLOCK_SIZE + 4, y * BLOCK_SIZE + 4, BLOCK_SIZE - 8, BLOCK_SIZE - 8,
                    SKYBLUE);
        }
        DrawRectangle(
                game.food.x * BLOCK_SIZE, game.food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, RED);
        DrawText(TextFormat("Score: %d", game.score), 0, 0, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
