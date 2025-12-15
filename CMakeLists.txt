cmake_minimum_required(VERSION 3.10)
project(Midterm VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../common/include
)

# Source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/main.cpp)

# Build executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Tell CMake where to find the .a libraries
link_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/../common/lib
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/../common/lib

)

# Link by library NAME (no full paths)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        mingw32
        SOIL
        freeglut
        opengl32
        glu32
        irrKlang
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_SOURCE_DIR_STRING="${PROJECT_SOURCE_DIR}")

include(CTest)
enable_testing()
