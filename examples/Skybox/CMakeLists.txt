cmake_minimum_required(VERSION 2.8)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

add_executable(Skybox Skybox.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(Skybox ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GFRAME_STATIC_LIB})

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

INSTALL(DIRECTORY ${GFrame_SOURCE_DIR}/assets/cubemap DESTINATION ${GFrame_SOURCE_DIR}/build/examples/Skybox/Debug/assets)
INSTALL(DIRECTORY ${GFrame_SOURCE_DIR}/shaders DESTINATION ${GFrame_SOURCE_DIR}/build/examples/Skybox/Debug)
