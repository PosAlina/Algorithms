/**
 * Use the command line to pass the string "LESS" or "GREATER",
 * followed by an array separated by spaces.
**/

#include <cstring>
#include <memory>

#include <sorter/sorter.hpp>
#include "sorter/time_meter.hpp"

int main(int argc, char **argv) {
    //TimeMeter time_meter(100000);
    //std::cout << time_meter.experiment_with_array_count() <<std::endl;
    //time_meter.print_first_comparings(30);
    //Size: 2 Time of quicksort: 0.0086102 Time of insertion sort: 0.0066583
    //Size: 3 Time of quicksort: 0.0092803 Time of insertion sort: 0.0065906
    //Size: 4 Time of quicksort: 0.0124727 Time of insertion sort: 0.0081147
    //Size: 5 Time of quicksort: 0.0166955 Time of insertion sort: 0.0109072
    //Size: 6 Time of quicksort: 0.019126 Time of insertion sort: 0.0126339
    //Size: 7 Time of quicksort: 0.0191871 Time of insertion sort: 0.0135917
    //Size: 8 Time of quicksort: 0.0241651 Time of insertion sort: 0.0170774
    //Size: 9 Time of quicksort: 0.0248125 Time of insertion sort: 0.0185692
    //Size: 10 Time of quicksort: 0.02706 Time of insertion sort: 0.0214239
    //Size: 11 Time of quicksort: 0.029049 Time of insertion sort: 0.0242625
    //Size: 12 Time of quicksort: 0.0323295 Time of insertion sort: 0.0286301
    //Size: 13 Time of quicksort: 0.0340963 Time of insertion sort: 0.0316434
    //Size: 14 Time of quicksort: 0.0389185 Time of insertion sort: 0.0387897
    //Size: 15 Time of quicksort: 0.0393646 Time of insertion sort: 0.0425735
    //14 - SIZE FOR INSERTION SORT
    auto first_argc_index = ((argc > 1) &&
            ((strcmp("LESS", argv[1]) == 0) ||
            (strcmp("GREATER", argv[1]) == 0))) ? 2 : 1;
    std::shared_ptr<int[]> a(new int[argc - first_argc_index]);
    for (auto i = first_argc_index; i < argc; i++) {
        std::size_t pos;
        a[i - first_argc_index] = std::stoi(std::string(argv[i]), &pos);
    }
    Sorter sorter;
    std::cout << "Entered array: ";
    sorter.print(a.get(), a.get() + argc - first_argc_index);
    if (first_argc_index == 2)
        sorter.sort(a.get(), a.get() + argc - first_argc_index,
                    (strcmp("LESS", argv[1]) == 0) ? LESS(int) : GREATER(int));
    else sorter.sort(a.get(), a.get() + argc - first_argc_index, LESS(int));
    std::cout << "Sorted array: ";
    sorter.print(a.get(), a.get() + argc - first_argc_index);
    return 0;
}