cmake_minimum_required(VERSION 3.2)

project(FunctionGraph)
SET(TARGET_SRC
	main.cpp
	src/Function.cpp
	src/Function.h
	libs/imgui/imgui.cpp
	libs/imgui/imgui_widgets.cpp
	libs/imgui/imgui_draw.cpp
	libs/imgui/imgui_tables.cpp
	libs/imgui/imgui_demo.cpp
	libs/imgui/imgui-sfml-master/imgui-SFML.cpp
)
add_executable(FunctionGraph ${TARGET_SRC})

set(SFML_DIR "C:/Users/Tort1k/source/repos/FunctionGraph/libs/SFML-2.5.1/lib/cmake/SFML")
 
target_compile_features(FunctionGraph PRIVATE cxx_std_17)

set(LIBRARY
	sfml-graphics
	opengl32
)
target_link_libraries(FunctionGraph ${LIBRARY})

find_package(OpenGL REQUIRED)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2.5 REQUIRED graphics network audio window system)
find_package(OpenGL)
target_include_directories(FunctionGraph PRIVATE include/ImGUI)
target_include_directories(FunctionGraph PUBLIC libs/imgui/imgui-sfml-master)
target_include_directories(FunctionGraph PUBLIC src/)

message(STATUS "Fetching ImGui-SFML...")

set(imgui_SOURCE_DIR "C:/Users/Tort1k/source/repos/FunctionGraph/libs/ImGUI")
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

add_subdirectory(libs/imgui/imgui-sfml-master)

