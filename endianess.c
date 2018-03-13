#include <stdio.h>

void SwapBytes(void *pv, size_t n)
{
	char *p = (char *)pv; 
	size_t lo, hi; 
	
	for (lo = 0, hi = n - 1; hi > lo; lo++, hi--)
	{
		char tmp = p[lo]; p[lo] = p[hi]; p[hi] = tmp;
	}
}

void checkEndianess(int *endianess){
	int num = 1;
	if (*(char *)&num == 1)
	{
		printf("\nLittle-Endian on client\n");
		*endianess = 0;
	}
	else
	{
		printf("Big-Endian on client\n");
		*endianess = 1;
	}
}
