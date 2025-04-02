#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "DinoGame.h"


// cactus
static DinoGame::Obstacle obs0 = {
        DinoGame::OBSTACLE_TYPE_CACTUS, DinoGame::OBSTACLE_SUBTYPE_CACTUS_0, 0, 0, 32, 33};
static DinoGame::Obstacle obs1 = {
        DinoGame::OBSTACLE_TYPE_CACTUS, DinoGame::OBSTACLE_SUBTYPE_CACTUS_1, 0, 0, 23, 46};
static DinoGame::Obstacle obs2 = {
        DinoGame::OBSTACLE_TYPE_CACTUS, DinoGame::OBSTACLE_SUBTYPE_CACTUS_2, 0, 0, 15, 33};
static DinoGame::Obstacle obs3 = {
        DinoGame::OBSTACLE_TYPE_CACTUS, DinoGame::OBSTACLE_SUBTYPE_CACTUS_3, 0, 0, 49, 33};
static DinoGame::Obstacle obs4 = {
        DinoGame::OBSTACLE_TYPE_CACTUS, DinoGame::OBSTACLE_SUBTYPE_CACTUS_4, 0, 0, 73, 47};
// bird
static DinoGame::Obstacle obs5 = {
        DinoGame::OBSTACLE_TYPE_BIRD, DinoGame::OBSTACLE_SUBTYPE_BIRD_5, 0, 27, 42, 36};
static DinoGame::Obstacle obs6 = {
        DinoGame::OBSTACLE_TYPE_BIRD, DinoGame::OBSTACLE_SUBTYPE_BIRD_6, 0, 27, 42, 36};
// spikes
static DinoGame::Obstacle obs7 = {
        DinoGame::OBSTACLE_TYPE_SPIKES, DinoGame::OBSTACLE_SUBTYPE_SPIKES_7, 0, 0, 810, 31};


DinoGame::DinoGame(size_t num_dinos_, size_t num_obstacles_)
{
    // NOLINTNEXTLINE
    srand(time(NULL));

    num_dinos = num_dinos_;
    dinos.resize(num_dinos);
    num_obstacles = num_obstacles_;
    obstacles.resize(num_obstacles);

    speedX = GAME_INIT_SPEEDX;
    jump_force = DINO_JUMP_FORCE;
    gravity = DINO_GRAVITY;
    plane_cooldown = DINO_PLANE_COOLDOWN;

    load_textures();

    reset();
}

DinoGame::~DinoGame()
{
    unload_textures();
}


void DinoGame::reset()
{
    running = true;
    first_obstacle = 0;
    last_obstacle = 10;
    num_dead = 0;
    distance = 0;
    best_dino_index = 0;
    collision_index = 0;
    reset_dinos();
    random_obstacles();
}

void DinoGame::reset_dinos()
{
    for (size_t i = 0; i < dinos.size(); ++i)
    {
        auto *dino = &dinos[i];
        // NOLINTNEXTLINE
        dino->x = DINO_X + rand() % 50;
        dino->y = 0;
        dino->direction = 0;
        dino->width = DINO_WIDTH_STANDUP;
        dino->height = DINO_HEIGHT_STANDUP;
        dino->plane_cooldown = 0;
        dino->distance = 0;
        dino->state = DINO_STATE_STANDUP;
        dino->sprite.color = colors[i % NUM_COLORS];
        dino->sprite.num_textures = NUM_DINO_SPRITES;
        dino->sprite.textures = dino_sprites;
        dino->sprite.animation_speed = ANIMATION_SPEED;
        dino->sprite.frame = 0;
        dino->sprite.frame_index = 0;
    }
}

void DinoGame::random_obstacles()
{
    double offset_x = OBSTACLE_INIT_X;
    for (auto &obstacle: obstacles)
    {
        get_random_obstacle(&obstacle);
        obstacle.x = offset_x;

        obstacle.sprite.color = WHITE;
        obstacle.sprite.num_textures = NUM_OBSTACLE_SPRITES;
        obstacle.sprite.textures = obstacle_sprites;
        obstacle.sprite.animation_speed = ANIMATION_SPEED;
        obstacle.sprite.frame = obstacle.subtype;
        obstacle.sprite.frame_index = obstacle.subtype;

        // NOLINTNEXTLINE
        offset_x = obstacle.x + obstacle.width + 200 + rand() % 300;
    }
}

void DinoGame::get_random_obstacle(Obstacle *obstacle)
{
    // NOLINTNEXTLINE
    const int rnd = rand() % 100;
    obstacle_type_t obstacle_type = OBSTACLE_TYPE_CACTUS;
    if (rnd >= 50 && rnd <= 98)
    {
        obstacle_type = OBSTACLE_TYPE_BIRD;
    }
    else if (rnd > 98)
    {
        obstacle_type = OBSTACLE_TYPE_SPIKES;
    }
    switch (obstacle_type)
    {
        case OBSTACLE_TYPE_CACTUS:
        {
            get_random_cactus(obstacle);
            break;
        }
        case OBSTACLE_TYPE_BIRD:
        {
            get_random_bird(obstacle);
            break;
        }
        case OBSTACLE_TYPE_SPIKES:
        {
            get_spikes(obstacle);
            break;
        }
    }
}


