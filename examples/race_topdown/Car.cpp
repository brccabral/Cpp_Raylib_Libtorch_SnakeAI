#include "Car.h"

Car::Car(Texture *texture_, Color color_)
{
    texture = texture_;
    color = color_;

    x = GetRandomValue(0, 200);
}

void Car::draw(Camera2D &camera)
{
    BeginMode2D(camera);
    DrawTextureEx(*texture, {x, y}, angle, 20.0 / texture->height, color);
    EndMode2D();
}
