#ifdef UNIXSYS
# define Sleep(a) usleep(a*1000)
#endif





#ifdef WIN32

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>

void usleep(int n) {
	Sleep(n);
}
#include <conio.h>
#include <stdio.h>


#if 1
int tty_setup() {
	//printf("tty setup\n");
	return 0;
}
int tty_reset_in() {
	//printf("tty reset\n");
	return 0;
}

int tty_get_char() { // return 0 if no char, else a char
	if (_kbhit()) {
		return _getch();
	}
	return 0;
}
void sleep(int n) {
	return;
}
#endif



#if 0
void main( void )
{
   /* Display message until key is pressed. */
   while( !_kbhit() )
      _cputs( "Hit me!! " );

   /* Use _getch to throw key away. */
   printf( "\nKey struck was '%c'\n", _getch() );
   _getch();
}

#endif







// ---- windows /\  here
#else
// ---- unix    \/  here   





#include <fcntl.h>
#include <stdio.h>
#include	<signal.h>
#include	"ourhdr.h"
#include	<termios.h>
#include	<unistd.h>
#include	<errno.h>		/* for definition of errno */
#include	<stdarg.h>		/* ANSI C header file */
static struct termios	save_termios;
static int				ttysavefd = -1;
static enum { RESET, RAW, CBREAK }	ttystate = RESET;
int fd_flags;
#ifdef TESTTTY2
int main() {
	char buf[2];
	int fd,i;
	
/* 	tty_test_main(); */
/* 	return 0; */
	
	tty_setup();
	
	printf("ok\n");
	for(i=0;i<10;i++) { 
		int n;
		sleep(1);
		n  = tty_get_char();
		printf("n=%d\n",n);
	}
    tty_reset_in();
}
#endif
static ttyok = 1;

int tty_get_char() {
    int n;
	char buf[2];
	if(ttyok==0)return 0;
	n = read(STDIN_FILENO,buf,1);
	if (n <= 0) {
		return 0;
	} else {
		return buf[0] & 255;
	}
	
}
static void	sig_catch(int);


int tty_setup(void)
{
	int		i;
	char	c;

	if (signal(SIGINT, sig_catch) == SIG_ERR)	/* catch signals */
		err_sys("signal(SIGINT) error");
	if (signal(SIGQUIT, sig_catch) == SIG_ERR)
		err_sys("signal(SIGQUIT) error");
	if (signal(SIGTERM, sig_catch) == SIG_ERR)
		err_sys("signal(SIGTERM) error");
	if (tty_cbreak(STDIN_FILENO) < 0) {
			err_sys("cannot enable interactive controls"); 	
	    	ttyok=0;
	    }

}
int tty_test_main(void)
{
	int		i;
	char	c;

	if (signal(SIGINT, sig_catch) == SIG_ERR)	/* catch signals */
		err_sys("signal(SIGINT) error");
	if (signal(SIGQUIT, sig_catch) == SIG_ERR)
		err_sys("signal(SIGQUIT) error");
	if (signal(SIGTERM, sig_catch) == SIG_ERR)
		err_sys("signal(SIGTERM) error");

	if (tty_raw(STDIN_FILENO) < 0)
		err_sys("tty_raw error");
	printf("Enter raw mode characters, terminate with DELETE\n");
	while ( (i = read(STDIN_FILENO, &c, 1)) == 1) {
		if ((c &= 255) == 0177)		/* 0177 = ASCII DELETE */
			break;
		printf("%o\n", c);
	}
	if (tty_reset(STDIN_FILENO) < 0)
		err_sys("tty_reset error");
	if (i <= 0)
		err_sys("read error");

	if (tty_cbreak(STDIN_FILENO) < 0)
		err_sys("tty_raw error");
	printf("\nEnter cbreak mode characters, terminate with SIGINT\n");
	while ( (i = read(STDIN_FILENO, &c, 1)) == 1) {
		c &= 255;
		printf("%o\n", c);
	}
	tty_reset(STDIN_FILENO);
	if (i <= 0)
		err_sys("read error");
	exit(0);
}


