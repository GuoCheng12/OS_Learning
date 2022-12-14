//
// Created by 巫国诚 on 2022/9/28.
//

#pragma once

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <mutex>
#include <algorithm>
#include <sys/mman.h>
#include <unordered_map>
#include <sys/types.h>

static const size_t MAX_BYTES = 256 * 1024;
static const size_t NFREELISTS = 208; // 桶数
static const size_t NPAGES = 129;
static const size_t PAGE_SHIFT = 13;
#ifdef __APPLE__
typedef unsigned long long PAGE_ID;
#endif
#ifdef _WIN64
typedef unsigned long long PAGE_ID;
#elif _WIN32
typedef size_t PAGE_ID;
#endif // _win32

inline static void *&NextObj(void *obj) {
    return *(void **) obj; // 取头部四/八个字节
}

inline static void *SystemAlloc(size_t kpage) {
    void *ptr = mmap(nullptr, kpage << 13, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("map mem error\n");
        ptr = nullptr;
        exit(-2);
    }
    return ptr;
}
inline static void SystemFree(void *ptr,size_t kpage){
    munmap(ptr,kpage);
}
// freelist 在central cache和 thread cache 都需要使用
class FreeList {
public:
    // 头插
    void Push(void *obj) {
        assert(obj);
        NextObj(obj) = _freeList;
        _freeList = obj;
        ++_size;
    }

    void PushRange(void *start, void *end, size_t n) {
        NextObj(end) = _freeList;
        _freeList = start;
        _size += n;
    }

    void PopRange(void *&start, void *&end, size_t n) {
        assert(n >= _size);
        start = _freeList;
        end = start;
        for (size_t i = 0; i < n - 1; ++i) {
            end = NextObj(end);
        }
        _freeList = NextObj(end);
        NextObj(end) = nullptr;
        _size -= n;
    }

    // 头删
    void *Pop() {
        assert(_freeList);
        void *obj = _freeList;
        _freeList = NextObj(obj);
        --_size;
        return obj;
    }

    bool Empty() {
        return _freeList == nullptr;
    }

    size_t &MaxSize() {
        return maxSize;
    }

    size_t Size() {
        return _size;
    }

private:
    void *_freeList = nullptr;
    size_t _size = 0;
    size_t maxSize = 1;
};

class Sizeclass {
public:
    //[1,128]               8byte对齐     freelist[0,16)
    //[128+1,1024]          16byte对齐    freelist[16,72)
    //[1024+1,8*1024]       128byte对齐   freelist[72,128)
    //[8*1024+1,64*1024]    1024byte对齐  freelist[128,184)
    //[64*1024+1,256*1024]  8*1024byte对齐 freelist[184,208)

    // 向下/上补齐
    static inline size_t _RoundUp(size_t bytes, size_t align) {
        size_t alignSize;
        if (bytes % align != 0) {
            alignSize = (bytes / align + 1) * (align);
            return alignSize;
        } else {
            return bytes;
        }
        //return (((bytes)+align - 1) & ~(align - 1));
    }

    // 计算对齐
    static inline size_t RoundUp(size_t bytes) {
        if (bytes <= 128) {
            return _RoundUp(bytes, 8);
        } else if (bytes <= 1024) {
            return _RoundUp(bytes, 16);
        } else if (bytes <= 8 * 1024) {
            return _RoundUp(bytes, 128);
        } else if (bytes <= 64 * 1024) {
            return _RoundUp(bytes, 1024);
        } else if (bytes <= 256 * 1024) {
            return _RoundUp(bytes, 8 * 1024);
        } else {
            return _RoundUp(bytes, 1 << PAGE_SHIFT);
        }
    }

    static inline size_t _Index(size_t bytes, size_t align_shift) {
        return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
//        if(bytes % align_shift == 0){
//            return bytes / align_shift - 1;
//        }else{
//            return bytes / align_shift;
//        }
    }

    // 计算映射的是哪一个桶的index
    static size_t Index(size_t bytes) {
        assert(bytes <= MAX_BYTES);
        static int grop_array[4] = {16, 56, 56, 56};
        if (bytes <= 128) {
            return _Index(bytes, 3);
        } else if (bytes <= 1024) {
            return _Index(bytes - 128, 4) + grop_array[0];
        } else if (bytes <= 8 * 1024) {
            return _Index(bytes - 1024, 7) + grop_array[0] + grop_array[1];
        } else if (bytes <= 64 * 1024) {
            return _Index(bytes - 8 * 1024, 10) + grop_array[0] + grop_array[1] + grop_array[2];
        } else if (bytes <= 256 * 1024) {
            return _Index(bytes - 64 * 1024, 13) + grop_array[0] + grop_array[1] + grop_array[2] + grop_array[3];
        } else {
            std::cerr << "something wrong.." << std::endl;
            exit(-1);
        }
        return -1;
    }

    // 控制一下CentralCache给ThreadCache的空间大小分配
    // [2,512) 一次批量移动多少个对象（慢启动）上限值
    // 小对象一次批量上限高
    // 小对象一次批量上限低
    static size_t NumMoveSize(size_t size) {
        assert(size > 0);
        int num = MAX_BYTES / size;
        if (num < 2) {
            num = 2;
        }
        if (num > 512) {
            num = 512;
        }
        return num;
    }

    static size_t NumMovePage(size_t size) {
        assert(size > 0);
        size_t num = NumMoveSize(size);
        size_t npage = num * size;
        npage >>= PAGE_SHIFT; // 右移动13位  和PAGE的转化
        if (npage == 0) {
            npage = 1;
        }
        return npage;
    }
};

// 管理多个连续页大块内存管理使用
struct Span {
    PAGE_ID _pageId = 0; // 大块内存起始页的页号
    size_t _n = 0;  // 页的数量

    Span *_next = nullptr; // 双向链表的结构 带头双向循环
    Span *_prev = nullptr;
    size_t _useCount = 0; // 切好小块内存，被分配给thread cache的计数
    size_t _objSize = 0; // 切好的小对象的大小
    void *_freeList = nullptr; // 切好的小块内存的自由链表
    bool _isUse = false; // 是否正在使用
};

class SpanList {
public:
    // Central Cache 和 Page Cache都需要一把Big Lock
    std::mutex _mtx;

    SpanList() {
        _head = new Span;
        _head->_next = _head;
        _head->_prev = _head;
    }

    Span *begin() {
        return _head->_next;
    }

    Span *end() {
        return _head;
    }

    bool Empty() {
        return _head->_next == _head;
    }

    void PushFront(Span *span) {
        Insert(begin(), span);
    }

    Span *PopFront() {
        Span *front = _head->_next;
        Erase(front);
        return front;
    }

    // 插入
    void Insert(Span *pos, Span *newSpan) {
        assert(pos);
        assert(newSpan);

        Span *prev = pos->_prev;
        prev->_next = newSpan;
        newSpan->_prev = prev;
        newSpan->_next = pos;
        pos->_prev = newSpan;
    }

    void Erase(Span *pos) {
        assert(pos);
        assert(pos != _head);
        Span *prev = pos->_prev;
        Span *next = pos->_next;

        prev->_next = next;
        next->_prev = prev;
    }

private:
    Span *_head;
};
