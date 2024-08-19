cmake_minimum_required(VERSION 3.16)

project(QtSensorAuth VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Sensors)

qt_standard_project_setup()

qt_add_executable(appQtSensorAuth
    main.cpp
)

qt_add_qml_module(appQtSensorAuth
    URI QtSensorAuth
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES DataReadingHandler.h DataReadingHandler.cpp
        QML_FILES ProgressXYBar.qml
        SOURCES
        SOURCES Pattern.h Pattern.cpp
        SOURCES logger.h logger.cpp
        RESOURCES
        RESOURCES
        QML_FILES ProgressZBar.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQtSensorAuth PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQtSensorAuth
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQtSensorAuth
    PRIVATE Qt6::Quick Qt6::Sensors
)

include(GNUInstallDirs)
install(TARGETS appQtSensorAuth
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
