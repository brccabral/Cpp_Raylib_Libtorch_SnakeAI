#pragma once

#include <array>
#include <deque>
#include "SnakeGameAI.h"
#include "Linear_QNet.h"
#include "QTrainer.h"


#define HIDDEN_SIZE 256

// has to be the number of possible actions, Agent.get_action
#define OUTPUT_SIZE 3

// how many previous moves will be stored in memory_deque
#define MAX_MEMORY 100000
#define BATCH_SIZE 750

template<typename T>
class LimitedDeque
{
public:

    explicit LimitedDeque(std::size_t max_size_) : max_size(max_size_)
    {}

    using Iterator = typename std::deque<T>::iterator;

    void push_back(T value)
    {
        if (deque.size() >= max_size)
        {
            deque.pop_front(); // Remove oldest element
        }
        deque.push_back(value);
    }

    void push_front(T value)
    {
        if (deque.size() >= max_size)
        {
            deque.pop_back(); // Remove newest element
        }
        deque.push_front(value);
    }

    size_t size()
    {
        return deque.size();
    }

    Iterator begin()
    {
        return deque.begin();
    }

    Iterator end()
    {
        return deque.end();
    }

private:

    std::deque<T> deque;
    std::size_t max_size;
};


class MemoryData
{
public:

    std::array<int, INPUT_SIZE> state;
    std::array<int, OUTPUT_SIZE> action;
    int reward;
    std::array<int, INPUT_SIZE> next_state;
    bool game_over;
};


class Agent
{
public:

    Agent(Linear_QNet *model_, QTrainer *trainer_);
    ~Agent();

    std::array<int, OUTPUT_SIZE> get_action(std::array<int, INPUT_SIZE> state);
    void toggle_training();

    void train_short_memory(
            std::array<int, INPUT_SIZE> state, std::array<int, OUTPUT_SIZE> action, int reward,
            std::array<int, INPUT_SIZE> next_state, bool game_over);
    void train_long_memory();

    void remember(
            std::array<int, INPUT_SIZE> state, std::array<int, OUTPUT_SIZE> action, int reward,
            std::array<int, INPUT_SIZE> next_state, bool game_over);

    int number_of_games = 0;

private:

    float epsilon = 0;
    Linear_QNet *model;
    QTrainer *trainer;
    bool is_training = true;
    c10::DeviceType device;
    LimitedDeque<MemoryData> memory_deque{MAX_MEMORY};

    std::array<int, OUTPUT_SIZE> get_play(std::array<int, INPUT_SIZE> state);
};
