/**
 * All numeric constants from TimeMeter and Sorter.
 */

#ifndef QUICKSORT_CONSTANTS_HPP
#define QUICKSORT_CONSTANTS_HPP

#define LESS(T) [](T a, T b) {return a < b;}
#define GREATER(T) [](T a, T b) {return a > b;}
#define LESS_OR_EQUAL(T) [](T a, T b) {return a <= b;}
#define GREATER_OR_EQUAL(T) [](T a, T b) {return a >= b;}

#define EMPTY_ARRAY_MESSAGE ""
#define ILLEGAL_ARG_ARRAY_EXC_MESSAGE "Error in setting the array\n"
#define ILLEGAL_ARG_COMP_EXC_MESSAGE "Error in compare for this type\n"
#define NULLPTR_EXC_START_MESSAGE "Empty pointer instead array`s beginning\n"
#define NULLPTR_EXC_LAST_MESSAGE "Empty pointer instead array`s end\n"
#define UNEXPECTED_MES "Unexpected error "

namespace constants {
    namespace sorter {
        const auto insert_len(14);
    }
    namespace time_meter {
        const auto experiment_count_default(3);
        const auto experiment_count_limit(50);
        const auto result_default(10);
    }
}

namespace const_sort = constants::sorter;
namespace const_time_meter = constants::time_meter;

#endif //QUICKSORT_CONSTANTS_HPP
