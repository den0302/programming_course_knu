cmake_minimum_required(VERSION 3.30)
project(Lab1)

set(CMAKE_CXX_STANDARD 20)


add_executable(Lab1 Lab1.cpp
        Employee.h
        Graph.h
        Employee.cpp
        Graph.cpp
        sql.h
        ZooGraph.h
        ZooGraph.cpp
        Creatures.h
        Creatures.cpp)

target_link_libraries(Lab1 PRIVATE Rpcrt4)
