//
// Created by 巫国诚 on 2022/10/31.
//
#include "PageCache.hpp"
#include "Common.hpp"

PageCache PageCache::_sInstan;

Span *PageCache::NewSpan(size_t k) {
    assert(k > 0 && k < NPAGES);
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