cmake_minimum_required(VERSION 2.8)

project(SimpleModel)
add_executable(SimpleModel SimpleModel.cpp ../../base/glsupport.cpp ../../geometries/Mesh.cpp ../../geometries/Geometry.cpp ../../materials/Cubemap.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

FIND_LIBRARY(OPENGL_LIBRARIES OpenGL)
FIND_LIBRARY(GLUT_LIBRARIES GLUT)
target_link_libraries(SimpleModel ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}) 

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")


                               
