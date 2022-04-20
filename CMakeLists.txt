cmake_minimum_required(VERSION 3.23)

project(gbmu)

set(CMAKE_CXX_STANDARD 20)

# Sources

add_executable(${PROJECT_NAME} src/main.cpp)

# Building Libs

set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(external/SFML)

# Linking

target_include_directories(${PROJECT_NAME} PUBLIC src/ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/include/)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
