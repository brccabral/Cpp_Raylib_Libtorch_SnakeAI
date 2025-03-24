#include <cstdlib>
#include "Agent.h"

Agent::Agent()
{
    model = new Linear_QNet(INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
    c10::DeviceType device;
    if (torch::cuda::is_available())
    {
        printf("Using CUDA\n");
        device = torch::kCUDA;
    }
    else
    {
        printf("Using CPU\n");
        device = torch::kCPU;
    }
    (*model)->to(device);
    (*model)->train();
}

Agent::~Agent()
{
    delete model;
}

void Agent::toggle_training()
{
    is_training = !is_training;
    if (is_training)
    {
        (*model)->train();
    }
    else
    {
        (*model)->eval();
    }
}


SnakeGameAI::action_t Agent::get_play(std::array<int, INPUT_SIZE> state)
{
    auto state0 = torch::tensor(state.data());
    auto prediction = (*model)->forward(state0);
    SnakeGameAI::action_t action = (SnakeGameAI::action_t) torch::argmax(prediction).item().toInt();
    return action;
}

SnakeGameAI::action_t Agent::get_action(std::array<int, INPUT_SIZE> state)
{
    if (!is_training)
    {
        {
            torch::NoGradGuard no_grad;
            return get_play(state);
        }
    }

    epsilon = 80 - number_of_games;
    SnakeGameAI::action_t action = SnakeGameAI::ACTION_STRAIGHT;

    if (rand() % 200 < epsilon)
    {
        action = (SnakeGameAI::action_t) (rand() % 3);
    }
    else
    {
        return get_play(state);
    }

    return action;
}
