#include "App.hpp"
#include "Clock.hpp"
#include "World.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("App", "[App]")
{
    Engine::App<> app;
    Engine::Clock clock;
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
