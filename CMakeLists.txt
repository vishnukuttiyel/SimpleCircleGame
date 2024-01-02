# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(sfml-app)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${SFML_INCLUDE_DIR} include imgui imgui-sfml)  # Add the include directories for ImGui and ImGui-SFML

# Set the output directory for build artifacts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" imgui/imgui_demo.cpp imgui/imgui.cpp imgui/imgui_widgets.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui-sfml/imgui-SFML.cpp)
file(GLOB_RECURSE HEADERS "include/*.h" imgui/*.h)

add_subdirectory(src)

# Executable
add_executable(sfml-app ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(sfml-app PRIVATE sfml-graphics sfml-window sfml-system)

# Link OpenGL
target_link_libraries(sfml-app PRIVATE ${OPENGL_LIBRARIES})

# Link your library
target_link_libraries(sfml-app PRIVATE SimpleCircleGameLib)

# Add testing subdirectory
add_subdirectory(test)