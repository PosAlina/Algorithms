/**
 * The universal memory manager
 * whose virtual methods will be overridden in inheritors of this class.
 */

#ifndef MEMORY_MANAGEMENT_MEMORY_ALLOCATOR_HPP
#define MEMORY_MANAGEMENT_MEMORY_ALLOCATOR_HPP

#include <iostream>

#include "FSA.hpp"
#include "CoalesceAllocator.hpp"

// Example:
//      MemoryAllocator allocator;
//      allocator.init();
//      int *pi = allocator.alloc( sizeof ( int ));
//      double *pd = allocator.alloc( sizeof ( double ));
//      int *pa = allocator.alloc(10 * sizeof ( int ));
//      allocator.dumpStat();
//      allocator.dumpBlocks();
//      allocator.free(pa);
//      allocator.free(pd);
//      allocator.free(pi);
//      allocator.destroy();
class MemoryAllocator {
    FSA allocator16;
    FSA allocator32;
    FSA allocator64;
    FSA allocator128;
    FSA allocator256;
    FSA allocator512;
    CoalesceAllocator coalesce_allocator;

#ifdef _DEBUG
    bool initialization_occurred;
    bool deinitialization_occurred;
#endif //DEBUG
public:
    MemoryAllocator();
    virtual ~MemoryAllocator();
    virtual void init();
    virtual void destroy();
    virtual void *alloc(size_t);
    virtual void free(void *);
    virtual void dumpStat() const;
    virtual void dumpBlocks() const;
private:
    virtual void *set_service_block();
};

#endif //MEMORY_MANAGEMENT_MEMORY_ALLOCATOR_HPP