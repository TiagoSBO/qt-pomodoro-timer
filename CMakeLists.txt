cmake_minimum_required(VERSION 3.19)
project(PomodoroTimer VERSION 1.0.0 LANGUAGES CXX)

# Configuração básica do CMake
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Multimedia)

# Adiciona o executável
qt_add_executable(PomodoroTimer
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/timer.cpp
    src/settingsdialog.cpp
    src/sessionlogs.cpp
    src/soundmanager.cpp
    src/helpwindow.cpp
    src/systemtrayiconhandler.cpp
    src/includes/timer.h
    src/includes/settingsdialog.h
    src/includes/sessionlogs.h
    src/includes/soundmanager.h
    src/includes/helpwindow.h
    src/includes/systemtrayiconhandler.h
    src/ui/mainwindow.ui
    src/ui/settingsdialog.ui
    src/ui/helpwindow.ui
    resources.qrc
    app_icon.rc
)

# O nome real do arquivo gerado (sem underline ou tudo minúsculo)
set_target_properties(PomodoroTimer PROPERTIES
    OUTPUT_NAME "Pomodoro Timer"
)

# Diretórios de inclusão
target_include_directories(PomodoroTimer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
)

# Bibliotecas necessárias
target_link_libraries(PomodoroTimer
    PRIVATE Qt::Core Qt::Widgets Qt6::Multimedia
)

# Instalação
include(GNUInstallDirs)

install(TARGETS PomodoroTimer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Script de deploy (para copiar dependências do Qt)
qt_generate_deploy_app_script(
    TARGET PomodoroTimer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
