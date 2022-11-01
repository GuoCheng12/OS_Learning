//
// Created by 巫国诚 on 2022/9/28.
//
#pragma once

#include "Common.hpp"

class ThreadCache {
public:
    //申请和释放内存
    void *allocate(size_t size);

    void deallocate(void *ptr, size_t size);

    // 从中心缓存获取对象
    void *FetchFromCentralCache(size_t index, size_t size);

private:
    // 哈希桶
    // NFREELISTS 为 算出来之后桶数
    FreeList _freeList[NFREELISTS];
};

static thread_local ThreadCache *pTLSThreadCache = nullptr;
