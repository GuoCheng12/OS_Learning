//
// Created by 巫国诚 on 2022/10/31.
//
#pragma once
#ifndef TEST1_PAGECACHE_HPP
#define TEST1_PAGECACHE_HPP

#include "Common.hpp"

// Page Cache的结构也是一个桶
// 如果Central Cache 中的span use count等于0
// 说明切分给thread cache中的小块内存全都归还回来，
// 则central cache把这个span还给page cache
// page cache通过页号，查看前后的相邻页是否空闲，是的话就合并，合并出更大的页，解决内存碎片问题
class PageCache {
public:
    static PageCache *GetInstance() {
        return &_sInstan;
    }

    Span *MapObjectToSpan(void *obj);

    void ReleaseSpanToPageCache(Span *span);

    std::mutex pagelock;

    // 获取一个K页的Span
    Span *NewSpan(size_t k);

private:
    SpanList _spanLists[NPAGES];
    // 最好别使用桶锁，因为桶锁锁住单个index并不能解决问题(如果此时多个线程访问大内存块span进行拆解？）
    std::unordered_map<PAGE_ID, Span *> _idSpanMap;

    PageCache() {}

    PageCache(const PageCache &) = delete;

    static PageCache _sInstan;
};


#endif //TEST1_PAGECACHE_HPP
