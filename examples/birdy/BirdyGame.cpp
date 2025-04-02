#include <cstdio>
#include <random>
#include "BirdyGame.h"


BirdyGame::BirdyGame(size_t num_birds)
{
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
}

void BirdyGame::draw()
{
    BeginDrawing();
    ClearBackground(SKYBLUE);
    for (size_t i = 0; i < birds.size(); ++i)
    {
        const auto *bird = &birds[i];
        DrawTexture(
                bird->sprite.textures[bird->sprite.frame_index], bird->x, bird->y,
                bird->sprite.color);
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
    parachute = LoadTexture("assets/parachute.png");
}

void BirdyGame::unload_textures() const
{
    for (const auto &bird_sprite: bird_sprites)
    {
        UnloadTexture(bird_sprite);
    }
    UnloadTexture(parachute);
}
