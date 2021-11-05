cmake_minimum_required(VERSION 3.20)
project(provaClientDav)

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

add_executable(provaClientDav main.cpp CalendarClient/CalendarClient.h CalendarClient/CalendarClient.cpp CalendarClient/DateUtils.h CalendarClient/DateUtils.cpp CalendarClient/CalendarEvent.h CalendarClient/CalendarEvent.cpp CalendarClient/CalendarClient_CalDAV.h CalendarClient/CalendarClient_CalDAV.cpp CalendarClient/CalendarClient_CalDAV_ReceiveResponse.cpp CalendarClient/CalendarClient_CalDAV_SendRequest.cpp CalendarClient/CalendarClient_ICS.h CalendarClient/CalendarClient_ICS.cpp CalendarClient/CalendarClient_CalDAV_StateMachine.cpp CalendarClient/CalendarClient_CalDAV_Utils.cpp CalendarClient/CalendarClient_ICS_ReceiveResonse.cpp CalendarClient/CalendarClient_ICS_SendRequest.cpp CalendarClient/CalendarClient_ICS_StateMachine.cpp CalendarClient/CalendarManager.cpp CalendarClient/CalendarManager.h CalendarClient/SimpleCrypt.h CalendarClient/SimpleCrypt.cpp)
target_link_libraries(provaClientDav
        Qt::Core
        # Qt::Core5Compat
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::Xml
        Qt::StateMachine
        )