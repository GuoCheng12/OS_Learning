//
// Created by 巫国诚 on 2022/10/16.
//

#ifndef TLS_TEST_TEST_H
#define TLS_TEST_TEST_H
typedef struct Student{
    int _name;
    int _age;
}Student;
typedef struct{
    int a;
    int b;
}myarg_t;
thread_local Student* s = new Student ;
__thread struct Student stu = {0,0};
#endif //TLS_TEST_TEST_H
