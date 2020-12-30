# build service
set(SOURCE_FILES LinkedListTest.cpp)

add_executable(runLinkedListTests ${SOURCE_FILES})
target_link_libraries(runLinkedListTests LinkedList gtest gtest_main)

add_test(runLinkedListTests runLinkedListTests)