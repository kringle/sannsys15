#include <stdlib.h>
#include <stdio.h>


void allocate(int value) {
int *ptr = NULL;
ptr = malloc(100000 * sizeof(int));
*ptr = value;
printf("test of allocated memory: %i\n", value);
if(ptr == NULL)
{
	perror("Malloc fail");
	exit(EXIT_FAILURE);
}

}

int main()
{
while(1)
{
allocate(5);

}
return 0;
}
