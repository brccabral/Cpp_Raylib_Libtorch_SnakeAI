#pragma once
#include <vector>
#include <array>
#include <raylib.h>


#define GAME_INIT_SPEEDX 2
#define DINO_JUMP_LIMIT 60
#define DINO_JUMP_FORCE 4
#define DINO_WIDTH_STANDUP 40
#define DINO_HEIGHT_STANDUP 43
#define DINO_WIDTH_CROUCH 56
#define DINO_HEIGHT_CROUCH 25
#define DINO_WIDTH_PLANE 70
#define DINO_HEIGHT_PLANE 37
#define DINO_PLANE_Y 85
#define DINO_PLANE_COOLDOWN 500
#define DINO_GRAVITY (-0.08)
#define DINO_X 50
#define OBSTACLE_INIT_X 800

#define CYAN CLITERAL(Color){0, 255, 255, 255} // CYAN
#define NUM_COLORS 8
#define NUM_DINO_SPRITES 12
#define NUM_OBSTACLE_SPRITES 8
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

class DinoGame
{
public:

    DinoGame(size_t num_dinos_, size_t num_obstacles_);
    ~DinoGame();

    typedef enum dino_actions_t
    {
        DINO_ACTION_NONE = 0,
        DINO_ACTION_JUMP,
        DINO_ACTION_CROUCH,
        DINO_ACTION_FLY,
        // DINO_ACTION_COUNT must be last
        DINO_ACTION_COUNT
    } dino_actions_t;

    void reset();
    void update();
    [[nodiscard]] std::vector<float> get_state(size_t dino_index) const;
    void apply_action(size_t dino_index, dino_actions_t action);
    static size_t get_state_size();
    [[nodiscard]] bool check_end_game() const;
    [[nodiscard]] std::array<size_t, 2> select_best_dinos() const;

    typedef enum dino_state_t
    {
        DINO_STATE_STANDUP = 0,
        DINO_STATE_CROUCH,
        DINO_STATE_JUMP,
        DINO_STATE_DEAD,
        DINO_STATE_FLYING
    } dino_state_t;

    typedef enum obstacle_type_t
    {
        OBSTACLE_TYPE_BIRD = 0,
        OBSTACLE_TYPE_CACTUS,
        OBSTACLE_TYPE_SPIKES,
    } obstacle_type_t;

    typedef enum obstacle_sub_type_t
    {
        OBSTACLE_SUBTYPE_CACTUS_0 = 0,
        OBSTACLE_SUBTYPE_CACTUS_1,
        OBSTACLE_SUBTYPE_CACTUS_2,
        OBSTACLE_SUBTYPE_CACTUS_3,
        OBSTACLE_SUBTYPE_CACTUS_4,
        OBSTACLE_SUBTYPE_BIRD_5,
        OBSTACLE_SUBTYPE_BIRD_6,
        OBSTACLE_SUBTYPE_SPIKES_7,
    } obstacle_sub_type_t;

    typedef struct Dino
    {
        double x{}, y{};
        double direction{};
        double width{}, height{};
        double plane_cooldown{};
        double distance{};
        dino_state_t state{};
        Sprite sprite{};
    } Dino;

    typedef struct Obstacle
    {
        obstacle_type_t type{};
        obstacle_sub_type_t subtype{};
        double x{}, y{};
        double width{}, height{};
        Sprite sprite{};
    } Obstacle;

    std::vector<Dino> dinos{};
    std::vector<Obstacle> obstacles{};
    double distance{};
    size_t num_dead{};
    size_t first_obstacle{};

private:

    double speedX{};
    double jump_force{};
    double gravity{};
    double plane_cooldown{};
    size_t num_dinos{};
    size_t num_obstacles{};
    size_t collision_index{};
    size_t last_obstacle{};
    size_t best_dino_index{};
    bool running{};

    Color colors[NUM_COLORS] = {GRAY, YELLOW, GREEN, RED, BLUE, CYAN, ORANGE, PURPLE};
    Texture dino_sprites[NUM_DINO_SPRITES]{};
    Texture obstacle_sprites[NUM_OBSTACLE_SPRITES]{};

    void reset_dinos();
    void random_obstacles();
    static void get_random_obstacle(Obstacle *obstacle);
    static void get_random_cactus(Obstacle *obstacle);
    static void get_random_bird(Obstacle *obstacle);
    static void get_spikes(Obstacle *obstacle);
    static float get_obstacle_distance(const Dino *dino, const Obstacle *obstacle);
    static bool check_collision(const Dino *dino, const Obstacle *obstacle);

    void load_textures();
    void unload_textures();
};
