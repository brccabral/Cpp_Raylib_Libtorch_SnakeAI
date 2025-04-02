#pragma once

#include <array>
#include <vector>
#include <raylib.h>

#define GAME_INIT_SPEEDX 2
#define BIRD_JUMP_FORCE 7
#define BIRD_JUMP_COOLDOWN 20
#define BIRD_PARACHUTE_COOLDOWN 200
#define BIRD_GRAVITY (-0.3)
#define BIRD_X 50
#define PIPES_GAP 175
#define PIPES_GAP_STEADY 75
#define PIPES_DISTANCE 275

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
    typedef enum bird_action_t
    {
        BIRD_ACTION_NONE = 0,
        BIRD_ACTION_JUMP,
        BIRD_ACTION_PARACHUTE,
        BIRD_ACTION_COUNT,
    } bird_action_t;

    void reset();
    void draw();
    void update();
    void apply_action(size_t bird_index, bird_action_t action);
    [[nodiscard]] bool check_end_game() const;
    static size_t get_state_size();

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
        double action_cooldown{};
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

    typedef struct Pipe
    {
        double x{}, y{};
        double width{}, height{};
        double speed_y{};
        Color color{};
        Texture *texture{};
    } Pipe;

    std::array<Pipe, 16> pipes;

private:

    Color colors[NUM_COLORS] = {GRAY, YELLOW, GREEN, RED, BLUE, CYAN, ORANGE, PURPLE};
    Texture bird_sprites[NUM_BIRD_SPRITES]{};
    Texture parachute_texture{};
    Texture floor_texture{};
    Texture pipe_down_texture{};
    Texture pipe_up_texture{};

    double distance{};
    double gravity{};
    double speed_x{};
    double jump_force{};
    size_t num_dead{};
    size_t best_bird_index{};
    size_t first_pipe{};
    size_t last_pipe{};

    void pipe_status(size_t index);
    bool collision(const Bird *bird) const;

    void load_textures();
    void unload_textures() const;
};
