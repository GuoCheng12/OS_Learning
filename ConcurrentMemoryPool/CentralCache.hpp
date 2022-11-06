//
// Created by 巫国诚 on 2022/10/26.
//
#pragma once

#include "Common.hpp"

class CentralCache {
public:
    static CentralCache *GetInStance() {
        return &_sInst;
    }
    // 获取一个非空的span
    Span *GetOneSpan(SpanList &list, size_t byte_size);

    size_t FetchRangeObj(void *&start, void *&end, size_t batchNum, size_t size);

    void ReleaseListToSpans(void *start, size_t byte_size);

private:
    SpanList _spanList[NFREELISTS];
private:
    //单例模式
    CentralCache() {}

    CentralCache(const CentralCache &) = delete;

    CentralCache &operator=(CentralCache const &) = delete;

    static CentralCache _sInst;
};