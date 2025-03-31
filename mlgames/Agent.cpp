#include <mlgames/Agent.h>


Agent::Agent(Linear_QNet *model_, QTrainer *trainer_, const c10::DeviceType device_)
{
    device = device_;

    model = model_;

    trainer = trainer_;
}


std::vector<int> Agent::get_play(const std::vector<double> &state, size_t count_samples) const
{
    const auto state0 = torch::tensor(state, torch::kFloat)
                                .reshape({(long) count_samples, (long) (*model)->input_size})
                                .to(device);
    (*model)->eval();
    auto prediction = torch::Tensor();
    {
        torch::NoGradGuard no_grad;
        prediction = (*model)->forward(state0);
    }
    std::vector<int> action(prediction.size(1), 0);
    action[torch::argmax(prediction).item().toInt()] = 1;
    return action;
}


std::vector<int> Agent::get_action(const std::vector<double> &state, size_t count_samples) const
{
    const int epsilon = 80 - number_of_games;

    // epsilon = tradeoff between exploration vs exploitation
    // in the beginning this is true for some time, later self.number_of_games is larger
    if (rand() % 200 < epsilon)
    {
        std::vector<int> action((*model)->output_size * count_samples, 0);
        action[rand() % (*model)->output_size] = 1;
        return action;
    }

    return get_play(state, count_samples);
}


void Agent::train_short_memory(
        const std::vector<double> &state, const std::vector<int> &action, int reward,
        const std::vector<double> &next_state, bool game_over) const
{
    trainer->train_step(1, state, action, {reward}, next_state, {game_over});
}


void Agent::remember(
        const std::vector<double> &state, const std::vector<int> &action, const int reward,
        const std::vector<double> &next_state, const bool game_over)
{
    memory_deque.push_back(
            {(*model)->input_size, (*model)->output_size, state, action, reward, next_state,
             game_over});
}


void Agent::train_long_memory()
{
    std::vector<MemoryData> samples;
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

    const size_t num_state = samples[0].num_state;
    const size_t num_action = samples[0].num_action;

    std::vector<double> states;
    std::vector<int> actions;
    std::vector<int> rewards;
    std::vector<double> next_states;
    std::vector<bool> game_overs;

    states.reserve(sample_size * num_state);
    actions.reserve(sample_size * num_action);
    rewards.reserve(sample_size);
    next_states.reserve(sample_size * num_state);
    game_overs.reserve(sample_size);

    for (size_t i = 0; i < sample_size; ++i)
    {
        states.insert(states.end(), samples[i].state.begin(), samples[i].state.end());
        next_states.insert(
                next_states.end(), samples[i].next_state.begin(), samples[i].next_state.end());
        actions.insert(actions.end(), samples[i].action.begin(), samples[i].action.end());
        rewards.push_back(samples[i].reward);
        game_overs.push_back(samples[i].game_over);
    }

    trainer->train_step(sample_size, states, actions, rewards, next_states, game_overs);
}
