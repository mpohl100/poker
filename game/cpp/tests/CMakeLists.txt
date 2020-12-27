add_executable(tests tests.cpp ClassifiedHandTests.cpp CardTests.cpp)
target_link_libraries(tests game CONAN_PKG::Catch2)
add_test(NAME test COMMAND $<TARGET_FILE:tests>)