 cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
 project(flock LANGUAGES CXX)
 set(CMAKE_CXX_STANDARD 11)
 set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

 # Find the required libraries (i.e., OpenGL, GLEW, and GLFW).
 find_package(OpenGL REQUIRED)
 find_package(GLEW REQUIRED)
 find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
 find_package(PkgConfig REQUIRED)
 pkg_search_module(GLFW REQUIRED glfw3)

 # Define a program target called window.
 add_executable(flock window.cpp shader.cpp flock.cpp gl_math.cpp)

 # Set the includes and libraries for the window target.
 target_include_directories(flock PUBLIC ${GLFW_INCLUDE_DIRS})
 target_link_libraries(flock ${GLFW_LIBRARIES} GLEW::GLEW OpenGL::GL Boost::program_options)

 install(TARGETS flock DESTINATION bin)