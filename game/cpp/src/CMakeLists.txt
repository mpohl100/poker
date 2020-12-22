set( LIB_SOURCES 
    Card52.cpp
    Deck52.cpp)

add_library( lib SHARED ${LIB_SOURCES} )
target_include_directories(lib PUBLIC ../include)