#pragma once
#include <list>
#include <raylib.h>
#include <vector>

class SkiObject
{
public:

    SkiObject() = default;
    ~SkiObject() = default;

    void update();

    enum object_type
    {
        TYPE_SKIER = 0,
        TYPE_SLALOM_ARROW_LEFT,
        TYPE_SLALOM_ARROW_RIGHT,
        TYPE_MOGUL_GROUP,
        TYPE_NOVICE,
        TYPE_DOG,
        TYPE_SNOWBOARDER,
        TYPE_ROCK,
        TYPE_STUMP,
        TYPE_MOGUL_SMALL,
        TYPE_MOGUL_LARGE,
        TYPE_TREE_SMALL,
        TYPE_TREE_DRIED,
        TYPE_TREE_LARGE,
        TYPE_RAINBOW,
        TYPE_START_LEFT,
        TYPE_START_RIGHT,
        TYPE_FINISH_LEFT,
        TYPE_FINISH_RIGHT,
        TYPE_SLALOM_SIGN,
        TYPE_TREE_SLALOM_SIGN,
        TYPE_FREESTYLE_SIGN,
        TYPE_LIFT_POLE,
        TYPE_LIFT_CHAIR,
        TYPE_YETI,
        TYPE_URINE,
        TYPE_TREE_WALK
    };

    enum object_state
    {
        STATE_PLAYER_LEFT,
        STATE_PLAYER_RIGHT,
        STATE_PLAYER_30_LEFT,
        STATE_PLAYER_30_RIGHT,
        STATE_PLAYER_60_LEFT,
        STATE_PLAYER_60_RIGHT,
        STATE_PLAYER_SIT,
        STATE_PLAYER_JUMP,
        STATE_PLAYER_HIT,
        STATE_PLAYER_DOWN,
        STATE_SLALOM_LEFT,
        STATE_SLALOM_RIGHT,
        STATE_SLALOM_SUCCESS,
        STATE_SLALOM_FAIL,
        STATE_NOVICE_NORMAL,
        STATE_NOVICE_SIT,
        STATE_NOVICE_GROUND,
        STATE_DOG_LEFT,
        STATE_DOG_RIGHT,
        STATE_DOG_HIT,
        STATE_SNOWBOARDER_LEFT,
        STATE_SNOWBOARDER_RIGHT,
        STATE_SNOWBOARDER_360,
        STATE_TREE_DRIED_NORMAL,
        STATE_TREE_DRIED_FIRE,
        STATE_LIFT_CHAIR_FULL,
        STATE_LIFT_CHAIR_NOVICE,
        STATE_LIFT_CHAIR_EMPTY,
        STATE_YETI_LEFT,
        STATE_YETI_RIGHT,
        STATE_YETI_LEFT_UP,
        STATE_YETI_RIGHT_UP,
        STATE_YETI_HAPPY,
        STATE_YETI_EATING,
        STATE_TREE_WALK_LEFT,
        STATE_TREE_WALK_RIGHT,
    };

    object_type type{};
    Vector2 position{};
    Vector2 direction{};
    size_t current_frame_index{};
    Rectangle current_frame_rectangle{};
    object_state state;
    double speed;
};

class SkiFree
{
public:

    SkiFree();
    ~SkiFree();

    void inputs();
    void update();
    void draw();

private:

    Texture2D all_textures;
    std::vector<Rectangle> frames;

    enum game_state_t
    {
        STATE_PRACTICE = 0,
        STATE_SLALOM,
        STATE_FREESTYLE,
        STATE_TREE_SLALOM
    };

    std::list<SkiObject *> long_live_objects;
    std::list<SkiObject> short_live_objects;

    Camera2D camera{};
    SkiObject player;

    game_state_t current_state{};
};
