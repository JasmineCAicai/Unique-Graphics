cmake_minimum_required(VERSION 3.19)
project(Assignment6)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add header file
set(OPENCV_H /opt/homebrew/Cellar/opencv/4.5.3_2/include/opencv4)
set(EIGEN_H /opt/homebrew/Cellar/eigen/3.3.9/include/eigen3)
include_directories(${OPENCV_H} ${EIGEN_H})

add_executable(Assignment6 main.cpp Object.h Vector.cpp Vector.h Sphere.h global.h Triangle.h Scene.cpp
Scene.h Light.h AreaLight.h BVH.cpp BVH.h Bounds3.h Ray.h Material.h Intersection.h Renderer.cpp Renderer.h)
target_link_libraries(Assignment6 ${OpenCV_LIBS})
#target_compile_options(Rasterizer PUBLIC -Wall -Wextra -pedantic)