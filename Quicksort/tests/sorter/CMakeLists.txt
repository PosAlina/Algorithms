# build service
set(SOURCE_FILES SorterTest.cpp)

add_executable(runSorterTests ${SOURCE_FILES})
target_link_libraries(runSorterTests Sorter gtest gtest_main)

add_test(runSorterTests runSorterTests)