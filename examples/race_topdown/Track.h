#pragma once
#include <raylib.h>
#include <vector>


inline size_t index_from_location(Vector2 pt, size_t width)
{
    return int(pt.y) * width + int(pt.x);
}

class Car;
class Track
{
public:

    Track();
    virtual ~Track();

    virtual void draw(const Camera2D &camera) = 0;
    virtual Vector3 get_car_start() = 0;
    void update_sensors(std::vector<Car> &cars);
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
