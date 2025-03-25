#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "SnakeGameAI.h"
#include "Agent.h"
#include "Linear_QNet.h"
#include "QTrainer.h"


#define LR (0.001)
#define GAMMA (0.9)

int main()
{
    srand(time(NULL));

    c10::DeviceType device = torch::kCPU;
    // if (torch::cuda::is_available())
    // {
    //     printf("Using CUDA\n");
    //     device = torch::kCUDA;
    // }
    // else
    // {
    //     printf("Using CPU\n");
    //     device = torch::kCPU;
    // }

    auto model = Linear_QNet(INPUT_SIZE, HIDDEN_SIZE, SnakeGameAI::ACTION_COUNT);
    model->to(device);
    model->train();
    auto optimizer = torch::optim::Adam(model->parameters(), torch::optim::AdamOptions{LR});
    auto trainer =
            QTrainer<INPUT_SIZE, SnakeGameAI::ACTION_COUNT>(&model, &optimizer, GAMMA, device);

    Agent<INPUT_SIZE, SnakeGameAI::ACTION_COUNT> agent(&model, &trainer, device);
    SnakeGameAI game;
    constexpr int BLOCK_SIZE = 20;

    int best_score = 0;

    InitWindow(640, 480, "SnakeAI");

    while (!WindowShouldClose())
    {
        auto state_old = game.get_state();
        auto action = agent.get_action(state_old);

        SnakeGameAI::action_t game_action{};
        for (size_t i = 0; i < action.size(); ++i)
        {
            if (action[i] == 1)
            {
                game_action = (SnakeGameAI::action_t) i;
                break;
            }
        }

        auto step_result = game.get_step(game_action);

        auto state_new = game.get_state();
        agent.train_short_memory(
                state_old, action, step_result.reward, state_new, step_result.game_over);

        agent.remember(state_old, action, step_result.reward, state_new, step_result.game_over);

        if (step_result.game_over)
        {
            ++agent.number_of_games;
            if (game.score > best_score)
            {
                best_score = game.score;
                save_model(&model);
            }

            printf("Game %d Score %d Record %d\n", agent.number_of_games, game.score, best_score);

            // train long memory (also called replay memory, or experience replay)
            agent.train_long_memory();
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
