/**
 * Selects the optimal length of the interval
 * in which the sorter should use insertion sorting.
 */

#ifndef QUICKSORT_TIME_METER_HPP
#define QUICKSORT_TIME_METER_HPP

#include "constants.hpp"
#include "sorter.hpp"

#define EXPERIMENT_COUNT_HIGH_LIMIT_MESSAGE "Too many experiments\n"
#define EXPERIMENT_COUNT_LOW_LIMIT_MESSAGE "Not enough experiments\n"
#define UNEXPECTED_EXP_MES "Unexpected error in experiment "

// Performs experiments on selecting the length of the interval
// in which the insertion sort takes place for the class Sorter.
// Example:
//      int experiment_count = 10;
//      TimeMeter time_meter(experiment_count);
//      int optimal_length = time_meter.experiment_with_array_count();
//      Sorter sorter(optimal_length);
class TimeMeter {
    int experiment_count;
public:
    explicit TimeMeter(int experiment_count = 3)
    :experiment_count(experiment_count) {}

    int experiment_with_array_count();
    void print_first_comparings(int) const;
private:
    bool is_for_insertion_sort(int) const;
};


#endif //QUICKSORT_TIME_METER_HPP
