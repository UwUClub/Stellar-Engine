#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include <chrono>
namespace Engine {
    class Clock
    {
        public:
            Clock() = default;
            ~Clock() = default;

            Clock(const Clock &other) = default;
            Clock &operator=(const Clock &other) = default;

            Clock(Clock &&other) noexcept = default;
            Clock &operator=(Clock &&other) noexcept = default;

            /**
             * @brief Get the time elapsed since the last call to restart
             *
             * @return the time elapsed since the last call to restart
             */
            [[nodiscard]] double getElapsedTime() const;

            /**
             * @brief Restart the clock
             *
             * @return the time elapsed since the last call to restart
             */
            double restart();

        protected:
            std::chrono::time_point<std::chrono::high_resolution_clock> _lastTime {
                std::chrono::high_resolution_clock::now()};
            std::chrono::duration<double> _delta {std::chrono::duration<double>(0)};
    };
} // namespace Engine

#endif /* !CLOCK_HPP_ */
