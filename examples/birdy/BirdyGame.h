#pragma once

#include <array>
#include <vector>
#include <raylib.h>

#define GAME_INIT_SPEEDX 2
#define BIRD_JUMP_FORCE 4
#define BIRD_GRAVITY (-0.08)
#define BIRD_X 50

#define CYAN CLITERAL(Color){0, 255, 255, 255} // CYAN
#define NUM_COLORS 8
#define NUM_BIRD_SPRITES 3
#define ANIMATION_SPEED (0.03)

typedef struct Sprite
{
    Color color;
    size_t frame_index;
    float frame;
    float animation_speed;
    size_t num_textures;
    Texture *textures;
} Sprite;

class BirdyGame
{
public:

    explicit BirdyGame(size_t num_birds);
    ~BirdyGame();

    void reset();
    void draw();
    void update();

    typedef enum bird_state_t
    {
        BIRD_STATE_FLYING = 0,
        BIRD_STATE_PARACHUTE,
        BIRD_STATE_DEAD,
    } bird_state_t;

    typedef struct Bird
    {
        double x{}, y{};
        double direction{};
        double width{}, height{};
        double distance{};
        double angle{};
        bird_state_t state{};
        Sprite sprite{};
    } Bird;

    std::vector<Bird> birds{};

    typedef struct Floor
    {
        double x{}, y{};
        Texture *texture{};
    } Floor;

    std::array<Floor, 8> floors;

private:

    Color colors[NUM_COLORS] = {GRAY, YELLOW, GREEN, RED, BLUE, CYAN, ORANGE, PURPLE};
    Texture bird_sprites[NUM_BIRD_SPRITES]{};
    Texture parachute_texture{};
    Texture floor_texture{};

    double distance{};
    double gravity{};
    double speed_x{};
    size_t num_dead{};
    size_t best_bird_index{};

    void load_textures();
    void unload_textures() const;
};
