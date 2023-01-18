# CMakeLists.txt for pet_sort program. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(pet_sort)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -O0 -ggdb")



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${VERSION_INCLUDE_DIR})




#---Create a shared library with generated dictionary
add_library(pet_sort 
${CMAKE_CURRENT_SOURCE_DIR}/src/Configuration.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Sorter.cpp)
target_link_libraries(pet_sort)

#---Create  a main program using the library
add_executable(sortPETFile ${CMAKE_CURRENT_SOURCE_DIR}/src/sortPETFile.cpp)



target_link_libraries(sortPETFile pet_sort)


target_compile_definitions(pet_sort PUBLIC __FAVOR_BSD)

# Final reporting
#
message(STATUS "#######################################################")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "#######################################################")
