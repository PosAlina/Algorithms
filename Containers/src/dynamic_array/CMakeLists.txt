# build service
set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/include/constants.hpp
        dynamic_array.cpp ${PROJECT_SOURCE_DIR}/include/dynamic_array/dynamic_array.hpp)

add_library(DynamicArray ${SOURCE_FILES})