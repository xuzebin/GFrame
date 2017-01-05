cmake_minimum_required(VERSION 2.8)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
SET(LIB_SRC base/glsupport.cpp geometries/Mesh.cpp geometries/Geometry.cpp materials/Cubemap.cpp)
add_library(GFrame ${LIB_SRC})

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")


                               