void DinoGame::get_random_cactus(Obstacle *obstacle)
{
    // NOLINTNEXTLINE
    switch (rand() % 5)
    {
        case 0:
        {
            *obstacle = obs0;
            break;
        }
        case 1:
        {
            *obstacle = obs1;
            break;
        }
        case 2:
        {
            *obstacle = obs2;
            break;
        }
        case 3:
        {
            *obstacle = obs3;
            break;
        }
        case 4:
        {
            *obstacle = obs4;
            break;
        }
        default:
        {
            (void) fprintf(stderr, "ERROR: Invalid obstacle type\n");
            exit(-1);
        }
    }
}

void DinoGame::get_random_bird(Obstacle *obstacle)
{
    // NOLINTNEXTLINE
    switch (rand() % 2 + 5)
    {
        case 5:
        {
            *obstacle = obs5;
            break;
        }
        case 6:
        {
            *obstacle = obs6;
            break;
        }
        default:
        {
            (void) fprintf(stderr, "ERROR: Invalid obstacle type\n");
            exit(-1);
        }
    }
    // NOLINTNEXTLINE
    obstacle->y += rand() % 65;
}

void DinoGame::get_spikes(Obstacle *obstacle)
{
    *obstacle = obs7;
}

void DinoGame::load_textures()
{
    for (int i = 0; i < NUM_DINO_SPRITES; ++i)
    {
        char filename[100] = {};
        (void) snprintf(filename, sizeof(filename) - 1, "assets/dino%d.png", i);
        filename[sizeof(filename) - 1] = '\0';
        dino_sprites[i] = LoadTexture(filename);
    }

    for (int i = 0; i < NUM_OBSTACLE_SPRITES; ++i)
    {
        char filename[100] = {};
        (void) snprintf(filename, sizeof(filename) - 1, "assets/obs%d.png", i);
        filename[sizeof(filename) - 1] = '\0';
        obstacle_sprites[i] = LoadTexture(filename);
    }
}

void DinoGame::unload_textures()
{
    for (const auto &dino_sprite: dino_sprites)
    {
        UnloadTexture(dino_sprite);
    }
    for (const auto &obstacle_sprite: obstacle_sprites)
    {
        UnloadTexture(obstacle_sprite);
    }
}

void DinoGame::update()
{
    for (size_t i = first_obstacle; i < num_obstacles; ++i)
    {
        auto *obstacle = &obstacles[i];
        if (obstacle->x + obstacle->width < 0)
        {
            first_obstacle = i + 1;
            ++last_obstacle;
            continue;
        }
        obstacle->x -= speedX;
        if (obstacle->type == OBSTACLE_TYPE_BIRD)
        {
            obstacle->sprite.frame += ANIMATION_SPEED;
            if (obstacle->sprite.frame >= 7)
            {
                obstacle->sprite.frame = 5;
            }
            obstacle->sprite.frame_index = obstacle->sprite.frame;
        }
    }

    distance += speedX;

    const Obstacle *collision_obstacle = &obstacles[collision_index];

    for (size_t i = 0; i < num_dinos; ++i)
    {
        Dino *dino = &dinos[i];
        if (dino->x < -dino->width)
        {
            continue;
        }
        if (dino->state == DINO_STATE_DEAD)
        {
            dino->x -= speedX;
            dino->sprite.frame = 4;
            dino->sprite.frame_index = 4;
            continue;
        }

        dino->distance += speedX;
        dino->sprite.frame += ANIMATION_SPEED;

        best_dino_index = i;
        if (dino->state == DINO_STATE_STANDUP)
        {
            if (dino->sprite.frame >= 2)
            {
                dino->sprite.frame = 0;
            }
        }
        else if (dino->state == DINO_STATE_CROUCH)
        {
            if (dino->sprite.frame >= 4)
            {
                dino->sprite.frame = 2;
            }
        }
        else if (dino->state == DINO_STATE_JUMP)
        {
            dino->direction += gravity;
            dino->y += dino->direction;
            if (dino->sprite.frame >= 6)
            {
                dino->sprite.frame = 4;
            }
            if (dino->y < 0)
            {
                dino->state = DINO_STATE_STANDUP;
                dino->y = 0;
                dino->direction = 0;
                dino->height = DINO_HEIGHT_STANDUP;
                dino->width = DINO_WIDTH_STANDUP;
                dino->sprite.frame = 0;
            }
        }
        else if (dino->state == DINO_STATE_FLYING)
        {
            dino->plane_cooldown -= 1;
            if (dino->sprite.frame >= 12)
            {
                dino->sprite.frame = 10;
            }
            if (dino->plane_cooldown <= 0)
            {
                dino->state = DINO_STATE_STANDUP;
                dino->y = 0;
                dino->direction = 0;
                dino->plane_cooldown = 0;
                dino->height = DINO_HEIGHT_STANDUP;
                dino->width = DINO_WIDTH_STANDUP;
                dino->sprite.frame = 0;
            }
        }

        dino->sprite.frame_index = dino->sprite.frame;

        if (check_collision(dino, &obstacles[collision_index]))
        {
            dino->state = DINO_STATE_DEAD;
            dino->width = DINO_WIDTH_STANDUP;
            dino->height = DINO_HEIGHT_STANDUP;
            dino->sprite.frame = 6;
            ++num_dead;
        }
    }

    if (DINO_X > collision_obstacle->x + collision_obstacle->width)
    {
        collision_index = (collision_index + 1) % num_obstacles;
    }
}

