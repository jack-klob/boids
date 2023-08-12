#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "flock.h"
#include "gl_math.h"
#include <chrono>
#include <iostream>
#include <boost/program_options.hpp>

class FrameLimiter
{
public:
    FrameLimiter(unsigned int fps) : fps_(fps) {}

    bool should_update()
    {
        auto now = std::chrono::steady_clock::now();
        auto dt = now - prev_update_;

        if(dt > std::chrono::duration<float>(1.f / fps_))
        {
            prev_update_ = now;
            return true;
        }

        return false;
    }

    float frame_dt() const
    {
        // return hard coded frame dt
        // less accurate but must be done for simulations to be repeatable with same seed
        return 1.f / fps_;
    }

private:
    unsigned int fps_;
    std::chrono::steady_clock::time_point prev_update_ = std::chrono::steady_clock::now();
};

namespace po = boost::program_options;

/// @brief handle command line arguments
/// @param argc 
/// @param argv 
/// @return a parameters struct set with the parameters for the simulation
parameters handle_arguments(int argc, char* argv[])
{
    auto range = [](float min, float max, char const *const opt_name)
    {
        return [opt_name, min, max](float v)
        {
            if(v < min || v > max)
            {
                throw po::validation_error(
                    po::validation_error::invalid_option_value, opt_name, std::to_string(v));
            }
        };
    };

    parameters params;

    po::options_description desc("Allowed options");
    try
    {
        desc.add_options()
            ("help,h", "help screen")
            ("cohesion", po::value<float>(&params.cohesion_factor)->default_value(0.5f)->notifier(range(0.f, 1.f, "cohesion")), "set cohesion factor | range [0.0, 1.0]")
            ("alignment", po::value<float>(&params.alignment_factor)->default_value(0.5f)->notifier(range(0.f, 1.f, "alignment")), "set alignment factor | range [0.0, 1.0]" )
            ("separation", po::value<float>(&params.separation_factor)->default_value(0.5f)->notifier(range(0.f, 1.f, "separation")), "set separation factor | range [0.0, 1.0]")
            ("n", po::value<int>(&params.n)->default_value(50)->notifier(range(1, INFINITY, "n")), "number of boids")
            ("seed", po::value<unsigned int>(), "seed for random number generator used")
            ("sight-distance", po::value<float>(&params.sight_dist)->default_value(50)->notifier(range(0.f, INFINITY, "sight-distance")), "boid sight distance (pixels) | range [0.0, inf)")
            ("sight-angle", po::value<float>(&params.sight_angle)->default_value(90.f)->notifier(range(0.f, 360.f, "sight-angle")), "boid field of view (degrees) | range [0.0, 360.0]")
            ("wrap", po::bool_switch(&params.wrap)->default_value(false), "wrap boids if outside screen");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << '\n';
            exit(0);
        }if(vm.count("seed"))
        {
            params.seed = vm["seed"].as<unsigned int>();
        }
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    return params;
}

int main(int argc, char* argv[])
{
    parameters params = handle_arguments(argc, argv);

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(params.width, params.height, "Flocking Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    // create and use shaders
    GLuint shader = loadShaders("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    glUseProgram(shader);

    // project to pixel space
    auto proj = make_ortho(0.f, params.width, 0.f, params.height);
    GLuint proj_loc = glGetUniformLocation(shader, "u_proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj[0][0]);

    FrameLimiter limiter(60);
    Flock flock(params);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if(limiter.should_update())
        {
            flock.update(limiter.frame_dt());
        }
        flock.draw();

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents();        // poll and process events
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}