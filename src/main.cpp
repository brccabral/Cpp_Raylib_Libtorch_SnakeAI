#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "SnakeGameAI.h"
#include "Agent.h"
#include "QTrainer.h"


#define LR 0.001
#define GAMMA 0.9

int main()
{
    srand(time(NULL));

    auto model = Linear_QNet(INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
    auto trainer = QTrainer(&model, LR, GAMMA);

    Agent agent(&model, &trainer);
    SnakeGameAI game;
    constexpr int BLOCK_SIZE = 20;

    InitWindow(640, 480, "SnakeAI");

    while (!WindowShouldClose())
    {
        auto state_old = game.get_state();
        auto action = agent.get_action(state_old);

        auto step_result = game.get_step(action);

        auto state_new = game.get_state();
        agent.train_short_memory(
                state_old, action, step_result.reward, state_new, step_result.game_over);

        if (step_result.game_over)
        {
            game.reset();
        }

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
