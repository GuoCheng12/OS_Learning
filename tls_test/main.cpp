#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>


void *PrintStudent(void *arg){
    myarg_t *args = (myarg_t*)arg;
    s->_age = args->a;
    s->_name = args->b;
    printf("%d ",s->_name);
    printf("%d\n",s->_age);
    return (void*)args;
}

int main() {
    pthread_t p1,p2;
    myarg_t args1 = {2,12};
    myarg_t args2 = {3,13};
    pthread_create(&p1,NULL,PrintStudent,&args1);
    pthread_create(&p2,NULL,PrintStudent,&args2);
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    return 0;
}
