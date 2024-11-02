# GLOBAL - SentientCrawlers

# === Includes ===
include(macros.cmake)

# === Configuration ===
cmake_minimum_required(VERSION 3.21)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable multi-processor build
enable_mp()

# Options
set(WARNINGS OFF CACHE BOOL "Enables clang-tidy and warnings on clang-like compilers")
if (WARNINGS)
	add_compile_options("-Wall")
    add_compile_options("-Wextra")
    add_compile_options("-Wpedantic")
    enable_clangtidy()
endif()

set(USE_LIBCPP OFF CACHE BOOL "Use the libc++ standard library")
if (USE_LIBCPP)
    add_link_options("-lc++")
    add_link_options("-lc++abi")
endif()

set(SCRWL_MSVC_STATIC_RUNTIME, OFF CACHE BOOL "Use static C runtime library when taregting MSVC ABI")
if (SCRWL_MSVC_STATIC_RUNTIME)
    use_static_msvc_crt()
endif()

# === Projects ===
project("SentientCrawlers")
add_subdirectory("SentientCrawlers")