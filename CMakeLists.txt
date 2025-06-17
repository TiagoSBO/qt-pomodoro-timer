cmake_minimum_required(VERSION 3.19)
project(PomodoroTimer VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Multimedia UiTools)
find_package(Python3 REQUIRED)
find_dependency(Qt6 REQUIRED COMPONENTS Core Network REQUIRED)

if (MINGW)
    enable_language(RC)
endif()

qt_add_executable(PomodoroTimer
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/timer.cpp
    src/settingsdialog.cpp
    src/sessionlogs.cpp
    src/soundmanager.cpp
    src/helpwindow.cpp
    src/systemtrayiconhandler.cpp
    src/singleApplication/singleapplication.cpp
    src/singleApplication/singleapplication_p.cpp
    src/updater/updater.cpp
    src/floatingtimerwindow.cpp
    src/qHotKey/qhotkey.cpp
    src/qHotKey/qhotkey_win.cpp
    src/statswindow.cpp
    src/daterangedialog.cpp
    src/db/dbstatsmanager.cpp
    src/models/focussession.cpp

    src/includes/timer.h
    src/includes/settingsdialog.h
    src/includes/sessionlogs.h
    src/includes/soundmanager.h
    src/includes/helpwindow.h
    src/includes/systemtrayiconhandler.h
    src/includes/singleApplication/singleapplication.h
    src/includes/singleApplication/singleapplication_p.h
    src/includes/updater/updater.h
    src/includes/app_version.h
    src/includes/floatingtimerwindow.h
    src/includes/qHotKey/qhotkey.h
    src/includes/qHotKey/qhotkey_p.h
    src/includes/statswindow.h
    src/includes/daterangedialog.h
    src/includes/db/dbstatsmanager.h
    src/includes/models/focussession.h

    src/ui/mainwindow.ui
    src/ui/settingsdialog.ui
    src/ui/helpwindow.ui
    src/ui/floatingtimerwindow.ui
    src/ui/statswindow.ui
    src/ui/daterangedialog.ui

    resources.qrc
    app_icon.rc
    build_deploy.py
)

set_target_properties(PomodoroTimer PROPERTIES
    OUTPUT_NAME "Pomodoro Timer"
)

target_include_directories(PomodoroTimer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
        ${CMAKE_CURRENT_SOURCE_DIR}/src/includes/db
        ${CMAKE_CURRENT_SOURCE_DIR}/src/includes/singleApplication
        ${CMAKE_CURRENT_SOURCE_DIR}/src/includes/qHotKey
        ${CMAKE_CURRENT_SOURCE_DIR}/src/qHotKey
        ${CMAKE_CURRENT_SOURCE_DIR}/src/models
)


target_link_libraries(PomodoroTimer
    PRIVATE Qt::Core Qt::Widgets Qt6::Multimedia Qt6::UiTools Qt6::Sql
)

include(GNUInstallDirs)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes/singleApplication
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes/qHotKey
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes/models

)

install(TARGETS PomodoroTimer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET PomodoroTimer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
