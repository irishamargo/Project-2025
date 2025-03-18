cmake_minimum_required(VERSION 3.10)
project(SFML_Test_Project)

find_package(SFML REQUIRED COMPONENTS graphics window system)

add_executable(sfml_test main.cpp)

target_link_libraries(sfml_test
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(sfml_test PRIVATE ${X11_LIBRARIES})
endif()
