#include <ctime>
#include <random>
#include "BirdyGame.h"

#include <mlgames/GenPopulation.h>
#include <mlgames/LinearGen.h>

#define MANUAL 0

int main()
{
    // NOLINTNEXTLINE
    srand(time(NULL));

    constexpr int hidden_layer_1 = 8;
    constexpr size_t count_birds = 200;

    constexpr int screen_width = 1366;
    constexpr int screen_height = 768;
    InitWindow(screen_width, screen_height, "Birdy");
#if MANUAL
    SetTargetFPS(60);
#endif

    {
        auto game = BirdyGame(count_birds);

        float record = 0;

        const auto net = LinearGen(
                BirdyGame::get_state_size(), BirdyGame::BIRD_ACTION_COUNT,
                std::vector<size_t>{hidden_layer_1});
        auto population = GenPopulation(count_birds, 0.9, 0.05, net);

        torch::NoGradGuard no_grad;

        size_t generation = 0;

        while (!WindowShouldClose())
        {
#if MANUAL
            BirdyGame::bird_action_t action = BirdyGame::BIRD_ACTION_NONE;
            if (IsKeyPressed(KEY_SPACE))
            {
                action = BirdyGame::BIRD_ACTION_JUMP;
            }
            if (IsKeyPressed(KEY_LEFT_CONTROL))
            {
                action = BirdyGame::BIRD_ACTION_PARACHUTE;
            }
            std::vector<float> inputs = game.get_state(0);
            game.apply_action(0, action);
#else
            for (size_t i = 0; i < count_birds; ++i)
            {
                if (game.birds[i].state == BirdyGame::BIRD_STATE_DEAD)
                {
                    continue;
                }
                std::vector<float> inputs = game.get_state(i);
                torch::Tensor x = torch::tensor(inputs, torch::kFloat)
                                          .reshape({1, (long) BirdyGame::get_state_size()});
                auto actions = population.members[i]->forward(x);
                auto action = torch::argmax(actions).item().toInt();
                game.apply_action(i, (BirdyGame::bird_action_t) action);
            }
#endif
            game.update();
            game.draw();
            if (game.check_end_game())
            {
                if (game.distance > record)
                {
                    record = game.distance;
                }
                printf("Generation %zu Distance: %.0f Dead: %zu Record %.0f\n", generation,
                       game.distance, game.num_dead, record);
                ++generation;
                save_model<>(population.members[game.best_bird_index]);
                population.apply_mutations(game.select_best_dinos());
                game.reset();
            }
        }
    }

    CloseWindow();
}
