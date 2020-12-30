/**
 * Implementation of an abstract data type -
 * a connected double linked list with chunks.
 */

#ifndef QUICKSORT_LINKED_LIST_HPP
#define QUICKSORT_LINKED_LIST_HPP

#include <iostream>
#include <memory>
#include <utility>

#include "constants.hpp"
#include "dynamic_array/dynamic_array.hpp"

#define EMPTY_HEAD "Head is empty"
#define EMPTY_TAIL "Tail is empty"
#define LAST_LIST_ELEMENT "It is last empty element of the list."

// Double linked list for elements with type T
// Example:
//      List<int> l;
//      for (int i = 0; i < 10; ++i) l.insertHead(i + 1);
//      for (auto it = l.iterator(); it.hasNext(); it.next())
//        std::cout << it.get() << std::endl;
template <typename T>
class List final {
// Auxiliary classes
    struct Chunk final {
        T *items;
        int size;
        Chunk *prev, *next;

        Chunk()
        : size(0), prev(nullptr), next(nullptr) {
            items = (T *)malloc(const_list::chunk_capacity<T> * sizeof(T));
            if (items == nullptr)
                throw std::runtime_error("Memory allocation error.");
        }
        ~Chunk() {
            for (auto i = 0; i < size; i++) items[i].~T();
            free(items);
        }
    };
public:
    class Iterator {
        List<T> *linked_list;
        Chunk *current_chunk;
        int chunk_index;
    public:
        Iterator(List<T> *linked_list)
        : linked_list(linked_list), current_chunk(linked_list->ptr_head),
        chunk_index(0) {}

        Iterator(const Iterator &iterator2)
        : linked_list(iterator2.linked_list),
        current_chunk(iterator2.current_chunk),
        chunk_index(iterator2.chunk_index) {}

        Iterator &operator =(const Iterator& iterator2) {
            if (&iterator2 == this) return *this;
            linked_list = iterator2.linked_list;
            current_chunk = iterator2.current_chunk;
            chunk_index = iterator2.chunk_index;
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
    private:
        void addNewChunk();
        void removeChunk();
    };

// Data
private:
    Chunk *ptr_head, *ptr_tail;
    int _size;

// Methods
public:
    int size() const;
    const T& head() const;
    const T& tail() const;

    List();
    List(const List<T> &);
    ~List();

    List<T> &operator =(const List<T>&);

    void insertHead(const T&);
    void insertTail(const T&);
    void removeHead();
    void removeTail();

