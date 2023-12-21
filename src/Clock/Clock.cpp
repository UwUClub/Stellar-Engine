#include "Clock.hpp"

namespace Engine {
    double Clock::getElapsedTime() const
    {
        return _delta.count() * 1000;
    }

    double Clock::restart()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        _delta = currentTime - _lastTime;
        _lastTime = currentTime;
        return getElapsedTime();
    }
} // namespace Engine
