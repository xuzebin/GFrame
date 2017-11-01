cmake_minimum_required(VERSION 2.8)

project(GFrame)

OPTION(BUILD_SRC "Build source files" OFF)

SET(GFRAME_STATIC_LIB ${PROJECT_SOURCE_DIR}/lib/libGFrame.a)

if (BUILD_SRC)
add_subdirectory(src)
else (BUILD_SRC)
add_subdirectory(examples)
add_subdirectory(examples/ColoredModel)
add_subdirectory(examples/TexturedModel)
add_subdirectory(examples/Trackball)
add_subdirectory(examples/Skybox)
add_subdirectory(examples/Button3D)
add_subdirectory(examples/OffscreenRendering)
add_subdirectory(examples/PostProcessing/FXAA)
add_subdirectory(examples/PostProcessing/ToneMapping)
add_subdirectory(examples/RollingSphere)
add_subdirectory(examples/GLFWTest)
endif (BUILD_SRC)

