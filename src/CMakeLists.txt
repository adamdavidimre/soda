cmake_minimum_required(VERSION 2.8)
project(SoDA)

# Enable ExternalProject CMake module
include(ExternalProject)
include(GenerateExportHeader)
# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ext)

# Activate C++11 support
if (UNIX)
    add_definitions("-std=c++11")
endif()

# Set the build type if it isn't already
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem regex program_options thread)
link_directories(${Boost_LIBRARY_DIRS})

ExternalProject_Add(
    rapidjson
    SVN_REPOSITORY "https://github.com/miloyip/rapidjson"
    SVN_REVISION -r 1510
    CONFIGURE_COMMAND "" # skip configure
    BUILD_COMMAND "" # skip build
    INSTALL_COMMAND "" # skip install
  )

set(RAPIDJSON_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/ext/src/rapidjson/trunk/include)

option(test "Build tests" OFF)
option(coverage "Build with coverage analysis support" OFF)
option(withoutcl "Build without command line tools" 0)

set(LINK_DL)
if (UNIX)
    set(LINK_DL dl)
endif()

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")


set(LINKING_METHOD SHARED)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK)
    set(LINKING_METHOD STATIC)
endif()

add_definitions(-DSODA_PLUGIN_DIR="${SoDA_BINARY_DIR}/plugin")

if(test)
    if(coverage)
        set(CMAKE_BUILD_TYPE, Debug)
        if(CMAKE_COMPILER_IS_GNUCC)
            set(CMAKE_C_FLAGS "-g -O0 -coverage")
            set(CMAKE_CXX_FLAGS "-g -O0 -coverage")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage")
        endif()

        include(${CMAKE_SOURCE_DIR}/cmake/lcov.cmake)
    endif(coverage)
endif(test)

add_subdirectory(lib/SoDA)
add_subdirectory(lib/SoDAEngine)
add_subdirectory(plugin)

if(NOT withoutcl)
    add_subdirectory(cl)

    # Copy scripts to build directory
    file(COPY scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

endif(NOT withoutcl)

if(test)
    # Add gtest
    ExternalProject_Add(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        # TODO: SVN revision was r671
        GIT_TAG release-1.7.0
        TIMEOUT 10
        # Force separate output paths for debug and release builds to allow easy
        # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
                   -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
                   -Dgtest_force_shared_crt=ON
        # Disable install step
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        # Wrap download, configure and build steps in a script to log output
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON)

    # Specify include dir
    ExternalProject_Get_Property(googletest source_dir)
    include_directories(${source_dir}/include)
    add_subdirectory(test/lib)

    enable_testing()
    add_test(
        NAME SoDATest
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/lib
        COMMAND SoDATest
    )

endif()
