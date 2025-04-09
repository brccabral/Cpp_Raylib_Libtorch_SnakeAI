#include <ctime>
#include <random>
#include <cstring>
#include <raylib.h>
#include "RaceTopDown.h"


#if !MANUAL
#include <mlgames/GenPopulation.h>
#include <mlgames/LinearGen.h>
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
#endif

    constexpr int hidden_layer_1 = 8;

    constexpr int SCREEN_WIDTH = 1366;
    constexpr int SCREEN_HEIGHT = 768;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Race TopDown");

#if MANUAL
    constexpr int num_cars = 1;
#else
    constexpr int num_cars = 1000;
#endif

    {
        auto game = RaceTopDown(num_cars);

        float record = 0;

#if !MANUAL
        size_t generation = 0;

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
#endif

        while (!WindowShouldClose())
        {
#if MANUAL
            int action = 0;
            if (IsKeyDown(KEY_LEFT))
            {
                action = action | 0b0001;
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                action = action | 0b0010;
            }
            if (IsKeyDown(KEY_UP))
            {
                action = action | 0b0100;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                action = action | 0b1000;
            }
            if (IsKeyPressed(KEY_W))
            {
                action = action | 0b0100;
            }
            if (IsKeyPressed(KEY_S))
            {
                action = action | 0b1000;
            }
            if (IsKeyPressed(KEY_A))
            {
                action = action | 0b0001;
            }
            if (IsKeyPressed(KEY_D))
            {
                action = action | 0b0010;
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
                if (game.max_distance > record)
                {
                    record = game.max_distance;
                }
#if !MANUAL
                printf("Generation %zu Distance: %d Dead: %zu Record %.0f\n", generation,
                       game.max_distance, game.num_dead, record);
                ++generation;
                save_model<>(population.members[game.best_car_index]);
                population.apply_mutations(game.select_best_cars());
#endif
                game.reset();
            }
        }
    }
    CloseWindow();
}
