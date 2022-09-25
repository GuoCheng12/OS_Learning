#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <assert.h>
jmp_buf Buf;
jmp_buf env;
void B();
void A(){
	if(setjmp(Buf) == 0){
		printf("Marked!\n");
		B();
	}
	else{
		printf("I am B returned\n");
	}
}
void B(){
	printf("I am B()\n");
	longjmp(Buf,1);
}
//////////////

double divide(double to,double by){
	if(by == 0)
		longjmp(env,1);
	return to / by;
}
void f(double to,double by){
	if(setjmp(env) == 0){
		divide(to,by);
	}
	else{
		printf("Cannot / 0\n");
	}
	printf("done\n");
}
int main(int args,char* argv[]){
	assert(args >= 2);
	double _by = atoi(argv[1]);
	f(2,_by);
	return 0;
}
