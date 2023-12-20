cmake_minimum_required(VERSION 3.15...3.23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O")

target_sources(${PROJECT_NAME}
    PRIVATE
    SparseArray.hpp
    Logger.hpp
    Main.cpp
        )

include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
