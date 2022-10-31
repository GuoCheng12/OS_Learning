#include <stdio.h>
#include <unistd.h>

int main(int argc,char** argv){
	int pipes[2];
	if(pipe(pipes) < 0 ){
		perror("pipe\n");
	}
	pid_t pid = fork();
	if(pid == 0){
		//child
		printf("i am child\n");
		close(pipes[0]);
		dup2(pipes[1],1);
		printf("i am child too");
	}else{
		close(pipes[1]);
		char ret[50] = {0};
		read(pipes[0],ret,50);
		printf("i am father\n");
		printf("%s",ret);
	}
	return 0;
	
}
