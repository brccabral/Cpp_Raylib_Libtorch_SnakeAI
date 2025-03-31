#include <mlgames/QTrainer.h>


QTrainer::QTrainer(
        LinearNN *model_, torch::optim::Optimizer *optimizer_, const double gamma_,
        const c10::DeviceType device_)
{
    model = model_;
    gamma = gamma_;

    device = device_;
    optimizer = optimizer_;
}


void QTrainer::train_step(
        size_t count_samples, const std::vector<double> &old_states_,
        const std::vector<int> &actions_, const std::vector<int> &rewards_,
        const std::vector<double> &new_states_, const std::vector<bool> &dones_)
{
    const torch::Tensor old_states =
            torch::tensor(old_states_, torch::kDouble)
                    .reshape({(long) count_samples, (long) (*model)->input_size})
                    .to(device);
    const torch::Tensor actions =
            torch::tensor(actions_, torch::kInt)
                    .reshape({(long) count_samples, (long) (*model)->output_size})
                    .to(device, torch::kDouble);
    const torch::Tensor new_states =
            torch::tensor(new_states_, torch::kDouble)
                    .reshape({(long) count_samples, (long) (*model)->input_size})
                    .to(device);

    // 1: predict Q values with current state
    (*model)->train();
    torch::Tensor pred_action = (*model)->forward(old_states);

    // 2: Q_new = r + y * max(next_predicted_Q_value) -> only do this if not done
    (*model)->eval();
    torch::Tensor target = pred_action.detach().clone();
    {
        torch::NoGradGuard no_grad;
        for (size_t index = 0; index < dones_.size(); ++index)
        {
            auto q_new = rewards_[index];
            const long max_index = torch::argmax(actions[index]).item().toLong();
            if (!dones_[index])
            {
                q_new = q_new +
                        gamma * (*model)->forward(new_states[index])[max_index].item().toDouble();
            }
            target[index][max_index] = q_new;
        }
    }

    (*model)->train();
    optimizer->zero_grad();
    const torch::Tensor loss = criterion(pred_action, target);
    loss.backward();
    optimizer->step();
}
