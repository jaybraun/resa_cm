#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*
void bye(void)
{
	printf("That is all folks\n");
}

void main()
{
	int rc;	int c_atexit(void (*function) (void));
	rc = c_atexit(bye);
	if (rc != 0) {
		fprintf(stderr, "cannot set exit function\n");
		exit(EXIT_FAILURE);
	}
	else 
		printf("exit function set\n");
	exit(EXIT_SUCCESS);
}
*/
int c_atexit(void (*exit_proc) (void))
/* set exit handler for normal termination */
{
	int rc;
	rc = atexit( exit_proc );
	return(rc);
}


