cmake_minimum_required(VERSION 3.10)

project(zbar_app)

#set_property(TARGET zbar_app PROPERTY C_STANDARD 99)
set(CMAKE_C_FLAGS "-std=c99")

SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

#SET(CMAKE_C_COMPILER   gcc)
#SET(CMAKE_CXX_COMPILER g++)

aux_source_directory(. DIR_SRCS)

include_directories("${PROJECT_SOURCE_DIR}/zbar_lib")
add_subdirectory(zbar_lib)


add_executable(zbar_app ${DIR_SRCS})

target_link_libraries(zbar_app zbarLib)
