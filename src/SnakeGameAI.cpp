#include <algorithm>
#include <cstdlib>
#include "SnakeGameAI.h"


inline bool operator==(const Vector2 &lhs, const Vector2 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

SnakeGameAI::SnakeGameAI()
{
    snake.push_back(head);
    snake.push_back({head.x - 1, head.y});
    snake.push_back({head.x - 2, head.y});
    place_food();
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
    if (std::find(snake.begin(), snake.end(), pt) != snake.end())
    {
        return true;
    }
    return false;
}


std::array<int, INPUT_SIZE> SnakeGameAI::get_state()
{
    Vector2 head = snake[0];

    // get points around the head
    Vector2 point_left = Vector2(head.x - 1, head.y);
    Vector2 point_right = Vector2(head.x + 1, head.y);
    Vector2 point_up = Vector2(head.x, head.y - 1);
    Vector2 point_down = Vector2(head.x, head.y + 1);

    bool is_direction_right = direction == SnakeGameAI::RIGHT;
    bool is_direction_left = direction == SnakeGameAI::LEFT;
    bool is_direction_up = direction == SnakeGameAI::UP;
    bool is_direction_down = direction == SnakeGameAI::DOWN;

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
            is_direction_right, is_direction_left, is_direction_up, is_direction_down,
            // food location
            food.x<head.x, // food left
                   food.x>
                    head.x, // food right
            food.y<head.y, // food up
                   food.y>
                    head.y, // food down
    };
}
