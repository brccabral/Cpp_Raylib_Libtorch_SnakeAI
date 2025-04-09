#include <ctime>
#include <random>
#include <raylib.h>
#include "RaceTopDown.h"

#include <mlgames/GenPopulation.h>
#include <mlgames/LinearGen.h>

#define MANUAL 0

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

    constexpr int hidden_layer_1 = 8;

    constexpr int SCREEN_WIDTH = 1366;
    constexpr int SCREEN_HEIGHT = 768;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Race TopDown");

#if MANUAL
    constexpr int num_cars = 1;
    SetTargetFPS(60);
#else
    constexpr int num_cars = 15;
#endif

    {
        auto game = RaceTopDown(num_cars);

        auto net = LinearGen(
                RaceTopDown::get_state_size(), Car::CAR_ACTION_COUNT,
                std::vector<size_t>{hidden_layer_1});
        if (net_filename != NULL)
        {
            std::cout << net_filename << "\n";
            load_model<>(net, net_filename);
        }
        net->to(device);
        auto population = GenPopulation(num_cars, 0.9, 0.05, net);

        torch::NoGradGuard no_grad;

        while (!WindowShouldClose())
        {
#if MANUAL
            int action = 0;
            if (IsKeyDown(KEY_LEFT))
            {
                action = 0b0001;
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                action = 0b0010;
            }
            if (IsKeyDown(KEY_UP))
            {
                action = action | 0b0100;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                action = action | 0b1000;
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
                torch::Tensor x = torch::tensor(inputs, torch::kFloat)
                                          .reshape({1, (long) RaceTopDown::get_state_size()});
                x.to(device);
                auto actions = population.members[i]->forward(x);
                auto action = torch::argmax(actions).item().toInt();
                game.apply_action(i, action);
            }
#endif

            game.update();
            game.draw();

            if (game.check_end_game())
            {
                game.reset();
            }
        }
    }
    CloseWindow();
}
