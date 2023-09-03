# Engine project
project("MayaEngine")

# Engine source files
add_library(
	${PROJECT_NAME}
	"thirdparty/glad/glad.c"
	"thirdparty/stb/stb_image.c"
	"src/launch.cpp"
	"src/deviceinfo.cpp"
	"src/window.cpp"
	"src/scene.cpp"
	"src/shader.cpp"
	"src/vertex_array.cpp"
	"src/transformation.cpp"
	"src/2D/graphics.cpp"
	"src/texture.cpp"
	"src/resources.cpp"
	"src/font.cpp"
	"src/value_tracker.cpp"
)

# Version: C++ 20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Check submodules
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/thirdparty/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodule was not download! GIT_SUBMODULE was turned off or failed.")
endif()

# Add external submodules
set(GLFW_BUILD_DOCS OFF)
add_subdirectory("thirdparty/glfw")
add_subdirectory("thirdparty/freetype")

# Include directories
target_include_directories(
	${PROJECT_NAME}
	PUBLIC "include"
	PUBLIC "thirdparty/glfw/include"
	PUBLIC "thirdparty/glad/include"
	PUBLIC "thirdparty/stb/include"
	PUBLIC "thirdparty/freetype/include"
)

# Link third party libraries
target_link_libraries(
	${PROJECT_NAME}
	glfw
	freetype
)

add_custom_target(copy_resources ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" "${PROJECT_BINARY_DIR}/res"
	COMMENT "Copying Maya Engine resources into engine/res"
)

add_dependencies(${PROJECT_NAME} copy_resources)