cmake_minimum_required(VERSION 3.12)
project(game-of-life)

# Add source files
set(SOURCES
  src/main.cpp
)

# Add header files
set(HEADERS
  include/Vector2D.h
)

# Specify the include directories
include_directories(include)

# Set up the project
add_executable(RunThisDummy ${SOURCES} ${HEADERS})

# Link SFML to your executable
find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
target_link_libraries(RunThisDummy PRIVATE sfml-system sfml-window sfml-graphics)
