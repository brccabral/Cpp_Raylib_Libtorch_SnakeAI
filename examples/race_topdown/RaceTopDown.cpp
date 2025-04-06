#include <raylib.h>
#include <raymath.h>
#include "RaceTopDown.h"


RaceTopDown::RaceTopDown()
{
    camera.zoom = 1.0f;
    tracks.emplace_back("assets/track1.png", Vector2(1347, 1732), Color(110, 110, 110, 255));
};

void RaceTopDown::update()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 delta = GetMouseDelta();
        delta *= -1.0f / camera.zoom;
        camera.target += delta;
    }

    // Zoom based on mouse wheel
    const float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        // Get the world point that is under the mouse
        const Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        // Set the offset to where the mouse is
        camera.offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera.target = mouseWorldPos;

        // Zoom increment
        // Uses log scaling to provide consistent zoom speed
        const float scale = 0.2f * wheel;
        camera.zoom = Clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);
    }
}

void RaceTopDown::draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    tracks[current_track].draw(camera);
    EndDrawing();
}
