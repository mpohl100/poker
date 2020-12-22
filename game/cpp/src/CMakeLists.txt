set( LIB_SOURCES 
    Card52.cpp
    Deck52.cpp
    Pot.cpp 
    Nlhe.cpp
    Stack.cpp
    Player.cpp
    )

set( LIBNAME game)

add_library( ${LIBNAME} SHARED ${LIB_SOURCES} )
target_include_directories(${LIBNAME} PUBLIC ../include)