/**
 * Sorting an template array
 * using a combination of quick and insertion sorting algorithms.
 */

#ifndef QUICKSORT_SORTER_HPP
#define QUICKSORT_SORTER_HPP

#include <stdexcept>
#include <iostream>
#include <stack>

#include "constants.hpp"

// Sorting an template array
// using a combination of recursive and iterative fast sorting algorithms
// for a large number of data and insertion sorting for a small number.
// Example:
//      int array[] = {7, 4, 1, 5};
//      Sorter sorter;
//      sorter.sort(array, array + 4, [](int a, int b) {return a < b;});
class Sorter {
    // insert_len is default
    // the class TimeMeter can help you choose length
    int short_interval_max_length;
public:
    explicit Sorter(int short_interval_init_length =
            const_sort::insert_len)
    : short_interval_max_length(short_interval_init_length) {}

    template<typename T, typename Compare> void sort(T *, T *, Compare);
    template<typename T> void print(T *, T *) const;

    //Selection
    template<typename T, typename Compare> void simple_quicksort(T *, T *, Compare);
    template<typename T, typename Compare> void simple_insertion_sort(T *, T *, Compare);
private:

    template<typename T, typename Compare> void quicksort(T *, T *, Compare);
    template<typename T, typename Compare>
        void insertion_sort(T *, T *, Compare);

    template<typename T, typename Compare> T select_pivot(T *, T *, Compare);
    template<typename T, typename Compare>
        T *partition(T *&, T *&, T, Compare comp);

    template<typename T> void swap(T *, T *);
};

// The function sends the array to the appropriate sorting for it:
// quick sort or insertion sort.
/// \tparam T - type of array elements
/// \tparam Compare - type of predicat
/// \param first - pointer to the beginning of the array
/// \param last - pointer to an element after the end of the array
/// \param comp - the comparison predicate for the specified types
template<typename T, typename Compare>
void Sorter::sort(T *first, T *last, Compare comp) {
    try {
        if ((last - first) <= 1) return;
        last--;
        quicksort(first, last, comp);
    }
    catch(std::exception &ex) {
        std::cerr << UNEXPECTED_MES << ex.what() << std::endl;
    }
}

// The function that prints an array
/// \tparam T - type of array elements
/// \param first - pointer to the beginning of the array
/// \param last -  pointer to an element after the end of the array
template<typename T>
void Sorter::print(T *first, T *last) const {
    if ((first == nullptr) || (last == nullptr)) return;
    for (auto i = first; i < last; i++) std::cout << *i << " ";
    std::cout << std::endl;
}

// The function sends the array to the first step of quick sort processing
// with the selected pivot,
// divides the array depending on the length of the resulting intervals,
// and sends it for processing either by insertion sorting for a small length,
// iterative for a greater long interval
// or recursive quick sort for a smaller long interval.
/// \tparam T - type of array elements
/// \tparam Compare - type of predicat
/// \param first - pointer to the beginning of the array
/// \param last - pointer to the last element of the array
/// \param comp - the comparison predicate for the specified types
template<typename T, typename Compare>
void Sorter::quicksort(T *first, T *last, const Compare comp) {
    while ((last - first) > short_interval_max_length) {
        auto border = partition(first, last,
                                select_pivot(first, last, comp), comp);
        auto first_length = border - first, second_length = last - (border + 1);
        if (first_length <= second_length) {
            quicksort(first, border, comp);
            first = border + 1;
        }
        else {
            quicksort(border + 1, last, comp);
            last = border;
        }
    }
    insertion_sort(first, last, comp);
}

// The function sorts the array by inserts
/// \tparam T - type of array elements
/// \tparam Compare - type of predicat
/// \param first - pointer to the beginning of the array
/// \param last - pointer to the last element of the array
/// \param comp - the comparison predicate for the specified types
template<typename T, typename Compare>
void Sorter::insertion_sort(T *first, T *last, const Compare comp) {
    for (auto right = first + 1; right <= last; right++) {
        T element = *right;
        T *current = right;
        while ((current > first) && (comp(element, *(current - 1)))) {
            *current = *(current - 1);
            current--;
        }
        *current = element;
    }
}

// The function finds pivot,
// in this case the median between
// the first, last, and middle elements of the array.
/// \tparam T - type of array elements
/// \tparam Compare - type of predicat
/// \param first - pointer to the beginning of the array
/// \param last - pointer to the last element of the array
/// \param comp - the unchanged comparison predicate for the specified types
/// \return T - value of pivot
template<typename T, typename Compare>
T Sorter::select_pivot(T *first, T *last, const Compare comp) {
    return *(comp(*first, *last)?
        (comp(*first, *(first + (last - first) / 2))?
            (first + (last - first) / 2):first):
        (comp(*last, *(first + (last - first) / 2))?
            (first + (last - first) / 2):last));
}

// The function rearranges all elements in the array at the selected interval,
// less than the pivot - [first; border], more - [border; last].
/// \tparam T - type of array elements
/// \tparam Compare - type of predicat
/// \param first - reference to the pointer to the beginning of the array
/// \param last - reference to the pointer to the last element of the array
/// \param pivot - unchanged value of pivot for this interval of the array
/// \param comp - the unchanged comparison predicate for the specified types
/// \return - reference to the pointer to the border element of the array
template<typename T, typename Compare>
T *Sorter::partition(T *&first, T *&last, const T pivot, const Compare comp) {
    auto left = first, right = last;
    while (true) {
        while (comp(*left, pivot)) left++;
        while (comp(pivot, *right)) right--;
        if (left >= right) return right;
        swap(left, right);
        left++;
        right--;
    }
}

// The function swaps the values of two variables stored at these addresses.
/// \tparam T - type of elements
/// \param first - pointer to the first element
/// \param second - pointer to the second element
template<typename T>
void Sorter::swap(T *first, T *second) {
    T temp = *first;
    *first = *second;
    *second = temp;
}

template<typename T, typename Compare>
void Sorter::simple_quicksort(T *first, T *last, const Compare comp) {
    while (first < last) {
        auto border = partition(first, last,
                                select_pivot(first, last, comp), comp);
        auto first_length = border - first, second_length = last - (border + 1);
        if (first_length <= second_length) {
            simple_quicksort(first, border, comp);
            first = border + 1;
        }
        else {
            simple_quicksort(border + 1, last, comp);
            last = border;
        }
    }
}

template<typename T, typename Compare>
void Sorter::simple_insertion_sort(T *first, T *last, const Compare comp) {
    for (auto right = first + 1; right <= last; right++) {
        T element = *right;
        T *current = right;
        while (current > first && comp(element, *(current - 1))) {
            *current = *(current - 1);
            current--;
        }
        *current = element;
    }
}

#endif //QUICKSORT_SORTER_HPP