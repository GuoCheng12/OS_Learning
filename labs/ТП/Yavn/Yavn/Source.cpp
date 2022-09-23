#include "Header.h"
#include <string.h>
#include <stdlib.h>


int min(int a, int b) {
	if (a < b) return a;
	else return b;
}

char* minDistance(char* Buffer, int count) {
	int *graph = (int *)malloc(count * count * sizeof(int));
	int *W = (int *)malloc(count * count * sizeof(int));
	for (int i = 0; i < strlen(Buffer); i++) {
		if (Buffer[i] >= 48 && Buffer[i] <= 57) {
			graph[i] = Buffer[i] - 48;
		}
	}

	for (int i = 0; i < count * count; i++)
		if (graph[i] != 0)
			W[i] = graph[i];
		else
			W[i] = 100;

	for (int k = 0; k < count; k++)
		for (int i = 0; i < count; i++)
			for (int j = 0; j < count; j++)
				if (W[i * count + j] != 100)
					W[i * count + j] = min(W[i * count + j], W[i * count + k] + W[k * count + j]);
				else
					W[i * count + j] = W[i * count + k] + W[k * count + j];
	//////////////////////////////

	////////////////////////////// create buffer for write
	for (int i = 0; i < count * count; i++) {
		if (Buffer[i] >= 48 && Buffer[i] <= 57) {
			if (W[i] == 100)
				Buffer[i] = char(48);
			else
				Buffer[i] = char(W[i] + 48);
		}
	}
	free(graph);
	free(W);
	return Buffer;
}