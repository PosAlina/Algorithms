/**
 * Implementation of an abstract data type - a dynamic array.
 */

#ifndef QUICKSORT_DYNAMIC_ARRAY_HPP
#define QUICKSORT_DYNAMIC_ARRAY_HPP

#include <iostream>
#include <memory>
#include <utility>

#include "constants.hpp"

#define LAST_ARRAY_ELEMENT "It is last empty element of the array."

// Dynamic array for elements with type T
// Example:
//      Array<int> a;
//      for (int i = 0; i < 10; ++i) a.insert(i + 1);
//      for (int i = 0; i < a.size(); ++i) a[i] *= 2;
//      for (auto it = a.iterator(); it.hasNext(); it.next())
//          std::cout << it.get() << std::endl;
template <typename T>
class Array final {
    T *items;
    int _size;
    int _capacity;

// Methods
public:
    int size() const;
    int capacity() const;

    Array();
    Array(int);
    Array(const Array<T> &);

    ~Array();

    const T& operator [](int) const;
    T& operator [](int);
    Array<T> &operator =(const Array<T>&);

    void insert(const T&);
    void insert(int, const T&);
    void remove(int);

private:
    bool increase(int);

// Auxiliary Class
public:
    class Iterator {
        Array<T> *dynamic_array;
        int _index;
    public:
        Iterator(Array<T> *dynamic_array, int index = 0)
        :dynamic_array(dynamic_array), _index(index) {}

        Iterator(const Iterator &iterator2)
        :_index(iterator2._index), dynamic_array(iterator2.dynamic_array){}

        Iterator &operator =(const Iterator& iterator2) {
            if (&iterator2 == this) return *this;
            dynamic_array = iterator2.dynamic_array;
            _index = iterator2._index;
            return *this;
        }

        const T& get() const;
        void set(const T&);
        void insert(const T&);
        void remove();
        bool hasNext() const;
        bool hasPrev() const;
        void next();
        void prev();
        void toIndex(int);
    };

    Iterator iterator() {return Iterator(this);}
    const Iterator iterator() const {return Iterator(this);}
};

/*
 * GETTERS
 */

// Returns the current_chunk size
// (the number of elements that actually exist in the array).
/// \tparam T - type of elements of the array
/// \return - current_chunk size
template<typename T>
int Array<T>::size() const {return _size;}

// Returns the capacity (size of the allocated memory).
/// \tparam T - type of elements of the array
/// \return - current_chunk capacity
template<typename T>
int Array<T>::capacity() const {return _capacity;}

/*
 * CONSTRUCTORS
 */

// The constructor without parameters
// allocates the memory needed to store a certain number of elements,
// using the value default _capacity
// (const_array::array_capacity).
/// \tparam T  - type of elements of the array
template<typename T>
Array<T>::Array()
:_size(0), _capacity(const_array::array_capacity) {
    items = (T *)malloc(_capacity * sizeof(T));
    if (items == nullptr) throw std::runtime_error("Memory allocation error.");
}

// The constructor with the _capacity parameter
// allocates the memory needed to store a certain number of elements,
// using an explicitly passed value.
/// \tparam T  - type of elements of the array
/// \param capacity - size of allocated memory for
template<typename T>
Array<T>::Array(int capacity)
:_size(0), _capacity(capacity > 0 ? capacity : const_array::array_capacity) {
    items = (T *)malloc(_capacity * sizeof(T));
    if (items == nullptr) throw std::runtime_error("Memory allocation error.");
}

// Copy constructor
/// \tparam T  - type of elements of the array
/// \param dynamic_array - object to copy
template<typename T>
Array<T>::Array(const Array<T> &dynamic_array)
:_size(dynamic_array.size()), _capacity(dynamic_array.capacity()) {
    items = (T *)malloc(dynamic_array.capacity() * sizeof(T));
    if (items == nullptr) throw std::runtime_error("Memory allocation error.");
    for (auto i = 0; i < dynamic_array.size(); i++)
        new (&items[i]) T(dynamic_array[i]);
}

/*
 * DESTRUCTOR
 */

// The destructor frees the memory allocated for storing elements.
// If necessary, when freeing memory, destructors of stored elements are called.
// unique_ptr - frees the data itself
/// \tparam T - type of elements of the array
template<typename T>
Array<T>::~Array() {
    for (auto i = 0; i < _size; i++) items[i].~T();
    free(items);
}

/*
 * OPERATORS
 */

