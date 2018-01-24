#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#define I_FIND 21259
#define I_PUSH 21250 

int asdevice_open (char display[], char geometry[], char title[], int *chan,int *pid)
{
	int	master, slave;
	char	slavename[64];
	char	r_buffer[1024];
	char	slave_str[5];
	struct  stat	slave_stat;
	char	xterm_cmd[256];
	char    *ptsname(int);
	int		i, setup;
	pid_t	child_pid;

	if((master = open("/dev/ptmx", O_RDWR)) == -1) {
		perror("open master");
		exit(1);
	}

	grantpt(master);
	unlockpt(master);

	strncpy(slavename, ptsname(master), 64);

	if((slave = open(slavename, O_RDWR)) == -1) {
		perror("open slave");
		exit(1);
	}
	*chan = slave;
	slave_str[0] = slave_str[1] = '0';
	slave_str[2] = 0;

	for(i=0;i<=1;i++) {
		if(slavename[strlen(slavename)-i-1] == '/')
			break;

		slave_str[1-i] = slavename[strlen(slavename)-i-1];
	}

	/*
	 * Set up slave stream
	 */
	if ((setup == ioctl(slave, I_FIND, "ldterm")) < 0) {
		perror("find ldterm");
		exit(1);
	}
 
	if (setup != setup) {

		if(ioctl(slave, I_PUSH, "ptem") < 0) {
			perror("I_PUSH ptem");
			exit(1);
		} 
		if(ioctl(slave, I_PUSH, "ldterm") < 0) {
			perror("I_PUSH ldterm");
			exit(1);
		}
		if(ioctl(slave, I_PUSH, "ttcompat") < 0) {
			perror("I_PUSH ttcompat");
			exit(1);
		}
	}
	/*
	 * Fork a child to execute the command.
	 */
	if((child_pid = fork()) < 0) {
		perror("ASTAB fork failure");
		exit(1);
	}

	if(child_pid == 0) {
		char	arg[80];

		sprintf(arg,"-S%s%1d -display %s -geometry %s",slave_str, master, display, geometry);
		//printf("%s \n", arg);		
		execl("/usr/bin/xterm", title, arg, 0);
		exit(0);
	}
	else {			/* parent process */
		*pid = child_pid;
		return (0);		
	}
}

int asdevice_close (int *chan,int *old_char[])
{
	close (*chan);
}
int asdevice_in (int *chan,char chr[],int *max,int *len)
{
	read((int)*chan, chr, 1);
	*len = 1;
}
int asdevice_out (int *chan,char chr[],int *len)
{
	write ((int)*chan, chr, (int)*len);
}
