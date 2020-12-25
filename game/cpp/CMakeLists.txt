cmake_minimum_required(VERSION 3.0.0)
project(poker VERSION 0.1.0)

find_program(CONAN conan)
if (NOT CONAN)
    message(FATAL_ERROR "Needs conan package manager: please visit https://conan.io and install")
endif ()

include(${CMAKE_SOURCE_DIR}/scripts/conan.cmake)

conan_check(VERSION 1.14.4 REQUIRED)
conan_add_remote(NAME bincrafters INDEX 1
        URL https://api.bintray.com/conan/bincrafters/public-conan)
conan_add_remote(NAME mpusz INDEX 2
        URL https://api.bintray.com/conan/mpusz/conan-mpusz)
conan_cmake_run(CONANFILE conanfile.txt
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)

if( NOT CMAKE_BUILD_TYPE )
     set( CMAKE_BUILD_TYPE Release )
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set( CMAKE_CXX_FLAGS_RELEASE "-O3")
set( CMAKE_CXX_FLAGS_DEBUG "-g") 

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif ()
else ()
    set (CMAKE_CXX_STANDARD 20)
endif ()

enable_testing()

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)