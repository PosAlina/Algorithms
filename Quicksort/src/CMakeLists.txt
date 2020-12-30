# add dependencies
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(sorter)

# build service
set(SOURCE_FILES main.cpp)
add_executable(Quicksort ${SOURCE_FILES})

target_link_libraries(Quicksort Sorter)