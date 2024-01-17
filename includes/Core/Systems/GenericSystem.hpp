#ifndef GENERICSYSTEM_HPP_
#define GENERICSYSTEM_HPP_

#include <functional>
#include <utility>
#include "Core/Components/Component.hpp"
#include "Core/World.hpp"
#include "System.hpp"

namespace Engine::Core {
    template<typename Func, ComponentConcept... Components>
    class GenericSystem : public System
    {
        private:
            Func _updateFunc;

        public:
            GenericSystem(Core::World &world, Func updateFunc)
                : System(world),
                  _updateFunc(updateFunc)
            {}

            void update() override
            {
                double deltaTime = _clock.getElapsedTime();

                _world.get().template query<Components...>().forEach(deltaTime, _updateFunc);
            }
    };

    template<ComponentConcept... Components, typename Func>
    std::pair<std::string, std::unique_ptr<System>> createSystem(World &aWorld, const std::string &aName,
                                                                 Func aUpdateFunc)
    {
        return std::pair<std::string, std::unique_ptr<System>>(
            std::make_pair(aName, std::make_unique<GenericSystem<Func, Components...>>(aWorld, aUpdateFunc)));
    }
} // namespace Engine::Core

#endif /* !GENERICSYSTEM_HPP_ */
