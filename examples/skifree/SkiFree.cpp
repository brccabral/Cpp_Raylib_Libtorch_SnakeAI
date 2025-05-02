#include "SkiFree.h"


SkiFree::SkiFree()
{
    all_textures = LoadTexture("assets/images.bmp");
    frames.resize(89);
    int prev_y = 0;
    frames[0] = Rectangle(0, prev_y += 0, 16, 32); // skier_down
    frames[1] = Rectangle(0, prev_y += 32, 16, 32); // skier_30_left
    frames[2] = Rectangle(0, prev_y += 32, 24, 28); // skier_60_left
    frames[3] = Rectangle(0, prev_y += 28, 24, 28); // skier_left_0
    frames[4] = Rectangle(0, prev_y += 28, 16, 32); // skier_30_right
    frames[5] = Rectangle(0, prev_y += 32, 24, 28); // skier_60_right
    frames[6] = Rectangle(0, prev_y += 28, 24, 28); // skier_right_0
    frames[7] = Rectangle(0, prev_y += 28, 24, 28); // skier_left_1
    frames[8] = Rectangle(0, prev_y += 28, 24, 28); // skier_right_1
    frames[9] = Rectangle(0, prev_y += 28, 24, 28); // skier_left_2
    frames[10] = Rectangle(0, prev_y += 28, 24, 28); // skier_right_2
    frames[11] = Rectangle(0, prev_y += 28, 32, 32); // skier_ouch
    frames[12] = Rectangle(0, prev_y += 32, 32, 24); // skier_sit
    frames[13] = Rectangle(0, prev_y += 24, 32, 32); // skier_jump_0
    frames[14] = Rectangle(0, prev_y += 32, 28, 31); // skier_trick_left_1
    frames[15] = Rectangle(0, prev_y += 31, 28, 31); // skier_trick_right_1
    frames[16] = Rectangle(0, prev_y += 31, 28, 34); // skier_trick_backwards
    frames[17] = Rectangle(0, prev_y += 34, 32, 26); // skier_hit
    frames[18] = Rectangle(0, prev_y += 26, 32, 32); // skier_jump_1
    frames[19] = Rectangle(0, prev_y += 32, 31, 24); // skier_jump_2
    frames[20] = Rectangle(0, prev_y += 24, 25, 31); // skier_trick_left_2
    frames[21] = Rectangle(0, prev_y += 31, 25, 31); // skier_trick_right_2
    frames[22] = Rectangle(0, prev_y += 31, 12, 24); // slalom_arrow_left
    frames[23] = Rectangle(0, prev_y += 24, 12, 24); // slalom_arrow_right
    frames[24] = Rectangle(0, prev_y += 24, 12, 24); // slalom_success
    frames[25] = Rectangle(0, prev_y += 24, 12, 24); // slalom_fail
    frames[26] = Rectangle(0, prev_y += 24, 64, 32); // mogul_group
    frames[27] = Rectangle(0, prev_y += 32, 24, 30); // novice_1
    frames[28] = Rectangle(0, prev_y += 30, 22, 29); // novice_2
    frames[29] = Rectangle(0, prev_y += 29, 21, 29); // novice_3
    frames[30] = Rectangle(0, prev_y += 29, 24, 24); // novice_sit
    frames[31] = Rectangle(0, prev_y += 24, 24, 24); // novice_ground
    frames[32] = Rectangle(0, prev_y += 24, 21, 15); // dog_1
    frames[33] = Rectangle(0, prev_y += 15, 21, 15); // dog_2
    frames[34] = Rectangle(0, prev_y += 15, 19, 19); // dog_hit_1
    frames[35] = Rectangle(0, prev_y += 19, 19, 19); // dog_hit_2
    frames[36] = Rectangle(0, prev_y += 19, 26, 30); // snowboarder_left
    frames[37] = Rectangle(0, prev_y += 30, 20, 30); // snowboarder_right
    frames[38] = Rectangle(0, prev_y += 30, 25, 31); // snowboarder_360_1
    frames[39] = Rectangle(0, prev_y += 31, 30, 29); // snowboarder_360_2
    frames[40] = Rectangle(0, prev_y += 29, 32, 32); // snowboarder_360_3
    frames[41] = Rectangle(0, prev_y += 32, 32, 32); // snowboarder_360_4
    frames[42] = Rectangle(0, prev_y += 32, 25, 29); // snowboarder_360_5
    frames[43] = Rectangle(0, prev_y += 29, 29, 25); // snowboarder_360_6
    frames[44] = Rectangle(0, prev_y += 25, 23, 11); // rock
    frames[45] = Rectangle(0, prev_y += 11, 16, 11); // stump
    frames[46] = Rectangle(0, prev_y += 11, 16, 4); // mogul_small
    frames[47] = Rectangle(0, prev_y += 4, 24, 8); // mogul_large
    frames[48] = Rectangle(0, prev_y += 8, 28, 32); // tree_small
    frames[49] = Rectangle(0, prev_y += 32, 22, 27); // tree_dried
    frames[50] = Rectangle(0, prev_y += 27, 32, 64); // tree_large
    frames[51] = Rectangle(0, prev_y += 64, 32, 8); // rainbow
    frames[52] = Rectangle(0, prev_y += 8, 93, 57); // title
    frames[53] = Rectangle(0, prev_y += 57, 52, 10); // version
    frames[54] = Rectangle(0, prev_y += 10, 92, 30); // numpad
    frames[55] = Rectangle(0, prev_y += 30, 63, 32); // restart_pause
    frames[56] = Rectangle(0, prev_y += 32, 42, 27); // start_left
    frames[57] = Rectangle(0, prev_y += 27, 42, 27); // start_right
    frames[58] = Rectangle(0, prev_y += 27, 50, 29); // finish_left
    frames[59] = Rectangle(0, prev_y += 29, 50, 29); // finish_right
    frames[60] = Rectangle(0, prev_y += 29, 40, 36); // slalom_sign
    frames[61] = Rectangle(0, prev_y += 36, 44, 36); // tree_slalom_sign
    frames[62] = Rectangle(0, prev_y += 36, 40, 35); // freestyle_sign
    frames[63] = Rectangle(0, prev_y += 35, 24, 64); // lift_pole
    frames[64] = Rectangle(0, prev_y += 64, 26, 32); // lift_chair_full
    frames[65] = Rectangle(0, prev_y += 32, 26, 32); // lift_chair_novice
    frames[66] = Rectangle(0, prev_y += 32, 26, 32); // lift_chair_empty
    frames[67] = Rectangle(0, prev_y += 32, 32, 48); // yeti_happy_1
    frames[68] = Rectangle(0, prev_y += 48, 32, 48); // yeti_happy_2
    frames[69] = Rectangle(0, prev_y += 48, 32, 48); // yeti_left_1
    frames[70] = Rectangle(0, prev_y += 48, 32, 48); // yeti_left_2
    frames[71] = Rectangle(0, prev_y += 48, 32, 48); // yeti_right_1
    frames[72] = Rectangle(0, prev_y += 48, 32, 48); // yeti_right_2
    frames[73] = Rectangle(0, prev_y += 48, 32, 48); // yeti_right_up_1
    frames[74] = Rectangle(0, prev_y += 48, 32, 48); // yeti_right_up_2
    frames[75] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_1
    frames[76] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_2
    frames[77] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_3
    frames[78] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_4
    frames[79] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_5
    frames[80] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_6
    frames[81] = Rectangle(0, prev_y += 48, 16, 8); // urine
    frames[82] = Rectangle(0, prev_y += 8, 22, 27); // tree_fire_1
    frames[83] = Rectangle(0, prev_y += 27, 22, 27); // tree_fire_2
    frames[84] = Rectangle(0, prev_y += 27, 22, 27); // tree_fire_3
    frames[85] = Rectangle(0, prev_y += 27, 8, 11); // mushroom
    frames[86] = Rectangle(0, prev_y += 11, 28, 32); // tree_walk_idle
    frames[87] = Rectangle(0, prev_y += 32, 28, 32); // tree_walk_right
    frames[88] = Rectangle(0, prev_y += 32, 28, 32); // tree_walk_left

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
    SkiObject *player = &long_live_objects.front();
    if (IsKeyDown(KEY_RIGHT))
    {
        switch (player->state)
        {
            case SkiObject::STATE_PLAYER_RIGHT:
            {
                // 6, 8, 10
                if (player->current_frame_index == 6)
                {
                    player->current_frame_index = 8;
                }
                else if (player->current_frame_index == 8)
                {
                    player->current_frame_index = 10;
                }
                else if (player->current_frame_index == 10)
                {
                    player->current_frame_index = 6;
                }
                break;
            }
            case SkiObject::STATE_PLAYER_LEFT:
            {
                player->state = SkiObject::STATE_PLAYER_60_LEFT;
                player->current_frame_index = 2;
                break;
            }
            case SkiObject::STATE_PLAYER_60_LEFT:
            {
                player->state = SkiObject::STATE_PLAYER_30_LEFT;
                player->current_frame_index = 1;
                break;
            }
            case SkiObject::STATE_PLAYER_30_LEFT:
            {
                player->state = SkiObject::STATE_PLAYER_DOWN;
                player->current_frame_index = 0;
                break;
            }
            case SkiObject::STATE_PLAYER_DOWN:
            {
                player->state = SkiObject::STATE_PLAYER_30_RIGHT;
                player->current_frame_index = 4;
                break;
            }
            case SkiObject::STATE_PLAYER_30_RIGHT:
            {
                player->state = SkiObject::STATE_PLAYER_60_RIGHT;
                player->current_frame_index = 5;
                break;
            }
            case SkiObject::STATE_PLAYER_60_RIGHT:
            {
                player->state = SkiObject::STATE_PLAYER_RIGHT;
                player->current_frame_index = 6;
                break;
            }
            default:
                break;
        }
    }
    if (IsKeyDown(KEY_LEFT))
    {
        switch (player->state)
        {
            case SkiObject::STATE_PLAYER_LEFT:
            {
                // 3, 7, 9
                if (player->current_frame_index == 3)
                {
                    player->current_frame_index = 7;
                }
                else if (player->current_frame_index == 7)
                {
                    player->current_frame_index = 9;
                }
                else if (player->current_frame_index == 9)
                {
                    player->current_frame_index = 3;
                }
                break;
            }
            case SkiObject::STATE_PLAYER_RIGHT:
            {
                player->state = SkiObject::STATE_PLAYER_60_RIGHT;
                player->current_frame_index = 5;
                break;
            }
            case SkiObject::STATE_PLAYER_60_RIGHT:
            {
                player->state = SkiObject::STATE_PLAYER_30_RIGHT;
                player->current_frame_index = 4;
                break;
            }
            case SkiObject::STATE_PLAYER_30_RIGHT:
            {
                player->state = SkiObject::STATE_PLAYER_DOWN;
                player->current_frame_index = 0;
                break;
            }
            case SkiObject::STATE_PLAYER_DOWN:
            {
                player->state = SkiObject::STATE_PLAYER_30_LEFT;
                player->current_frame_index = 1;
                break;
            }
            case SkiObject::STATE_PLAYER_30_LEFT:
            {
                player->state = SkiObject::STATE_PLAYER_60_LEFT;
                player->current_frame_index = 2;
                break;
            }
            case SkiObject::STATE_PLAYER_60_LEFT:
            {
                player->state = SkiObject::STATE_PLAYER_LEFT;
                player->current_frame_index = 3;
                break;
            }
            default:
                break;
        }
    }
}
