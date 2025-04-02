#include <ctime>
#include <random>
#include "BirdyGame.h"

#include <mlgames/GenPopulation.h>
#include <mlgames/LinearGen.h>

int main()
{
    srand(time(NULL));

    constexpr int hidden_layer_1 = 6;
    constexpr size_t count_birds = 200;

    constexpr int screen_width = 1366;
    constexpr int screen_height = 768;
    InitWindow(screen_width, screen_height, "Birdy");

    {
        auto game = BirdyGame(count_birds);

        size_t record = 0;

        const auto net = LinearGen(
                BirdyGame::get_state_size(), BirdyGame::BIRD_ACTION_COUNT,
                std::vector<size_t>{hidden_layer_1});
        auto population = GenPopulation(count_birds, 0.9, net);

        torch::NoGradGuard no_grad;

        size_t generation = 0;

        while (!WindowShouldClose())
        {
            for (size_t i = 0; i < 200; ++i)
            {
                if (game.birds[i].state == BirdyGame::BIRD_STATE_DEAD)
                {
                    continue;
                }
                int action_prob = rand() % 100;
                int action = BirdyGame::BIRD_ACTION_PARACHUTE;
                if (action_prob < 90)
                {
                    action = BirdyGame::BIRD_ACTION_NONE;
                }
                else if (action_prob < 95)
                {
                    action = BirdyGame::BIRD_ACTION_JUMP;
                }
                game.apply_action(i, (BirdyGame::bird_action_t) action);
            }
            game.update();
            game.draw();
            if (game.check_end_game())
            {
                game.reset();
            }
        }
    }

    CloseWindow();
}
