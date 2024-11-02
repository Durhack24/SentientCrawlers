# EXECUTABLE PROJECT - SentientCrawlers

# === Includes ===
cmake_policy(SET CMP0069 NEW)
include(../macros.cmake)

# === Create Executable ===
add_executable(SentientCrawlers "main.cpp")

# === Feature Enables ===
if (DHT_MSVC_STATIC_RUNTIME)
    set_property(TARGET SentientCrawlers PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set_property(TARGET SentientCrawlers PROPERTY CXX_STANDARD 20)
set_arch(SentientCrawlers)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    enable_ipo(SentientCrawlers)
endif()