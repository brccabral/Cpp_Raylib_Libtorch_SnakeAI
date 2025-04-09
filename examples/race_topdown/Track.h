#pragma once
#include <raylib.h>
#include <vector>


class Track
{
public:

    Track();
    virtual ~Track();

    virtual void draw(const Camera2D &camera) = 0;
    virtual Vector3 get_car_start() = 0;
    int get_width() const;
    int get_height() const;
    std::vector<int> distances;

protected:

    Texture texture;
    Vector2 start{};
    Vector2 finish{};
    const char *file;

    void set_distances(Color track_color);
};
