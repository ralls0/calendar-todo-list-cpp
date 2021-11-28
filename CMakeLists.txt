cmake_minimum_required(VERSION 3.20)
project(calendar_todo_list_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        NetworkAuth
        REQUIRED)

add_executable(calendar_todo_list_cpp
        main.cpp
        src/DateUtils.h
        src/DateUtils.cpp
        )
target_link_libraries(calendar_todo_list_cpp
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::NetworkAuth
        )
