#include <ctime>
#include <random>
#include <cstring>
#include <raylib.h>
#include "RaceTopDown.h"
#include <plotter/Plotter.h>


#define NET_LIB_TORCH 1
#define NET_LIB_EIGEN 2
#define NET_LIB_DNA 3


#if !MANUAL

#define NET_LIB NET_LIB_EIGEN

#if (NET_LIB == NET_LIB_TORCH)
#include <mlgames/GenPopulation.h>
#include <mlgames/LinearGen.h>
#elif (NET_LIB == NET_LIB_EIGEN)
#include <mlgames/NetGen.h>
#include <mlgames/NetPopulation.h>
#elif (NET_LIB == NET_LIB_DNA)
#include <mlgames/NetDNA.h>
#include <mlgames/PopulationDNA.h>
#endif
#endif

#define do_shift(argc, argv)                                                                       \
    do                                                                                             \
    {                                                                                              \
        if ((argc) > 1)                                                                            \
        {                                                                                          \
            (argv)++;                                                                              \
            (argc)--;                                                                              \
        }                                                                                          \
    }                                                                                              \
    while (0)

int main(int argc, char *argv[])
{
    // NOLINTNEXTLINE
    srand(time(NULL));

#if !MANUAL

    const char *net_filename = NULL;
    while (argc > 1)
    {
        const char *command = argv[1];
        if (strcmp(command, "--net") == 0)
        {
            net_filename = argv[2];
            do_shift(argc, argv);
        }
        do_shift(argc, argv);
    }

#if NET_LIB == NET_LIB_TORCH
    c10::DeviceType device = torch::kCPU;
    if (torch::cuda::is_available())
    {
        printf("Using CUDA\n");
        device = torch::kCUDA;
    }
    else
    {
        printf("Using CPU\n");
        device = torch::kCPU;
    }

#endif
#endif

    constexpr int hidden_layer_1 = 6;

    constexpr int SCREEN_WIDTH = 1366;
    constexpr int SCREEN_HEIGHT = 768;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Race TopDown");

#if MANUAL
    constexpr int num_cars = 1;
    SetTargetFPS(60);
#else
    constexpr int num_cars = 1000;
#endif

    {
        auto game = RaceTopDown(num_cars);
        auto plotter = Plotter();
        auto distance_chart = plotter.create_chart("Distances", "Generation", "Distance");
        plotter.set_chart_flags(
                distance_chart, ImPlotAxisFlags_AutoFit,
                ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_LockMin);
        plotter.set_chart_limits(distance_chart, 0, 1, 0, 1);
        auto record_data = plotter.create_data();
        auto best_data = plotter.create_data();
        auto record_series = plotter.create_series(SERIES_LINE, record_data, "Record");
        auto best_series = plotter.create_series(SERIES_LINE, best_data, "Best");

        float record = 0;

#if !MANUAL
        size_t generation = 0;
#if NET_LIB == NET_LIB_TORCH

        auto net = LinearGen(
                RaceTopDown::get_state_size(), Car::get_action_count(),
                std::vector<size_t>{hidden_layer_1});
        if (net_filename != NULL)
        {
            std::cout << net_filename << "\n";
            load_model<>(net, net_filename);
        }
        net->to(device);
        auto population = GenPopulation(num_cars, 0.9, 0.05, net);

        torch::NoGradGuard no_grad;
#elif (NET_LIB == NET_LIB_EIGEN)
        auto net =
                NetGen(RaceTopDown::get_state_size(), Car::get_action_count(),
                       std::vector<size_t>{hidden_layer_1});
        auto population = NetPopulation(num_cars, 0.9, 0.05, net);
#elif (NET_LIB == NET_LIB_DNA)
        auto net =
                NetDNA(1, RaceTopDown::get_state_size(), hidden_layer_1, Car::get_action_count());
        auto population = PopulationDNA(num_cars, 0.9, 0.05, net);
        float *actions = new float[Car::get_action_count()];
#endif
#endif

        while (!WindowShouldClose())
        {
#if MANUAL
            int action = 0;
            if (IsKeyDown(KEY_LEFT))
            {
                action = action | Car::CAR_ACTION_LEFT;
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                action = action | Car::CAR_ACTION_RIGHT;
            }
            if (IsKeyDown(KEY_UP))
            {
                action = action | Car::CAR_ACTION_ACCELERATE;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                action = action | Car::CAR_ACTION_BREAK;
            }
            if (IsKeyPressed(KEY_W))
            {
                action = action | Car::CAR_ACTION_ACCELERATE;
            }
            if (IsKeyPressed(KEY_S))
            {
                action = action | Car::CAR_ACTION_BREAK;
            }
            if (IsKeyPressed(KEY_A))
            {
                action = action | Car::CAR_ACTION_LEFT;
            }
            if (IsKeyPressed(KEY_D))
            {
                action = action | Car::CAR_ACTION_RIGHT;
            }
            game.apply_action(0, action);
#else
            for (size_t i = 0; i < num_cars; ++i)
            {
                if (game.cars[i].car_state == Car::CAR_STATE_DEAD)
                {
                    continue;
                }
                std::vector<float> inputs = game.get_state(i);
                int action = 0;
#if NET_LIB == NET_LIB_TORCH
                torch::Tensor x = torch::tensor(inputs, torch::kFloat)
                                          .reshape({1, (long) RaceTopDown::get_state_size()});
                x.to(device);
                auto actions = population.members[i]->forward(x);
                if (actions[0][0].item<float>() > 0)
                {
                    action = action | Car::CAR_ACTION_LEFT;
                }
                if (actions[0][1].item<float>() > 0)
                {
                    action = action | Car::CAR_ACTION_RIGHT;
                }
                if (actions[0][2].item<float>() > 0)
                {
                    action = action | Car::CAR_ACTION_ACCELERATE;
                }
                if (actions[0][3].item<float>() > 0)
                {
                    action = action | Car::CAR_ACTION_BREAK;
                }
#elif (NET_LIB == NET_LIB_EIGEN)
                std::vector<double> input_double(inputs.size());
                for (size_t j = 0; j < inputs.size(); ++j)
                {
                    input_double[j] = inputs[j];
                }
                Eigen::Map<MLMatrix> x(input_double.data(), 1, RaceTopDown::get_state_size());
                auto actions = population.members[i].forward(x, true);
                if (actions[0] > 0)
                {
                    action = action | Car::CAR_ACTION_LEFT;
                }
                if (actions[1] > 0)
                {
                    action = action | Car::CAR_ACTION_RIGHT;
                }
                if (actions[2] > 0)
                {
                    action = action | Car::CAR_ACTION_ACCELERATE;
                }
                if (actions[3] > 0)
                {
                    action = action | Car::CAR_ACTION_BREAK;
                }
#elif (NET_LIB == NET_LIB_DNA)
                population.members[i].forward(inputs.data(), actions);
                if (actions[0] > 0)
                {
                    action = action | Car::CAR_ACTION_LEFT;
                }
                if (actions[1] > 0)
                {
                    action = action | Car::CAR_ACTION_RIGHT;
                }
                if (actions[2] > 0)
                {
                    action = action | Car::CAR_ACTION_ACCELERATE;
                }
                if (actions[3] > 0)
                {
                    action = action | Car::CAR_ACTION_BREAK;
                }
#endif
                game.apply_action(i, action);
            }
#endif

            game.update();

            BeginDrawing();
            ClearBackground(BLACK);
            game.draw();
            plotter.draw();
            EndDrawing();

            if (game.check_end_game())
            {
                if (game.max_distance > record)
                {
                    record = game.max_distance;
                }
                plotter.push_data(record_data, generation, record);
                plotter.push_data(best_data, generation, game.max_distance);
#if !MANUAL
                printf("Generation %zu Distance: %d Dead: %zu Record %.0f\n", generation,
                       game.max_distance, game.num_dead, record);
                ++generation;
#if NET_LIB == NET_LIB_TORCH
                save_model<>(population.members[game.best_car_index]);
#endif
                population.apply_mutations(game.select_best_cars());
#endif
                game.reset();
            }
        }
#if (NET_LIB == NET_LIB_DNA)
        delete[] actions;
#endif
    }
    CloseWindow();
}
