add_executable(allocator_test allocator_test.cpp)
target_link_libraries(allocator_test PRIVATE mystl GTest::gtest_main)
add_test(NAME allocator_test COMMAND allocator_test)
