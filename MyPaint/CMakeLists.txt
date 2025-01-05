cmake_minimum_required(VERSION 3.16)

project(MyPaint VERSION 0.1 LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
    SourceFiles/main.cpp
    SourceFiles/mainwindow.cpp
    SourceFiles/mainwindow.h
    SourceFiles/shapes/IFigure.h
    SourceFiles/shapes/rectangle.h
    SourceFiles/shapes/rectangle.cpp
    SourceFiles/shapes/ellipse.h
    SourceFiles/shapes/ellipse.cpp
    SourceFiles/shapes/triangle.h
    SourceFiles/shapes/triangle.cpp
    SourceFiles/shapes/figure_factory.h
    SourceFiles/shapes/figure_factory.cpp
    SourceFiles/utils/file_manager.h
    SourceFiles/utils/file_manager.cpp
    SourceFiles/TODO.txt
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MyPaint
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        SourceFiles/stdafx.h
    )
else()
    add_executable(MyPaint
        ${PROJECT_SOURCES}
    )
endif()


target_include_directories(MyPaint PRIVATE
    ${CMAKE_SOURCE_DIR}/SourceFiles
    ${CMAKE_SOURCE_DIR}/SourceFiles/shapes
    ${CMAKE_SOURCE_DIR}/SourceFiles/utils
)

target_precompile_headers(MyPaint PRIVATE SourceFiles/stdafx.h)

target_link_libraries(MyPaint PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MyPaint)
endif()
set_target_properties(MyPaint PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


include(GNUInstallDirs)
install(TARGETS MyPaint
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MyPaint)
endif()

