# add dependencies
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(dictionary)

# build service
set(SOURCE_FILES main.cpp)
add_executable(Search ${SOURCE_FILES})

target_link_libraries(Search Dictionary)