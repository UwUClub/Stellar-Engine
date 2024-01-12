/*
** EPITECH PROJECT, 2023
** ECS
** File description:
** World
*/

#include "World.hpp"
#include <algorithm>
#include <spdlog/spdlog.h>

namespace Engine::Core {
    std::size_t World::createEntity()
    {
        std::size_t newIdx = 0;

        if (_ids.empty()) {
            newIdx = _nextId;
            _nextId++;
        } else {
            const auto smallestIdx = std::min_element(_ids.begin(), _ids.end());

            _ids.erase(smallestIdx);
            newIdx = *smallestIdx;
        }
        spdlog::debug("Creating entity {}", newIdx);
        for (const auto &component : _components) {
            auto initFunc = getInitFunc(component.first);

            initFunc(*this, newIdx);
        }
        return newIdx;
    }

    void World::killEntity(std::size_t aIndex)
    {
        spdlog::debug("Killing entity {}", aIndex);
        _ids.push_back(aIndex);

        for (const auto &component : _components) {
            auto eraseFunc = getEraseFunc(component.first);

            eraseFunc(*this, aIndex);
        }
    }

    void World::runSystems()
    {
        for (std::size_t idx = 0; idx < _nextId; idx++) {
            for (auto &system : _systems) {
                auto time = system.second.second.getElapsedTime();
                system.second.first(*this, time, idx);
            }
        }
    }

    void World::init()
    {
        for (auto &system : _systems) {
            system.second.second.restart();
        }
    }
} // namespace Engine::Core
