#include <raylib.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "PegSolitaire.h"
#include <mlgames/Agent.h>
#include <mlgames/Linear_QNet.h>
#include <mlgames/QTrainer.h>


#define LR (0.001)
#define GAMMA (0.9)
#define HIDDEN_SIZE (256)
// memory usage (if CPU -> RAM, if GPU -> GPU_MEM)
#define BATCH_SIZE (10000)

#define do_shift(argc, argv)                                                                       \
    do                                                                                             \
    {                                                                                              \
        if ((argc) > 1)                                                                            \
        {                                                                                          \
            (argv)++;                                                                              \
            (argc)--;                                                                              \
        }                                                                                          \
    }                                                                                              \
    while (0)


void draw_board(const PegSolitaire *game, const float peg_size, const Vector2 peg_dimension)
{
    const PegSolitaire::peg_status *pegs = game->get_pegs();
    if (pegs == NULL)
    {
        return;
    }

    const size_t cols = game->get_cols();
    const size_t rows = game->get_rows();
    const size_t cursor = game->get_cursor();
    const int selected = game->get_selected();


    for (size_t r = 0; r < rows; ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            const Vector2 position = {c * peg_size, r * peg_size};
            const size_t index = game->index_from_2d(c, r);
            switch (pegs[index])
            {
                case PegSolitaire::PEG_STATUS_EMPTY:
                {
                    DrawRectangleV(position, peg_dimension, WHITE);
                    DrawRectangleLines(
                            position.x, position.y, peg_dimension.x, peg_dimension.y, BLACK);
                    break;
                }
                case PegSolitaire::PEG_STATUS_FILLED:
                {
                    DrawRectangleV(position, peg_dimension, DARKGREEN);
                    DrawRectangleLines(
                            position.x, position.y, peg_dimension.x, peg_dimension.y, BLACK);
                    break;
                }
                case PegSolitaire::PEG_STATUS_TRACED:
                {
                    DrawRectangleV(position, peg_dimension, DARKBLUE);
                    DrawRectangleLines(
                            position.x, position.y, peg_dimension.x, peg_dimension.y, BLACK);
                    break;
                }
                default:
                {
                    break;
                }
            }
            DrawRectangleLinesEx(
                    (Rectangle) {(cursor % cols) * peg_size, (cursor / cols) * peg_size,
                                 peg_dimension.y, peg_dimension.y},
                    3, YELLOW);
            if (selected == -1)
            {
                if (game->can_move(c, r))
                {
                    DrawRectangle(
                            c * peg_size + peg_size / 2, r * peg_size + peg_size / 2, peg_size / 4,
                            peg_size / 4, RED);
                }
            }
            else
            {
                if (game->can_receive(c, r))
                {
                    DrawRectangle(
                            c * peg_size + peg_size / 2, r * peg_size + peg_size / 2, peg_size / 4,
                            peg_size / 4, RED);
                }
            }
        }
    }

    if (selected != -1)
    {
        const Vector2 position = {
                ((selected % cols) * peg_size) + (peg_size / 2),
                ((selected / cols) * peg_size) + (peg_size / 2)};
        DrawCircleV(position, peg_dimension.x / 2, YELLOW);
    }
}

void draw_game(
        const PegSolitaire *game, size_t peg_size, Vector2 peg_dimension, int reward,
        int best_score)
{
    BeginDrawing();
    ClearBackground(BLACK);
    draw_board(game, peg_size, peg_dimension);
    switch (game->status)
    {
        case PegSolitaire::GAME_OVER_LOST:
        {
            DrawText("Game Over - You Lost", 500, 20, 20, RED);
            break;
        }
        case PegSolitaire::GAME_OVER_WON:
        {
            DrawText("Game Over - You Win", 500, 20, 20, GREEN);
            break;
        }
        default:
        {
            DrawText("Playing", 500, 20, 20, WHITE);
            break;
        }
    }
    DrawText(TextFormat("Reward %d", reward), 500, 40, 20, WHITE);
    DrawText(TextFormat("Selections %lu", game->selections), 500, 60, 20, WHITE);
    DrawText(TextFormat("Score %lu", game->get_score()), 500, 80, 20, WHITE);
    DrawText(TextFormat("Best %lu", best_score), 500, 100, 20, WHITE);
    EndDrawing();
}

int main(int argc, char *argv[])
{
    // NOLINTNEXTLINE
    srand(time(NULL));

    c10::DeviceType device = torch::kCPU;
    if (torch::cuda::is_available())
    {
        printf("Using CUDA\n");
        device = torch::kCUDA;
    }
    else
    {
        printf("Using CPU\n");
        device = torch::kCPU;
    }

    const char *net_filename = NULL;
    while (argc > 1)
    {
        char *command = argv[1];
        if (strcmp(command, "--net") == 0)
        {
            net_filename = argv[2];
            do_shift(argc, argv);
        }
        do_shift(argc, argv);
    }

    PegSolitaire game;
    game.init_game(PegSolitaire::BOARD_TYPE_ENGLISH);
    game.set_status(PegSolitaire::GAME_PLAYING);

    auto model = Linear_QNet(game.get_state_size(), HIDDEN_SIZE, game.get_action_count());
    model->to(device);
    model->train();
    auto optimizer = torch::optim::Adam(model->parameters(), torch::optim::AdamOptions{LR});
    auto trainer = QTrainer(&model, &optimizer, GAMMA, device);

    Agent agent(&model, &trainer, device, BATCH_SIZE);

    size_t best_score = game.get_score();

    InitWindow(800, 600, "Peg Solitaire");

    constexpr uint peg_size = 64;
    constexpr Vector2 peg_dimension = {peg_size, peg_size};

    bool is_manual = false;
    int reward{};
    bool game_over = false;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_M))
        {
            is_manual = !is_manual;
        }

        if (is_manual)
        {
            if (IsKeyPressed(KEY_R))
            {
                game.reset();
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                auto [mouse_x, mouse_y] = GetMousePosition();
                if (mouse_x < game.get_cols() * peg_size && mouse_y < game.get_rows() * peg_size)
                {
                    int x = (int) mouse_x / peg_size;
                    int y = (int) mouse_y / peg_size;
                    int index = game.index_from_2d(x, y);
                    auto [r, g] = game.get_step(index);
                    reward = r;
                    game_over = g;
                }
            }
            draw_game(&game, peg_size, peg_dimension, reward, best_score);
        }
        else
        {
            auto state_old = game.get_state();
            auto action = agent.get_action(state_old, 1);

            int index{};
            for (size_t i = 0; i < action.size(); ++i)
            {
                if (action[i] == 1)
                {
                    index = i;
                    break;
                }
            }

            auto [r, g] = game.get_step(index);
            reward = r;
            game_over = g;

            draw_game(&game, peg_size, peg_dimension, reward, best_score);

            auto state_new = game.get_state();
            agent.train_short_memory(state_old, action, reward, state_new, game_over);

            agent.remember(state_old, action, reward, state_new, game_over);

            if (game_over)
            {
                ++agent.number_of_games;
                size_t score = game.get_score();
                if (score < best_score)
                {
                    best_score = score;
                    save_model(&model);
                }

                printf("Game %d Score %zu Record %zu time %.2f batch %d max memory %d\n",
                       agent.number_of_games, score, best_score, GetTime(), BATCH_SIZE, MAX_MEMORY);

                // train long memory (also called replay memory, or experience replay)
                agent.train_long_memory();
                game.reset();
            }
        }
    }

    CloseWindow();
}
