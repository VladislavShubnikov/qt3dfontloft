cmake_minimum_required(VERSION 4.2)

set(APP_PRJ "loftviewer")
project(${APP_PRJ} VERSION 0.1.0 LANGUAGES C CXX)


# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# To use QT headers during clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# check QTDIR is defined in the system env vars
if (NOT DEFINED ENV{QTDIR})
  message(STATUS "env var QTDIR is not defined in system")
else()

  # use qt path to cmake files from QT
  set(PATH_QT_CMAKE "$ENV{QTDIR}/lib/cmake")

  # print path
  message(STATUS "detected path to qt cmake: ${PATH_QT_CMAKE}")

  set(CMAKE_PREFIX_PATH "${PATH_QT_CMAKE}")

endif()


# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (WIN32)
    set(PLATF "Windows")

    set(PLATFORM_COMMON_FLAGS -W4)

endif()

if (UNIX)
    set(PLATF "Unix")

    set(PLATFORM_COMMON_FLAGS
        -fvisibility=internal
        -fPIC
        -Wno-missing-field-initializers
        -Wfloat-equal
        -Wfloat-conversion
        -Wpedantic
        -fno-guess-branch-probability
        -fno-expensive-optimizations)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(PLATFORM_COMMON_FLAGS "${PLATFORM_COMMON_FLAGS}" -g -O0)
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        # set -Og flag for RelWithDebInfo: it allows stack trace debugging only.
        # set -O2 flag allows variable value evaluation, and this is
        # a bit slow than -O3
        # -g to save debugging information
        set(PLATFORM_COMMON_FLAGS "${PLATFORM_COMMON_FLAGS}" -g -O2)
    else()
        # do not apply -Ofast (== -O3 -ffast-math)
        set(PLATFORM_COMMON_FLAGS "${PLATFORM_COMMON_FLAGS}" -O3)
    endif()

endif()

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(${PLATFORM_COMMON_FLAGS})

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# Find the Qt library components
find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL Test Concurrent Core)

# print current compiler
message(STATUS "Current c++ compiler is: ${CMAKE_CXX_COMPILER_ID}")

# -------------------------------------------------
# Application project
# -------------------------------------------------
set(APP_SOURCES 
  src/main.cpp
  src/colorgenerator.cpp
  src/colorgenerator.h
  src/contour.cpp
  src/contour.h
  src/contourtools.cpp
  src/contourtools.h
  src/geo.cpp
  src/geo.h
  src/glviewwidget.cpp
  src/glviewwidget.h
  src/glmesh3d.cpp
  src/glmesh3d.h
  src/mesh3d.cpp
  src/mesh3d.h
  src/mesh3dbuilder.cpp
  src/mesh3dbuilder.h
  src/mesh3dsaver.cpp
  src/mesh3dsaver.h
  src/meshclip.cpp
  src/meshclip.h
  src/meshtrianglebuilder.cpp
  src/meshtrianglebuilder.h
  src/meshtools.cpp
  src/meshtools.h
  src/mainwindow.h
  src/mainwindow.cpp
  src/json.hpp
  src/plane3d.cpp
  src/plane3d.h
  src/testclip.cpp
  src/testclip.h
  src/mainwindow.ui)

# add resource qrc file
set (APP_FILES ${APP_SOURCES} res/resources.qrc)

add_executable(${APP_PRJ} ${APP_FILES})

# Add the Qt5 Widgets for linking
target_link_libraries(${APP_PRJ} Qt5::Widgets Qt5::OpenGL Qt5::Concurrent Qt5::Core)

# select c++20 or later
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set_property(TARGET ${APP_PRJ} PROPERTY CXX_STANDARD 20)
else()
    set_property(TARGET ${APP_PRJ} PROPERTY CXX_STANDARD 20)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_features(${APP_PRJ} PRIVATE cxx_std_20)
else()
    target_compile_features(${APP_PRJ} PRIVATE cxx_std_20)
endif()
