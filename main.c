#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include "function.h"

int main()
{
	printf("%s\n","Hello World !");
	generateMaze(3,4);
	printf("\n%s\n","Fin Programme");
	
    return EXIT_SUCCESS;
}

