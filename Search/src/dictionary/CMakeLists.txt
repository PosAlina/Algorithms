# build service
set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/include/constants.hpp
        Dictionary.cpp ${PROJECT_SOURCE_DIR}/include/dictionary/Dictionary.hpp)

add_library(Dictionary ${SOURCE_FILES})