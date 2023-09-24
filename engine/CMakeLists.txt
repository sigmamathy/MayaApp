# Engine project
project("MayaEngine")

# Engine source files
add_library(
	${PROJECT_NAME}
	"thirdparty/glad/glad.c"
	"thirdparty/stb/stb_image.c"
	"source/launch.cpp"
	"source/deviceinfo.cpp"
	"source/window.cpp"
	"source/transformation.cpp"
	"source/value_tracker.cpp"
	"source/scene.cpp"
	"source/shader.cpp"
	"source/vertex_array.cpp"
	"source/texture.cpp"
	"source/font.cpp"
	"source/audio_stream.cpp"
	"source/2D/graphics.cpp"
	"source/3D/model.cpp"
 "source/3D/graphics.cpp" "source/3D/camera.cpp")

if (MSVC)
add_compile_options(/wd4819)
endif()

# Version: C++ 20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Configure submodule settings
set(GLFW_BUILD_DOCS OFF)
set(ASSIMP_BUILD_STATIC_LIB ON)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_ZLIB OFF)

# Add external submodules
add_subdirectory("thirdparty/glfw")
add_subdirectory("thirdparty/freetype")
add_subdirectory("thirdparty/portaudio")
add_subdirectory("thirdparty/sndfile")
add_subdirectory("thirdparty/assimp")

# Include directories
target_include_directories(
	${PROJECT_NAME}
	PUBLIC "include"
	PUBLIC "thirdparty/glfw/include"
	PUBLIC "thirdparty/glad/include"
	PUBLIC "thirdparty/stb/include"
	PUBLIC "thirdparty/freetype/include"
	PUBLIC "thirdparty/portaudio/include"
	PUBLIC "thirdparty/sndfile/include"
	PUBLIC "thirdparty/assimp/include"
)

# Link third party libraries
target_link_libraries(
	${PROJECT_NAME}
	glfw
	freetype
	PortAudio
	sndfile
	assimp
)

add_custom_target(copy_resources ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resource" "${PROJECT_BINARY_DIR}/resource"
	COMMENT "Copying Maya Engine resources into engine/resource"
)

add_dependencies(${PROJECT_NAME} copy_resources)