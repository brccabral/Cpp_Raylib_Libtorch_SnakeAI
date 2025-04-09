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
    delete[] distances.values;
    UnloadTexture(texture);
}

void Track::set_distances(Color track_color)
{
    const Image image = LoadImage(file);
    texture = LoadTextureFromImage(image);

    distances = Distances(image.width, image.height);

    std::list<DistanceLoc> nodes;
    nodes.emplace_back(start, 1);

    auto CheckNewDistance = [&](const Vector2 &pt, int distance, std::list<DistanceLoc> &new_nodes)
    {
        const Color color = GetImageColor(image, pt.x, pt.y);
        if (ColorIsEqual(color, track_color) && distances[pt] == 0)
        {
            new_nodes.emplace_back(pt, distance + 1);
        }
    };

    int max_distance = 0;

    while (!nodes.empty())
    {
        std::list<DistanceLoc> new_nodes;
        for (auto &[p, distance]: nodes)
        {
            if (distance > max_distance)
            {
                max_distance = distance;
            }
            distances[p] = distance;

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
                    if (a.p.x == b.p.x)
                    {
                        return a.p.y < b.p.y;
                    }
                    return a.p.x < b.p.x;
                });
        new_nodes.unique([&](const DistanceLoc &a, const DistanceLoc &b) { return a.p == b.p; });
        nodes.clear();
        nodes.insert(nodes.begin(), new_nodes.begin(), new_nodes.end());
    }

    update_finish_line_distances(max_distance + 1);

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
