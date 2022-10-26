//
// Created by 巫国诚 on 2022/10/16.
//

#pragma once
#include "ThreadCache.hpp"
static void* ConcurrentAlloc(size_t size){
    if(pTLSThreadCache == nullptr){
        pTLSThreadCache = new ThreadCache;
    }
    std::cout  << pthread_self() <<  ":" <<pTLSThreadCache << std::endl;
    return pTLSThreadCache->allocate(size);
}
static void ConcurrentFree(void* ptr){

}


