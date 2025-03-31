#pragma once

#include <random>
#include <deque>
#include "QTrainer.h"


// how many previous moves will be stored in memory_deque
#define MAX_MEMORY 100000000


template<typename T>
class LimitedDeque
{
public:

    explicit LimitedDeque(const std::size_t max_size_) : max_size(max_size_)
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

    size_t num_state;
    size_t num_action;
    std::vector<double> state;
    std::vector<int> action;
    int reward;
    std::vector<double> next_state;
    bool game_over;
};


class Agent
{
public:

    Agent(LinearNN *model_, QTrainer *trainer_, c10::DeviceType device_, size_t batch_size_);

    std::vector<int> get_action(const std::vector<double> &state, size_t count_samples) const;

    void train_short_memory(
            const std::vector<double> &state, const std::vector<int> &action, int reward,
            const std::vector<double> &next_state, bool game_over) const;
    void train_long_memory();

    void remember(
            const std::vector<double> &state, const std::vector<int> &action, int reward,
            const std::vector<double> &next_state, bool game_over);

    int number_of_games = 0;

private:

    LinearNN *model;
    QTrainer *trainer;
    c10::DeviceType device;
    LimitedDeque<MemoryData> memory_deque{MAX_MEMORY};

    size_t batch_size{};

    std::vector<int> get_play(const std::vector<double> &state, size_t count_samples) const;
};
