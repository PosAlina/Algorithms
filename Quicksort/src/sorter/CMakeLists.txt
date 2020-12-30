# build service
set(SOURCE_FILES sorter.cpp ${PROJECT_SOURCE_DIR}/include/sorter/sorter.hpp
        time_meter.cpp ${PROJECT_SOURCE_DIR}/include/sorter/time_meter.hpp)

add_library(Sorter ${SOURCE_FILES})