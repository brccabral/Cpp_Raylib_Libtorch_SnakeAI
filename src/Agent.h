#pragma once

#include <array>
#include <deque>
#include "SnakeGameAI.h"
#include "Linear_QNet.h"


#define HIDDEN_SIZE 256

// has to be the number of possible actions, Agent.get_action
#define OUTPUT_SIZE 1

class Agent
{
public:

    Agent(Linear_QNet *model_);
    ~Agent();

    SnakeGameAI::action_t get_action(std::array<int, INPUT_SIZE> state);
    void toggle_training();

private:

    int number_of_games = 0;
    float epsilon = 0;
    Linear_QNet *model;
    bool is_training = true;
    c10::DeviceType device;

    SnakeGameAI::action_t get_play(std::array<int, INPUT_SIZE> state);
};
