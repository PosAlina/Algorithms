# build service
set(SOURCE_FILES DictionaryTest.cpp)

add_executable(runDictionaryTests ${SOURCE_FILES})
target_link_libraries(runDictionaryTests Dictionary gtest gtest_main)

add_test(runDictionaryTests runDictionaryTests)