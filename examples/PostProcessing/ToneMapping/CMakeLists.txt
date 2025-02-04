cmake_minimum_required(VERSION 2.8)

add_executable(ToneMapping ToneMapping.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(ToneMapping ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${GFRAME_STATIC_LIB} "-framework Cocoa -framework IOKit -framework CoreVideo")

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

INSTALL(DIRECTORY ${GFrame_SOURCE_DIR}/assets/models/monk DESTINATION ${GFrame_SOURCE_DIR}/build/examples/PostProcessing/ToneMapping/Debug/assets/models)
INSTALL(DIRECTORY ${GFrame_SOURCE_DIR}/shaders DESTINATION ${GFrame_SOURCE_DIR}/build/examples/PostProcessing/ToneMapping/Debug)