bool DinoGame::check_collision(const Dino *dino, const Obstacle *obstacle)
{
    if (dino->x + dino->width < obstacle->x)
    {
        return false;
    }
    if (dino->x > obstacle->x + obstacle->width)
    {
        return false;
    }
    if (dino->y + dino->height < obstacle->y)
    {
        return false;
    }
    if (dino->y > obstacle->y + obstacle->height)
    {
        return false;
    }
    return true;
}

size_t DinoGame::get_state_size()
{
    return 6;
}


std::vector<float> DinoGame::get_state(size_t dino_index) const
{
    std::vector<float> result;
    result.resize(get_state_size());

    result[0] = get_obstacle_distance(&dinos[dino_index], &obstacles[collision_index]);
    result[1] = obstacles[collision_index].width;
    result[2] = obstacles[collision_index].height;
    result[3] = obstacles[collision_index].y;
    result[4] = speedX;
    result[5] = dinos[dino_index].y;

    return result;
}

float DinoGame::get_obstacle_distance(const Dino *dino, const Obstacle *obstacle)
{
    return obstacle->x - dino->x;
}

void DinoGame::apply_action(size_t dino_index, dino_actions_t action)
{
    auto *dino = &dinos[dino_index];
    switch (action)
    {
        case DINO_ACTION_JUMP:
        {
            if (dino->y > 0)
            {
                break;
            }
            dino->state = DINO_STATE_JUMP;
            dino->height = DINO_HEIGHT_STANDUP;
            dino->width = DINO_WIDTH_STANDUP;
            dino->direction = jump_force;
            dino->y = 0;
            dino->sprite.frame = 4;
            break;
        }
        case DINO_ACTION_CROUCH:
        {
            if (dino->y != 0)
            {
                break;
            }
            if (dino->state != DINO_STATE_CROUCH)
            {
                dino->sprite.frame = 2;
            }
            dino->state = DINO_STATE_CROUCH;
            dino->width = DINO_WIDTH_CROUCH;
            dino->height = DINO_HEIGHT_CROUCH;
            dino->direction = 0;
            dino->y = 0;
            break;
        }
        case DINO_ACTION_FLY:
        {
            if (dino->y != 0)
            {
                break;
            }
            if (dino->state != DINO_ACTION_FLY)
            {
                dino->sprite.frame = 10;
            }
            dino->state = DINO_STATE_FLYING;
            dino->width = DINO_WIDTH_PLANE;
            dino->height = DINO_HEIGHT_PLANE;
            dino->direction = 0;
            dino->y = DINO_PLANE_Y;
            dino->plane_cooldown = plane_cooldown;
            break;
        }
        case DINO_ACTION_NONE:
        {
            if (dino->plane_cooldown > 0)
            {
                break;
            }
            if (dino->y > 0)
            {
                break;
            }
            if (dino->state != DINO_STATE_STANDUP)
            {
                dino->sprite.frame = 0;
            }
            dino->state = DINO_STATE_STANDUP;
            dino->width = DINO_WIDTH_STANDUP;
            dino->height = DINO_HEIGHT_STANDUP;
            dino->direction = 0;
            dino->y = 0;
            break;
        }
        default:
        {
            (void) fprintf(stderr, "ERROR: Invalid action\n");
            break;
        }
    }
}

bool DinoGame::check_end_game() const
{
    return num_dinos == num_dead || first_obstacle >= num_obstacles;
}

std::array<size_t, 2> DinoGame::select_best_dinos() const
{
    std::vector<std::pair<double, size_t>> dinos_distances;
    dinos_distances.reserve(num_dinos);
    for (size_t i = 0; i < dinos.size(); ++i)
    {
        dinos_distances.emplace_back(dinos[i].distance, i);
    }
    std::ranges::sort(
            dinos_distances, [](const auto &a, const auto &b) { return a.first > b.first; });
    return {dinos_distances[0].second, dinos_distances[1].second};
}
