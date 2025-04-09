#pragma once
#include <raylib.h>


class Track
{
public:

    Track();
    virtual ~Track();

    virtual void draw(const Camera2D &camera) = 0;
    virtual Vector3 get_car_start() = 0;
    virtual void reset() = 0;
    virtual void update_finish_line_distances(int max_distance) = 0;

    int get_width() const;
    int get_height() const;
    int **distances;


protected:

    Texture texture;
    Vector2 start{};
    Vector2 finish{};
    const char *file;

    void set_distances(Color track_color);
};
