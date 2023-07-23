 cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
 project(window LANGUAGES CXX)
 set(CMAKE_CXX_STANDARD 11)
 set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

 # Find the required libraries (i.e., OpenGL, GLEW, and GLFW).
 find_package(OpenGL REQUIRED)
 find_package(GLEW REQUIRED)
 find_package(PkgConfig REQUIRED)
 pkg_search_module(GLFW REQUIRED glfw3)

 # Define a program target called window.
 add_executable(window window.cpp)

 # Set the includes and libraries for the window target.
 target_include_directories(window PUBLIC ${GLFW_INCLUDE_DIRS})
 target_link_libraries(window ${GLFW_LIBRARIES} GLEW::GLEW OpenGL::GL)