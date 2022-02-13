cmake_minimum_required(VERSION 3.19)
project(Assignment7)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add header file
set(OPENCV_H /opt/homebrew/Cellar/opencv/4.5.3_2/include/opencv4)
set(EIGEN_H /opt/homebrew/Cellar/eigen/3.3.9/include/eigen3)
include_directories(${OPENCV_H} ${EIGEN_H})

add_executable(Assignment7 main.cpp Object.hpp Vector.cpp Vector.hpp Sphere.hpp global.hpp Triangle.hpp Scene.cpp
        Scene.hpp Light.hpp AreaLight.hpp BVH.cpp BVH.hpp Bounds3.hpp Ray.hpp Material.hpp Intersection.hpp
        Renderer.cpp Renderer.hpp)
target_link_libraries(Assignment7 ${OpenCV_LIBS})
#target_compile_options(Rasterizer PUBLIC -Wall -Wextra -pedantic)