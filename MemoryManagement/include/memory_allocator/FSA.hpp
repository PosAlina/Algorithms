/**
 * The specific memory manager with fixed size of memory
 * that inherits from a generic memory allocator.
 */

#ifndef MEMORY_MANAGEMENT_FSA_HPP
#define MEMORY_MANAGEMENT_FSA_HPP

#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <set>

using byte = unsigned char;

// Approach Fixed-size Memory Allocation.
class FSA {
    size_t blocks_number;
    size_t block_size;

//buffer
//buffer_size
public:
    FSA(size_t, size_t blocks_number = 1);
    //~FSA();
    // void init();
    // void destroy();
    void *alloc(size_t);
    //
public:
    ~FSA();

    void init();
    void destroy();
    virtual void free(void* p);
    virtual void dumpStat() const;
    virtual void dumpBlocks() const;

private:
    byte* getAddresOfBlock(size_t n);
    size_t getOrderOfBlockByAddres(byte* addr);
    size_t nAlloc;
    size_t nFree;
    std::set<size_t > allocBlocks_;
    size_t nUsed_;
    size_t nNextForAlloc;
    byte* pool_;
};

#endif //MEMORY_MANAGEMENT_FSA_HPP
