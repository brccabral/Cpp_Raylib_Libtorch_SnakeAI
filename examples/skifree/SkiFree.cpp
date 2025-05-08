#include "SkiFree.h"

#include <cassert>
#include <cstdio>
#include <raymath.h>


void SkiObject::update()
{
    switch (type)
    {
        case TYPE_SKIER:
        {
            switch (state)
            {
                case STATE_PLAYER_30_LEFT:
                case STATE_PLAYER_30_RIGHT:
                {
                    speed += 0.3;
                    break;
                }
                case STATE_PLAYER_60_LEFT:
                case STATE_PLAYER_60_RIGHT:
                {
                    speed += 0.6;
                    break;
                }
                case STATE_PLAYER_DOWN:
                {
                    speed += 1;
                    break;
                }
                case STATE_PLAYER_LEFT:
                case STATE_PLAYER_RIGHT:
                {
                    if (speed > 0)
                    {
                        speed -= 5;
                    }
                    break;
                }
                default:
                    break;
            }
            if (speed > 25)
            {
                speed = 25;
            }
            if (std::abs(speed) < 0.001)
            {
                speed = 0;
            }
            speed = std::max(speed, 0.0);
            break;
        }
        case TYPE_YETI:
        {
            switch (state)
            {
                case STATE_YETI_HAPPY_1:
                {
                    --state_countdown;
                    if (state_countdown <= 0)
                    {
                        state = STATE_YETI_HAPPY_2;
                        current_frame_index = 68;
                        state_countdown = 9;
                        offset_y = 0;
                    }
                    break;
                };
                case STATE_YETI_HAPPY_2:
                {
                    --state_countdown;
                    if (state_countdown == 9)
                    {
                        offset_y = 0;
                    }
                    else if (state_countdown == 8 | state_countdown == 1)
                    {
                        offset_y = -5;
                    }
                    else if (state_countdown == 7 | state_countdown == 2)
                    {
                        offset_y = -10;
                    }
                    else if (state_countdown == 6 | state_countdown == 3)
                    {
                        offset_y = -15;
                    }
                    else if (state_countdown == 5 | state_countdown == 4)
                    {
                        offset_y = -20;
                    }
                    else
                    {
                        state = STATE_YETI_HAPPY_1;
                        current_frame_index = 67;
                        state_countdown = GetRandomValue(1, 25);
                        offset_y = 0;
                    }
                    break;
                }
                case STATE_YETI_LEFT:
                {
                    if (current_frame_index == 69)
                    {
                        current_frame_index = 70;
                    }
                    else
                    {
                        current_frame_index = 69;
                    }
                    break;
                }
                case STATE_YETI_RIGHT:
                {
                    if (current_frame_index == 71)
                    {
                        current_frame_index = 72;
                    }
                    else
                    {
                        current_frame_index = 71;
                    }
                    break;
                }
                case STATE_YETI_UP:
                {
                    if (current_frame_index == 73)
                    {
                        current_frame_index = 74;
                    }
                    else
                    {
                        current_frame_index = 73;
                    }
                    break;
                }
                case STATE_YETI_EATING:
                {
                    ++current_frame_index;
                    if (current_frame_index == 80)
                    {
                        state = STATE_YETI_HAPPY_1;
                        current_frame_index = 67;
                        state_countdown = GetRandomValue(1, 25);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        default:
            break;
    }

    if (direction.x != 0 || direction.y != 0)
    {
        velocity = direction * speed;
    }

    position += velocity;
}

Rectangle SkiObject::get_location() const
{
    return Rectangle(
            position.x, position.y, current_frame_rectangle.width, current_frame_rectangle.height);
}

SkiFree::SkiFree()
{
    all_textures = LoadTexture("assets/images.png");
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
    frames[73] = Rectangle(0, prev_y += 48, 32, 48); // yeti_up_1
    frames[74] = Rectangle(0, prev_y += 48, 32, 48); // yeti_up_2
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
    frames[88] = Rectangle(0, prev_y + 32, 28, 32); // tree_walk_left

    camera.zoom = 1.0f;

    slalom_sign.type = SkiObject::TYPE_SLALOM_SIGN;
    slalom_sign.position = Vector2(-180, 270);
    slalom_sign.current_frame_index = 60;
    slalom_sign.current_frame_rectangle = frames[60];

    freestyle_sign.type = SkiObject::TYPE_FREESTYLE_SIGN;
    freestyle_sign.position = Vector2(0, 270);
    freestyle_sign.current_frame_index = 62;
    freestyle_sign.current_frame_rectangle = frames[62];

    tree_slalom_sign.type = SkiObject::TYPE_TREE_SLALOM_SIGN;
    tree_slalom_sign.position = Vector2(180, 270);
    tree_slalom_sign.current_frame_index = 61;
    tree_slalom_sign.current_frame_rectangle = frames[61];

    restart_pause_sign.type = SkiObject::TYPE_RESTART_PAUSE_SIGN;
    restart_pause_sign.position = Vector2(90, 0);
    restart_pause_sign.current_frame_index = 55;
    restart_pause_sign.current_frame_rectangle = frames[55];

    numpad_sign.type = SkiObject::TYPE_NUMPAD;
    numpad_sign.position = Vector2(90, -45);
    numpad_sign.current_frame_index = 54;
    numpad_sign.current_frame_rectangle = frames[54];

    start_left_freestyle_sign.type = SkiObject::TYPE_START_LEFT;
    start_left_freestyle_sign.position = Vector2(-160, 800);
    start_left_freestyle_sign.current_frame_index = 56;
    start_left_freestyle_sign.current_frame_rectangle = frames[56];

    start_right_freestyle_sign.type = SkiObject::TYPE_START_RIGHT;
    start_right_freestyle_sign.position = Vector2(180, 800);
    start_right_freestyle_sign.current_frame_index = 57;
    start_right_freestyle_sign.current_frame_rectangle = frames[57];

    start_left_slalom_sign.type = SkiObject::TYPE_START_LEFT;
    start_left_slalom_sign.position = Vector2(-540, 800);
    start_left_slalom_sign.current_frame_index = 56;
    start_left_slalom_sign.current_frame_rectangle = frames[56];

    start_right_slalom_sign.type = SkiObject::TYPE_START_RIGHT;
    start_right_slalom_sign.position = Vector2(-240, 800);
    start_right_slalom_sign.current_frame_index = 57;
    start_right_slalom_sign.current_frame_rectangle = frames[57];

    start_left_tree_slalom_sign.type = SkiObject::TYPE_START_LEFT;
    start_left_tree_slalom_sign.position = Vector2(260, 800);
    start_left_tree_slalom_sign.current_frame_index = 56;
    start_left_tree_slalom_sign.current_frame_rectangle = frames[56];

    start_right_tree_slalom_sign.type = SkiObject::TYPE_START_RIGHT;
    start_right_tree_slalom_sign.position = Vector2(560, 800);
    start_right_tree_slalom_sign.current_frame_index = 57;
    start_right_tree_slalom_sign.current_frame_rectangle = frames[57];

    finish_left_slalom_sign.type = SkiObject::TYPE_FINISH_LEFT;
    finish_left_slalom_sign.position = Vector2(-540, 10800);
    finish_left_slalom_sign.current_frame_index = 58;
    finish_left_slalom_sign.current_frame_rectangle = frames[58];

    finish_right_slalom_sign.type = SkiObject::TYPE_FINISH_RIGHT;
    finish_right_slalom_sign.position = Vector2(-240, 10800);
    finish_right_slalom_sign.current_frame_index = 59;
    finish_right_slalom_sign.current_frame_rectangle = frames[59];

    finish_left_tree_slalom_sign.type = SkiObject::TYPE_FINISH_LEFT;
    finish_left_tree_slalom_sign.position = Vector2(260, 20800);
    finish_left_tree_slalom_sign.current_frame_index = 58;
    finish_left_tree_slalom_sign.current_frame_rectangle = frames[58];

    finish_right_tree_slalom_sign.type = SkiObject::TYPE_FINISH_RIGHT;
    finish_right_tree_slalom_sign.position = Vector2(560, 20800);
    finish_right_tree_slalom_sign.current_frame_index = 59;
    finish_right_tree_slalom_sign.current_frame_rectangle = frames[59];

    finish_left_freestyle_sign.type = SkiObject::TYPE_FINISH_LEFT;
    finish_left_freestyle_sign.position = Vector2(-160, 20800);
    finish_left_freestyle_sign.current_frame_index = 58;
    finish_left_freestyle_sign.current_frame_rectangle = frames[58];

    finish_right_freestyle_sign.type = SkiObject::TYPE_FINISH_RIGHT;
    finish_right_freestyle_sign.position = Vector2(180, 20800);
    finish_right_freestyle_sign.current_frame_index = 59;
    finish_right_freestyle_sign.current_frame_rectangle = frames[59];

    yeti_1.type = SkiObject::TYPE_YETI;
    yeti_2.type = SkiObject::TYPE_YETI;

    long_live_objects.emplace_back(&slalom_sign);
    long_live_objects.emplace_back(&freestyle_sign);
    long_live_objects.emplace_back(&tree_slalom_sign);
    long_live_objects.emplace_back(&restart_pause_sign);
    long_live_objects.emplace_back(&numpad_sign);
    long_live_objects.emplace_back(&start_left_freestyle_sign);
    long_live_objects.emplace_back(&start_right_freestyle_sign);
    long_live_objects.emplace_back(&start_left_slalom_sign);
    long_live_objects.emplace_back(&start_right_slalom_sign);
    long_live_objects.emplace_back(&start_left_tree_slalom_sign);
    long_live_objects.emplace_back(&start_right_tree_slalom_sign);
    long_live_objects.emplace_back(&finish_left_slalom_sign);
    long_live_objects.emplace_back(&finish_right_slalom_sign);
    long_live_objects.emplace_back(&finish_left_tree_slalom_sign);
    long_live_objects.emplace_back(&finish_right_tree_slalom_sign);
    long_live_objects.emplace_back(&finish_left_freestyle_sign);
    long_live_objects.emplace_back(&finish_right_freestyle_sign);

    characters_objects.emplace_back(&player);
    characters_objects.emplace_back(&yeti_1);
    characters_objects.emplace_back(&yeti_2);

    for (size_t i = 0; i < 13; ++i)
    {
        lift_poles_objects.emplace_back();
        auto *pole = &lift_poles_objects.back();

        pole->type = SkiObject::TYPE_LIFT_POLE;
        pole->position.x = -90;
        pole->position.y = -65 * 20 + 128 * 20 * i;
        pole->current_frame_index = 63;
        pole->current_frame_rectangle = frames[63];
    }

    for (size_t i = 0; i < 24; ++i)
    {
        auto flag = SkiObject();
        if (i % 2 == 0)
        {
            flag.type = SkiObject::TYPE_SLALOM_ARROW_LEFT;
            flag.position.x = -460;
            flag.position.y = 60 * 20 + 20 * 20 * i;
            flag.current_frame_index = 22;
            flag.current_frame_rectangle = frames[22];
        }
        else
        {
            flag.type = SkiObject::TYPE_SLALOM_ARROW_RIGHT;
            flag.position.x = -320;
            flag.position.y = 60 * 20 + 20 * 20 * i;
            flag.current_frame_index = 23;
            flag.current_frame_rectangle = frames[23];
        }
        slalom_flags_objects.push_back(flag);
    }

    for (size_t i = 0; i < 39; ++i)
    {
        auto flag = SkiObject();
        if (i % 2 == 0)
        {
            flag.type = SkiObject::TYPE_SLALOM_ARROW_LEFT;
            flag.position.x = 340;
            flag.position.y = 65 * 20 + 25 * 20 * i;
            flag.current_frame_index = 22;
            flag.current_frame_rectangle = frames[22];
        }
        else
        {
            flag.type = SkiObject::TYPE_SLALOM_ARROW_RIGHT;
            flag.position.x = 480;
            flag.position.y = 65 * 20 + 25 * 20 * i;
            flag.current_frame_index = 23;
            flag.current_frame_rectangle = frames[23];
        }
        slalom_flags_objects.push_back(flag);
    }

    reset();
};

SkiFree::~SkiFree()
{
    UnloadTexture(all_textures);
}

void SkiFree::draw() const
{
    BeginMode2D(camera);
    for (const auto &long_live_object: long_live_objects)
    {
        DrawTextureRec(
                all_textures, frames[long_live_object->current_frame_index],
                Vector2(long_live_object->position.x,
                        long_live_object->position.y + long_live_object->offset_y),
                WHITE);
    }
    for (const auto &characters: characters_objects)
    {
        DrawTextureRec(
                all_textures, frames[characters->current_frame_index],
                Vector2(characters->position.x, characters->position.y + characters->offset_y),
                WHITE);
    }
    for (const auto &lift_poles: lift_poles_objects)
    {
        DrawTextureRec(
                all_textures, frames[lift_poles.current_frame_index], lift_poles.position, WHITE);
    }
    for (const auto &slalom_flags: slalom_flags_objects)
    {
        DrawTextureRec(
                all_textures, frames[slalom_flags.current_frame_index], slalom_flags.position,
                WHITE);
    }
    for (const auto &short_live_obj: short_live_objects)
    {
        DrawTextureRec(
                all_textures, frames[short_live_obj.current_frame_index], short_live_obj.position,
                WHITE);
    }
    EndMode2D();

    DrawRectangle(0, 0, GetScreenWidth(), 30, BLUE);
    const char *text;
    if (is_paused)
    {
        text = "Ski Paused... Press F3 to continue";
    }
    else
    {
        text = "Ski Free";
    }
    const int width = MeasureText(text, 22);
    DrawText(text, (GetScreenWidth() - width) / 2, 5, 22, WHITE);

    DrawRectangleLinesEx(Rectangle(650, 30, 150, 76), 2, BLACK);
    DrawText("Time: 0:00:00.00", 655, 32, 14, BLACK);
    DrawText(TextFormat("Dist: %02dm", int(player.position.y / 20)), 655, 48, 14, BLACK);
    DrawText(TextFormat("Speed: %.0fm/s", Vector2Length(player.velocity)), 655, 64, 14, BLACK);
    DrawText("Style: 0", 655, 80, 14, BLACK);
}

void SkiFree::inputs()
{
    if (IsKeyPressed(KEY_F2))
    {
        reset();
    }
    if (IsKeyPressed(KEY_F3))
    {
        is_paused = !is_paused;
        if (is_paused)
        {
            is_waiting_action = true;
        }
        else
        {
            is_waiting_action = false;
        }
    }
    if (is_paused && IsKeyPressed(KEY_T))
    {
        is_waiting_action = true;
    }
    if (is_paused && !is_waiting_action)
    {
        return;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_KP_6))
    {
        is_waiting_action = false;
        switch (player.state)
        {
            case SkiObject::STATE_PLAYER_RIGHT:
            {
                // 6, 8, 10
                if (player.current_frame_index == 6)
                {
                    player.current_frame_index = 8;
                }
                else if (player.current_frame_index == 8)
                {
                    player.current_frame_index = 10;
                }
                else if (player.current_frame_index == 10)
                {
                    player.current_frame_index = 6;
                }
                player.direction = {1, 0};
                player.speed = 10;
                break;
            }
            case SkiObject::STATE_PLAYER_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_60_LEFT;
                player.current_frame_index = 2;
                player.direction.x = cosf(150 * DEG2RAD);
                player.direction.y = sinf(150 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_60_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_30_LEFT;
                player.current_frame_index = 1;
                player.direction.x = cosf(120 * DEG2RAD);
                player.direction.y = sinf(120 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_30_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_DOWN;
                player.current_frame_index = 0;
                player.direction.x = cosf(90 * DEG2RAD);
                player.direction.y = sinf(90 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_DOWN:
            {
                player.state = SkiObject::STATE_PLAYER_30_RIGHT;
                player.current_frame_index = 4;
                player.direction.x = cosf(60 * DEG2RAD);
                player.direction.y = sinf(60 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_30_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_60_RIGHT;
                player.current_frame_index = 5;
                player.direction.x = cosf(30 * DEG2RAD);
                player.direction.y = sinf(30 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_60_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_RIGHT;
                player.current_frame_index = 6;
                player.direction = {1, 0};
                break;
            }
            default:
                break;
        }
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_KP_4))
    {
        is_waiting_action = false;
        switch (player.state)
        {
            case SkiObject::STATE_PLAYER_LEFT:
            {
                // 3, 7, 9
                if (player.current_frame_index == 3)
                {
                    player.current_frame_index = 7;
                }
                else if (player.current_frame_index == 7)
                {
                    player.current_frame_index = 9;
                }
                else if (player.current_frame_index == 9)
                {
                    player.current_frame_index = 3;
                }
                player.direction = {-1, 0};
                player.speed = 10;
                break;
            }
            case SkiObject::STATE_PLAYER_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_60_RIGHT;
                player.current_frame_index = 5;
                player.direction.x = cosf(30 * DEG2RAD);
                player.direction.y = sinf(30 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_60_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_30_RIGHT;
                player.current_frame_index = 4;
                player.direction.x = cosf(60 * DEG2RAD);
                player.direction.y = sinf(60 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_30_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_DOWN;
                player.current_frame_index = 0;
                player.direction = {0, 1};
                break;
            }
            case SkiObject::STATE_PLAYER_DOWN:
            {
                player.state = SkiObject::STATE_PLAYER_30_LEFT;
                player.current_frame_index = 1;
                player.direction.x = cosf(120 * DEG2RAD);
                player.direction.y = sinf(120 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_30_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_60_LEFT;
                player.current_frame_index = 2;
                player.direction.x = cosf(150 * DEG2RAD);
                player.direction.y = sinf(150 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_60_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_LEFT;
                player.current_frame_index = 3;
                player.direction = {-1, 0};
                break;
            }
            default:
                break;
        }
    }
    if (IsKeyPressed(KEY_KP_9))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_RIGHT;
        player.current_frame_index = 6;
        player.direction = {1, 0};
    }
    if (IsKeyPressed(KEY_KP_7))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_LEFT;
        player.current_frame_index = 3;
        player.direction = {-1, 0};
    }
    if (IsKeyPressed(KEY_KP_3))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_30_RIGHT;
        player.current_frame_index = 4;
        player.direction.x = cosf(60 * DEG2RAD);
        player.direction.y = sinf(60 * DEG2RAD);
    }
    if (IsKeyPressed(KEY_KP_1))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_30_LEFT;
        player.current_frame_index = 1;
        player.direction.x = cosf(120 * DEG2RAD);
        player.direction.y = sinf(120 * DEG2RAD);
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_KP_2))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_DOWN;
        player.current_frame_index = 0;
        player.direction = {0, 1};
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_KP_8))
    {
        is_waiting_action = false;
        switch (player.state)
        {
            case SkiObject::STATE_PLAYER_LEFT:
            {
                if (player.current_frame_index == 3)
                {
                    player.current_frame_index = 7;
                }
                else if (player.current_frame_index == 7)
                {
                    player.current_frame_index = 9;
                }
                else if (player.current_frame_index == 9)
                {
                    player.current_frame_index = 3;
                }
                player.direction = {0, -1};
                player.speed = 10;
                break;
            }
            case SkiObject::STATE_PLAYER_RIGHT:
            {
                // 6, 8, 10
                if (player.current_frame_index == 6)
                {
                    player.current_frame_index = 8;
                }
                else if (player.current_frame_index == 8)
                {
                    player.current_frame_index = 10;
                }
                else if (player.current_frame_index == 10)
                {
                    player.current_frame_index = 6;
                }
                player.direction = {0, -1};
                player.speed = 10;
                break;
            }
            default:
                break;
        }
    }
}

void SkiFree::update()
{
    if (is_paused && !IsKeyPressed(KEY_T))
    {
        return;
    }
    Vector2 pos_before = player.position;
    for (const auto characters: characters_objects)
    {
        characters->update();
    }

    // teleport yeti_1 based on player position thresholds
    if (player.position.y >= 2000 * 20 && pos_before.y < 2000 * 20)
    {
        yeti_1.position.y = 2100 * 20;
    }
    if (player.position.y < 0 && pos_before.y >= 0)
    {
        yeti_1.position.y = -130 * 20;
    }

    // teleport objects at 2050 for game continuity
    if (player.position.y >= 2050 * 20 && pos_before.y < 2050 * 20)
    {
        // teleport objects up mountain
        for (auto &obj: short_live_objects)
        {
            obj.position.y -= 2050 * 20 * 2;
        }
        for (auto &obj: characters_objects)
        {
            obj->position.y -= 2050 * 20 * 2;
        }
    }
    if (player.position.y <= -2050 * 20 && pos_before.y > -2050 * 20)
    {
        // teleport objects down mountain
        for (auto &obj: short_live_objects)
        {
            obj.position.y += 2050 * 20 * 2;
        }
        for (auto &obj: characters_objects)
        {
            obj->position.y += 2050 * 20 * 2;
        }
    }

    manage_objects();
    for (const auto long_live_object: long_live_objects)
    {
        long_live_object->update();
    }
    for (auto &short_live_object: short_live_objects)
    {
        short_live_object.update();
    }
    camera.offset = GetWorldToScreen2D(player.position, camera);
    camera.target = player.position;
    Vector2 delta = Vector2(GetScreenWidth() / 2.0, GetScreenHeight() / 2.0 - 150) -
                    GetWorldToScreen2D(player.position, camera);
    delta *= -1.0f / camera.zoom;
    camera.target += delta;
}

void SkiFree::manage_objects()
{
    auto new_area = Rectangle(
            player.position.x - area_size / 2, player.position.y - area_size / 2, area_size,
            area_size);

    if (abs(new_area.x - current_area.x) < 250 && abs(new_area.y - current_area.y) < 250)
    {
        return;
    }

    short_live_objects.remove_if([&new_area](const SkiObject &object)
                                 { return !CheckCollisionPointRec(object.position, new_area); });

    auto get_new_position = [&new_area]()
    {
        return Vector2(
                GetRandomValue(new_area.x, new_area.x + new_area.width),
                GetRandomValue(new_area.y, new_area.y + new_area.height));
    };

    auto check_collision_skiobjects = [&](SkiObject ski_object)
    {
        Rectangle ski_obj_loc = ski_object.get_location();
        for (const auto *ll: long_live_objects)
        {
            if (CheckCollisionRecs(ski_obj_loc, ll->get_location()))
            {
                return true;
            }
        }
        for (const auto &sl: short_live_objects)
        {
            if (CheckCollisionRecs(ski_obj_loc, sl.get_location()))
            {
                return true;
            }
        }
        return false;
    };

    auto check_slalom_area = [](SkiObject ski_object)
    {
        // Slalom area allows only TYPE_MOGUL_GROUP
        if (ski_object.type == SkiObject::TYPE_MOGUL_GROUP)
        {
            return false;
        }
        Rectangle ski_obj_loc = ski_object.get_location();
        static Rectangle slalom_area = Rectangle(-540, 800, 300, 10080);
        if (CheckCollisionRecs(ski_obj_loc, slalom_area))
        {
            return true;
        }
        return false;
    };

    auto check_tree_slalom_area = [](SkiObject ski_object)
    {
        // Tree-Slalom area allows only tree types
        if (ski_object.type == SkiObject::TYPE_TREE_DRIED ||
            ski_object.type == SkiObject::TYPE_TREE_LARGE ||
            ski_object.type == SkiObject::TYPE_TREE_SMALL ||
            ski_object.type == SkiObject::TYPE_TREE_WALK)
        {
            return false;
        }
        Rectangle ski_obj_loc = ski_object.get_location();
        static Rectangle slalom_area = Rectangle(260, 800, 300, 20080);
        if (CheckCollisionRecs(ski_obj_loc, slalom_area))
        {
            return true;
        }
        return false;
    };

    const size_t current_num_objs = short_live_objects.size();

    for (size_t i = 0; i < num_elements_in_area - current_num_objs; i++)
    {
        auto new_object = SkiObject();
        const auto type = (SkiObject::object_type) GetRandomValue(
                SkiObject::TYPE_MOGUL_GROUP, SkiObject::TYPE_RAINBOW);
        new_object.type = type;
        switch (type)
        {
            case SkiObject::TYPE_MOGUL_GROUP:
            {
                new_object.current_frame_index = 26;
                break;
            }
            case SkiObject::TYPE_ROCK:
            {
                new_object.current_frame_index = 44;
                break;
            }
            case SkiObject::TYPE_STUMP:
            {
                new_object.current_frame_index = 45;
                break;
            }
            case SkiObject::TYPE_MOGUL_SMALL:
            {
                new_object.current_frame_index = 46;
                break;
            }
            case SkiObject::TYPE_MOGUL_LARGE:
            {
                new_object.current_frame_index = 47;
                break;
            }
            case SkiObject::TYPE_TREE_SMALL:
            {
                new_object.current_frame_index = 48;
                break;
            }
            case SkiObject::TYPE_TREE_DRIED:
            {
                new_object.current_frame_index = 49;
                break;
            }
            case SkiObject::TYPE_TREE_LARGE:
            {
                new_object.current_frame_index = 50;
                break;
            }
            case SkiObject::TYPE_RAINBOW:
            {
                new_object.current_frame_index = 51;
                break;
            }
            default:
            {
                (void) fprintf(stderr, "Invalid object type %d\n", type);
                assert(0);
            }
        }
        new_object.current_frame_rectangle = frames[new_object.current_frame_index];
        do
        {
            new_object.position = get_new_position();
        }
        while (CheckCollisionPointRec(new_object.position, current_area) ||
               check_collision_skiobjects(new_object) || check_slalom_area(new_object) ||
               check_tree_slalom_area(new_object));
        short_live_objects.push_back(new_object);
    }

    current_area = new_area;
}

void SkiFree::reset()
{
    SetRandomSeed(0);
    player.type = SkiObject::TYPE_SKIER;
    player.position = Vector2(0, 0);
    player.current_frame_index = 3;
    player.current_frame_rectangle = frames[player.current_frame_index];
    player.speed = 0;
    player.direction = {1, 0};
    player.state = SkiObject::STATE_PLAYER_LEFT;

    current_area = Rectangle(player.position.x - 100, player.position.y - 100, 100, 100);

    short_live_objects.clear();

    is_paused = false;
    is_waiting_action = true;

    yeti_1.state = SkiObject::STATE_YETI_HAPPY_1;
    yeti_1.position = Vector2(0, -130 * 20);
    yeti_1.current_frame_index = 67;
    yeti_1.current_frame_rectangle = frames[67];
    yeti_1.state_countdown = GetRandomValue(1, 25);

    yeti_2.state = SkiObject::STATE_YETI_HAPPY_1;
    yeti_2.position = Vector2(100, 2000 * 20);
    yeti_2.current_frame_index = 67;
    yeti_2.current_frame_rectangle = frames[67];
    yeti_2.state_countdown = GetRandomValue(1, 25);

    manage_objects();
}
