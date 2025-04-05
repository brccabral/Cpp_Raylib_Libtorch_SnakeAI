#include <raylib.h>
#include "RaceTopDown.h"


RaceTopDown::RaceTopDown()
{
    tracks.emplace_back("assets/track1.png", Vector2(1347, 1732), Color(110, 110, 110, 255));
};

void RaceTopDown::update()
{}

void RaceTopDown::draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    tracks[current_track].draw();
    EndDrawing();
}
