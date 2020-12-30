/**
 * Selects the optimal length of the interval
 * in which the sorter should use insertion sorting
 * (using the array int example).
 */

#include <chrono>
#include <stdexcept>
#include <random>
#include <windows.h>

#include "sorter/time_meter.hpp"
#include "sorter/sorter.hpp"
#include "constants.hpp"

std::mt19937 mersenne(static_cast<int>(time(0)));

// The function calculates the average interval length from several experiments.
/// \return the optimal length of the interval
/// in which the sorter should use insertion sorting.
int TimeMeter::experiment_with_array_count() {
    try {
        int optimal_length = 2;
        while(true) {
            if (!is_for_insertion_sort(optimal_length)) return optimal_length - 1;
            optimal_length++;
        }
    }
    catch(std::exception &ex) {
        std::cerr << UNEXPECTED_EXP_MES << ex.what() << std::endl;
    }
    return const_time_meter::result_default;
}

// The function determine this size of arrays
// is more suitable for sorting цшер штыукешщты.
/// \param size - size of the array
/// \return - an array of this size is sorted faster by insertions
bool TimeMeter::is_for_insertion_sort(int size) const {
    int array[size];
    for (auto i = 0; i < size; i++) array[i] = mersenne();
    Sorter sorter(0);
    sorter.sort(array, array + size, LESS(int));
    double quick = 0, insert = 0;
    for (auto i = 0; i < experiment_count; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        sorter.simple_quicksort(array, array + size - 1, GREATER(int));
        auto end = std::chrono::high_resolution_clock::now();
        quick += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        sorter.simple_insertion_sort(array, array + size - 1, LESS(int));
        end = std::chrono::high_resolution_clock::now();
        insert += std::chrono::duration<double>(end - start).count();
    }
    std::cout << "Size: " << size << " Time of quicksort: " << quick << " Time of insertion sort: " << insert << std::endl;
    return insert <= quick;
}

void TimeMeter::print_first_comparings(int count) const {
    for (auto size = 2; size < count + 2; size++) {
        int array[size];
        for (auto i = 0; i < size; i++) array[i] = mersenne();
        Sorter sorter(0);
        sorter.sort(array, array + size, LESS(int));
        double quick = 0, insert = 0;
        for (auto i = 0; i < experiment_count; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            sorter.simple_quicksort(array, array + size - 1, GREATER(int));
            auto end = std::chrono::high_resolution_clock::now();
            quick += std::chrono::duration<double>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            sorter.simple_insertion_sort(array, array + size - 1, LESS(int));
            end = std::chrono::high_resolution_clock::now();
            insert += std::chrono::duration<double>(end - start).count();
        }
        std::cout << "Size: " << size << " Time of quicksort: " << quick << " Time of insertion sort: " << insert
                  << std::endl;
    }
}

