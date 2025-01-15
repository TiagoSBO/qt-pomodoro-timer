cmake_minimum_required(VERSION 3.19)
project(pomodoro_timer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(pomodoro_timer
    WIN32 MACOSX_BUNDLE
    main.cpp        # Arquivo no diretório src/
    timer.cpp       # Arquivo no diretório src/
    timer.h             # Arquivo na raiz
    mainwindow.ui       # Arquivo na raiz
)

target_link_libraries(pomodoro_timer
    PRIVATE
        Qt::Core
        Qt::Widgets
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

