#include <algorithm>
#include <cstdio>
#include <random>
#include "BirdyGame.h"


BirdyGame::BirdyGame(size_t num_birds)
{
    gravity = BIRD_GRAVITY;
    speed_x = GAME_INIT_SPEEDX;
    jump_force = BIRD_JUMP_FORCE;

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
    first_pipe = 0;
    last_pipe = pipes.size() - 1;

    for (size_t i = 0; i < birds.size(); ++i)
    {
        auto *bird = &birds[i];
        // NOLINTNEXTLINE
        bird->x = BIRD_X + rand() % 100;
        // NOLINTNEXTLINE
        bird->y = GetScreenHeight() / 2;
        bird->direction = 0;
        bird->angle = 0;
        bird->distance = 0;
        bird->action_cooldown = 0;
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

    prev_x = GetScreenWidth() - 100;
    for (size_t i = 0; i < pipes.size(); i += 2)
    {
        pipes[i].x = prev_x;
        pipes[i].texture = &pipe_down_texture;
        pipes[i].width = pipe_down_texture.width;
        pipes[i].height = pipe_down_texture.height;
        pipes[i + 1].x = prev_x;
        pipes[i + 1].texture = &pipe_up_texture;
        pipes[i + 1].width = pipe_up_texture.width;
        pipes[i + 1].height = pipe_up_texture.height;

        pipe_status(i);

        prev_x = pipes[i].x + PIPES_DISTANCE;
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

    for (auto &pipe: pipes)
    {
        DrawTexture(*pipe.texture, pipe.x, GetScreenHeight() - pipe.y, pipe.color);
    }

    for (size_t i = 0; i < birds.size(); ++i)
    {
        const auto *bird = &birds[i];
        if (bird->state == BIRD_STATE_PARACHUTE)
        {
            DrawTexture(
                    parachute_texture, bird->x,
                    GetScreenHeight() - bird->y - bird->height - parachute_texture.height, WHITE);
        }
        DrawTexturePro(
                bird->sprite.textures[bird->sprite.frame_index],
                Rectangle(0.0, 0.0, bird->width, bird->height),
                Rectangle(
                        bird->x + bird->width / 2.0,
                        GetScreenHeight() - bird->y - bird->height / 2.0, bird->width,
                        bird->height),
                Vector2(bird->width / 2.0, bird->height / 2.0), bird->angle, bird->sprite.color);
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

    Image pipe = LoadImage("assets/pipe.png");
    pipe_down_texture = LoadTextureFromImage(pipe);
    ImageRotate(&pipe, 180);
    pipe_up_texture = LoadTextureFromImage(pipe);
    UnloadImage(pipe);
}

void BirdyGame::unload_textures() const
{
    for (const auto &bird_sprite: bird_sprites)
    {
        UnloadTexture(bird_sprite);
    }
    UnloadTexture(parachute_texture);
    UnloadTexture(floor_texture);
    UnloadTexture(pipe_down_texture);
    UnloadTexture(pipe_up_texture);
}

void BirdyGame::update()
{
    distance += speed_x;

    for (auto &floor: floors)
    {
        floor.x -= speed_x;
        if (floor.x + floor.texture->width < 0)
        {
            floor.x = GetScreenWidth() - 1;
        }
    }
    for (size_t i = 0; i < pipes.size(); i += 2)
    {
        Pipe *pipe_down = &pipes[i];
        Pipe *pipe_up = &pipes[i + 1];
        pipe_down->x -= speed_x;
        pipe_up->x -= speed_x;
        if (pipe_down->speed_y != 0)
        {
            pipe_down->y += pipe_down->speed_y;
            pipe_up->y += pipe_up->speed_y;
            if (pipe_down->speed_y > 0 &&
                pipe_down->y - pipe_down->height > GetScreenHeight() - PIPES_GAP)
            {
                pipe_down->speed_y *= -1;
                pipe_up->speed_y *= -1;
            }
            if (pipe_up->speed_y < 0 && pipe_up->y < PIPES_GAP)
            {
                pipe_down->speed_y *= -1;
                pipe_up->speed_y *= -1;
            }
        }
        if (pipe_down->x + pipe_down->texture->width < 0)
        {
            first_pipe = (first_pipe + 2) % pipes.size();
            pipe_down->x = pipes[last_pipe].x + PIPES_DISTANCE;
            pipe_up->x = pipes[last_pipe].x + PIPES_DISTANCE;
            last_pipe = i;
            pipe_status(i);
        }
    }
    for (size_t i = 0; i < birds.size(); ++i)
    {
        auto *bird = &birds[i];
        if (bird->x + bird->width < 0)
        {
            continue;
        }
        if (bird->state == BIRD_STATE_DEAD)
        {
            bird->x -= speed_x;
            continue;
        }

        best_bird_index = i;

        bird->distance += speed_x;
        bird->action_cooldown -= 1.0;
        if (bird->state == BIRD_STATE_FLYING)
        {
            bird->angle += 1.0;
            bird->angle = std::max(std::min(bird->angle, 88.0), 0.0);
            bird->direction += gravity;
            bird->y += bird->direction;
        }
        else if (bird->state == BIRD_STATE_PARACHUTE)
        {
            if (bird->action_cooldown < 0)
            {
                bird->state = BIRD_STATE_FLYING;
            }
        }

        if (collision(bird) || bird->y > (GetScreenHeight() + PIPES_GAP) || bird->y < 75)
        {
            birds[i].state = BIRD_STATE_DEAD;
            ++num_dead;
        }
    }
}

void BirdyGame::apply_action(size_t bird_index, bird_action_t action)
{
    auto *bird = &birds[bird_index];
    if (bird->action_cooldown > 0)
    {
        return;
    }
    switch (action)
    {
        case BIRD_ACTION_JUMP:
        {
            bird->direction = jump_force;
            bird->action_cooldown = BIRD_JUMP_COOLDOWN;
            bird->angle = 0;
            bird->state = BIRD_STATE_FLYING;
            break;
        }
        case BIRD_ACTION_PARACHUTE:
        {
            bird->direction = 0;
            bird->action_cooldown = BIRD_PARACHUTE_COOLDOWN;
            bird->angle = 20;
            bird->state = BIRD_STATE_PARACHUTE;
            break;
        }
        default:
            break;
    }
}

void BirdyGame::pipe_status(size_t index)
{
    // NOLINTNEXTLINE
    double prob = (rand() % 10000) / 100.0;
    if (prob < 10)
    {
        pipes[index].color = RED;
        pipes[index + 1].color = RED;

        pipes[index].y = pipes[index].height + GetScreenHeight() / 2.0 + PIPES_GAP_STEADY / 2.0;
        pipes[index + 1].y = GetScreenHeight() / 2.0 - PIPES_GAP_STEADY / 2.0;

        pipes[index].speed_y = 0;
        pipes[index + 1].speed_y = 0;
    }
    else
    {
        pipes[index].color = WHITE;
        pipes[index + 1].color = WHITE;

        // NOLINTNEXTLINE
        const double rand_y = 50 + PIPES_GAP + (rand() % (GetScreenHeight() - PIPES_GAP * 2 - 50));

        pipes[index].y = pipes[index].height + rand_y + PIPES_GAP / 2.0;
        pipes[index + 1].y = rand_y - PIPES_GAP / 2.0;

        int direction = (rand() % 2) * 2 - 1;
        double speed_y = (rand() % 500) / 100.0;
        pipes[index].speed_y = direction * speed_y;
        pipes[index + 1].speed_y = direction * speed_y;
    }
}

bool BirdyGame::collision(const Bird *bird) const
{
    for (const auto &pipe: pipes)
    {
        const auto rect_pipe = Rectangle(pipe.x, pipe.y - pipe.height, pipe.width, pipe.height);
        const auto rect_bird =
                Rectangle(bird->x, bird->y - bird->height, bird->width, bird->height);
        if (CheckCollisionRecs(rect_pipe, rect_bird))
        {
            return true;
        }
    }
    return false;
}

bool BirdyGame::check_end_game() const
{
    return num_dead >= birds.size();
}

size_t BirdyGame::get_state_size()
{
    return 4;
}

std::vector<float> BirdyGame::get_state(size_t index)
{
    std::vector<float> result;
    result.resize(get_state_size());

    const auto *bird = &birds[index];
    const auto *pipe = find_next_pipe(bird);

    result[0] = distance_x_to_obstacle(bird, pipe);
    result[1] = distance_y_to_obstacle(bird, pipe);
    result[2] = pipe->speed_y;
    result[3] = pipe->color == RED ? PIPES_GAP_STEADY : PIPES_GAP;

    return result;
}

BirdyGame::Pipe *BirdyGame::find_next_pipe(const Bird *bird)
{
    Pipe *result = &pipes[first_pipe];
    if (result->x + result->width < bird->x)
    {
        result = &pipes[(first_pipe + 2) % pipes.size()];
    }
    return result;
}

float BirdyGame::distance_x_to_obstacle(const Bird *bird, const Pipe *pipe)
{
    return pipe->x - bird->x;
}

float BirdyGame::distance_y_to_obstacle(const Bird *bird, const Pipe *pipe)
{
    return pipe->y - bird->y;
}

std::array<size_t, 2> BirdyGame::select_best_dinos() const
{
    std::vector<std::pair<double, size_t>> birds_distances;
    birds_distances.reserve(birds.size());
    for (size_t i = 0; i < birds.size(); ++i)
    {
        birds_distances.emplace_back(birds[i].distance, i);
    }
    std::ranges::sort(
            birds_distances, [](const auto &a, const auto &b) { return a.first > b.first; });
    return {birds_distances[0].second, birds_distances[1].second};
}
