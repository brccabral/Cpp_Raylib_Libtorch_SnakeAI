#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <raylib.h>

typedef enum Direction
{
    RIGHT = 1,
    LEFT,
    UP,
    DOWN
} Direction;

bool operator==(const Vector2 &lhs, const Vector2 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

class SnakeGameAI
{
    int w = 32;
    int h = 24;
    Direction direction = RIGHT;
    Vector2 head = Vector2(w / 2.0f, h / 2.0f);
    void new_food()
    {
        const int x = rand() % w;
        const int y = rand() % h;
        food = Vector2(x, y);
    }
    void place_food()
    {
        new_food();
        while (std::find(snake.begin(), snake.end(), food) != snake.end())
        {
            new_food();
        }
    };

public:

    SnakeGameAI()
    {
        snake.push_back(head);
        snake.push_back({head.x - 1, head.y});
        snake.push_back({head.x - 2, head.y});
        place_food();
    };
    std::vector<Vector2> snake;
    Vector2 food{};
    int score = 0;
};

int main()
{
    srand(time(NULL));
    SnakeGameAI game;
    constexpr int BLOCK_SIZE = 20;

    InitWindow(640, 480, "SnakeAI");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        for (auto [x, y]: game.snake)
        {
            DrawRectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLUE);
            DrawRectangle(
                    x * BLOCK_SIZE + 4, y * BLOCK_SIZE + 4, BLOCK_SIZE - 8, BLOCK_SIZE - 8,
                    SKYBLUE);
        }
        DrawRectangle(
                game.food.x * BLOCK_SIZE, game.food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, RED);
        DrawText(TextFormat("Score: %d", game.score), 0, 0, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
