#include <rlImGui.h>
#include <imgui.h>
#include <plotter/Plotter.h>
#include <implot.h>

#include <utility>

Plotter::Plotter()
{
    rlImGuiSetup(true);
    ImPlot::CreateContext();

    ImGui::StyleColorsDark();
    ImPlot::StyleColorsDark();
}

Plotter::~Plotter()
{
    rlImGuiShutdown();
}

void Plotter::draw()
{
    rlImGuiBegin();
    bool open = true;
    if (ImGui::Begin("Plots", &open))
    {
        for (auto p = 0; p < plots.size(); ++p)
        {
            auto &plot = plots[p];
            if (ImPlot::BeginPlot(plot.title.c_str()))
            {
                ImPlot::SetupAxes(
                        plot.x_label.c_str(), plot.y_label.c_str(), ImPlotAxisFlags_AutoFit,
                        ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_LockMin);
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
                ImPlot::PlotLine(plot.title.c_str(), plot.x.data(), plot.y.data(), plot.count);
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }
    rlImGuiEnd();
}

int Plotter::add_plot(std::string title, std::string x_label, std::string y_label)
{
    plots.emplace_back();
    const int index = plots.size() - 1;
    plots[index].title = std::move(title);
    plots[index].x_label = std::move(x_label);
    plots[index].y_label = std::move(y_label);
    return index;
}

void Plotter::push_data(int index, double x, double y)
{
    plots[index].push_back(x, y);
}


Plot2DData::Plot2DData() = default;
Plot2DData::~Plot2DData() = default;

void Plot2DData::push_back(double x_, double y_)
{
    x.push_back(x_);
    y.push_back(y_);
    ++count;
}
