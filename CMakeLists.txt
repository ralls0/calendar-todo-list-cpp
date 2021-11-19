cmake_minimum_required(VERSION 3.20)
project(QtNetWOAuth2)

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

add_executable(QtNetWOAuth2 main.cpp OAuth/OAuth.h OAuth/OAuth.cpp ClientCalDAV/ClientCalDAV.h ClientCalDAV/ClientCalDAV.cpp)
target_link_libraries(QtNetWOAuth2
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::NetworkAuth
        )
