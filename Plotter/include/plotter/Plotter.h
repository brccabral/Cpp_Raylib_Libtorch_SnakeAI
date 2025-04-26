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

    Series();
    ~Series();

    SeriesType type{};
    size_t data_index{};
    std::string label{};
};

class Chart
{
public:

    Chart();
    ~Chart();

    std::string title;
    std::string x_label;
    std::string y_label;

    std::vector<size_t> series;
};


class Plotter
{
public:

    Plotter();
    ~Plotter();
    void draw();

    size_t create_chart(std::string title, std::string x_label, std::string y_label);
    size_t create_data();
    size_t create_series(SeriesType type, size_t data_index, std::string label);
    void add_series_to_chart(size_t chart_index, size_t series_index);
    void push_data(size_t data_index, double x, double y);

    std::vector<Chart> charts;
    std::vector<Series> series;
    std::vector<Plot2DData> datas;
};
