//
// Created by 巫国诚 on 2022/9/28.
//
#include "ThreadCache.hpp"
inline void* ThreadCache::FetchFromCentralCache(size_t index, size_t size) {
    return nullptr;
}
inline void* ThreadCache::allocate(size_t size) { //
    assert(size <= MAX_BYTES);
    size_t allignSize = Sizeclass::RoundUp(size);
    size_t index = Sizeclass::Index(size);
    if(!_freeList[index].Empty()){ // 看是否为空，不为空就弹出一块空间地址出去
        return _freeList[index].Pop();
    }else{
        return FetchFromCentralCache(index,allignSize); //为空，就去Central Cache里面申请
    }

}
inline void ThreadCache::deallocate(void *ptr, size_t size) {
    assert(ptr);
    assert(size <= MAX_BYTES);
    size_t index = Sizeclass::Index(size);
    _freeList[index].Push(ptr); // 找出对应映射自由链表桶，
}
