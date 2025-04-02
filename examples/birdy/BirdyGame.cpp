#include <cstdio>
#include <random>
#include "BirdyGame.h"


BirdyGame::BirdyGame(size_t num_birds)
{
    gravity = BIRD_GRAVITY;
    speed_x = GAME_INIT_SPEEDX;

    birds.resize(num_birds);
    load_textures();
    reset();
}

BirdyGame::~BirdyGame()
{
    unload_textures();
}

void BirdyGame::reset()
{
    num_dead = 0;
    distance = 0;
    best_bird_index = 0;

    for (size_t i = 0; i < birds.size(); ++i)
    {
        auto *bird = &birds[i];
        // NOLINTNEXTLINE
        bird->x = BIRD_X + rand() % 100;
        // NOLINTNEXTLINE
        bird->y = GetScreenHeight() / 2;
        bird->direction = 0;
        bird->distance = 0;
        bird->state = BIRD_STATE_FLYING;
        bird->sprite.color = colors[i % NUM_COLORS];
        bird->sprite.frame = 0;
        bird->sprite.frame_index = 0;
        bird->sprite.textures = bird_sprites;
        bird->sprite.animation_speed = ANIMATION_SPEED;
        bird->width = bird->sprite.textures[bird->sprite.frame_index].width;
        bird->height = bird->sprite.textures[bird->sprite.frame_index].height;
    }

    double prev_x{};
    for (auto &floor: floors)
    {
        floor.x = prev_x;
        floor.y = 75;
        floor.texture = &floor_texture;
        prev_x = floor.x + floor.texture->width - 1;
    }
}

void BirdyGame::draw()
{
    BeginDrawing();
    ClearBackground(SKYBLUE);

    for (auto &floor: floors)
    {
        DrawTexture(*floor.texture, floor.x, GetScreenHeight() - floor.y, WHITE);
    }

    for (size_t i = 0; i < birds.size(); ++i)
    {
        const auto *bird = &birds[i];
        DrawTexture(
                bird->sprite.textures[bird->sprite.frame_index], bird->x,
                GetScreenHeight() - bird->y - bird->height, bird->sprite.color);
    }
    EndDrawing();
}

void BirdyGame::load_textures()
{
    for (size_t i = 0; i < NUM_BIRD_SPRITES; ++i)
    {
        char filename[100]{};
        (void) snprintf(filename, sizeof(filename) - 1, "assets/bird%zu.png", i);
        filename[sizeof(filename) - 1] = '\0';
        bird_sprites[i] = LoadTexture(filename);
    }
    parachute_texture = LoadTexture("assets/parachute.png");
    floor_texture = LoadTexture("assets/floor.png");
}

void BirdyGame::unload_textures() const
{
    for (const auto &bird_sprite: bird_sprites)
    {
        UnloadTexture(bird_sprite);
    }
    UnloadTexture(parachute_texture);
    UnloadTexture(floor_texture);
}

void BirdyGame::update()
{
    for (auto &floor: floors)
    {
        floor.x -= speed_x;
        if (floor.x + floor.texture->width < 0)
        {
            floor.x = GetScreenWidth() - 1;
        }
    }
    for (size_t i = 0; i < birds.size(); ++i)
    {
        auto *bird = &birds[i];
        if (bird->state == BIRD_STATE_DEAD)
        {
            continue;
        }
        bird->direction += gravity;
        bird->y += bird->direction;
        if (bird->y < 75)
        {
            bird->state = BIRD_STATE_DEAD;
        }
    }
}
