add_executable(sharedptr_test sharedptr_test.cpp)
target_link_libraries(sharedptr_test PRIVATE mystl GTest::gtest_main)
add_test(NAME sharedptr_test COMMAND sharedptr_test)
