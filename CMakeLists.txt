cmake_minimum_required(VERSION 3.19)
project(pomodoro_timer LANGUAGES CXX)

# Configuração básica do CMake
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Multimedia)

qt_add_executable(pomodoro_timer
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/timer.cpp
    src/settingsdialog.cpp
    src/sessionlogs.cpp
    src/soundmanager.cpp
    includes/timer.h
    includes/settingsdialog.h
    includes/sessionlogs.h
    includes/soundmanager.h
    ui/mainwindow.ui
    ui/settingsdialog.ui
    resources.qrc
    styles/styles.qss

)

# Diretórios de inclusão
target_include_directories(pomodoro_timer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes  # Inclui a pasta includes/
        ${Qt6Core_INCLUDE_DIRS}  # Diretórios do Qt Core
        ${Qt6Sql_INCLUDE_DIRS}   #
)

target_link_libraries(pomodoro_timer
    PRIVATE Qt::Core Qt::Widgets Qt6::Multimedia
)

include(GNUInstallDirs)

install(TARGETS pomodoro_timer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET pomodoro_timer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

