 cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
 project(flock LANGUAGES CXX)
 set(CMAKE_CXX_STANDARD 20)
 set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

 set(OpenGL_GL_PREFERENCE GLVND)
 set(Boost_NO_WARN_NEW_VERSIONS 1)

 # Find the required libraries (i.e., OpenGL, GLEW, and GLFW).
 find_package(OpenGL REQUIRED)
 find_package(GLEW REQUIRED)
 find_package(Boost 1.54.0 COMPONENTS program_options REQUIRED)
 find_package(PkgConfig REQUIRED)
 pkg_search_module(GLFW REQUIRED glfw3)

 # Define a program target.
 add_executable(flock src/main.cpp src/shader.cpp src/flock.cpp src/gl_math.cpp)

 # Set the includes and libraries for the executable.
 target_include_directories(flock PUBLIC ${GLFW_INCLUDE_DIRS})
 target_link_libraries(flock ${GLFW_LIBRARIES} GLEW::GLEW OpenGL::GL Boost::program_options)

 install(TARGETS flock DESTINATION bin)
 install(PROGRAMS demo DESTINATION bin)