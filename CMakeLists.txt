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
        src/Utils/ColorUtils.h 
        src/Utils/ColorUtils.cpp
        src/ClientCalDAV/ClientCalDAV.h
        src/ClientCalDAV/ClientCalDAV.cpp
        src/ClientCalDAV/DeleteEvent.cpp
        src/ClientCalDAV/RetrieveChangedEvent.cpp
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
        src/ClientCalDAV/RetrieveCTag.cpp
        src/ClientCalDAV/Parsing.cpp
        src/ClientCalDAV/SaveTask.cpp
        src/GuiComponents/Calendar/Calendar.cpp
        src/GuiComponents/Calendar/Calendar.h
        src/GuiComponents/Calendar/CustomDialog.cpp
        src/GuiComponents/Calendar/CustomDialog.h
        src/GuiComponents/Calendar/QFrameExtended.cpp
        src/GuiComponents/Calendar/QFrameExtended.h
        src/GuiComponents/Calendar/QLabelEvent.cpp
        src/GuiComponents/Calendar/QLabelEvent.h
        src/GuiComponents/Calendar/QPushButtonExtended.cpp
        src/GuiComponents/Calendar/QPushButtonExtended.h
        src/GuiComponents/Calendar/QWidgetExtended.cpp
        src/GuiComponents/Calendar/QWidgetExtended.h
        src/GuiComponents/Calendar/Date.cpp
        src/GuiComponents/Calendar/Date.h
        src/GuiComponents/Calendar/Category.cpp
        src/GuiComponents/Calendar/Category.h
        src/GuiComponents/Calendar/DateUtil.cpp
        src/GuiComponents/Calendar/DateUtil.h
        src/GuiComponents/MainWindow.cpp
        src/GuiComponents/MainWindow.h
        src/ClientCalDAV/GetEvents.cpp
        src/ClientCalDAV/Handles.cpp
        src/ClientCalDAV/MachineState.cpp src/GuiComponents/ToDoList/ToDoList.cpp src/GuiComponents/ToDoList/ToDoList.h src/Task/TaskElement.cpp src/Task/TaskElement.h src/Task/TasksManager.cpp src/Task/TasksManager.h src/ClientCalDAV/ShareCalendar.cpp src/GuiComponents/ShareCalendar/ShareCalendarDialog.cpp src/GuiComponents/ShareCalendar/ShareCalendarDialog.h src/GuiComponents/ToDoList/QPushButtonExtendedTD.cpp src/GuiComponents/ToDoList/QPushButtonExtendedTD.h src/GuiComponents/ToDoList/QCheckBoxExtended.cpp src/GuiComponents/ToDoList/QCheckBoxExtended.h)
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
