#include <raylib.h>
#include "DinoGame.h"

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
    InitWindow(screen_width, screen_height, "Dino");

    {
        auto game = DinoGame(2000, 2000);

        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(WHITE);
            DrawFPS(10, 300);
            DrawText(TextFormat("Distance: %f", game.distance), 10, 320, 10, BLACK);
            DrawText(TextFormat("Dead: %lu", game.num_dead), 10, 330, 10, BLACK);
            DrawText(TextFormat("Obstacles: %lu", game.first_obstacle), 10, 340, 10, BLACK);
            draw_dinos(&game, screen_height);
            draw_obstacles(&game, screen_height);
            EndDrawing();
        }
    }

    CloseWindow();
}
