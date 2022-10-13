#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#define MAX_COMM 1024
static char command[MAX_COMM];
int isInput(){
    memset(command,0x00,MAX_COMM);
    printf("gcshell:> ");
    fflush(stdout); 
    if(scanf("%[^\n]%*c",command) == 0){
        getchar();
        return -1;
    }
    return 0;
}
// 解析stdin
char ** do_parse(char *buff){
    int argc = 0;
    static char *argv[32]; // 将commond 放入 argv
    char *ptr = buff;
    while(*ptr != '\0'){
        if(!isspace(*ptr)){
            argv[argc++] = ptr;
            while((!isspace(*ptr) && (*ptr) != '\0')){
                ptr++;
            }
        }else{
            while(isspace(*ptr)){
                *ptr = '\0';
                ptr++;
            }
        }
    }
    argv[argc] == NULL;
    return argv;
    
}
void do_exec(char *buff){
    char ** argv = {NULL};
    pid_t pid = fork();
    // 复制状态机
    if(pid == 0){
        argv = do_parse(buff);
        if(argv[0] == NULL){
            exit(-1); // 推出当前状态机
        }
        execvp(argv[0],argv);
    }else{
        waitpid(pid,NULL,0);
    }
}
// 创建一个小的shell
int main(int argc, char ** argv){
    while(1){
        if(isInput() < 0){
            continue;
        }
        do_exec(command);
    }
    return 0;
}