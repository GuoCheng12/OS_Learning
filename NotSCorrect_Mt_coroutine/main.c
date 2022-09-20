#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <setjmp.h>

#define MAX_COUNT 5
jmp_buf SCHEDULER , MAIN; // jump buff

typedef struct PCB_NODE *PCB_ptr;
typedef struct PCB_NODE{ // pointer to PCB
   jmp_buf env; // jump buff
   int Name;    // thread ID
   PCB_ptr next;// next PCB
}PCB;

// We need two more jump buffers MAIN and SCHEDULER
// The former is used to save the main program's environment, and the latter is for scheduler
// Because the main program and the scheduler are not scheduled by the scheduler, they are not in the PCB list.
// There are two pointers: Head pointing to the head of the PCB list and Current to the running thread.

PCB_ptr Current;
PCB_ptr Head;
PCB_ptr work;
#define THREAD_CREATE(funtion,name){                           \
                                    if (setjmp(MAIN) == 0)     \
                                    (funtion)(name);           \
}


// THREAD_INIT() can be part of THREAD_CREATE().

#define THREAD_INIT(name){                                     \
                         work = (PCB_ptr) malloc(sizeof(PCB)); \
                         work->Name = name;                    \
                         if(Head == NULL){                     \
                           Head = work;                        \
                         }                                     \
                         else{                                 \
                           Current->next = work;               \
                         }                                     \
                         work->next = Head;                    \
                         Current = work;                       \
                         if(setjmp(work->env) == 0)            \
                            longjmp(MAIN,1);                   \
}

#define THREAD_YIELD(name) {if(setjmp(Current->env) == 0) longjmp(SCHEDULER,1);}

void Scheduler(void){
    if(setjmp(SCHEDULER) == 0){  // setup return point
        longjmp(MAIN,1);          // get back to main
    }
    Current = Current->next;      // re-enter. Get next
    longjmp(Current->env,1);      // jump to its environ
}

void func_1(int name){
    int i;
    THREAD_INIT(name);
    while (1){
        for(int i = 0; i <= MAX_COUNT; i++) {
            printf("func_1() execting\n");
        }
        THREAD_YIELD(name);
    }
}

void func_2(int name){
    int i;
    THREAD_INIT(name);
    while (1){
        for(int i = 0; i <= MAX_COUNT; i++) {
            printf("func_2() execting\n");
        }
        THREAD_YIELD(name);
    }
}

void func_3(int name){
    int i;
    THREAD_INIT(name);
    while (1){
        for(int i = 0; i <= MAX_COUNT; i++) {
            printf("func_3() execting\n");
        }
        THREAD_YIELD(name);
    }
}

void func_4(int name){
    int i;
    THREAD_INIT(name);
    while (1){
        for(int i = 0; i <= MAX_COUNT; i++) {
            printf("func_4() execting\n");
        }
        THREAD_YIELD(name);
    }
}
int main(void) {
    Head = Current = NULL;
    THREAD_CREATE(func_1,1);
    THREAD_CREATE(func_2,2);
    THREAD_CREATE(func_3,3);
    THREAD_CREATE(func_4,4);
    if(setjmp(MAIN) == 0){
        Scheduler();
    }
    longjmp(SCHEDULER,1);
    return 0;
}
