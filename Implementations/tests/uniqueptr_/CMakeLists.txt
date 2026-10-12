add_executable(uptr_test uptr_test.cpp)
target_link_libraries(uptr_test PRIVATE mystl Catch2::Catch2WithMain)
add_test(NAME uptr_test COMMAND uptr_test)
