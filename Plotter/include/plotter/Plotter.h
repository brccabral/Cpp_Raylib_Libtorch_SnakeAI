#pragma once

#include <implot.h>
#include <string>
#include <vector>

class Plot2DData
{
public:

    Plot2DData();
    ~Plot2DData();

    void push_back(double x_, double y_);

    std::vector<double> x{};
    std::vector<double> y{};
    size_t count = 0;
    size_t id{};
};

typedef enum SeriesType
{
    SERIES_LINE = 0,
} SeriesType;

class Series
{
public:

    Series(SeriesType type, size_t data_index, std::string label);
    ~Series();

    SeriesType type{};
    size_t data_index{};
    std::string label{};
};

class Chart
{
public:

    Chart(std::string title, std::string x_label, std::string y_label);
    ~Chart();

    std::string title;
    std::string x_label;
    std::string y_label;
    ImPlotAxisFlags x_axis_flags{};
    ImPlotAxisFlags y_axis_flags{};

    double x_min{};
    double x_max{};
    double y_min{};
    double y_max{};

    std::vector<size_t> series;
};


class Plotter
{
public:

    Plotter();
    ~Plotter();
    void draw() const;

    size_t
    create_chart(const std::string &title, const std::string &x_label, const std::string &y_label);
    size_t create_data();
    size_t create_series(SeriesType type, size_t data_index, const std::string &label);
    void add_series_to_chart(size_t chart_index, size_t series_index);
    void push_data(size_t data_index, double x, double y);
    void set_chart_flags(
            size_t chart_index, ImPlotAxisFlags x_axis_flags_, ImPlotAxisFlags y_axis_flags_);
    void
    set_chart_limits(size_t chart_index, double x_min, double x_max, double y_min, double y_max);

    std::vector<Chart> charts;
    std::vector<Series> series;
    std::vector<Plot2DData> datas;
};
