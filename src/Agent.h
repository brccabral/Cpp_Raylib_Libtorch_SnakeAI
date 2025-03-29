#pragma once

#include <cstdlib>
#include <random>
#include <array>
#include <deque>
#include "QTrainer.h"


#define HIDDEN_SIZE 256

// how many previous moves will be stored in memory_deque
#define MAX_MEMORY 10000000

// memory usage (if CPU -> RAM, if GPU -> GPU_MEM)
#define BATCH_SIZE 1000

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

template<int N_states, int N_actions>
class MemoryData
{
public:

    std::array<int, N_states> state;
    std::array<int, N_actions> action;
    int reward;
    std::array<int, N_states> next_state;
    bool game_over;
};


template<int N_states, int N_actions>
class Agent
{
public:

    Agent(Linear_QNet *model_, QTrainer<N_states, N_actions> *trainer_, c10::DeviceType device_);

    std::array<int, N_actions> get_action(std::array<int, N_states> state);

    void train_short_memory(
            std::array<int, N_states> state, std::array<int, N_actions> action, int reward,
            std::array<int, N_states> next_state, bool game_over);
    void train_long_memory();

    void remember(
            const std::array<int, N_states> &state, std::array<int, N_actions> action, int reward,
            const std::array<int, N_states> &next_state, bool game_over);

    int number_of_games = 0;

private:

    Linear_QNet *model;
    QTrainer<N_states, N_actions> *trainer;
    c10::DeviceType device;
    LimitedDeque<MemoryData<N_states, N_actions>> memory_deque{MAX_MEMORY};

    std::array<int, N_actions> get_play(std::array<int, N_states> state);
};

template<int N_states, int N_actions>
Agent<N_states, N_actions>::Agent(
        Linear_QNet *model_, QTrainer<N_states, N_actions> *trainer_, const c10::DeviceType device_)
{
    device = device_;

    model = model_;

    trainer = trainer_;
}

template<int N_states, int N_actions>
std::array<int, N_actions> Agent<N_states, N_actions>::get_play(std::array<int, N_states> state)
{
    auto state0 =
            torch::from_blob(state.data(), {state.size()}, torch::kInt).to(device, torch::kFloat);
    (*model)->eval();
    auto prediction = torch::Tensor();
    {
        torch::NoGradGuard no_grad;
        prediction = (*model)->forward(state0);
    }
    // std::cout << prediction << "\n";
    std::array<int, N_actions> action{};
    action[torch::argmax(prediction).item().toInt()] = 1;
    return action;
}

template<int N_states, int N_actions>
std::array<int, N_actions> Agent<N_states, N_actions>::get_action(std::array<int, N_states> state)
{
    const int epsilon = 80 - number_of_games;

    // epsilon = tradeoff between exploration vs exploitation
    // in the beginning this is true for some time, later self.number_of_games is larger
    if (rand() % 200 < epsilon)
    {
        std::array<int, N_actions> action{};
        action[rand() % 3] = 1;
        return action;
    }

    return get_play(state);
}

template<int N_states, int N_actions>
void Agent<N_states, N_actions>::train_short_memory(
        std::array<int, N_states> state, std::array<int, N_actions> action, int reward,
        std::array<int, N_states> next_state, bool game_over)
{
    trainer->train_step({state}, {action}, {reward}, {next_state}, {game_over});
}

template<int N_states, int N_actions>
void Agent<N_states, N_actions>::remember(
        const std::array<int, N_states> &state, const std::array<int, N_actions> action,
        const int reward, const std::array<int, N_states> &next_state, const bool game_over)
{
    memory_deque.push_back({state, action, reward, next_state, game_over});
}

template<int N_states, int N_actions>
void Agent<N_states, N_actions>::train_long_memory()
{
    std::vector<MemoryData<N_states, N_actions>> samples;
    size_t sample_size;

    if (memory_deque.size() > BATCH_SIZE)
    {
        sample_size = BATCH_SIZE;
        samples.resize(sample_size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::ranges::sample(
                memory_deque.begin(), memory_deque.end(), samples.begin(), sample_size, gen);
    }
    else
    {
        sample_size = memory_deque.size();
        samples.reserve(sample_size);
        samples.insert(samples.end(), memory_deque.begin(), memory_deque.end());
    }


    std::vector<std::array<int, N_states>> states;
    std::vector<std::array<int, N_actions>> actions;
    std::vector<int> rewards;
    std::vector<std::array<int, N_states>> next_states;
    std::vector<bool> game_overs;

    states.reserve(sample_size);
    actions.reserve(sample_size);
    rewards.reserve(sample_size);
    next_states.reserve(sample_size);
    game_overs.reserve(sample_size);

    for (size_t i = 0; i < sample_size; ++i)
    {
        states.push_back(samples[i].state);
        actions.push_back(samples[i].action);
        rewards.push_back(samples[i].reward);
        next_states.push_back(samples[i].next_state);
        game_overs.push_back(samples[i].game_over);
    }

    trainer->train_step(states, actions, rewards, next_states, game_overs);
}
