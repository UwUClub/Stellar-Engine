include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets

find_package(Catch2 QUIET)
find_package(spdlog QUIET)
find_package(Boost QUIET)

message(STATUS "Catch2_FOUND: ${Catch2_FOUND}")
message(STATUS "spdlog_FOUND: ${spdlog_FOUND}")
message(STATUS "Boost_FOUND: ${Boost_FOUND}")

function(patatocs_setup_dependencies)

    # For each dependency, see if it's
    # already been provided to us by a parent project

    if(NOT TARGET Catch2::Catch2WithMain AND NOT Catch2_FOUND)
        cpmaddpackage("gh:catchorg/Catch2@3.3.2")
    endif()

    if(NOT TARGET spdlog::spdlog AND NOT spdlog_FOUND)
        cpmaddpackage("gh:gabime/spdlog@1.12.0")
    endif()

    if(NOT TARGET Boost::boost AND NOT Boost_FOUND)
        cpmaddpackage("gh:boostorg/boost@1.78.0")
    endif()

endfunction()
