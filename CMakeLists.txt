cmake_minimum_required(VERSION 3.20)
project(calendar_todo_list_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        # Core5Compat
        Gui
        Widgets
        Network
        Xml
        StateMachine
        REQUIRED)

add_executable(calendar_todo_list_cpp
        main.cpp
        src/CalDAV/CalendarClient.h
        src/CalDAV/CalendarClient.cpp
        src/DateUtils.h
        src/DateUtils.cpp
        src/CalDAV/CalendarEvent.h
        src/CalDAV/CalendarEvent.cpp
        src/CalDAV/CalendarClient_CalDAV.h src/CalDAV/CalendarClient_CalDAV.cpp src/CalDAV/CalendarManager.h src/CalDAV/CalendarManager.cpp src/SimpleCrypt.h src/SimpleCrypt.cpp)

target_link_libraries(calendar_todo_list_cpp
        Qt::Core
        # Qt::Core5Compat
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::Xml
        Qt::StateMachine
        )