#include <cstdlib>
#include "Agent.h"

Agent::Agent()
{}

Agent::~Agent()
{}

SnakeGameAI::action_t Agent::get_action(std::array<int, INPUT_SIZE> state)
{
    epsilon = 80 - number_of_games;
    SnakeGameAI::action_t action = SnakeGameAI::ACTION_STRAIGHT;

    if (rand() % 200 < epsilon)
    {
        action = (SnakeGameAI::action_t) (rand() % 3);
    }

    return action;
}
