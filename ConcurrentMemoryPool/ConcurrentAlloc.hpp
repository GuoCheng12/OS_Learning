//
// Created by 巫国诚 on 2022/10/16.
//

#pragma once
#include "ThreadCache.hpp"
void* ConcurrentAlloc(size_t size){
    if(pTLSThreadCache == nullptr){
        pTLSThreadCache = new ThreadCache;
    }
    std::cout  << ":" <<pTLSThreadCache << std::endl;
    return pTLSThreadCache->allocate(size);
}
void ConcurrentFree(void* ptr){

}


