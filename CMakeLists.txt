cmake_minimum_required(VERSION 3.15)

project(Cidr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
include("FindSDL2.cmake")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(INCLUDE_DIR "./include")
	file(GLOB SRC "./demo.cpp" "./src/*" ${INCLUDE_DIR}/*)
else()
	file(GLOB SRC "./demo.cpp" "./cidr.hpp")
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
endif()
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
