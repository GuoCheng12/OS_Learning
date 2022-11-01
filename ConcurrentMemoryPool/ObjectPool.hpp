//
// Created by 巫国诚 on 2022/9/28.
//

#pragma once

#include "Common.hpp"
// 向堆申请k个大块页


template<class T>
class ObjectPool {
public:
    //开辟新的Object
    T *New() {
        //  使用还回来的memoryspace来补充
        T *object = nullptr;
        if (memoryListforfree) {
            void *next = *(void **) object; // 指向下个内存块的指针
            object = memoryListforfree;
            memoryListforfree = next;
            return object;
        } else {
            if (leftMemory < sizeof(T)) {
                leftMemory = 128 * 1024;
                memoryPool = (char *) malloc(leftMemory);
                if (memoryPool == nullptr) {
                    throw std::bad_alloc();
                }
            }
            object = (T *) memoryPool;
            // 防止出现64bits下出现的报错
            size_t objSize = sizeof(T) < sizeof(void *) ? sizeof(void *) : sizeof(T);
            memoryPool += objSize;
            leftMemory -= sizeof(T);
        }
        new(object)T; // 初始化
        return object;
    }

    //删除对象
    void Delete(T *obj) {
        //  if(memoryListforfree == nullptr) { // 第一次分配
        //      memoryListforfree = obj;
        //      *(void **) obj = nullptr; // 32/64bits 下操作系统，都可适用这样分配前四/八个字节
        //  }else{ // 第二次分配
        //      *(void **)obj = memoryListforfree;
        //      memoryListforfree = obj;
        //  }

        //显示调用
        obj->~T();
        *(void **) obj = memoryListforfree;
        memoryListforfree = obj;
    }

private:
    void *memoryListforfree = nullptr;
    size_t leftMemory = 0;
    char *memoryPool = nullptr; // 定长内存 内存池
};


