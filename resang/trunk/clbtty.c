/* Set terminal (tty) into "raw" mode: no line or other processing done
   Terminal handling documentation:
       curses(3X)  - screen handling library.
       tput(1)     - shell based terminal handling.
       terminfo(4) - SYS V terminal database.
       termcap     - BSD terminal database. Obsoleted by above.
       termio(7I)  - terminal interface (ioctl(2) - I/O control).
       termios(3)  - preferred terminal interface (tc* - terminal control).
*/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void tty_init(void);
void rtty_atexit(void);
int c_rtty_reset(void);
void ttyin(char *c, int *clear);
void tty_set_echo(int *ival);
void echoff(void);
void echo_on(void);
void ttyin(char *c, int *clear);
void ttydly(char *c, int *delayms);
void ttynow(char *c, int *dummy);
void tty_gname(char *name);
void ttyout(char *c);
void fatal(char *mess);
void ttyout_flush(void);

static struct termios orig_termios;  /* TERMinal I/O Structure */
static struct termios cur_tc;		 /* current terminal i/o chacteristics */
static int ttyfd = STDIN_FILENO;     /* STDIN_FILENO is 0 by default */
static int first_time = 1;           /* Need to call tty_init once */
  
void tty_init()
{
    /* check that input is from a tty */
    if (! isatty(ttyfd)) fatal("not on a tty");

    /* store current tty settings in orig_termios */
    if (tcgetattr(ttyfd,&orig_termios) < 0) fatal("can't get tty settings");

    /* register the tty reset with the exit handler */
    if (atexit(rtty_atexit) != 0) fatal("atexit: can't register tty reset");

	first_time = 0;

}

/* exit handler for tty reset */
void rtty_atexit(void)  /* NOTE: If the program terminates due to a signal   */
{                      /* this code will not run.  This is for exit()'s     */
   c_rtty_reset();        /* only.  For resetting the terminal after a signal, */
}                      /* a signal handler which calls tty_reset is needed. */

/* reset tty - useful also for restoring the terminal when this process
   wishes to temporarily relinquish the tty
*/
int c_rtty_reset(void)
   {
    /* flush and reset */
    if (tcsetattr(ttyfd,TCSAFLUSH,&orig_termios) < 0) return -1;
    return 0;
   }
void tty_set_echo(int *ival)
{
	if (*ival == 0) echoff;
	else            echo_on;
}

/* put terminal in raw mode - see termio(7I) for modes */
void echoff(void)
   {
    struct termios raw;


    raw = orig_termios;  /* copy original and then modify below */

    /* input modes - clear indicated ones giving: no break, no CR to NL, 
       no parity check, no strip char, no start/stop output (sic) control */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    /* output modes - clear giving: no post processing such as NL to CR+NL */
    raw.c_oflag &= ~(OPOST);

    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);

    /* local modes - clear giving: echoing off, canonical off (no erase with 
       backspace, ^U,...),  no extended functions */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);

    /* local modes - signal chars (^Z,^C) */
    raw.c_lflag |= (ISIG);


    /* put terminal in raw mode after flushing */
    if (tcsetattr(ttyfd,TCSAFLUSH,&raw) < 0) fatal("can't set raw mode");

	/* save current terminal characteristic */
	cur_tc = raw;
   }

/* put terminal in raw mode - see termio(7I) for modes */
void echo_on(void)
   {
    struct termios raw;

    raw = orig_termios;  /* copy original and then modify below */

    /* input modes - clear indicated ones giving: no break, no CR to NL, 
       no parity check, no strip char, no start/stop output (sic) control */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_iflag |= (ICRNL);


    /* output modes - clear giving: no post processing such as NL to CR+NL */
    raw.c_oflag &= ~(OPOST);

    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);

    /* local modes - canonical off (no erase with 
       backspace, ^U,...),  no extended functions */
    raw.c_lflag &= ~(ICANON | IEXTEN);

    /* local modes - echoing on */
    raw.c_lflag |= (ICANON | ECHO | ISIG);

    /* put terminal in raw echo mode after flushing */
    if (tcsetattr(ttyfd,TCSAFLUSH,&raw) < 0) fatal("can't set raw mode");

	/* save current terminal characteristic */
	cur_tc = raw;
   }

/* Read one char from tty in raw mode */ 

