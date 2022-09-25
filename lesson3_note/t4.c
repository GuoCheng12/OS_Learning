#include <stdio.h>


int FetchAndAdd(int *ptr){
	int old = *ptr;
	*ptr = old + 1;
	return old;
}

int main(void){
	int c = 3;
	printf("%d\n",FetchAndAdd(&c));
	return 0;
}
