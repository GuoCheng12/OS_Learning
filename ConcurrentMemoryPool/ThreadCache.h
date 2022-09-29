//
// Created by 巫国诚 on 2022/9/28.
//

#ifndef TEST1_THREADCACHE_H
#define TEST1_THREADCACHE_H
#include "Common.h"
class ThreadCache{
public:
    //申请和释放内存
    void* allocate(size_t size);
    void deallocate(void *ptr,size_t size);
private:


};
#endif //TEST1_THREADCACHE_H
