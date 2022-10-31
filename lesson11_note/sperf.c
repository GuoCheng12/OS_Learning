#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define ERR_EXIT(m)do{perror(m);exit(EXIT_FAILURE);}while(0)

char *exec_envp[] = { "PATH=/bin", NULL, };
//char *exec_argv[] = { "strace", "ls", NULL, };
int main(int argc, char *argv[]) {
    int fildes[2];
    if(pipe(fildes) == -1) ERR_EXIT("pipe error\n");
    char* my_arg[50] = {"strace","-T"};
    for(int i = 1; i < argc; i++){
        my_arg[i+1] = argv[i]; // main参数
    }
    pid_t pid = fork();
    if(pid < 0){
        ERR_EXIT("fork error\n");
    }
    if(pid == 0){ // child
        close(fildes[0]); // 关闭读端
        dup2(fildes[1],2); // fildes[1]写入端口 2: error
        execve("/usr/bin/strace",my_arg,exec_envp);
		close(fildes[1]);
    }
    else{
        close(fildes[1]); //关闭写端
        char PB[6553600] = {0};
        read(fildes[0],PB,6553600);
        //printf("%s",PB);
		//已经可以获取到新的管道
		// .... coding here 
    }
   // execve("strace",          exec_argv, exec_envp);
   // execve("/bin/strace",     exec_argv, exec_envp);
   // execve("/usr/bin/strace", exec_argv, exec_envp);
   // perror(argv[0]);
   // exit(EXIT_FAILURE);
   return 0;
}
void analyse(){
		
}
