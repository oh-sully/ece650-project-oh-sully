cmake_minimum_required(VERSION 2.8)
project(project)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -pthread ${CMAKE_CXX_FLAGS}")

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)

# create the main executable
## add additional .cpp files if needed
#add_executable(ece650-project ece650-project.cpp parse_line.cpp parse_line.hpp register.cpp register.hpp vertexAdjList.cpp vertexAdjList.hpp ece650-a4.hpp)
add_executable(project project.cpp parse_line.cpp parse_line.hpp register.cpp register.hpp vertexAdjList.cpp vertexAdjList.hpp ece650-a4.hpp)
add_executable(openfile openfile.cpp)

# link MiniSAT libraries
target_link_libraries(project minisat-lib-static)
