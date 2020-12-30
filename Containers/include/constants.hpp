/**
 * All numeric constant for dynamic array and linked list.
 */

#ifndef ATD_CONSTANTS_HPP
#define ATD_CONSTANTS_HPP

namespace constants {
    namespace dynamic_array {
        const auto array_capacity(8);
    }
    namespace linked_list {
        // 64 - 2 * sizeof(void *)- sizeof(int))
        // 64 - 2 * 8 - 4
        const auto default_chunk_capacity(44);
        template<typename T>
        const int chunk_capacity((default_chunk_capacity >= (2 * sizeof(T))) ?
        (default_chunk_capacity / sizeof(T)) : 2);
    }
}

namespace const_array = constants::dynamic_array;
namespace const_list = constants::linked_list;

#endif //ATD_CONSTANTS_HPP
