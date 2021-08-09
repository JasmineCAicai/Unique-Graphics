cmake_minimum_required(VERSION 3.19)
project(Assignment2)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp)

# Add header file
set(OPENCV_H /opt/homebrew/Cellar/opencv/4.5.3_2/include/opencv4)
set(EIGEN_H /opt/homebrew/Cellar/eigen/3.3.9/include/eigen3)
include_directories(${OPENCV_H} ${EIGEN_H})

add_executable(Assignment2 main.cpp rasterizer.cpp rasterizer.h Triangle.cpp Triangle.h global.h)
target_link_libraries(Assignment2 ${OpenCV_LIBS})