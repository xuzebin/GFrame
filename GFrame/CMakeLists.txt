cmake_minimum_required(VERSION 2.8)

project(GFrame)
add_executable(GFrame main.cpp base/glsupport.cpp geometries/Mesh.cpp geometries/Geometry.cpp materials/Cubemap.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

FIND_LIBRARY(OPENGL_LIBRARIES OpenGL)
FIND_LIBRARY(GLUT_LIBRARIES GLUT)
target_link_libraries(GFrame ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}) 

#add_library(GLFrame STATIC main.cpp)
#install(TARGETS GFrame DESTINATION lib)

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")


                               
