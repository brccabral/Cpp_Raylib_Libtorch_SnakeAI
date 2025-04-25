#include <string>
#include <vector>

class Plot2DData
{
public:

    Plot2DData();
    ~Plot2DData();

    void push_back(double x_, double y_);

    std::string title;
    std::string x_label;
    std::string y_label;

    std::vector<double> x{};
    std::vector<double> y{};
    int count = 0;
};


class Plotter
{
public:

    Plotter();
    ~Plotter();
    void draw();

    int add_plot(std::string title, std::string x_label, std::string y_label);
    void push_data(int index, double x, double y);

private:

    std::vector<Plot2DData> plots;
};
