#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_
#include <functional>
#include "Core/Clock.hpp"
namespace Engine::Core {
    class World;

    class System
    {
        public:
            bool _isActivated = true;

        protected:
            std::reference_wrapper<Core::World> _world;
            Clock _clock;

        public:
            explicit System(Core::World &aWorld)
                : _world(aWorld)
            {}
            virtual ~System() = default;
            virtual void update() = 0;

            System &operator=(const System &) = default;
            System &operator=(System &&) = default;

            System(const System &) = default;
            System(System &&) = default;
    };
} // namespace Engine::Core

#endif /* !SYSTEM_HPP_ */
