#include <cstddef>
#include "Core/SparseArray.hpp"
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

struct hp1
{
        int hp;
};

struct hp2
{
        int hp;
};

void takeDamage(Engine::Core::World & /*world*/, double /*delta*/, std::size_t /*idx*/, hp1 &hp1, hp2 &hp2)
{
    hp1.hp -= 1;
    hp2.hp -= 2;
}

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

        world.registerComponent<hp1>();
        world.registerComponent<hp2>();
        world.addSystem<hp1, hp2>(takeDamage);

        auto entity = world.createEntity();
        auto &hp1Comp = world.addComponentToEntity(entity, hp1 {hps});
        auto &hp2Comp = world.addComponentToEntity(entity, hp2 {hps});

        world.runSystems();
        REQUIRE(hp1Comp.hp == hps - 1);
        REQUIRE(hp2Comp.hp == hps - 2);
    }
}
