#pragma once

#include <vector>

typedef struct StepResult
{
    int reward = 0;
    bool game_over = false;
} StepResult;

class AbstractGame
{
public:

    virtual ~AbstractGame() = default;

    virtual size_t get_state_size() const = 0;
    virtual size_t get_action_count() const = 0;
    virtual int get_score() const = 0;
    virtual StepResult get_step(int action) = 0;
    virtual std::vector<int> get_state() const = 0;
    virtual void reset() = 0;
};
