cmake_minimum_required(VERSION 3.13)
project(LxCrystal VERSION 1.1)

#Disable in source-builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# --------------------  Sources --------------------
file(GLOB_RECURSE CXX_SOURCES RELATIVE  ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE C_SOURCES RELATIVE    ${CMAKE_SOURCE_DIR} "src/*.c")
include_directories("include/")
include_directories("src/")
include_directories("src/core")
add_executable(LxCrystal ${CXX_SOURCES}  ${C_SOURCES})

# -------------------- Libraries --------------------
# GLFW
add_subdirectory("lib/glfw3")
target_link_libraries(LxCrystal glfw ${GLFW_LIBRARIES})
# ---------------------------------------------------

set(CMAKE_BUILD_TYPE, RELEASE)
