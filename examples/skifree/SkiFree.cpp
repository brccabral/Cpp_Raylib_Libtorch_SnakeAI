#include "SkiFree.h"
#include <raymath.h>

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
    restart_pause_sign.position = Vector2(180, 0);
    restart_pause_sign.current_frame_index = 55;
    restart_pause_sign.current_frame_rectangle = frames[55];

    long_live_objects.emplace_back(&player);
    long_live_objects.emplace_back(&slalom_sign);
    long_live_objects.emplace_back(&freestyle_sign);
    long_live_objects.emplace_back(&tree_slalom_sign);
    long_live_objects.emplace_back(&restart_pause_sign);

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
                long_live_object->position, WHITE);
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
    if (!is_waiting_action)
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
                player.speed = 1;
                player.direction = {1, 0};
                break;
            }
            case SkiObject::STATE_PLAYER_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_60_LEFT;
                player.current_frame_index = 2;
                player.speed = 1;
                player.direction.x = cosf(150 * DEG2RAD);
                player.direction.y = sinf(150 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_60_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_30_LEFT;
                player.current_frame_index = 1;
                player.speed = 1;
                player.direction.x = cosf(120 * DEG2RAD);
                player.direction.y = sinf(120 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_30_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_DOWN;
                player.current_frame_index = 0;
                player.speed = 1;
                player.direction.x = cosf(90 * DEG2RAD);
                player.direction.y = sinf(90 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_DOWN:
            {
                player.state = SkiObject::STATE_PLAYER_30_RIGHT;
                player.current_frame_index = 4;
                player.speed = 1;
                player.direction.x = cosf(60 * DEG2RAD);
                player.direction.y = sinf(60 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_30_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_60_RIGHT;
                player.current_frame_index = 5;
                player.speed = 1;
                player.direction.x = cosf(30 * DEG2RAD);
                player.direction.y = sinf(30 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_60_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_RIGHT;
                player.current_frame_index = 6;
                player.speed = 1;
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
                player.speed = 1;
                player.direction = {-1, 0};
                break;
            }
            case SkiObject::STATE_PLAYER_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_60_RIGHT;
                player.current_frame_index = 5;
                player.speed = 1;
                player.direction.x = cosf(30 * DEG2RAD);
                player.direction.y = sinf(30 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_60_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_30_RIGHT;
                player.current_frame_index = 4;
                player.speed = 1;
                player.direction.x = cosf(60 * DEG2RAD);
                player.direction.y = sinf(60 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_30_RIGHT:
            {
                player.state = SkiObject::STATE_PLAYER_DOWN;
                player.current_frame_index = 0;
                player.speed = 1;
                player.direction = {0, 1};
                break;
            }
            case SkiObject::STATE_PLAYER_DOWN:
            {
                player.state = SkiObject::STATE_PLAYER_30_LEFT;
                player.current_frame_index = 1;
                player.speed = 1;
                player.direction.x = cosf(120 * DEG2RAD);
                player.direction.y = sinf(120 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_30_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_60_LEFT;
                player.current_frame_index = 2;
                player.speed = 1;
                player.direction.x = cosf(150 * DEG2RAD);
                player.direction.y = sinf(150 * DEG2RAD);
                break;
            }
            case SkiObject::STATE_PLAYER_60_LEFT:
            {
                player.state = SkiObject::STATE_PLAYER_LEFT;
                player.current_frame_index = 3;
                player.speed = 1;
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
        player.speed = 1;
        player.direction = {1, 0};
    }
    if (IsKeyPressed(KEY_KP_7))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_LEFT;
        player.current_frame_index = 3;
        player.speed = 1;
        player.direction = {-1, 0};
    }
    if (IsKeyPressed(KEY_KP_3))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_30_RIGHT;
        player.current_frame_index = 4;
        player.speed = 1;
        player.direction.x = cosf(60 * DEG2RAD);
        player.direction.y = sinf(60 * DEG2RAD);
    }
    if (IsKeyPressed(KEY_KP_1))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_30_LEFT;
        player.current_frame_index = 1;
        player.speed = 1;
        player.direction.x = cosf(120 * DEG2RAD);
        player.direction.y = sinf(120 * DEG2RAD);
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_KP_2))
    {
        is_waiting_action = false;
        player.state = SkiObject::STATE_PLAYER_DOWN;
        player.current_frame_index = 0;
        player.speed = 1;
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
                player.speed = 1;
                player.direction = {0, -1};
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
                player.speed = 1;
                player.direction = {0, -1};
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

void SkiObject::update()
{
    Vector2 velocity{};
    if (direction.x != 0 || direction.y != 0)
    {
        velocity = direction * speed;
    }
    position += velocity;
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
                default:
                    break;
            }
            if (speed > 25)
            {
                speed = 25;
            }
            break;
        }
        default:
            break;
    }
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

    const size_t current_num_objs = short_live_objects.size();

    for (size_t i = 0; i < num_elements_in_area - current_num_objs; i++)
    {
        auto new_object = SkiObject();
        new_object.type = SkiObject::TYPE_TREE_SMALL;
        new_object.current_frame_index = 48;
        new_object.current_frame_rectangle = frames[48];
        Vector2 position;
        do
        {
            position = get_new_position();
        }
        while (CheckCollisionPointRec(position, current_area));
        new_object.position = position;
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

    manage_objects();
}
