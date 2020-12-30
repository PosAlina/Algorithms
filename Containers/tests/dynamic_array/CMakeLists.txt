# build service
set(SOURCE_FILES DynamicArrayTest.cpp)

add_executable(runDynamicArrayTests ${SOURCE_FILES})
target_link_libraries(runDynamicArrayTests DynamicArray gtest gtest_main)

add_test(runDynamicArrayTests runDynamicArrayTests)