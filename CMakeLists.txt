include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Run)

project(MyProject VERSION 1.0)

find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

# ===== GLAD ====================================================== #

add_library(GLAD STATIC ${GLAD_PATH}/src/gl.c)
target_include_directories(GLAD PUBLIC ${GLAD_PATH}/include)

# ===== Main ====================================================== #

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c ${FAST_OBJ_PATH}/fast_obj.c)
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wuninitialized -Wextra -Wmaybe-uninitialized)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/pch.h)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CGLM_PATH}/include
  ${STB_PATH}/include
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLAD m Freetype::Freetype)

