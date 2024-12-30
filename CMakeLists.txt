cmake_minimum_required(VERSION 3.29)
project(Project_POO)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCE_FILES
        src/main.cpp
        src/simulators/Simulator.cpp
        src/Buffer.cpp
        src/Map.cpp
        src/Caravan.cpp
        include/Buffer.h
        include/city.h
        src/city.cpp
        include/Wallet.h
        src/Wallet.cpp
        src/Commands.cpp
        include/Commands.h
        src/simulators/Simulator_MapActions.cpp
        src/simulators/Simulator_CaravanActions.cpp
        src/simulators/Simulator_Storage.cpp
        include/BarbarianCaravan.h
        include/Item.h
        src/Item.cpp
        src/Fight.cpp
        include/Fight.h
)

add_executable(Project_POO ${SOURCE_FILES})


