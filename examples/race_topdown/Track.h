#pragma once
#include <raylib.h>
#include <vector>


class Track
{
public:

    Track(const char *file_, Vector2 start_, Vector2 finish_, Color track_color);
    ~Track();

    void draw(const Camera2D &camera);

private:

    Texture texture;
    std::vector<int> distances;
    Vector2 start{};
    Vector2 finish{};
    const char *file;

    void set_distances(Color track_color);
};
