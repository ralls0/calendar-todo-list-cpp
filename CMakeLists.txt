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
        src/Utils/DateUtils.h
        src/Utils/DateUtils.cpp
        src/Utils/SimpleCrypt.h
        src/Utils/SimpleCrypt.cpp
        src/ClientCalDAV/CalendarClient.h
        src/ClientCalDAV/CalendarClient.cpp
        src/ClientCalDAV/CalendarClient_CalDAV.h
        src/ClientCalDAV/CalendarClient_CalDAV.cpp
        src/ClientCalDAV/DeleteEvent.cpp
        src/ClientCalDAV/GetChangedEvent.cpp
        src/ClientCalDAV/SaveEvent.cpp
        src/CalendarManager/CalendarManager.h
        src/CalendarManager/CalendarManager.cpp
        src/CalendarEvent/CalendarEvent.h
        src/CalendarEvent/CalendarEvent.cpp
        src/OAuth2/OAuth.h
        src/OAuth2/OAuth.cpp
        src/GuiComponents/NewCalendar/NewCalendarDialog.h
        src/GuiComponents/NewCalendar/NewCalendarDialog.cpp
        src/GuiComponents/NewEvent/NewEventDialog.cpp
        src/GuiComponents/NewEvent/NewEventDialog.h
        src/ClientCalDAV/GetCTag.cpp src/ClientCalDAV/Parsing.cpp src/ClientCalDAV/GetChangedActivity.cpp src/ClientCalDAV/SaveActivity.cpp src/GuiComponents/Calendar/Calendar.cpp src/GuiComponents/Calendar/Calendar.h src/GuiComponents/Calendar/ViewComponents/CustomDialog.cpp src/GuiComponents/Calendar/ViewComponents/CustomDialog.h src/GuiComponents/Calendar/ViewComponents/QFrameExtended.cpp src/GuiComponents/Calendar/ViewComponents/QFrameExtended.h src/GuiComponents/Calendar/ViewComponents/QLabelEvent.cpp src/GuiComponents/Calendar/ViewComponents/QLabelEvent.h src/GuiComponents/Calendar/ViewComponents/QPushButtonExtended.cpp src/GuiComponents/Calendar/ViewComponents/QPushButtonExtended.h src/GuiComponents/Calendar/ViewComponents/QWidgetExtended.cpp src/GuiComponents/Calendar/ViewComponents/QWidgetExtended.h src/GuiComponents/Calendar/ViewComponents/API/APImain.cpp src/GuiComponents/Calendar/ViewComponents/API/APImain.h src/GuiComponents/Calendar/ViewComponents/Model/Date.cpp src/GuiComponents/Calendar/ViewComponents/Model/Date.h src/GuiComponents/Calendar/ViewComponents/Model/Event.cpp src/GuiComponents/Calendar/ViewComponents/Model/Event.h src/GuiComponents/Calendar/ViewComponents/Model/Category.cpp src/GuiComponents/Calendar/ViewComponents/Model/Category.h src/GuiComponents/Calendar/ViewComponents/Utils/DateUtils.cpp src/GuiComponents/Calendar/ViewComponents/Utils/DateUtils.h src/Utils/ColorUtils.h src/Utils/ColorUtils.cpp)
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
