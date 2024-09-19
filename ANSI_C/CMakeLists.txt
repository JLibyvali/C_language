cmake_minimum_required(VERSION 3.28)
project(C_fromK_R LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")
set(SOURCES ${PROJECT_SOURCE_DIR}/prog.c)

set(HEADERS
    ${PROJECT_SOURCE_DIR}/Prac2.h ${PROJECT_SOURCE_DIR}/Prac5.h
    ${PROJECT_SOURCE_DIR}/Prac6.h ${PROJECT_SOURCE_DIR}/Prac7.h
    ${PROJECT_SOURCE_DIR}/Prac8.h)

message(STATUS "==================================================")

message(STATUS "==================================================")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# linking math library
target_link_libraries(${PROJECT_NAME} m)
