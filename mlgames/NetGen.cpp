#include <ATen/core/interned_strings.h>
#include <mlgames/NetGen.h>

NetGen::NetGen(size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_)
{
    ml_input_size = input_size_;
    ml_output_size = output_size_;
    hidden_sizes = hidden_sizes_;
    ml_count_hidden = hidden_sizes.size();
    setup();
}

void NetGen::copy(const NetGen &other)
{
    ml_input_size = other.ml_input_size;
    ml_output_size = other.ml_output_size;
    hidden_sizes = other.hidden_sizes;
    ml_count_hidden = hidden_sizes.size();

    ml_hidden_w = new MLMatrix[ml_count_hidden];
    ml_hidden_b = new MLVector[ml_count_hidden];
    ml_zh = new MLMatrix[ml_count_hidden];
    ml_ah = new MLMatrix[ml_count_hidden];
    zh = new MLMatrix[ml_count_hidden];
    ah = new MLMatrix[ml_count_hidden];

    for (size_t i = 0; i < other.ml_count_hidden; i++)
    {
        ml_hidden_w[i] = other.ml_hidden_w[i];
        ml_hidden_b[i] = other.ml_hidden_b[i];
    }
    ml_output_w = other.ml_output_w;
    ml_output_b = other.ml_output_b;
}

NetGen::NetGen(const NetGen &other)
{
    copy(other);
}

NetGen::NetGen(NetGen &&other) noexcept
{
    ml_input_size = other.ml_input_size;
    ml_output_size = other.ml_output_size;
    hidden_sizes = other.hidden_sizes;
    ml_count_hidden = other.ml_count_hidden;

    ml_hidden_w = other.ml_hidden_w;
    ml_hidden_b = other.ml_hidden_b;
    ml_zh = other.ml_zh;
    ml_ah = other.ml_ah;
    zh = other.zh;
    ah = other.ah;

    ml_output_w = other.ml_output_w;
    ml_output_b = other.ml_output_b;

    other.ml_hidden_w = nullptr;
    other.ml_hidden_b = nullptr;
    other.ml_zh = nullptr;
    other.ml_ah = nullptr;
    other.zh = nullptr;
    other.ah = nullptr;
}

NetGen &NetGen::operator=(const NetGen &other)
{
    if (this != &other)
    {
        delete[] ml_hidden_w;
        delete[] ml_hidden_b;
        delete[] ml_zh;
        delete[] ml_ah;
        delete[] zh;
        delete[] ah;
        copy(other);
    }
    return *this;
}

void NetGen::setup()
{
    ml_hidden_w = new MLMatrix[ml_count_hidden];
    ml_hidden_b = new MLVector[ml_count_hidden];
    ml_zh = new MLMatrix[ml_count_hidden];
    ml_ah = new MLMatrix[ml_count_hidden];
    zh = new MLMatrix[ml_count_hidden];
    ah = new MLMatrix[ml_count_hidden];

    size_t prev = ml_input_size;
    size_t current = ml_input_size;
    for (size_t i = 0; i < ml_count_hidden; i++)
    {
        current = hidden_sizes[i];
        ml_hidden_w[i].resize(current, prev);
        ml_hidden_w[i] = MLMatrix::Random(current, prev);
        ml_hidden_w[i] = ml_hidden_w[i].array() / 2.0F;
        ml_hidden_b[i].resize(current);
        ml_hidden_b[i] = MLVector::Random(current);
        ml_hidden_b[i] = ml_hidden_b[i].array() / 2.0F;
        prev = current;
    }
    ml_output_w = MLMatrix::Random(ml_output_size, current);
    ml_output_w = ml_output_w.array() / 2.0F;
    ml_output_b = MLVector::Random(ml_output_size);
    ml_output_b = ml_output_b.array() / 2.0F;
}

NetGen::~NetGen()
{
    delete[] ml_hidden_w;
    delete[] ml_hidden_b;
    delete[] ml_zh;
    delete[] ml_ah;
    delete[] zh;
    delete[] ah;
}

void NetGen::forward(const MLMatrix &X, const bool guard)
{
    if (guard)
    {
        // inputs is vector
        zh[0] = ml_hidden_w[0] * X.transpose();
        zh[0] = zh[0].colwise() + ml_hidden_b[0];
        ah[0] = relu(zh[0]);

        for (size_t i = 1; i < ml_count_hidden; ++i)
        {
            zh[i] = ml_hidden_w[i] * zh[i - 1];
            zh[i] = zh[i].colwise() + ml_hidden_b[i];
            ah[i] = relu(zh[i]);
        }

        zo = ml_output_w * ah[ml_count_hidden - 1];
        zo = zo.colwise() + ml_output_b;
        ao = softmax(zo);
        return;
    }

    // inputs is vector
    ml_zh[0] = ml_hidden_w[0] * X.transpose();
    ml_zh[0] = ml_zh[0].colwise() + ml_hidden_b[0];
    ml_ah[0] = relu(ml_zh[0]);

    for (size_t i = 1; i < ml_count_hidden; ++i)
    {
        ml_zh[i] = ml_hidden_w[i] * ml_zh[i - 1];
        ml_zh[i] = ml_zh[i].colwise() + ml_hidden_b[i];
        ml_ah[i] = relu(ml_zh[i]);
    }

    ml_zo = ml_output_w * ml_ah[ml_count_hidden - 1];
    ml_zo = ml_zo.colwise() + ml_output_b;
    ml_ao = softmax(ml_zo);
}

MLMatrix relu(const MLMatrix &m)
{
    return m.cwiseMax(0);
}

MLMatrix softmax(const MLMatrix &m)
{
    MLMatrix e = (m - m.colwise().maxCoeff().replicate(m.rows(), 1)).array().exp();
    return e.array().rowwise() / e.array().colwise().sum();
}

NetGen NetGen::clone() const
{
    NetGen result = NetGen(ml_hidden_w[0].cols(), ml_output_w.rows(), hidden_sizes);

    for (size_t i = 0; i < ml_count_hidden; ++i)
    {
        result.ml_hidden_w[i] = ml_hidden_w[i];
        result.ml_hidden_b[i] = ml_hidden_b[i];
    }
    result.ml_output_w = ml_output_w;
    result.ml_output_b = ml_output_b;

    return result;
}

size_t NetGen::get_output_index(size_t sample_index, const bool guard)
{
    MLIndex maxIndex;
    if (guard)
    {
        ao.col(sample_index).maxCoeff(&maxIndex);
    }
    else
    {
        ml_ao.col(sample_index).maxCoeff(&maxIndex);
    }
    return maxIndex;
}
