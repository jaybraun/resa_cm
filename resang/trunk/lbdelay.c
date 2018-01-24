#include <unistd.h>

void delay(int *msec)
{
	int imsec = *msec;
	usleep(imsec * 1000);
}
