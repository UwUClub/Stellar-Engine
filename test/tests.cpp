#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include "ECS.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("App", "[App]")
{
    Engine::App app;
    auto world = std::make_unique<Engine::Core::World>();

    SECTION("Add a world")
    {
        app.addWorld(0, std::move(world));
        REQUIRE(app[0] != nullptr);
    }
    SECTION("Add a world with a key already used")
    {
        app.addWorld(0, std::move(world));
        REQUIRE_THROWS_AS(app.addWorld(0, std::move(world)), Engine::AppExceptionKeyAlreadyExists);
    }
    SECTION("Get a world with a key not used")
    {
        REQUIRE_THROWS_AS(app[0], Engine::AppExceptionKeyNotFound);
    }
    SECTION("Get a world with a key used")
    {
        app.addWorld(0, std::move(world));
        REQUIRE_NOTHROW(app[0]);
    }
    SECTION("Get a world with a key used and check if it's the same")
    {
        app.addWorld(0, std::move(world));
        REQUIRE(app[0] == app[0]);
    }
    SECTION("Get a world with a key used and check if it's not the same")
    {
        app.addWorld(0, std::move(world));
        app.addWorld(1, std::move(world));
        REQUIRE(app[0] != app[1]);
    }
    SECTION("Get a world with a key used and check if it's not the same after a move")
    {
        app.addWorld(0, std::move(world));
        auto tmp = std::move(app[0]);
        REQUIRE(app[0] != tmp);
    }
}

struct hp1 : public Engine::Component
{
    public:
        explicit hp1(int aHp)
            : hp(aHp)
        {}
        int hp;
};

struct hp2 : public Engine::Component
{
    public:
        explicit hp2(int aHp)
            : maxHp(aHp)
        {}
        int maxHp;
};

template<typename... Components>
class MySystemClass : public Engine::Core::System
{
    public:
        explicit MySystemClass(Engine::Core::World &world)
            : Engine::Core::System(world)
        {}

        MySystemClass(const MySystemClass &) = default;
        MySystemClass(MySystemClass &&) = default;

        MySystemClass &operator=(const MySystemClass &) = default;
        MySystemClass &operator=(MySystemClass &&) = default;

        void update() override
        {
            _world.get().template query<Components...>().forEach(
                _clock.getElapsedTime(), [this](Engine::Core::World & /*world*/, double /*deltaTime*/, std::size_t idx,
                                                Components &...components) {
                    this->updateSystem(_world.get(), _clock.getElapsedTime(), idx, components...);
                });
            _clock.restart();
        }

    private:
        void updateSystem(Engine::Core::World & /*world*/, double /*deltaTime*/, std::size_t /*idx*/, hp1 &hp1Comp,
                          hp2 &hp2Comp)
        {
            hp1Comp.hp--;
            hp2Comp.maxHp -= 2;
        }
};

TEST_CASE("World", "[World]")
{
    Engine::Core::World world;

    SECTION("Create an entity")
    {
        auto entity = world.createEntity();
        REQUIRE(entity == 0);
    }
    SECTION("Create an entity and check if it's not the same")
    {
        auto entity = world.createEntity();
        REQUIRE(entity != world.createEntity());
    }
    SECTION("Create an entity and kill it")
    {
        auto entity = world.createEntity();
        world.killEntity(entity);
        REQUIRE(entity == world.createEntity());
    }

    SECTION("Run a system")
    {
        constexpr int hps = 10;
        auto MySystem = Engine::Core::createSystem<hp1, hp2>(
            world, "MySystem",
            [](Engine::Core::World & /*world*/, double /*deltaTime*/, std::size_t /*idx*/, hp1 &cop1, hp2 &cop2) {
                cop1.hp--;
                cop2.maxHp -= 2;
            });

        world.registerComponents<hp1, hp2>();
        world.addSystem(MySystem);

        auto entity = world.createEntity();
        auto entity2 = world.createEntity();
        auto entity3 = world.createEntity();
        auto &hp1Comp = world.addComponentToEntity(entity, hp1 {hps});
        auto &hp2Comp = world.addComponentToEntity(entity, hp2 {hps});
        auto &hp1Comp2 = world.addComponentToEntity(entity3, hp1 {hps});
        auto &hp2Comp2 = world.addComponentToEntity(entity3, hp2 {hps});
        auto &hp2Comp3 = world.addComponentToEntity(entity2, hp2 {hps});

        world.runSystems();
        REQUIRE(hp1Comp.hp == hps - 1);
        REQUIRE(hp2Comp.maxHp == hps - 2);
        REQUIRE(hp1Comp2.hp == hps - 1);
        REQUIRE(hp2Comp2.maxHp == hps - 2);
        REQUIRE(hp2Comp3.maxHp == hps);
        auto secondSystem = std::make_pair<std::string, std::unique_ptr<Engine::Core::System>>(
            "MySystemClass", std::make_unique<MySystemClass<hp1, hp2>>(world));
        world.addSystem(secondSystem);
        world.runSystems();
        REQUIRE(hp1Comp.hp == hps - 3);
        REQUIRE(hp2Comp.maxHp == hps - 6);
        REQUIRE(hp1Comp2.hp == hps - 3);
        REQUIRE(hp2Comp2.maxHp == hps - 6);
        REQUIRE(hp2Comp3.maxHp == hps);
        world.removeSystem(MySystem.first);
        world.runSystems();
        REQUIRE(hp1Comp.hp == hps - 4);
        REQUIRE(hp2Comp.maxHp == hps - 8);
        REQUIRE(hp1Comp2.hp == hps - 4);
        REQUIRE(hp2Comp2.maxHp == hps - 8);
        REQUIRE(hp2Comp3.maxHp == hps);
    }
}

