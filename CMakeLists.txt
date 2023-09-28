cmake_minimum_required(VERSION 3.24)
project(SpaceInvaders)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Animation.hpp DrawText.hpp Enemy.hpp EnemyManager.hpp Global.hpp Ufo.hpp Player.hpp Animation.cpp DrawText.cpp Enemy.cpp EnemyManager.cpp Player.cpp Ufo.cpp)
add_executable(HelloSFML ${SOURCE_FILES})
include_directories(/usr/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(HelloSFML sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
