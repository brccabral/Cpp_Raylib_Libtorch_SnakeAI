#pragma once

#include <array>
#include <deque>
#include "SnakeGameAI.h"


class Agent
{
    int number_of_games = 0;
    float epsilon = 0;

public:

    Agent();
    ~Agent();

    SnakeGameAI::action_t get_action(std::array<int, INPUT_SIZE> state);
};
