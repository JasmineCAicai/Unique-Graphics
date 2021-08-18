cmake_minimum_required(VERSION 3.19)
project(Assignment3_1)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp)

# Add header file
set(OPENCV_H /opt/homebrew/Cellar/opencv/4.5.3_2/include/opencv4)
set(EIGEN_H /opt/homebrew/Cellar/eigen/3.3.9/include/eigen3)
include_directories(${OPENCV_H} ${EIGEN_H})

add_executable(Assignment3_1 main.cpp global.h OBJ_Loader.h rasterizer.cpp rasterizer.h Shader.h Texture.cpp Texture.h Triangle.cpp Triangle.h)
target_link_libraries(Assignment3_1 ${OpenCV_LIBS})
#target_compile_options(Rasterizer PUBLIC -Wall -Wextra -pedantic)