#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
	char* argv[] = {"echo","hello world",NULL};
	execlp(argv[0],argv[0],argv[1],NULL);
	return 0;
}
