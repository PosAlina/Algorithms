# build service
set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/include/constants.hpp
        linked_list.cpp ${PROJECT_SOURCE_DIR}/include/linked_list/linked_list.hpp)

add_library(LinkedList ${SOURCE_FILES})