cmake_minimum_required(VERSION 2.8)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

add_executable(FXAA FXAA.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(FXAA ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GFRAME_STATIC_LIB})

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

INSTALL(DIRECTORY ${GFrame_SOURCE_DIR}/assets/models/torus DESTINATION ${GFrame_SOURCE_DIR}/build/examples/PostProcessing/FXAA/Debug/assets/models)
INSTALL(DIRECTORY ${GFrame_SOURCE_DIR}/shaders DESTINATION ${GFrame_SOURCE_DIR}/build/examples/PostProcessing/FXAA/Debug)
