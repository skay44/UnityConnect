
#include <iostream>
#include "interface.h"


int main()
{
	int size;
	void* data;
	data = getAllData(size);
	printf("size: %d\n", size);
	printf("%d\n", tester());
	data = getAllData(size);
	printf("size: %d", size);
}

