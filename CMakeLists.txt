cmake_minimum_required(VERSION 3.20)
project(docSrv)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -ggdb -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Wall -Os -DNDEBUG")


if (NOT CMAKE_BUILD_TYPE) 
    message(STATUS "Using DEBUG build type for default")
    set(CMAKE_BUILD_TYPE DEBUG)
endif()


add_executable(main docSrv.cpp)
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/incs)
target_link_libraries(main PRIVATE ws2_32  PRIVATE pthread )
target_link_options(main PRIVATE "-Wl,--stack,20000000")
target_link_options(main PRIVATE "-fuse-ld=lld")