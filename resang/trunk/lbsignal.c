#include <signal.h>
#include <unistd.h>

int kill_parent();

void c_signal_sigtstp (void (*signal_proc))
/* set exit handler for cntl-z */
{
	(void) signal(SIGTSTP, signal_proc);
}


int kill_parent()
{
    int rc = kill(getppid(), SIGKILL);
    return rc;
}
