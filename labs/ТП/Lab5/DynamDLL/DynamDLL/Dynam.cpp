#include "Dynam.h"
#include <stdlib.h>
#include <string.h>


int* getColors(char* Buffer, int count) {
	int *graph = (int *)malloc(count * count * sizeof(int));

	for (int i = 0; i < strlen(Buffer); i++) {
		if (Buffer[i] >= 48 && Buffer[i] <= 57) {
			graph[i] = Buffer[i] - 48;
		}
	}

	int *colors = (int *)malloc(count * count * sizeof(int));
	for (int i = 1; i < count; i++)
		colors[i] = 0;
	colors[0] = 1;
	int k = 1;

	while (k != count) {
		for (int i = 0; i < count * count; i++)
			if (graph[i] == 0 && colors[i] == 0)
				colors[i] = k;
			else
				if (colors[i] == 0)
					colors[i] = k + 1;
		k = k + 1;
	}
	return colors;


}