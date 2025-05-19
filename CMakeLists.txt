cmake_minimum_required(VERSION 3.10)
project(CrosswordSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(CrosswordSolver
    main.cpp
    Number.cpp
    workingKeys.cpp
    Cell.cpp
    GridManager.cpp
)

target_link_libraries(CrosswordSolver
    sfml-graphics
    sfml-window
    sfml-system
)
