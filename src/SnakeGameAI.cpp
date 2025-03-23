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
