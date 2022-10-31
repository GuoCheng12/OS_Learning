//
// Created by 巫国诚 on 2022/9/28.
//
#include "ThreadCache.hpp"
#include "CentralCache.hpp"
inline void* ThreadCache::FetchFromCentralCache(size_t index, size_t size) {
    // 慢开始的反馈调节算法
    // 1.一开始不会向Central Cache一次性批量要太多内存,可能用不完
    // 2.如果一直都有size大小的内存申请需求，那么batchNum就会不断增长，直到上限
    // 3.size越大，一次向central cache要的batchNum就越小
    // 4.size越小，一次向central cache要的batchNum就越大
    void* start = nullptr;
    void* end = nullptr;
    size_t batchNum = std::min(_freeList[index].MaxSize(),Sizeclass::NumMoveSize(size));
    if(_freeList[index].MaxSize() == batchNum){
        _freeList[index].MaxSize() += 1;
    }
    size_t actualNum = CentralCache::GetInStance()->FetchRangeObj(start,end,batchNum,size);
    assert(actualNum > 1);
    if(actualNum == 1){
        assert(start == end);
        return start;
    }else{
        _freeList[index].PushRange(NextObj(start),end);
        return start;
    }


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
