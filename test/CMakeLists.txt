# test/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Locate GTest
find_package(GTest REQUIRED)

# Add test executable
add_executable(sfml-app-tests
    test_main.cpp
    test_vec2.cpp  # Add your test files here
)

# Link against your library and GTest
target_link_libraries(sfml-app-tests PRIVATE SimpleCircleGameLib GTest::GTest GTest::Main)
