#include <algorithm>
#include <cstdlib>
#include "SnakeGameAI.h"

#include <cstring>
#include <list>


inline bool operator==(const Vector2 &lhs, const Vector2 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

SnakeGameAI::SnakeGameAI()
{
    reset();
};

void SnakeGameAI::new_food()
{
    const int x = rand() % w;
    const int y = rand() % h;
    food = Vector2(x, y);
}

void SnakeGameAI::place_food()
{
    new_food();
    while (std::find(snake.begin(), snake.end(), food) != snake.end())
    {
        new_food();
    }
};

bool SnakeGameAI::is_collision(const Vector2 pt)
{
    // hits boundary
    if (pt.x > w - 1 || pt.x < 0 || pt.y > h - 1 || pt.y < 0)
    {
        return true;
    }
    // hits itself
    if (std::find(snake.begin() + 1, snake.end(), pt) != snake.end())
    {
        return true;
    }
    return false;
}

size_t SnakeGameAI::index_from_location(Vector2 pt)
{
    return pt.y * w + pt.x;
}

void SnakeGameAI::update_field()
{
    memset(distance_field, -1, sizeof(int) * w * h);
    std::list<FieldLoc> nodes;
    nodes.emplace_back(food, 1);
    while (!nodes.empty())
    {
        std::list<FieldLoc> new_nodes;
        for (auto &n: nodes)
        {
            distance_field[index_from_location(n.p)] = n.distance;
            Vector2 right = n.p + p_right;
            Vector2 left = n.p + p_left;
            Vector2 up = n.p + p_up;
            Vector2 down = n.p + p_down;
            if (!is_collision(right) && distance_field[index_from_location(right)] == 0)
            {
                new_nodes.emplace_back(right, n.distance + 1);
            }
            if (!is_collision(left) && distance_field[index_from_location(left)] == 0)
            {
                new_nodes.emplace_back(left, n.distance + 1);
            }
            if (!is_collision(up) && distance_field[index_from_location(up)] == 0)
            {
                new_nodes.emplace_back(up, n.distance + 1);
            }
            if (!is_collision(down) && distance_field[index_from_location(down)] == 0)
            {
                new_nodes.emplace_back(down, n.distance + 1);
            }
        }

        new_nodes.sort([&](const FieldLoc &a, const FieldLoc &b)
                       { return index_from_location(a.p) < index_from_location(b.p); });
        new_nodes.unique([&](const FieldLoc &a, const FieldLoc &b) { return a.p == b.p; });
        nodes.clear();
        nodes.insert(nodes.begin(), new_nodes.begin(), new_nodes.end());
    }
}

std::array<int, INPUT_SIZE> SnakeGameAI::get_state()
{
    update_field();
    Vector2 head = snake[0];

    // get points around the head
    Vector2 point_left = Vector2(head.x - 1, head.y);
    Vector2 point_right = Vector2(head.x + 1, head.y);
    Vector2 point_up = Vector2(head.x, head.y - 1);
    Vector2 point_down = Vector2(head.x, head.y + 1);

    bool is_direction_right = direction == RIGHT;
    bool is_direction_left = direction == LEFT;
    bool is_direction_up = direction == UP;
    bool is_direction_down = direction == DOWN;

    bool is_food_left = food.x < head.x;
    bool is_food_right = food.x > head.x;
    bool is_food_up = food.y < head.y;
    bool is_food_down = food.y > head.y;

    bool right_reaches_food = distance_field[index_from_location(point_right)] > 0;
    bool left_reaches_food = distance_field[index_from_location(point_left)] > 0;
    bool up_reaches_food = distance_field[index_from_location(point_up)] > 0;
    bool down_reaches_food = distance_field[index_from_location(point_down)] > 0;

    return std::array<int, INPUT_SIZE>{
            // Danger straight (same direction)
            (is_direction_right && is_collision(point_right) ||
             (is_direction_left && is_collision(point_left)) ||
             (is_direction_up && is_collision(point_up)) ||
             (is_direction_down && is_collision(point_down))),
            // Danger right (danger is at the right of current direction)
            (is_direction_right && is_collision(point_down)) ||
                    (is_direction_left && is_collision(point_up)) ||
                    (is_direction_up && is_collision(point_right)) ||
                    (is_direction_down && is_collision(point_left)),
            // Danger left (danger is at the left of current direction)
            (is_direction_right && is_collision(point_up)) ||
                    (is_direction_left && is_collision(point_down)) ||
                    (is_direction_up && is_collision(point_left)) ||
                    (is_direction_down && is_collision(point_right)),
            // current direction
            is_direction_right,
            is_direction_left,
            is_direction_up,
            is_direction_down,
            // food location
            is_food_left,
            is_food_right,
            is_food_up,
            is_food_down,
            // point reaches food
            right_reaches_food,
            left_reaches_food,
            up_reaches_food,
            down_reaches_food,
    };
}

void SnakeGameAI::reset()
{
    direction = RIGHT;

    head.x = w / 2;
    head.y = h / 2;

    snake.clear();
    snake.push_back(head);
    snake.push_back({head.x - 1, head.y});
    snake.push_back({head.x - 2, head.y});

    score = 0;
    place_food();
    frame_iteration = 0;
}

void SnakeGameAI::move(action_t action)
{
    int current_index = 0;
    for (; current_index < 4; ++current_index)
    {
        if (direction == clock_wise_direction[current_index])
        {
            break;
        }
    }
    int next_index = current_index;
    switch (action)
    {
        case ACTION_STRAIGHT:
        {
            break;
        }
        case ACTION_LEFT:
        {
            next_index = ((current_index - 1) % 4 + 4) % 4;
            break;
        }
        case ACTION_RIGHT:
        {
            next_index = (current_index + 1) % 4;
        }
    }

    direction = clock_wise_direction[next_index];

    float x = head.x;
    float y = head.y;
    switch (direction)
    {
        case RIGHT:
        {
            ++x;
            break;
        }
        case LEFT:
        {
            --x;
            break;
        }
        case UP:
        {
            --y;
            break;
        }
        case DOWN:
        {
            ++y;
            break;
        }
    }
    head = Vector2{x, y};
}

StepResult SnakeGameAI::get_step(action_t action)
{
    StepResult result;
    ++frame_iteration;

    move(action);
    snake.insert(snake.begin(), head);

    if (is_collision(head) || frame_iteration > 100 * snake.size())
    {
        result.game_over = true;
        result.reward = -10;
        return result;
    }

    if (head == food)
    {
        ++score;
        place_food();
        result.reward = 10;
        return result;
    }
    snake.pop_back();
    return result;
}
