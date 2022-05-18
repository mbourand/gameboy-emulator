cmake_minimum_required(VERSION 3.23)

project(gbmu)

set(CMAKE_CXX_STANDARD 20)

# Sources

add_executable(${PROJECT_NAME} src/main.cpp src/Memory/Memory.cpp src/CPU/CPU.cpp src/CPU/OpCodes.cpp src/Cartridge/Cartridge.cpp src/Clock/Clock.cpp src/PPU/PPU.cpp src/MBC/NoMBC.cpp src/MBC/MBC1.cpp src/Controller/Controller.cpp src/Gameboy/Gameboy.cpp)

# Building Libs

set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(external/SFML)

# Linking

target_include_directories(${PROJECT_NAME} PUBLIC src/ PUBLIC src/Controller PUBLIC src/Cartridge PUBLIC src/Gameboy PUBLIC src/Memory PUBLIC src/CPU PUBLIC src/Clock PUBLIC src/PPU PUBLIC src/MBC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/include/)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
