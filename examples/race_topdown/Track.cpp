#include "common.h"
#include <list>
#include <raylib.h>
#include "Track.h"


struct DistanceLoc
{
    Vector2 p;
    int distance;
};

static Vector2 p_right = {1, 0};
static Vector2 p_left = {-1, 0};
static Vector2 p_up = {0, -1};
static Vector2 p_down = {0, 1};

Track::Track() = default;

Track::~Track()
{
    UnloadTexture(texture);
}

void Track::set_distances(Color track_color)
{
    const Image image = LoadImage(file);
    texture = LoadTextureFromImage(image);

    distances.resize(image.width * image.height, 0);

    std::list<DistanceLoc> nodes;
    nodes.emplace_back(start, 1);

    auto CheckNewDistance = [&](const Vector2 &pt, int distance, std::list<DistanceLoc> &new_nodes)
    {
        const Color color = GetImageColor(image, pt.x, pt.y);
        if (ColorIsEqual(color, track_color) &&
            distances[index_from_location(pt, image.width)] == 0)
        {
            new_nodes.emplace_back(pt, distance + 1);
        }
    };

    while (!nodes.empty())
    {
        std::list<DistanceLoc> new_nodes;
        for (auto &[p, distance]: nodes)
        {
            distances[index_from_location(p, image.width)] = distance;

            Vector2 right = p + p_right;
            Vector2 left = p + p_left;
            Vector2 up = p + p_up;
            Vector2 down = p + p_down;

            CheckNewDistance(right, distance, new_nodes);
            CheckNewDistance(left, distance, new_nodes);
            CheckNewDistance(up, distance, new_nodes);
            CheckNewDistance(down, distance, new_nodes);
        }

        new_nodes.sort(
                [&](const DistanceLoc &a, const DistanceLoc &b)
                {
                    return index_from_location(a.p, image.width) <
                           index_from_location(b.p, image.width);
                });
        new_nodes.unique([&](const DistanceLoc &a, const DistanceLoc &b) { return a.p == b.p; });
        nodes.clear();
        nodes.insert(nodes.begin(), new_nodes.begin(), new_nodes.end());
    }

    UnloadImage(image);
}

int Track::get_width() const
{
    return texture.width;
}

int Track::get_height() const
{
    return texture.height;
}
