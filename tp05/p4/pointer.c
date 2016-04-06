#include <stdio.h>
typedef unsigned int uint;
int main(void) {

	uint *ptr = (uint *)0x9070;
	printf("Old Value of ptr : %x\n",ptr);
	ptr += 0x04;
	printf("New Value of ptr : %x\n",ptr);
	return 0;
}
