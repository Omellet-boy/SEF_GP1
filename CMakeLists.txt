
cmake_minimum_required(VERSION 3.16)
project(SEMS)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Charts)

add_executable(SEMS
    main.cpp
    mainwindow.cpp
    mainwindow.h
    simulator.cpp
    simulator.h
    database.cpp
    database.h
    loginwindow.h
    loginwindow.cpp
    settingwindow.h
    settingwindow.cpp
    battery_monitoring_page.h
    battery_monitoring_page.cpp
    email_alert.py
    solar_monitoring_page.h
    solar_monitoring_page.cpp
)

target_link_libraries(SEMS PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql Qt6::Charts)
set(CMAKE_AUTOMOC ON)
