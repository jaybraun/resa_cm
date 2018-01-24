#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*
void main()
{
	void c_exit(int status);
	c_exit(EXIT_SUCCESS);
	
	printf(" error if this message is printed\n" );
}
*/
void c_exit(int *status)
/* cause normal process termination */
{
/*	printf("Program Exit Status = %d \n", *status ); */
	exit( *status );
}


