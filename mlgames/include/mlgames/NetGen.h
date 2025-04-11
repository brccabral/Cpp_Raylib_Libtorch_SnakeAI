#pragma once

#include <Eigen/Dense>

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MLMatrix;
typedef Eigen::Vector<double, Eigen::Dynamic> MLVector;
typedef Eigen::Index MLIndex;
using ml_random_ptr = double (*)(double, double);
inline ml_random_ptr ml_random = [](const double a, const double b)
{ return Eigen::internal::random(a, b); };

class NetGen
{
public:

    NetGen(size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_);
    NetGen(const NetGen &other);
    NetGen(NetGen &&other) noexcept;
    NetGen &operator=(const NetGen &other);
    ~NetGen();

    MLVector forward(const MLMatrix &X, bool guard);
    NetGen clone() const;
    void mutate(double mutation_rate);

    size_t ml_input_size{};
    size_t ml_output_size{};
    size_t ml_count_hidden{};
    std::vector<size_t> hidden_sizes{};
    MLMatrix *ml_hidden_w{};
    MLVector *ml_hidden_b{};
    MLMatrix ml_output_w{};
    MLVector ml_output_b{};

    MLMatrix *ml_zh{}, *ml_ah{};

private:

    MLMatrix ml_ao{};

    void setup();
    void copy(const NetGen &other);
};

MLMatrix relu(const MLMatrix &m);
MLMatrix softmax(const MLMatrix &m);
