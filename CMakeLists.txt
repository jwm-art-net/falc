cmake_minimum_required(VERSION 2.6)

if (COMMAND CMAKE_POLICY)
  cmake_policy (SET CMP0011 NEW)
endif (COMMAND CMAKE_POLICY)

project(falc)

set(falc_VERSION_MAJOR 0)
set(falc_VERSION_MINOR 1)
set(BINDIR  "bin" CACHE STRING "Where to install binaries")
set(ARCHIVE_NAME falc-0.${falc_VERSION_MAJOR}.${falc_VERSION_MINOR})

include (CheckCSourceCompiles)
INCLUDE (CheckIncludeFiles)


mark_as_advanced(EXECUTABLE_OUTPUT_PATH)
mark_as_advanced(CMAKE_BUILD_TYPE)
mark_as_advanced(CMAKE_INSTALL_PREFIX)

option (BuildForDebug "Include gdb debugging support" ON)


set (BuildOptionsDebug "-O0 -ggdb" CACHE STRING "Debug build flags")
set (BuildOptionsRelease "-O2" CACHE STRING "Release build flags")

add_definitions(-Wall -Wextra)

if (BuildForDebug)
    set (CMAKE_BUILD_TYPE "Debug")
    set (DEBUG 1)
    set (CMAKE_C_FLAGS_DEBUG ${BuildOptionsDebug})
    message (STATUS "Building for ${CMAKE_BUILD_TYPE}, flags: ${CMAKE_C_FLAGS_DEBUG}")
else (BuildForDebug)
    set (CMAKE_BUILD_TYPE "Release")
    set (DEBUG 0)
    set (CMAKE_C_FLAGS_RELEASE ${BuildOptionsRelease})
    message (STATUS "Building for ${CMAKE_BUILD_TYPE}, flags: ${CMAKE_C_FLAGS_RELEASE}")
endif (BuildForDebug)


CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/config.h )

link_libraries(m)


file (GLOB FALC_SOURCES *.c)
add_executable(falc ${FALC_SOURCES})

