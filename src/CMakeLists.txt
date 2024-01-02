# src/CMakeLists.txt
add_library(SimpleCircleGameLib
    main.cpp
    vec2.cpp
)

target_include_directories(SimpleCircleGameLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
