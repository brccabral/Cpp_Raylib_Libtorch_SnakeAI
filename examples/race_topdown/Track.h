#pragma once
#include <raylib.h>
#include <vector>


inline size_t index_from_location(Vector2 pt, size_t width)
{
    return pt.y * width + pt.x;
}

class Car;
class Track
{
public:

    Track();
    virtual ~Track();

    virtual void draw(const Camera2D &camera) = 0;
    virtual void set_car_start(Car *car) = 0;
    int get_width() const;
    int get_height() const;

protected:

    Texture texture;
    std::vector<int> distances;
    Vector2 start{};
    Vector2 finish{};
    const char *file;

    void set_distances(Color track_color);
};
