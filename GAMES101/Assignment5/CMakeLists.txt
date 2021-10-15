cmake_minimum_required(VERSION 3.19)
project(Assignment5)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add header file
set(OPENCV_H /opt/homebrew/Cellar/opencv/4.5.3_2/include/opencv4)
set(EIGEN_H /opt/homebrew/Cellar/eigen/3.3.9/include/eigen3)
include_directories(${OPENCV_H} ${EIGEN_H})

add_executable(Assignment5 main.cpp global.h Light.h Object.h Renderer.cpp Renderer.h Scene.cpp Scene.h Sphere.h Triangle.h Vector.h)
target_link_libraries(Assignment5 ${OpenCV_LIBS})