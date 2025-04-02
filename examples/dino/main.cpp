#include <cstdio>
#include <raylib.h>
#include "DinoGame.h"

#include <mlgames/GenPopulation.h>
#include <mlgames/LinearGen.h>


void draw_dinos(const DinoGame *game, const size_t screen_height)
{
    for (auto &dino: game->dinos)
    {
        DrawTexture(
                dino.sprite.textures[dino.sprite.frame_index], dino.x,
                screen_height - dino.y - dino.height, dino.sprite.color);
    }
}
void draw_obstacles(const DinoGame *game, const size_t screen_height)
{
    for (auto &obstacle: game->obstacles)
    {
        DrawTexture(
                obstacle.sprite.textures[obstacle.sprite.frame_index], obstacle.x,
                screen_height - obstacle.y - obstacle.height, obstacle.sprite.color);
    }
}

int main()
{
    constexpr size_t screen_width = 1366;
    constexpr size_t screen_height = 768;
    constexpr size_t count_dinos = 200;

    InitWindow(screen_width, screen_height, "Dino");

    {
        auto game = DinoGame(count_dinos, 2000);

        size_t record = 0;

        const auto net = std::make_shared<LinearGen>(
                DinoGame::get_state_size(), DinoGame::DINO_ACTION_COUNT, std::vector<size_t>{8});
        auto population = GenPopulation(count_dinos, 0.9, net);

        torch::NoGradGuard no_grad;

        while (!WindowShouldClose())
        {
            for (size_t d = 0; d < count_dinos; ++d)
            {
                if (game.dinos[d].state == DinoGame::DINO_STATE_DEAD)
                {
                    continue;
                }
                std::vector<float> inputs = game.get_state(d);
                torch::Tensor x = torch::tensor(inputs, torch::kFloat)
                                          .reshape({1, (long) DinoGame::get_state_size()});
                auto actions = population.members[d]->forward(x);
                auto action = torch::argmax(actions).item().toInt();
                game.apply_action(d, (DinoGame::dino_actions_t) action);
            }

            game.update();

            BeginDrawing();
            ClearBackground(WHITE);
            DrawFPS(10, 300);
            DrawText(TextFormat("Distance: %.0f", game.distance), 10, 320, 10, BLACK);
            DrawText(TextFormat("Dead: %zu", game.num_dead), 10, 330, 10, BLACK);
            DrawText(TextFormat("Obstacles: %zu", game.first_obstacle), 10, 340, 10, BLACK);
            DrawText(TextFormat("Record: %zu", record), 10, 350, 10, BLACK);
            draw_dinos(&game, screen_height);
            draw_obstacles(&game, screen_height);
            EndDrawing();

            if (game.check_end_game())
            {
                if (game.first_obstacle > record)
                {
                    record = game.first_obstacle;
                }
                printf("Distance: %.0f Dead: %zu Obstacles: %zu Record %zu\n", game.distance,
                       game.num_dead, game.first_obstacle, record);
                population.apply_mutations(game.select_best_dinos());
                game.reset();
            }
        }
    }

    CloseWindow();
}
