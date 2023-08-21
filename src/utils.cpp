#include "utils.h"
#include <chrono>
#include <iostream>
#include <boost/program_options.hpp>

FrameLimiter::FrameLimiter(unsigned int fps) : fps_(fps) {}

bool FrameLimiter::should_update()
{
    auto now = std::chrono::steady_clock::now();
    auto dt = now - prev_update_;

    if (dt > std::chrono::duration<float>(1.f / fps_))
    {
        prev_update_ = now;
        return true;
    }

    return false;
}

float FrameLimiter::frame_dt() const
{
    // return hard coded frame dt
    // less accurate but must be done for simulations to be repeatable with same seed
    return 1.f / fps_;
}

namespace po = boost::program_options;

Flock::parameters handle_arguments(int argc, char *argv[])
{
    auto range = [](float min, float max, char const *const opt_name)
    {
        return [opt_name, min, max](float v)
        {
            if (v < min || v > max)
            {
                throw po::validation_error(
                    po::validation_error::invalid_option_value, opt_name, std::to_string(v));
            }
        };
    };

    Flock::parameters params;

    po::options_description desc("Allowed options");
    try
    {
        desc.add_options()("help,h", "help screen")("cohesion", po::value<float>(&params.cohesion_factor)->default_value(0.5f)->notifier(range(0.f, 1.f, "cohesion")), "set cohesion factor | range [0.0, 1.0]")("alignment", po::value<float>(&params.alignment_factor)->default_value(0.5f)->notifier(range(0.f, 1.f, "alignment")), "set alignment factor | range [0.0, 1.0]")("separation", po::value<float>(&params.separation_factor)->default_value(0.5f)->notifier(range(0.f, 1.f, "separation")), "set separation factor | range [0.0, 1.0]")("n", po::value<int>(&params.n)->default_value(50)->notifier(range(1, INFINITY, "n")), "number of boids")("seed", po::value<unsigned int>(), "seed for random number generator used")("sight-distance", po::value<float>(&params.sight_dist)->default_value(50)->notifier(range(0.f, INFINITY, "sight-distance")), "boid sight distance (pixels) | range [0.0, inf)")("sight-angle", po::value<float>(&params.sight_angle)->default_value(90.f)->notifier(range(0.f, 360.f, "sight-angle")), "boid field of view (degrees) | range [0.0, 360.0]")("separation-distance", po::value<float>(&params.separation_dist)->default_value(25.f)->notifier(range(0.f, INFINITY, "separation-distance")), "boid separation distance | range [0.0, inf)")("wrap", po::bool_switch(&params.wrap)->default_value(false), "wrap boids if outside screen");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << '\n';
            exit(0);
        }
        if (vm.count("seed"))
        {
            params.seed = vm["seed"].as<unsigned int>();
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    return params;
}