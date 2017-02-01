cmake_minimum_required(VERSION 2.8)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
file(GLOB_RECURSE SRC_FILES *.cpp)                                                 
add_library(GFrame ${SRC_FILES})

#install(TARGETS GFrame DESTINATION /Users/xuzebin/Projects/GLFramework/GFrame/lib/)

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
