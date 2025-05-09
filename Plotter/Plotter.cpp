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

Plot2DData::Plot2DData() = default;
Plot2DData::~Plot2DData() = default;

Chart::Chart(std::string title, std::string x_label, std::string y_label)
    : title(std::move(title)), x_label(std::move(x_label)), y_label(std::move(y_label))
{}
Chart::~Chart() = default;

Series::Series(SeriesType type, size_t data_index, std::string label)
    : type(type), data_index(data_index), label(std::move(label))
{}
Series::~Series() = default;

void Plotter::draw() const
{
    rlImGuiBegin();
    bool open = true;
    if (ImGui::Begin("Plots", &open))
    {
        for (const auto &chart: charts)
        {
            if (ImPlot::BeginPlot(chart.title.c_str()))
            {
                ImPlot::SetupAxes(
                        chart.x_label.c_str(), chart.y_label.c_str(), chart.x_axis_flags,
                        chart.y_axis_flags);
                ImPlot::SetupAxesLimits(chart.x_min, chart.x_max, chart.y_min, chart.y_max);
                for (const auto &series_: series)
                {
                    auto &data = datas[series_.data_index];
                    if (series_.type == SERIES_LINE)
                    {
                        ImPlot::PlotLine(
                                series_.label.c_str(), data.x.data(), data.y.data(), data.count);
                    }
                }
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }
    rlImGuiEnd();
}

size_t Plotter::create_chart(
        const std::string &title, const std::string &x_label, const std::string &y_label)
{
    charts.emplace_back(title, x_label, y_label);
    return charts.size() - 1;
}

void Plot2DData::push_back(double x_, double y_)
{
    x.push_back(x_);
    y.push_back(y_);
    ++count;
}

size_t Plotter::create_series(SeriesType type, size_t data_index, const std::string &label)
{
    series.emplace_back(type, data_index, label);
    return series.size() - 1;
}

size_t Plotter::create_data()
{
    datas.emplace_back();
    const size_t index = datas.size() - 1;
    return index;
}

void Plotter::add_series_to_chart(size_t chart_index, size_t series_index)
{
    charts[chart_index].series.emplace_back(series_index);
}

void Plotter::push_data(size_t data_index, double x, double y)
{
    datas[data_index].push_back(x, y);
}

void Plotter::set_chart_flags(
        size_t chart_index, ImPlotAxisFlags x_axis_flags_, ImPlotAxisFlags y_axis_flags_)
{
    charts[chart_index].x_axis_flags = x_axis_flags_;
    charts[chart_index].y_axis_flags = y_axis_flags_;
}

void Plotter::set_chart_limits(
        size_t chart_index, double x_min, double x_max, double y_min, double y_max)
{
    charts[chart_index].x_min = x_min;
    charts[chart_index].x_max = x_max;
    charts[chart_index].y_min = y_min;
    charts[chart_index].y_max = y_max;
}
