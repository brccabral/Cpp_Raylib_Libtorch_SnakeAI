#pragma once

#include <array>
#include <deque>
#include "SnakeGameAI.h"
#include "Linear_QNet.h"
#include "QTrainer.h"


#define HIDDEN_SIZE 256

// has to be the number of possible actions, Agent.get_action
#define OUTPUT_SIZE 1

class Agent
{
public:

    Agent(Linear_QNet *model_, QTrainer *trainer_);
    ~Agent();

    SnakeGameAI::action_t get_action(std::array<int, INPUT_SIZE> state);
    void toggle_training();

    void train_short_memory(
            std::array<int, INPUT_SIZE> state, SnakeGameAI::action_t action, int reward,
            std::array<int, INPUT_SIZE> next_state, bool game_over);

private:

    int number_of_games = 0;
    float epsilon = 0;
    Linear_QNet *model;
    QTrainer *trainer;
    bool is_training = true;
    c10::DeviceType device;

    SnakeGameAI::action_t get_play(std::array<int, INPUT_SIZE> state);
};
