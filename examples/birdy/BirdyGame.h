#pragma once

#include <array>
#include <vector>
#include <raylib.h>

#define GAME_INIT_SPEEDX 2
#define BIRD_JUMP_FORCE 6
#define BIRD_JUMP_COOLDOWN 1
#define BIRD_PARACHUTE_COOLDOWN 100
#define BIRD_GRAVITY (-0.3)
#define BIRD_X 50
#define PIPES_GAP 175
#define PIPES_GAP_STEADY 90
#define PIPES_DISTANCE 275
#define ALLOW_PIPE_STEADY_COOLDOWN 2500

#define CYAN CLITERAL(Color){0, 255, 255, 255} // CYAN
#define NUM_COLORS 9
#define NUM_BIRD_SPRITES 3
#define ANIMATION_SPEED (0.03)

typedef struct SkiObject
{
    Color color{};
    size_t frame_index{};
    float frame{};
    float animation_speed{};
    size_t num_textures{};
    Texture *textures{};
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
    [[nodiscard]] bool check_end_game();
    static size_t get_state_size();
    std::vector<float> get_state(size_t dino_index);
    [[nodiscard]] std::array<size_t, 2> select_best_dinos() const;

    double distance{};
    size_t num_dead{};
    size_t best_bird_index{};

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

    Color colors[NUM_COLORS] = {GRAY, YELLOW, GREEN, RED, BLUE, CYAN, ORANGE, PURPLE, WHITE};
    Texture bird_sprites[NUM_BIRD_SPRITES]{};
    Texture parachute_texture{};
    Texture floor_texture{};
    Texture pipe_down_texture{};
    Texture pipe_up_texture{};

    double gravity{};
    double speed_x{};
    double jump_force{};
    double allow_pipe_steady_cooldown{};
    size_t first_pipe{};
    size_t last_pipe{};
    size_t count_pipes{};
    size_t num_runs{};

    void pipe_status(size_t index);
    bool collision(const Bird *bird) const;
    Pipe *find_next_pipe(const Bird *bird);
    static float distance_x_to_obstacle(const Bird *bird, const Pipe *pipe);
    static float distance_y_to_obstacle(const Bird *bird, const Pipe *pipe);

    void load_textures();
    void unload_textures() const;
};

inline bool operator==(Color c1, Color c2)
{
    return c1.a == c2.a && c1.b == c2.b && c1.g == c2.g && c1.r == c2.r;
}
