cmake_minimum_required(VERSION 3.20.0)

# Application name
project("MayaApp")

# Source files
add_executable(${PROJECT_NAME} "app/main.cpp")

# Version: C++ 20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Download Git submodules
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodule as needed
    option(GIT_SUBMODULE "Check submodule during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}.")
        endif()
    endif()
endif()

# Load engine subdirectory
add_subdirectory("engine")

# Engine include directory
target_include_directories(${PROJECT_NAME} PUBLIC "enigne/include")

# Engine link library
target_link_libraries(${PROJECT_NAME} MayaEngine)