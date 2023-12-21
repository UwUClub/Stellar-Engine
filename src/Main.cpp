#include <cstddef>
#include <iostream>
#include "SparseArray.hpp"
#include "World.hpp"

struct Test
{
        int a;
        int b;
};

struct Test2
{
        int a;
        int b;
};

void TestFunc(Engine::World &aWorld, double deltaTime, std::size_t aIndex, Test &aTest, Test2 &aTest2)
{
    std::cout << "TestFunc called with index " << aIndex << std::endl;
    std::cout << "aTest.a = " << aTest.a << std::endl;
    std::cout << "aTest.b = " << aTest.b << std::endl;
    std::cout << "aTest2.a = " << aTest2.a << std::endl;
    std::cout << "aTest2.b = " << aTest2.b << std::endl;
    aTest2.a = 42;
    aTest.a++;
}

void TestFunc2(Engine::World &aWorld, double deltaTime, std::size_t aIndex)
{
    std::cout << "TestFunc2 called with index " << aIndex << std::endl;
    std::cout << "deltaTime = " << deltaTime << std::endl;
}

int main()
{
    Engine::World world;
    auto &test = world.registerComponent<Test>();
    auto &test2 = world.registerComponent<Test2>();
    world.addSystem<Test, Test2>(TestFunc);
    world.addSystem(TestFunc2);

    world.createEntity();
    world.createEntity();
    world.createEntity();
    world.createEntity();

    test.emplace(0, Test {1, 2});
    test.emplace(2, Test {3, 4});
    test2.emplace(2, Test2 {5, 6});
    return 0;
}