void ttyin(char *c, int *clear)
{
    int bytesread;
	int fd;
	char c_in;

	if (first_time == 1) tty_init();

	if (cur_tc.c_cc[VMIN] == 1 && cur_tc.c_cc[VTIME] == 0){
	}
	else {
	    /* control chars - set return condition: min number of bytes and timer */
	    cur_tc.c_cc[VMIN] = 1; cur_tc.c_cc[VTIME] = 0; /* after 1 byte, no timer    */

	    /* put terminal in raw echo mode after flushing */
	    if (tcsetattr(ttyfd,TCSAFLUSH,&cur_tc) < 0) fatal("can't set raw mode");
	}

	fd = ttyfd;
    bytesread = read(fd, c, 1);
    if (bytesread < 0) fatal("read error");
    if (bytesread == 0) {        /* 0 bytes inputed, must have timed out */
       *c = '\0';
    }
	return;
}

void ttydly(char *c, int *delayms)
{
    int bytesread;
	int fd;
	char c_in;
	int delayts;
	delayts = *delayms / 100;
	if (delayts == 0) delayts = 1;    /* minimum .1 sec delay; 0 means no timer */

	if (first_time == 1) tty_init();

	if (cur_tc.c_cc[VMIN] == 0 && cur_tc.c_cc[VTIME] == delayts){
	}
	else {
	    /* control chars - set return condition: min number of bytes and timer */
	    cur_tc.c_cc[VMIN] = 0; cur_tc.c_cc[VTIME] = delayts; /* after 0 bytes, timer */

 	   /* put terminal in raw echo mode after flushing */
	   if (tcsetattr(ttyfd,TCSAFLUSH,&cur_tc) < 0) fatal("can't set raw mode");
	}


	fd = ttyfd;
    bytesread = read(fd, c, 1);
    if (bytesread < 0) fatal("read error");
    if (bytesread == 0) {        /* 0 bytes inputed, must have timed out */
       *c = '\0';
    }
	return;
}

void ttynow(char *c, int *dummy)
{
    int bytesread;
	int fd;
	char c_in;

	if (first_time == 1) tty_init();

	if (cur_tc.c_cc[VMIN] == 0 && cur_tc.c_cc[VTIME] == 0){
	}
	else {
	    /* control chars - set return condition: min number of bytes and timer */
	    cur_tc.c_cc[VMIN] = 0; cur_tc.c_cc[VTIME] = 0; /* after 1 byte, no timer    */

	    /* put terminal in raw echo mode after flushing */
	    if (tcsetattr(ttyfd,TCSAFLUSH,&cur_tc) < 0) fatal("can't set raw mode");
	}

	fd = ttyfd;
    bytesread = read(fd, c, 1);
    if (bytesread < 0) fatal("read error");
    if (bytesread == 0) {        /* 0 bytes inputed, must have timed out */
       *c = '\0';
    }
	return;
}

void tty_gname(char *name) {

    int bytesread;
	int kk;
	int fd;
	char *myname;

	if (first_time == 1) tty_init(); 
	echo_on();
	if (cur_tc.c_cc[VMIN] == 1 && cur_tc.c_cc[VTIME] == 0){
	}
	else {
	    /* control chars - set return condition: min number of bytes and timer */
	    cur_tc.c_cc[VMIN] = 1; cur_tc.c_cc[VTIME] = 0; /* after 1 byte, 2 sec timer*/

	    /* put terminal in raw echo mode after flushing */
	    if (tcsetattr(ttyfd,TCSAFLUSH,&cur_tc) < 0) fatal("can't set raw mode");
	}
	fd = ttyfd;
	myname = name;
	*myname = '\0';
    bytesread = read(fd, myname, 80);
    if (bytesread < 0) fatal("tty_gname read error");
	myname = myname + bytesread - 1;
	*myname = '\0';

	return;
}

void ttyout(char *c)
{
	int fd;
	char c_out;
	int n_out;

	if (first_time == 1) tty_init();

	fd = ttyfd;
	c_out = *c;
	n_out = 1;
	write(fd, c, n_out);
}

void fatal(char *message)
   {
    fprintf(stderr,"fatal error: %s\n",message);
    exit(1);
   }

void ttyout_flush(void)
   {

    /* flush terminal buffer */
    if (tcsetattr(ttyfd,TCSAFLUSH,&cur_tc) < 0) fatal("ca't flush tty");
   }
