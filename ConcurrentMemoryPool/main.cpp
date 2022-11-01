#include "ConcurrentAlloc.hpp"
#include "ThreadCache.cpp"

void *Alloc1(void *arg) {
    for (size_t i = 0; i < (size_t) arg; ++i) {
        void *ptr = ConcurrentAlloc(6);
    }
    return (void *) arg;
}

void *Alloc2(void *arg) {

    for (size_t i = 0; i < (size_t) arg; ++i) {
        void *ptr = ConcurrentAlloc(7);
    }
    return (void *) arg;
}

void TestConcurrentAlloc(){
    void* p1 = ConcurrentAlloc(6);
    void* p2 = ConcurrentAlloc(8);
    void* p3 = ConcurrentAlloc(3);
    void* p4 = ConcurrentAlloc(2);
}
int main() {
    TestConcurrentAlloc();
//    pthread_t p1,p2;
//    pthread_create(&p1,NULL,Alloc1,(void*)5);
//    pthread_join(p1,NULL);
//    pthread_create(&p2,NULL,Alloc2,(void*)7);
//    pthread_join(p2,NULL);
    return 0;
}