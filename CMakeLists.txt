cmake_minimum_required(VERSION 3.20)
project(calendar_todo_list_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Core5Compat
        Gui
        Widgets
        Network
        NetworkAuth
        Xml
        StateMachine
        REQUIRED)

add_executable(calendar_todo_list_cpp
        main.cpp
        src/DateUtils.h
        src/DateUtils.cpp
        src/SimpleCrypt.h
        src/SimpleCrypt.cpp
        src/ClientCalDAV/CalendarClient.h
        src/ClientCalDAV/CalendarClient.cpp
        src/ClientCalDAV/CalendarClient_CalDAV.h
        src/ClientCalDAV/CalendarClient_CalDAV.cpp
        src/ClientCalDAV/DeleteEvent.cpp
        src/ClientCalDAV/GetChangedEvent.cpp
        src/ClientCalDAV/SaveEvent.cpp
        src/ClientCalDAV/SyncToken.cpp
        src/CalendarManager/CalendarManager.h
        src/CalendarManager/CalendarManager.cpp
        src/CalendarEvent/CalendarEvent.h
        src/CalendarEvent/CalendarEvent.cpp
        src/OAuth2/OAuth.h
        src/OAuth2/OAuth.cpp
        src/GuiComponents/NewCalendar/WindowNewCalendar.h
        src/GuiComponents/NewCalendar/WindowNewCalendar.cpp
        src/GuiComponents/NewCalendar/NewCalendarDialog.h
        src/GuiComponents/NewCalendar/NewCalendarDialog.cpp
        src/GuiComponents/NewEvent/NewEventDialog.cpp
        src/GuiComponents/NewEvent/NewEventDialog.h
        src/ClientCalDAV/getCTag.cpp)
target_link_libraries(calendar_todo_list_cpp
        Qt::Core
        Qt::Core5Compat
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::NetworkAuth
        Qt::Xml
        Qt::StateMachine
        )
