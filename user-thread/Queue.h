//
// Created by 巫国诚 on 2022/9/21.
//
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#ifndef USER_THREAD_QUEUE_H
#define USER_THREAD_QUEUE_H
#define SIZE 100
typedef struct Queue {
    int _arr[SIZE];
    int _head;
    int _tail;
}Queue;

void queue_init(Queue *queue){
    memset(queue->_arr,0,SIZE);
    queue->_head = 0;
    queue->_tail = 0;
}

void queue_destroy(Queue *queue){
    queue->_head = 0;
    queue->_tail = 0;
}

int queue_size(Queue *queue){
    return queue->_tail - queue->_head;
}

bool queue_full(Queue *queue){
    //return queue->_tail - queue->_head == SIZE ? true : false;
    return queue_size(queue) == SIZE ? true : false;
}

bool queue_empty(Queue *queue){
    return queue_size(queue) == 0 ? true : false;
}
bool queue_push_back(Queue *queue,int ){
    if(queue_full(queue)){
        return false;
    }


}

bool queue_pop(){}

bool queue_front(){}
#endif //USER_THREAD_QUEUE_H