static void sig_catch(int signo)
{
	printf("signal caught, reseting terminal\n");
	tty_reset(STDIN_FILENO);
	exit(0);
}

static int tty_cbreak(int fd)	/* put terminal into a cbreak mode */
{
	struct termios	buf;

	if (tcgetattr(fd, &save_termios) < 0)
		return(-1);

	buf = save_termios;	/* structure copy */

	buf.c_lflag &= ~(ECHO | ICANON);
					/* echo off, canonical mode off */

	buf.c_cc[VMIN] = 0;	/* Case d: 1 byte at a time, if nothing there, return 0 */
	buf.c_cc[VTIME] = 0;

	if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
		return(-1);
	ttystate = CBREAK;
	ttysavefd = fd;
	return(0);
}


static int tty_raw(int fd)		/* put terminal into a raw mode */
{
	struct termios	buf;

	if (tcgetattr(fd, &save_termios) < 0)
		return(-1);

	buf = save_termios;	/* structure copy */

	buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
					/* echo off, canonical mode off, extended input
					   processing off, signal chars off */

	buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
					/* no SIGINT on BREAK, CR-to-NL off, input parity
					   check off, don't strip 8th bit on input,
					   output flow control off */

	buf.c_cflag &= ~(CSIZE | PARENB);
					/* clear size bits, parity checking off */
	buf.c_cflag |= CS8;
					/* set 8 bits/char */

	buf.c_oflag &= ~(OPOST);
					/* output processing off */

	buf.c_cc[VMIN] = 1;	/* Case B: 1 byte at a time, no timer */
	buf.c_cc[VTIME] = 0;

	if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
		return(-1);
	ttystate = RAW;
	ttysavefd = fd;
	return(0);
}

static int tty_reset(int fd)		/* restore terminal's mode */
{
	if (ttystate != CBREAK && ttystate != RAW)
		return(0);

	if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
		return(-1);
	ttystate = RESET;
	return(0);
}

int tty_reset_in() {
	return tty_reset(STDIN_FILENO);
}

static void tty_atexit(void)		/* can be set up by atexit(tty_atexit) */
{
	if (ttysavefd >= 0)
		tty_reset(ttysavefd);
}


struct termios *tty_termios(void)		/* let caller see original tty state */
{
	return(&save_termios);
}
static void	err_doit(int, const char *, va_list);

char	*pname = NULL;		/* caller can set this from argv[0] */

/* Nonfatal error related to a system call.
 * Print a message and return. */


void err_ret(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error related to a system call.
 * Print a message and terminate. */


static void err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	/* exit(1); */
}

/* Fatal error related to a system call.
 * Print a message, dump core, and terminate. */


static void err_dump(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	abort();		/* dump core and terminate */
	exit(1);		/* shouldn't get here */
}

/* Nonfatal error unrelated to a system call.
 * Print a message and return. */


static void err_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error unrelated to a system call.
 * Print a message and terminate. */


static void err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	exit(1);
}

/* Print a message and return to caller.
 * Caller specifies "errnoflag". */


static void err_doit(int errnoflag, const char *fmt, va_list ap)
{
	int		errno_save;
	char	buf[MAXLINE];

	errno_save = errno;		/* value caller might want printed */
	vsprintf(buf, fmt, ap);
	if (errnoflag)
		sprintf(buf+strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);		/* flushes all stdio output streams */
	return;
}
#endif

#ifdef TESTTTY
main() { int i,c;

	tty_setup();
	for(i = 0 ; i <  100 ; i++ ) {
		Sleep(100);
		c = tty_get_char();
		if (c) {
			fprintf(stderr,"\nc = %d '%c'\n",c,c);fflush(stderr);
		} else {
			fprintf(stderr,"---%d--\r",i);fflush(stderr);

		}
	}
	tty_reset_in();
}
#endif
