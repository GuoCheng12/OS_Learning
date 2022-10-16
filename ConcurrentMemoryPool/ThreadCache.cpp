//
// Created by 巫国诚 on 2022/9/28.
//
#include "ThreadCache.hpp"
inline void* ThreadCache::allocate(size_t size) {
    assert(size <= MAX_BYTES);
    size_t allignSize = Sizeclass::RoundUp(size);
    size_t index = Sizeclass::Index(size);
    if(!_freeList[index].Empty()){
        return _freeList[index].Pop();
    }else{
        return FetchFromCentralCache(index,allignSize);
    }

}
inline void ThreadCache::deallocate(void *ptr, size_t size) {
}
inline void* ThreadCache::FetchFromCentralCache(size_t index, size_t size) {

}