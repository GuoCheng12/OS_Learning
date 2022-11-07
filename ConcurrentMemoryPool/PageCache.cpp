//
// Created by 巫国诚 on 2022/10/31.
//
#include "PageCache.hpp"
#include "Common.hpp"

PageCache PageCache::_sInstan;

Span *PageCache::NewSpan(size_t k) {
    assert(k > 0);
    // 大于128页的直接向堆申请
    if (k > NPAGES - 1) {
        void *ptr = SystemAlloc(k);
        Span *span = new Span;
        span->_pageId = (PAGE_ID) ptr >> PAGE_SHIFT;
        span->_n = k;
        _idSpanMap[span->_pageId] = span;
        return span;
    }
    // 先检查第k个桶里面有没有span
    if (!_spanLists[k].Empty()) {
        return _spanLists->PopFront();
    }
    // 检查一下后面的桶有没有span，如果有的话就把它进行切分
    // 切分成一个k页的span和一个n-k页的span k页的span返回给
    // central cache
    // n-k 页的span挂在第n-k桶上去
    for (size_t i = k + 1; i < NPAGES; ++i) {
        if (!_spanLists[i].Empty()) {
            Span *nSpan = _spanLists[i].PopFront();
            Span *kSpan = new Span;
            // 在nSpan的头部切一个k页下来
            // K页span返回
            // nSpan再挂到对应映射的位置
            kSpan->_pageId = nSpan->_pageId;
            kSpan->_n = k;
            nSpan->_pageId += k;
            nSpan->_n -= k;
            _spanLists[nSpan->_n].PushFront(nSpan);
            // 存储nSpan 的首尾页号跟nSpan映射，方便page cache回收内存时
            // 进行的合并查找
            _idSpanMap[nSpan->_pageId] = nSpan;
            _idSpanMap[nSpan->_pageId + nSpan->_n - 1] = nSpan;
            // 建立id和span的映射关系,方便central cache回收小块内存时
            for (PAGE_ID i = 0; i < kSpan->_n; ++i) {
                _idSpanMap[kSpan->_pageId + i] = kSpan;
            }
            return kSpan;
        }
    }
    // 走到这里已经说明没有大页的span了
    // 这个时候去找堆要一个128页的span

    Span *bigSpan = new Span;
    void *ptr = SystemAlloc(NPAGES - 1);
    bigSpan->_pageId = (PAGE_ID) ptr >> PAGE_SHIFT; // 找页号
    bigSpan->_n = NPAGES - 1;
    _spanLists[bigSpan->_n].PushFront(bigSpan);
    return NewSpan(k); // 递归去再次切割

}

Span *PageCache::MapObjectToSpan(void *obj) {
    PAGE_ID id = (PAGE_ID) obj >> PAGE_SHIFT;
    auto ret = _idSpanMap.find(id);
    if (ret != _idSpanMap.end()) {
        return ret->second;
    } else {
        assert(false);
    }
}

void PageCache::ReleaseSpanToPageCache(Span *span) {
    // 大于128页的直接还给堆
    if (span->_n > NPAGES - 1) {
        void *ptr = (void *) (span->_pageId << PAGE_SHIFT);
        SystemFree(ptr, span->_n);
        delete span;
        return;
    }
    // 对span前后的页，尝试进行合并，缓解内存碎片问题
    // 往前获取span
    while (true) {
        PAGE_ID preId = span->_pageId - 1;
        auto ret = _idSpanMap.find(preId);
        // 前面的页号没有，不进行合并
        if (ret == _idSpanMap.end()) {
            break;
        }
        // 前面相邻页的span正在被使用，不进行合并
        Span *prevSpan = ret->second;
        if (prevSpan->_isUse) {
            break;
        }
        // 合并出超过128页的span 没办法管理.. 不进行合并
        if (prevSpan->_n + span->_n > NPAGES - 1) {
            break;
        }
        span->_pageId = prevSpan->_pageId;
        span->_n += prevSpan->_n;
        _spanLists[prevSpan->_n].Erase(prevSpan);
        delete prevSpan;
    }
    // 往后获取span
    while (true) {
        PAGE_ID newId = span->_pageId + span->_n;
        auto ret = _idSpanMap.find(newId);
        if (ret == _idSpanMap.end()) {
            break;
        }
        Span *nextSpan = ret->second;
        if (nextSpan->_isUse) {
            break;
        }
        if (nextSpan->_n + span->_n > NPAGES - 1) {
            break;
        }
        _spanLists[nextSpan->_n].Erase(nextSpan);
        delete nextSpan;
    }
    _spanLists[span->_n].PushFront(span);
    span->_isUse = false;
    _idSpanMap[span->_pageId] = span;
    _idSpanMap[span->_pageId + span->_n - 1] = span;
}
