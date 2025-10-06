cmake_minimum_required(VERSION 3.30)
project(Lab1)

set(CMAKE_CXX_STANDARD 20)


add_executable(Lab1 Lab1.cpp
        Creatures/Employee.h
        Creatures/Animals.h
        Creatures/Animals.cpp
        Creatures/Employee.cpp
        Graphs/Graph.h
        Graphs/Graph.cpp
        Graphs/ZooGraph.h
        Graphs/ZooGraph.cpp
        Logger.h
        sql.h
        Accounts/Account.h
        Accounts/Account.cpp
        Accounts/AuthManager.h
        Accounts/AuthManager.cpp
        Menus/AdminMenu.h
        Menus/AdminMenu.cpp
        Menus/ManagerMenu.h
        Menus/ManagerMenu.cpp
        Menus/Menu.h
        Menus/Menu.cpp
        Creatures/AnimalManager.h
        Creatures/AnimalManager.cpp
        Creatures/EmployeeManager.h
        Creatures/EmployeeManager.cpp)

target_link_libraries(Lab1 PRIVATE Rpcrt4)
