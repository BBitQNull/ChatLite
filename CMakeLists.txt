cmake_minimum_required(VERSION 3.10.0)

project(ChatLite)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src/server)
add_subdirectory(src/client)