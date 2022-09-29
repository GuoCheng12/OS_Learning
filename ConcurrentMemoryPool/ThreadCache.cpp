//
// Created by 巫国诚 on 2022/9/28.
//
#include "Common.h"
#include "ThreadCache.h"

void* ThreadCache::allocate(size_t size) {
    assert(size <= MAX_BYTES);
    size_t allignSize = Sizeclass::RoundUp(size);
    size_t index = Sizeclass::Index(size);

}
void ThreadCache::deallocate(void *ptr, size_t size) {

}