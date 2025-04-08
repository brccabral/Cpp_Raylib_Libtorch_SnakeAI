#include <list>
#include <raylib.h>
#include <raymath.h>
#include "Track.h"
#include "Car.h"


struct DistanceLoc
{
    Vector2 p;
    int distance;
};

inline bool operator==(const Color &lhs, const Color &rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

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
    nodes.emplace_back(finish, 1);

    auto CheckNewDistance = [&](const Vector2 &pt, int distance, std::list<DistanceLoc> &new_nodes)
    {
        const Color color = GetImageColor(image, pt.x, pt.y);
        if (color == track_color && distances[index_from_location(pt, image.width)] == 0)
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

void Track::update_sensors(std::vector<Car> &cars)
{
    for (auto &car: cars)
    {
        for (int i = 0; i < NUM_SENSORS; ++i)
        {
            Vector2 sensor_position = car.position;
            double sensor_angle = car.angle - 90 + i * 180.0 / NUM_SENSORS;
            Vector2 step = Vector2(cos(sensor_angle * DEG2RAD), sin(sensor_angle * DEG2RAD));
            while (true)
            {
                if (distances[index_from_location(sensor_position, texture.width)] == 0)
                {
                    car.sensors_distance[i] = Vector2Distance(car.position, sensor_position);
                    break;
                }
                sensor_position += step;
            }
        }
    }
}
