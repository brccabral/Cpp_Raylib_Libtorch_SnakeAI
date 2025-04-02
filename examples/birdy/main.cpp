#include <ctime>
#include <random>
#include "BirdyGame.h"

int main()
{
    srand(time(NULL));

    constexpr int screen_width = 1366;
    constexpr int screen_height = 768;
    InitWindow(screen_width, screen_height, "Birdy");

    {
        auto game = BirdyGame(200);

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