    Iterator iterator() {return Iterator(this);}
    const Iterator iterator() const {return Iterator(this);}
private:
    Chunk *createChunk(Chunk *new_chunk = nullptr, Chunk *prev = nullptr,
                       Chunk *next = nullptr) {
        auto current_chunk = (Chunk *) malloc(sizeof(Chunk));
        if (current_chunk == nullptr) return nullptr;
        if (new_chunk == nullptr) {
            current_chunk->items =
                    (T *)malloc(const_list::chunk_capacity<T> * sizeof(T));
            if (current_chunk->items == nullptr) return nullptr;
            current_chunk->size = 0;
        }
        else {
            current_chunk->items = (T *)malloc(new_chunk->size * sizeof(T));
            if (current_chunk->items == nullptr) return nullptr;
            current_chunk->size = new_chunk->size;
            for (auto i = 0; i < current_chunk->size; i++)
                new(&current_chunk->items[i]) T(new_chunk->items[i]);
        }
        current_chunk->prev = prev;
        current_chunk->next = next;
        return current_chunk;
    }
    void removeList(Chunk *head);
};

/*
 * GETTERS
 */

// Returns the current_chunk size
/// \tparam T - type of elements of the list
/// \return - current_chunk size
template<typename T>
int List<T>::size() const {return _size;}

// Returns the current_chunk value in the head of the list.
/// \tparam T - type of elements of the list
/// \return - current_chunk value in the head of the list
template<typename T>
const T& List<T>::head() const {
    if (_size != 0) return ptr_head->items[0];
    throw std::out_of_range(EMPTY_HEAD);
}

// Returns the current_chunk value in the tail of the list.
/// \tparam T - type of elements of the list
/// \return - current_chunk value in the tail of the list
template<typename T>
const T& List<T>::tail() const {
    if (_size != 0) return ptr_tail->items[ptr_tail->size - 1];
    throw std::out_of_range(EMPTY_TAIL);
}

/*
 * CONSTRUCTORS
 */

// The constructor without parameters
// allocates the memory needed to store 1 chunk.
/// \tparam T  - type of elements of the list
template<typename T>
List<T>::List()
:_size(0) {
    ptr_head = createChunk();
    if (ptr_head == nullptr)
        throw std::runtime_error("Memory allocation error.");
    ptr_tail = ptr_head;
}

// Copy constructor
/// \tparam T  - type of elements of the list
/// \param linked_list - object to copy
template<typename T>
List<T>::List(const List<T> &linked_list)
:_size(linked_list._size){
    ptr_head = createChunk(linked_list.ptr_head);
    if (ptr_head == nullptr)
        throw std::runtime_error("Memory allocation error.");
    ptr_tail = ptr_head;

    auto new_chunk = (linked_list.ptr_head)->next;
    while (new_chunk != nullptr) {
        auto new_tail = createChunk(new_chunk, ptr_tail);
        if (new_tail == nullptr) {
            removeList(ptr_head);
            throw std::runtime_error("Memory allocation error.");
        }
        ptr_tail->next = new_tail;
        ptr_tail = new_tail;
        new_chunk = new_chunk->next;
    }
}

/*
 * DESTRUCTOR
 */

// The destructor frees the memory allocated for storing elements.
// If necessary, when freeing memory, destructors of stored elements are called.
// unique_ptr - frees the data itself
/// \tparam T - type of elements of the array
template<typename T>
List<T>::~List() {//removeList(ptr_head);
    auto current_chunk = ptr_tail;
    while (current_chunk != nullptr) {
        auto next_chunk = current_chunk->prev;
        current_chunk->~Chunk();
        free(current_chunk);
        current_chunk = next_chunk;
    }
}

template<typename T>
void List<T>::removeList(Chunk *head) {
    auto current_chunk = head;
    while (current_chunk != nullptr) {
        auto next_chunk = current_chunk->next;
        current_chunk->~Chunk();
        free(current_chunk);
        current_chunk = next_chunk;
    }
}

/*
 * OPERATORS
 */

// Copy assignment operator
/// \tparam T  - type of elements of the list
/// \param linked_list - object to copy
template<typename T>
List<T>& List<T>::operator =(const List<T> &linked_list) {
    if (&linked_list == this) return *this;
    auto new_head = createChunk(linked_list.ptr_head);
    if (new_head == nullptr) return *this;
    auto new_tail = new_head;
    auto other_chunk = (linked_list.ptr_head)->next;
    while (other_chunk != nullptr) {
        auto new_chunk = createChunk(other_chunk, new_tail);
        if (new_chunk == nullptr) {
            removeList(new_head);
            return *this;
        }
        new_tail->next = new_chunk;
        new_tail = new_chunk;
        other_chunk = other_chunk->next;
    }
    auto current_chunk = ptr_head;
    while (current_chunk != nullptr) {
        auto next_chunk = current_chunk->next;
        current_chunk->~Chunk();
        free(current_chunk);
        current_chunk = next_chunk;
    }
    _size = linked_list._size;
    ptr_head = new_head;
    ptr_tail = new_tail;
    return *this;
}

/*
 * INSERT
 */

// Inserts the passed value at the head of the list.
/// \tparam T - type of elements of the list
/// \param value - value of the new list head element
template<typename T>
void List<T>::insertHead(const T &value) {
    if (ptr_head->size == const_list::chunk_capacity<T>) {
        auto current_chunk = createChunk(nullptr, nullptr, ptr_head);
        if (current_chunk == nullptr) return;
        ptr_head->prev = current_chunk;
        ptr_head = current_chunk;
    }
    else
        for (auto i = ptr_head->size; i > 0; i--)
            ptr_head->items[i] = std::move(ptr_head->items[i - 1]);
    new (&ptr_head->items[0]) T(value);
    ptr_head->size++;
    _size++;
}

// Inserts the passed value at the tail of the list.
/// \tparam T - type of elements of the list
/// \param value - value of the new list tail element
template<typename T>
void List<T>::insertTail(const T &value) {
    if (ptr_tail->size == const_list::chunk_capacity<T>) {
        auto current_chunk = createChunk(nullptr, ptr_tail);
        if (current_chunk == nullptr) return;
        ptr_tail->next = current_chunk;
        ptr_tail = current_chunk;
    }
    new (&ptr_tail->items[ptr_tail->size]) T(value);
    ptr_tail->size++;
    _size++;
}

/*
 * REMOVE
 */

// Remove value at the head of the list.
/// \tparam T - type of elements of the list
template<typename T>
void List<T>::removeHead() {
    if (ptr_head->size == 0) return;
    ptr_head->items[0].~T();
    for (auto i = 0; i < (ptr_head->size - 1); i++)
        ptr_head->items[i] = std::move(ptr_head->items[i + 1]);
    ptr_head->size--;
    _size--;
    if ((ptr_head->size == 0) && (_size > 0)) {
        auto new_head = ptr_head->next;
        new_head->prev = nullptr;
        free(ptr_head);
        ptr_head = new_head;
    }
}

// Remove value at the tail of the list.
/// \tparam T - type of elements of the list
template<typename T>
void List<T>::removeTail() {
    if (ptr_tail->size == 0) return;
    ptr_tail->items[ptr_tail->size - 1].~T();
    ptr_tail->size--;
    _size--;
    if ((ptr_tail->size == 0) && (_size > 0)) {
        auto new_tail = ptr_tail->prev;
        new_tail->next = nullptr;
        free(ptr_tail);
        ptr_tail = new_tail;
    }
}

/*
 * Methods of Iterator
 */

template<typename T>
const T &List<T>::Iterator::get() const {
    return current_chunk->items[chunk_index];
}

template<typename T>
void List<T>::Iterator::set(const T& value) {
    new (&current_chunk->items[chunk_index]) T(value);
}

template<typename T>
void List<T>::Iterator::insert(const T& value) {
    if (current_chunk->size == const_list::chunk_capacity<T>) addNewChunk();
    else
        for (auto i = current_chunk->size; i > chunk_index; i--)
            current_chunk->items[i] = std::move(current_chunk->items[i - 1]);
    new (&current_chunk->items[chunk_index]) T(value);
    current_chunk->size++;
    linked_list->_size++;
}

template<typename T>
void List<T>::Iterator::addNewChunk() {
    auto new_chunk = linked_list->createChunk(
            nullptr, current_chunk, current_chunk->next);
    if (new_chunk == nullptr) return;
    if (current_chunk->next == nullptr) linked_list->ptr_tail = new_chunk;
    else (current_chunk->next)->prev = new_chunk;
    current_chunk->next = new_chunk;
    for (auto i = chunk_index; i < current_chunk->size; i++)
        new_chunk->items[i - chunk_index] = std::move(current_chunk->items[i]);
    new_chunk->size = current_chunk->size - chunk_index;
    current_chunk->size = chunk_index;
}

template<typename T>
void List<T>::Iterator::remove()  {
    if ((current_chunk->size == 0) || (chunk_index == current_chunk->size)) return;
    current_chunk->items[chunk_index].~T();
    for (auto i = chunk_index; i < (current_chunk->size - 1); i++)
        current_chunk->items[i] = std::move(current_chunk->items[i + 1]);
    current_chunk->size--;
    linked_list->_size--;
    if ((current_chunk->size == 0) && (linked_list->_size > 0)) removeChunk();
}

template<typename T>
void List<T>::Iterator::removeChunk() {
    if (current_chunk->next != nullptr)
        (current_chunk->next)->prev = current_chunk->prev;
    else linked_list->ptr_tail = current_chunk->prev;
    if (current_chunk->prev != nullptr)
        (current_chunk->prev)->next = current_chunk->next;
    else linked_list->ptr_head = current_chunk->next;

    Chunk *new_current_chunk;
    if (current_chunk->next != nullptr) {
        new_current_chunk = current_chunk->next;
        chunk_index = 0;
    }
    else {
        new_current_chunk = current_chunk->prev;
        chunk_index = new_current_chunk->size - 1;
    }
    free(current_chunk);
    current_chunk = new_current_chunk;
}

template<typename T>
bool List<T>::Iterator::hasNext() const {
    return (current_chunk->next != nullptr) || (chunk_index < (current_chunk->size - 1));
}

template<typename T>
bool List<T>::Iterator::hasPrev() const {
    return (current_chunk->prev != nullptr) || (chunk_index > 0);
}

template<typename T>
void List<T>::Iterator::next() {
    if (!hasNext()) return;
    if (chunk_index < (current_chunk->size - 1)) chunk_index++;
    else {
        current_chunk = current_chunk->next;
        chunk_index = 0;
    }
}

template<typename T>
void List<T>::Iterator::prev() {
    if (!hasPrev()) return;
    if (chunk_index > 0) chunk_index--;
    else {
        current_chunk = current_chunk->prev;
        chunk_index = current_chunk->size - 1;
    }
}

#endif //QUICKSORT_LINKED_LIST_HPP
