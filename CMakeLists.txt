cmake_minimum_required(VERSION 3.29)
project(Project_POO)

set(CMAKE_CXX_STANDARD 20)

include_directories(include) #procura os ficheiros .h auto

set(SOURCE_FILES
        src/main.cpp
        src/Simulator.cpp
        src/Buffer.cpp
        src/Map.cpp
        src/Caravan.cpp
        include/Buffer.h
        include/city.h
        src/city.cpp
        include/Wallet.h
        src/Wallet.cpp
)

add_executable(Project_POO ${SOURCE_FILES})


