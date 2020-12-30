//
// Created by pospali on 16.12.2020.
//

#ifndef MEMORYMANAGEMENT_COALESCEALLOCATOR_HPP
#define MEMORYMANAGEMENT_COALESCEALLOCATOR_HPP

class CoalesceAllocator {
//
public:
    CoalesceAllocator(size_t size);
    ~CoalesceAllocator();

    void init();
    void destroy();
    void* alloc(size_t size);
    virtual void free(void* p);
    virtual void dumpStat() const;
    virtual void dumpBlocks() const;
    byte* getDataPtr();
    void dumpCH(bool doSortByOffset = false) const;

private:
    struct Header {
        size_t id;
        size_t size;
        Header* prev;
        Header* next;
    };

    static size_t getOverheadBytes();
    static void* getUserPointer(const Header* hdr);
    static Header** getHeaderPointer(const Header* hdr);
    static Header* remove(Header* hdr);
    static void insertAfter(Header* after, Header* hdr);

    size_t nAlloc;
    size_t nFree;
    size_t size_;
    byte* data_;
    Header* freeList_;
};


#endif //MEMORYMANAGEMENT_COALESCEALLOCATOR_HPP
