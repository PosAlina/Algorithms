# add dependencies
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(dynamic_array)
add_subdirectory(linked_list)

# build service
set(SOURCE_FILES main.cpp)
add_executable(Containers ${SOURCE_FILES})

target_link_libraries(Containers DynamicArray LinkedList)