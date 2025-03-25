#include <cstdlib>
#include <random>
#include "Agent.h"

Agent::Agent(Linear_QNet *model_, QTrainer *trainer_, c10::DeviceType device_)
{
    device = device_;

    model = model_;
    (*model)->train();

    trainer = trainer_;
}

Agent::~Agent()
{}

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

std::array<int, OUTPUT_SIZE> Agent::get_play(std::array<int, INPUT_SIZE> state)
{
    auto state0 = torch::from_blob(state.begin(), {state.size()}).to(device);
    auto prediction = (*model)->forward(state0);
    std::array<int, OUTPUT_SIZE> action{};
    action[torch::argmax(prediction).item().toInt()] = 1;
    return action;
}

std::array<int, OUTPUT_SIZE> Agent::get_action(std::array<int, INPUT_SIZE> state)
{
    if (!is_training)
    {
        {
            torch::NoGradGuard no_grad;
            return get_play(state);
        }
    }

    epsilon = 80 - number_of_games;

    // epsilon = tradeoff between exploration vs exploitation
    // in the beginning this is true for some time, later self.number_of_games is larger
    if (rand() % 200 < epsilon)
    {
        std::array<int, OUTPUT_SIZE> action{};
        action[rand() % 3] = 1;
        return action;
    }

    return get_play(state);
}

void Agent::train_short_memory(
        std::array<int, INPUT_SIZE> state, std::array<int, OUTPUT_SIZE> action, int reward,
        std::array<int, INPUT_SIZE> next_state, bool game_over)
{
    trainer->train_step<INPUT_SIZE, OUTPUT_SIZE>(
            {state}, {action}, {reward}, {next_state}, {game_over});
}

void Agent::remember(
        const std::array<int, INPUT_SIZE> &state, const std::array<int, OUTPUT_SIZE> action,
        const int reward, const std::array<int, INPUT_SIZE> &next_state, const bool game_over)
{
    memory_deque.push_back({state, action, reward, next_state, game_over});
}

void Agent::train_long_memory()
{
    size_t sample_size = memory_deque.size();
    if (sample_size > BATCH_SIZE)
    {
        sample_size = BATCH_SIZE;
    }

    std::vector<MemoryData> samples(sample_size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::ranges::sample(
            memory_deque.begin(), memory_deque.end(), samples.begin(), samples.size(), gen);

    std::vector<std::array<int, INPUT_SIZE>> states;
    std::vector<std::array<int, OUTPUT_SIZE>> actions;
    std::vector<int> rewards;
    std::vector<std::array<int, INPUT_SIZE>> next_states;
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

    trainer->train_step<INPUT_SIZE, OUTPUT_SIZE>(states, actions, rewards, next_states, game_overs);
}
