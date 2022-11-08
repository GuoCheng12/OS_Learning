//
// Created by 巫国诚 on 2022/10/26.
//
#include "CentralCache.hpp"
#include "PageCache.hpp"

CentralCache CentralCache::_sInst;

size_t CentralCache::FetchRangeObj(void *&start, void *&end, size_t batchNum, size_t size) {
    size_t index = Sizeclass::Index(size);
    _spanList[index]._mtx.lock();
    Span *span = GetOneSpan(_spanList[index], size);
    assert(span);
    assert(span->_freeList);
    // 从span中获取batchNum个对象
    // 如果不够batchNum个，有多少拿多少

    start = span->_freeList;
    end = start;
    size_t i = 0;
    size_t actualNum = 1;
    while (i < batchNum - 1 && NextObj(end) != nullptr) {
        end = NextObj(end);
        i++;
        actualNum++;
    }
    span->_freeList = NextObj(end);
    NextObj(end) = nullptr;
    span->_useCount += actualNum;
    _spanList[index]._mtx.unlock();
    return actualNum;
}

// 去Page Cache去拿Span
Span *CentralCache::GetOneSpan(SpanList &list, size_t byte_size) {
    // 查看当前的spanlist中是否还有未分配的span
    Span *it = list.begin();
    while (it != list.end()) {
        if (it->_freeList != nullptr) {
            return it;
        } else {
            it = it->_next;
        }
    }
    // 先把central cache的桶锁释放掉，这样别的线程还内存的时候不会堵塞
    list._mtx.unlock();
    PageCache::GetInstance()->pagelock.lock();
    // 没有空闲span 需要去Page cache 中去找
    auto span = PageCache::GetInstance()->NewSpan(Sizeclass::NumMovePage(byte_size));
    span->_isUse = true;
    span->_objSize = byte_size;
    PageCache::GetInstance()->pagelock.unlock();
    // 切割Span
    // 计算span的大块内存的起始地址和大块内存的大小（字节数）
    char *address = (char *) (span->_pageId << PAGE_SHIFT);
    size_t bytes = span->_n << PAGE_SHIFT;

    // 把大块内存切成自由链表连接起来
    char *end = address + bytes;
    // 1.先切一块下来做头，方便尾插入
    span->_freeList = address;
    address += byte_size;
    void *tail = span->_freeList;
    // 2.将这段空间不断切割
    while (address < end) {
        NextObj(tail) = address;
        tail = NextObj(tail);
        address += byte_size;
    }
    NextObj(tail) = nullptr;
    // 切好
    list._mtx.lock();
    list.PushFront(span);
    return span;
}

void CentralCache::ReleaseListToSpans(void *start, size_t size) {
    size_t index = Sizeclass::Index(size);
    _spanList[index]._mtx.lock();
    while (start != nullptr) {
        void *next = NextObj(start);
        //找到对应的span
        Span *span = PageCache::GetInstance()->MapObjectToSpan(start);
        NextObj(start) = span->_freeList;
        span->_freeList = start;
        span->_useCount--;
        if(span->_useCount == 0){
            //所有span切分出来的小块内存都回来了
            //可以回收回page cache
            _spanList[index].Erase(span);
            span->_freeList = nullptr;
            span->_next = nullptr;
            span->_prev = nullptr;
            _spanList[index]._mtx.unlock();
            PageCache::GetInstance()->pagelock.lock();
            PageCache::GetInstance()->MapObjectToSpan(span);
            PageCache::GetInstance()->pagelock.unlock();
            _spanList[index]._mtx.lock();
        }
        start = next;
    }
    _spanList[index]._mtx.unlock();
}