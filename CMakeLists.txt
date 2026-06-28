cmake_minimum_required(VERSION 3.25)
project(mtms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_GUI "Build GUI with Qt6" OFF)
option(BUILD_CLI "Build CLI application" ON)

if(NOT BUILD_CLI AND NOT BUILD_GUI)
    message(FATAL_ERROR "You must enable at least one target to build: BUILD_CLI or BUILD_GUI.")
endif()


# CORE LIBRARY TARGET
add_library(mtms-core STATIC
    src/core/alphabet.cpp
    src/core/alphabet.hpp
    src/core/direction.hpp
    src/core/project.cpp
    src/core/project.hpp
    src/core/state.hpp
    src/core/string.cpp
    src/core/string.hpp
    src/core/symbol.hpp
    src/core/tape.cpp
    src/core/tape.hpp
    src/core/tm.cpp
    src/core/tm.hpp
    src/core/transition.hpp
)

target_include_directories(mtms-core SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLOBAL_INCLUDES}
)


# CLI EXECUTABLE TARGET
if(BUILD_CLI)
    add_executable(mtms-cli
        src/cli/cli.hpp
        src/cli/cli.cpp
        src/cli/main.cpp
        src/cli/program.cpp
        src/cli/program.hpp
    )

    # Link engine to CLI
    target_link_libraries(mtms-cli PRIVATE mtms-core)
    target_include_directories(mtms-cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/cli)
else()
    message(STATUS "BUILD_CLI is set to OFF. mtms-cli will not be built.")
endif()


# GUI EXECUTABLE TARGET
if(BUILD_GUI)
    find_package(Qt6 COMPONENTS Widgets REQUIRED)

    # Enable automatic generation of Qt files
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    add_executable(mtms-gui
        src/gui/main.cpp
        src/gui/gui.cpp
        src/gui/MainWindow.cpp
    )

    # Link both engine and Qt libs
    target_link_libraries(mtms-gui PRIVATE 
        mtms-core 
        Qt6::Widgets
    )
    target_include_directories(mtms-gui PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
    )
else()
    message(STATUS "BUILD_GUI is set to OFF. mtms-gui will not be built.")
endif()

# DOCS
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND doxygen ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen"
    )
endif()
