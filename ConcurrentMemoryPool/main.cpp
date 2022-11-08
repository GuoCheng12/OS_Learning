#include "ConcurrentAlloc.hpp"
#include "ThreadCache.cpp"
#include <vector>

void *Alloc1(void *arg) {
    std::vector<void *> v;
    for (size_t i = 0; i < (size_t) arg; ++i) {
        void *ptr = ConcurrentAlloc(6);
        v.push_back(ptr);
    }
    for (auto e: v) {
        std::cout << e << std::endl;
    }
    return (void *) arg;
}

void *Alloc2(void *arg) {
    std::vector<void *> v;
    for (size_t i = 0; i < (size_t) arg; ++i) {
        void *ptr = ConcurrentAlloc(7);
        v.push_back(ptr);
    }
    for (auto e: v) {
        std::cout << e << std::endl;
    }
    return (void *) arg;
}

void TestConcurrentAlloc2() {
    for (size_t i = 0; i < 5; i++) {
        void *p1 = ConcurrentAlloc(6);
        std::cout << p1 << std::endl;
    }
}

void TestConcurrentAlloc() {
    void *p1 = ConcurrentAlloc(6);
    void *p2 = ConcurrentAlloc(8);
    void *p3 = ConcurrentAlloc(3);
    void *p4 = ConcurrentAlloc(2);
    std::cout << p1 << std::endl;
    std::cout << p2 << std::endl;
    std::cout << p3 << std::endl;
    std::cout << p4 << std::endl;

    ConcurrentFree(p1);
    ConcurrentFree(p2);
    ConcurrentFree(p3);
    ConcurrentFree(p4);
}

void AllocBigSize() {
    constexpr int len1 = 257 * 1024;
    void *p1 = ConcurrentAlloc(len1);
    ConcurrentFree(p1);
    constexpr int len2 = 129 * 8 * 1024;
    void *p2 = ConcurrentAlloc(len2);
    ConcurrentFree(p2);
}


int main() {
//   pthread_t p1,p2;
    //AllocBigSize();
    TestConcurrentAlloc();
//    pthread_create(&p1, nullptr,Alloc1,(void*)8);
//    pthread_create(&p2, nullptr,Alloc2,(void*)7);
//    pthread_join(p2, nullptr);
//    pthread_join(p1, nullptr);
//    return 0;
}