TEST_CASE("Query")
{
    Engine::Core::World world;
    constexpr int hps = 10;

    struct Player : public Engine::Component
    {};

    SECTION("getAllEntities")
    {
        world.registerComponents<hp1, hp2, Player>();

        world.createEntity();
        world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(1, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(1, hp2 {hps});
        world.addComponentToEntity(0, Player {});

        auto query = world.query<hp1, hp2, Player>().getAllEntities();
        REQUIRE(query.size() == 1);
        REQUIRE(query[0] == 0);
    }

    SECTION("getAllEntities 2")
    {
        world.registerComponents<hp1, hp2, Player>();

        world.createEntity();
        world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(1, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(1, hp2 {hps});
        world.addComponentToEntity(0, Player {});
        world.addComponentToEntity(1, Player {});

        auto query = world.query<hp1, hp2, Player>().getAllEntities();
        REQUIRE(query.size() == 2);
        REQUIRE(query[0] == 0);
        REQUIRE(query[1] == 1);
    }

    SECTION("getAllEntities 3")
    {
        world.registerComponents<hp1, hp2, Player>();

        world.createEntity();
        world.createEntity();
        world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(1, hp1 {hps});
        world.addComponentToEntity(2, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(1, hp2 {hps});
        world.addComponentToEntity(2, hp2 {hps});
        world.addComponentToEntity(0, Player {});
        world.addComponentToEntity(2, Player {});

        auto query = world.query<hp1, hp2, Player>().getAllEntities();
        REQUIRE(query.size() == 2);
        REQUIRE(query[0] == 0);
        REQUIRE(query[1] == 2);
    }

    SECTION("getComponentsOfEntity")
    {
        world.registerComponents<hp1, hp2, Player>();

        world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(0, Player {});

        auto query = world.query<hp1, hp2, Player>().getComponentsOfEntity(0);
        const hp1 &hp1Comp = std::get<0>(query);
        const hp2 &hp2Comp = std::get<1>(query);
        const Player &playerComp = std::get<2>(query);

        REQUIRE(hp1Comp.hp == hps);
        REQUIRE(hp2Comp.maxHp == hps);
        REQUIRE_NOTHROW(playerComp);
    }

    SECTION("getAll")
    {
        world.registerComponents<hp1, hp2, Player>();

        auto idx1 = world.createEntity();
        world.createEntity();
        auto idx2 = world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(1, hp1 {hps});
        world.addComponentToEntity(2, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(1, hp2 {hps});
        world.addComponentToEntity(2, hp2 {hps});
        world.addComponentToEntity(0, Player {});
        world.addComponentToEntity(2, Player {});

        auto query = world.query<hp1, hp2, Player>().getAll();
        REQUIRE(query.size() == 2);
        REQUIRE(std::get<0>(query[0]) == idx1);
        REQUIRE(std::get<0>(query[1]) == idx2);
    }
}
