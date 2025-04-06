#pragma once
#include <raylib.h>
#include <vector>


class Track
{
public:

    Track(const char *file, Vector2 finish, Color track_color);
    ~Track();

    void draw(const Camera2D &camera);

private:

    Texture texture;
    std::vector<int> distances;
};