// The indexing operator allows you to access an array element by index
// for reading.
/// \tparam T - type of elements of the array
/// \param index - index of the selected array element
/// \return - unchanged selected element
template<typename T>
const T& Array<T>::operator [](int index) const {
    if ((index < 0) || (index >= _size))
        throw std::invalid_argument("Error index");
    return items[index];
}

// The indexing operator allows you to access an array element by index
// for writing.
/// \tparam T - type of elements of the array
/// \param index - index of the selected array element
/// \return - changed selected element
template<typename T>
T& Array<T>::operator [](int index) {
    if ((index < 0) || (index >= _size))
        throw std::invalid_argument("Error index");
    return items[index];
}

// Copy assignment operator
/// \tparam T  - type of elements of the array
/// \param dynamic_array - object to copy
template<typename T>
Array<T>& Array<T>::operator =(const Array<T> &dynamic_array) {
    if (&dynamic_array == this) return *this;
    auto new_items = (T *)malloc(dynamic_array.capacity() * sizeof(T));
    if (new_items == nullptr) return *this;
    for (auto i = 0; i < dynamic_array.size(); i++)
        new (&new_items[i]) T(dynamic_array[i]);
    for (auto i = 0; i < _size; i++) items[i].~T();
    free(items);
    items = new_items;
    _size = dynamic_array.size();
    _capacity = dynamic_array.capacity();
    return *this;
}

/*
 * INSERT
 */

// Inserts the passed value at the end of the array.
/// \tparam T - type of elements of the array
/// \param value - value of the new array element to change
template<typename T>
void Array<T>::insert(const T& value) {insert(_size, value);}

// Inserts the passed value at the specified position,
// increasing the array size by 1 and,
// if necessary, shifting existing elements to the right.
// If there is not enough memory to add an element,
// it reallocates memory by copying existing elements
// to a new area using move semantics.
// Then the old block is released.
// Memory allocation occurs each time with an increase of 2 times
// relative to the current_chunk size.
/// \tparam T - type of elements of the array
/// \param index - index of the array element to change
/// \param value - new value of the array element to change
template<typename T>
void Array<T>::insert(int index, const T& value) {
    if ((index < 0) || (index > _size)) return;
    if (_size != _capacity)
        for (auto i = _size; i > index; i--) items[i] = std::move(items[i - 1]);
    else if (!increase(index)) return;
    new (&items[index]) T(value);
    _size++;
}

// The function doubles the memory for the array
// and allocates empty space for a new element at the index.
/// \tparam T - type of elements of the array
/// \param index - index of the array element to change
/// \return - successful or not memory allocation
template<typename T>
bool Array<T>::increase(int index) {
    auto new_items = (T *)malloc(_capacity * 2 * sizeof(T));
    if (new_items == nullptr) return false;
    for (auto i = 0; i < index; i++) new_items[i] = std::move(items[i]);
    for (auto i = index; i < _size; i++) new_items[i + 1] = std::move(items[i]);
    free(items);
    items = new_items;
    _capacity *= 2;
    return true;
}

/*
 * REMOVE
 */

// Deletes an element from the specified array position,
// shifting the remaining elements to the left
// (memory is not freed).
/// \tparam T - type of elements of the array
/// \param index - index of the array element to remove
template<typename T>
void Array<T>::remove(int index) {
    if ((index >= _size) || (index < 0)) return;
    items[index].~T();
    for (auto i = index + 1; i < _size; i++) items[i - 1] = std::move(items[i]);
    _size--;
}

/*
 * Methods of Iterator
 */

template<typename T>
const T &Array<T>::Iterator::get() const {return (*dynamic_array)[_index];}

template<typename T>
void Array<T>::Iterator::set(const T& value) {(*dynamic_array)[_index] = value;}

template<typename T>
void Array<T>::Iterator::insert(const T& value) {
    dynamic_array->insert(_index, value);
}

template<typename T>
void Array<T>::Iterator::remove() {dynamic_array->remove(_index);}

template<typename T>
bool Array<T>::Iterator::hasNext() const {
    return _index < (dynamic_array->size() - 1);
}

template<typename T>
bool Array<T>::Iterator::hasPrev() const {return _index > 0;}

template<typename T>
void Array<T>::Iterator::next() {if (hasNext()) _index++;}

template<typename T>
void Array<T>::Iterator::prev() {if (hasPrev()) _index--;}

template<typename T>
void Array<T>::Iterator::toIndex(int index) {
    if ((index >= 0) && (index < dynamic_array->size())) _index = index;
}

#endif //QUICKSORT_DYNAMIC_ARRAY_HPP
