cmake_minimum_required(VERSION 4.3)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 26)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.5.1
)

set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(glfw)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(STB_IMAGE external/stb/stb_image.cpp)

add_executable(learn_opengl main.cpp
        src/shader.cpp
)
target_include_directories(learn_opengl PUBLIC external/stb include)
target_link_libraries(learn_opengl PRIVATE glfw glad STB_IMAGE)

add_custom_command(TARGET learn_opengl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:learn_opengl>/assets"
        COMMENT "Copying assets to build directory"
)
