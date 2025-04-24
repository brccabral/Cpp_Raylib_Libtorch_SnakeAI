#include <rlImGui.h>
#include <imgui.h>
#include <plotter/Plotter.h>

Plotter::Plotter()
{
    rlImGuiSetup(true);
}

Plotter::~Plotter()
{
    rlImGuiShutdown();
}

void Plotter::draw()
{
    rlImGuiBegin();
    bool open = true;
    if (ImGui::Begin("Test Window", &open))
    {
        ImGui::TextUnformatted("hello world");
    }
    ImGui::End();
    rlImGuiEnd();
}
