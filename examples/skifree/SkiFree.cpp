#include "SkiFree.h"


SkiFree::SkiFree()
{
    all_textures = LoadTexture("assets/images.bmp");
    int prev_y = 0;
    frames.emplace_back(0, 0, 16, 32); // skier_down
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 16, 32); // skier_30_left
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 28); // skier_60_left
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 28); // skier_left_0
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 16, 32); // skier_30_right
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 28); // skier_60_right
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 28); // skier_right_0
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 28); // skier_left_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 28); // skier_right_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 28); // skier_left_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 28); // skier_right_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 32); // skier_ouch
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 24); // skier_sit
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 32); // skier_jump_0
    frames.emplace_back(
            0, prev_y += frames[frames.size() - 1].height, 28, 31); // skier_trick_left_1
    frames.emplace_back(
            0, prev_y += frames[frames.size() - 1].height, 28, 31); // skier_trick_right_1
    frames.emplace_back(
            0, prev_y += frames[frames.size() - 1].height, 28, 34); // skier_trick_backwards
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 26); // skier_hit
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 32); // skier_jump_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 31, 24); // skier_jump_2
    frames.emplace_back(
            0, prev_y += frames[frames.size() - 1].height, 25, 31); // skier_trick_left_2
    frames.emplace_back(
            0, prev_y += frames[frames.size() - 1].height, 25, 31); // skier_trick_right_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 12, 24); // slalom_arrow_left
    frames.emplace_back(
            0, prev_y += frames[frames.size() - 1].height, 12, 24); // slalom_arrow_right
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 12, 24); // slalom_success
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 12, 24); // slalom_fail
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 64, 32); // mogul_group
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 30); // novice_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 22, 29); // novice_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 21, 29); // novice_3
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 24); // novice_sit
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 24); // novice_ground
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 21, 15); // dog_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 21, 15); // dog_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 19, 19); // dog_hit_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 19, 19); // dog_hit_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 26, 30); // snowboarder_left
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 20, 30); // snowboarder_right
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 25, 31); // snowboarder_360_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 30, 29); // snowboarder_360_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 32); // snowboarder_360_3
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 32); // snowboarder_360_4
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 25, 29); // snowboarder_360_5
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 29, 25); // snowboarder_360_6
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 23, 11); // rock
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 16, 11); // stump
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 16, 4); // mogul_small
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 8); // mogul_large
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 28, 32); // tree_small
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 22, 27); // tree_dried
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 64); // tree_large
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 8); // rainbow
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 93, 57); // title
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 52, 10); // version
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 92, 30); // numpad
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 63, 32); // restart_pause
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 42, 27); // start_left
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 42, 27); // start_right
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 50, 29); // finish_left
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 50, 29); // finish_right
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 40, 36); // slalom_sign
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 44, 36); // tree_slalom_sign
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 40, 35); // freestyle_sign
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 24, 64); // lift_pole
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 26, 32); // lift_chair_full
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 26, 32); // lift_chair_novice
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 26, 32); // lift_chair_empty
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_happy_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_happy_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_left_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_left_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_right_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_right_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_right_up_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_right_up_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_eating_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_eating_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_eating_3
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_eating_4
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_eating_5
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 32, 48); // yeti_eating_6
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 16, 8); // urine
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 22, 27); // tree_fire_1
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 22, 27); // tree_fire_2
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 22, 27); // tree_fire_3
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 8, 11); // mushroom
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 28, 32); // tree_walk_idle
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 28, 32); // tree_walk_right
    frames.emplace_back(0, prev_y += frames[frames.size() - 1].height, 28, 32); // tree_walk_left

    long_live_objects.emplace_back();
    SkiObject *player = &long_live_objects.front();
    player->type = SkiObject::TYPE_SKIER;
    player->position = Vector2(20, 20);
    player->current_frame_index = 3;
    player->current_frame_rectangle = frames[player->current_frame_index];
};

SkiFree::~SkiFree()
{
    UnloadTexture(all_textures);
}

void SkiFree::draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    for (const auto &long_live_object: long_live_objects)
    {
        DrawTextureRec(
                all_textures, frames[long_live_object.current_frame_index],
                long_live_object.position, WHITE);
    }
    EndDrawing();
}

void SkiFree::inputs()
{
    if (IsKeyPressed(KEY_RIGHT))
    {
        SkiObject *obj = &long_live_objects.front();
        ++obj->current_frame_index;
        if (obj->current_frame_index >= frames.size())
        {
            obj->current_frame_index = 0;
        }
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        SkiObject *obj = &long_live_objects.front();
        if (obj->current_frame_index == 0)
        {
            obj->current_frame_index = frames.size();
        }
        --obj->current_frame_index;
    }
}
