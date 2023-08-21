#ifndef utils_h
#define utils_h

#include "flock.h"
#include <chrono>

/// @brief class to limit frames
class FrameLimiter
{
public:
    /// @brief FrameLimiter constructor
    /// @param fps frames per second limit
    FrameLimiter(unsigned int fps);

    /// @brief check if the current simulation should be updated
    /// @return true if the simulation should be updated
    bool should_update();

    /// @brief get the time difference from the last updated frame
    /// @return the time difference
    float frame_dt() const;

private:
    unsigned int fps_;
    std::chrono::steady_clock::time_point prev_update_ = std::chrono::steady_clock::now();
};

/// @brief handle command line arguments
/// @param argc
/// @param argv
/// @return a parameters struct set with the parameters for the simulation
Flock::parameters handle_arguments(int argc, char *argv[]);

#endif