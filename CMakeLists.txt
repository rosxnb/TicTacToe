cmake_minimum_required(VERSION 3.27)
project(TicTacToe VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address")

add_executable(TicTacToe
src/main.cpp
src/app_delegate.cpp
src/view_delegate.cpp
src/renderer.cpp
src/game_controller.m
src/keys_code.mm
src/utility.cpp
src/math.cpp
)

set(PLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
set_target_properties(TicTacToe PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${PLIST_FILE}")

target_include_directories(TicTacToe PRIVATE dependencies/include/metal-cpp)
target_include_directories(TicTacToe PRIVATE dependencies/include/metal-cpp-extensions)
target_link_libraries(TicTacToe 
"-framework Metal"
"-framework Foundation"
"-framework Cocoa"
"-framework CoreGraphics"
"-framework MetalKit"
"-framework QuartzCore"
"-framework GameController"
)
