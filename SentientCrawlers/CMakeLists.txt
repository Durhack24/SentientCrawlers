# EXECUTABLE PROJECT - SentientCrawlers

# === Includes ===
cmake_policy(SET CMP0069 NEW)
include(../macros.cmake)
include(FindOpenGL)
include(FindGlew)

# === Create Executable ===
add_executable(SentientCrawlers "main.cpp" "ImGui/imconfig.h" "ImGui/imgui.cpp" "ImGui/imgui.h" "ImGui/imgui_demo.cpp" "ImGui/imgui_draw.cpp" "ImGui/imgui_internal.h" "ImGui/imgui_tables.cpp" "ImGui/imgui_widgets.cpp" "ImGui/imstb_rectpack.h" "ImGui/imstb_textedit.h" "ImGui/imstb_truetype.h" "ImGui/imgui_impl_opengl3.h" "ImGui/imgui_impl_opengl3.cpp" "ImGui/imgui_impl_opengl3_loader.h" "ImGui/imgui_impl_glfw.h" "ImGui/imgui_impl_glfw.cpp"   "UI/Interface.h" "UI/Interface.cpp" "UI/init.h" "UI/init.cpp" "UI/err.h" "UI/err.cpp" "UI/theme.h" "UI/theme.cpp"  "UI/App.h" "UI/App.cpp" "Resources/ResourceManager.cpp" "Resources/ResourceManager.h" "Simulator/Crawler.h" "Simulator/Crawler.cpp" "Simulator/Brain.h" "Simulator/Brain.cpp" "Simulator/Simulator.h" "Simulator/Simulator.cpp" "Simulator/Map.h" "Simulator/Map.cpp" "Simulator/CSVReader.cpp" "Simulator/CSVReader.h" "Simulator/CollegeBar.h"  "Simulator/Point.h" "Simulator/CollegeBar.cpp" "UI/Image.h" "UI/Image.cpp" "UI/glall.h" "UI/glerr.h" "UI/glerr.cpp" "UI/stb_image.h" "UI/stb_image.cpp"
        Simulator/Point.cpp
        Simulator/colorutils.cpp
        Simulator/colorutils.h "UI/TrainingRenderer.h" "UI/ShowcaseRenderer.h" "UI/Renderer.h" "UI/ShowcaseRenderer.cpp" "UI/TrainingRenderer.cpp" "UI/Renderer.cpp" "Simulator/BS_thread_pool.hpp")

# === Libraries ===
# OpenGL
target_link_libraries(SentientCrawlers PRIVATE OpenGL::GL)

# Glew
target_include_directories(SentientCrawlers PRIVATE GLEW_INCLUDE_DIRS)
target_link_libraries(SentientCrawlers PRIVATE GLEW::glew_s)

# Glfw
find_package(glfw3 REQUIRED)
target_link_libraries(SentientCrawlers PRIVATE glfw)

# === Feature Enables ===
if (SCRWL_MSVC_STATIC_RUNTIME)
    set_property(TARGET SentientCrawlers PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set_property(TARGET SentientCrawlers PROPERTY CXX_STANDARD 20)
set_arch(SentientCrawlers)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    enable_ipo(SentientCrawlers)
endif()