cmake_minimum_required(VERSION 3.15...3.23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O")

add_subdirectory(Clock)

target_sources(${PROJECT_NAME}
    PRIVATE
    Main.cpp
    World.cpp
        )


target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> ${Boost_LIBRARIES})
