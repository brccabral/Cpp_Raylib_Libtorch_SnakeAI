#include <mlgames/MultiLayer.h>

MultiLayer::MultiLayer() = default;
MultiLayer::~MultiLayer() = default;

void save_model(std::shared_ptr<MultiLayer> model, const char *filename)
{
    torch::save(model, filename);
}

void load_model(std::shared_ptr<MultiLayer> model, const char *filename)
{
    torch::load(model, filename);
}
