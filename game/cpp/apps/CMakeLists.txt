link_directories(${CMAKE_SOURCE_DIR}/src)

set( EXCEUTABLE_NAME main)

add_executable(${EXCEUTABLE_NAME} main.cpp)
target_link_libraries(${EXCEUTABLE_NAME} game)
target_include_directories(${EXCEUTABLE_NAME} PUBLIC ../include) 