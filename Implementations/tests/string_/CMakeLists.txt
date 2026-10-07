add_executable(string_test string_test.cpp)
target_link_libraries(string_test PRIVATE mystl Catch2::Catch2WithMain)
add_test(NAME string_test COMMAND string_test)
