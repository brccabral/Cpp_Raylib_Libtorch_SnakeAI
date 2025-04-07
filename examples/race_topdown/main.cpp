#include <raylib.h>
#include "RaceTopDown.h"


int main()
{
    constexpr int SCREEN_WIDTH = 1366;
    constexpr int SCREEN_HEIGHT = 768;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Race TopDown");

    float turn_cooldown = 0.0f;
    float acc_cooldown = 0.0f;

    {
        auto game = RaceTopDown(15);
        while (!WindowShouldClose())
        {
            const float delta_time = GetFrameTime();

            int action = 0;
            if (IsKeyDown(KEY_LEFT) && turn_cooldown <= 0)
            {
                action = 1;
                turn_cooldown = 0.003;
            }
            if (IsKeyDown(KEY_RIGHT) && turn_cooldown <= 0)
            {
                action = 2;
                turn_cooldown = 0.003;
            }
            if (IsKeyDown(KEY_UP) && acc_cooldown <= 0)
            {
                action = 3;
                acc_cooldown = 0.003;
            }
            if (IsKeyDown(KEY_DOWN) && acc_cooldown <= 0)
            {
                action = 4;
                acc_cooldown = 0.003;
            }
            game.apply_action(0, action);

            turn_cooldown -= delta_time;
            acc_cooldown -= delta_time;
            turn_cooldown = std::max(turn_cooldown, 0.0f);
            acc_cooldown = std::max(acc_cooldown, 0.0f);

            game.update();
            game.draw();
        }
    }
    CloseWindow();
}
