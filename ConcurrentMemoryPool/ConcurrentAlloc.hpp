//
// Created by 巫国诚 on 2022/10/16.
//

#pragma once
#include "PageCache.hpp"
#include "ThreadCache.hpp"

static void *ConcurrentAlloc(size_t size) {
    if(size > MAX_BYTES){
        size_t alignSize = Sizeclass::RoundUp(size);
        size_t kpage = alignSize >> PAGE_SHIFT;
        PageCache::GetInstance()->pagelock.lock();
        Span* span = PageCache::GetInstance()->NewSpan(kpage);
        PageCache::GetInstance()->pagelock.unlock();
        void *ptr = (void*)(span->_pageId << PAGE_SHIFT);
        return ptr;
    }else{
        if (pTLSThreadCache == nullptr) {
            pTLSThreadCache = new ThreadCache;
        }
        std::cout << pthread_self() << ":" << pTLSThreadCache << std::endl;
        return pTLSThreadCache->allocate(size);
    }
}

static void ConcurrentFree(void *ptr,size_t size) {
    if(size > MAX_BYTES){
        Span *span = PageCache::GetInstance()->MapObjectToSpan(ptr);
        PageCache::GetInstance()->pagelock.lock();
        PageCache::GetInstance()->ReleaseSpanToPageCache(span);
        PageCache::GetInstance()->pagelock.unlock();
    }else{
        assert(pTLSThreadCache);
        pTLSThreadCache->deallocate(ptr,size);
    }
}


