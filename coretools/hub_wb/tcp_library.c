// socket hub and socket library
// Copyright Eric Taylor (c) 2001,2003
// All rights reserved

/*
compile time defs, each is either defined or not and tested via ifdef/ifndef

These tcp/ip routines comprise 2 main layers. The tcp2.* layer is standalone
in that it does not require nor import any of the args, util, etc. functions
meaning that it can be used standalone. Of course, it does reference all the
standard socket stuff.

There are 2 other layers, called the conn and connection layers. These are
both enabled with the CONN_CODE def. This layer of code requires the util
libary.

The primary difference is that the tcp2.* layer does not allocate memory via
malloc and is therefore more compatible with simscript memory management.

UNIXSYS				-- This chooses between unix and windows implementations

The next two should be used together. Theoretically, one could build just the
hub code as a function and invoke it from another program. Realistically, this
is never done; the HUB define will define a main function that calls the hub
function.

HUBMAIN				-- This enables the main function that invokes the hub code
HUB					-- actual hub code - should be used with HUBMAIN

I64					-- use this when compiling on a 64 bit linux system

These next 3 are defined automatically if HUBMAIN is defined.

CONN_CODE			-- defines the connection layer, atop the tcp2.* layer
TEP_CODE			-- defines the teprintf function
DEBUG_CODE			-- some debug code, not called except via debuggers

HIDE_TCP			-- if defined, the tcp2.* functions are declared static

*/


#define VERS "2.26"

//#define VAX 1
#ifdef VAX
#define HUBMAIN 1
#define UNIXSYS 1
#define HUB 1
#endif

#ifdef HIDE_TCP
#define TCP_STATIC static
#else
#define TCP_STATIC
#endif

#ifdef I64
#define IN_T long long
#else
#define IN_T int
#endif

#ifdef HUBMAIN
#define CONN_CODE
#define TEP_CODE
#define DEBUG_CODE
#endif

#ifdef FULL_LIBRARY
#define CONN_CODE
#endif

#ifdef TCP_LIBRARY_ONLY
#undef CONN_CODE
#undef TEP_CODE
#undef DEBUG_CODE
#endif

//#pragma warning( disable : 4201 4127 4244 4189 4057)   /*      */
//#pragma warning( disable : 4100 )   /*  un refed parameter  */
//#pragma warning( disable : 4001 )   /*  single line comment */
//#pragma warning( disable : 4505 )   /*  unrefed function    */
/* *****************************************************************************
 simples.c - Simple TCP server using Winsock 1.1 on PC and sockets on unix

To build on unix:

   gcc -DTESTING -DUNIXSYS simples.c -lsocket -lnsl         solaris
   gcc -DTESTING -DUNIXSYS simples.c                        linux

The symbol TESTING is optional, it will generate the test code if defined

On vax/vms (must remove all / / type comments first)
cc xxx.c   - no particular options needed

$ here := SYS$DISK:'F$DIRECTORY()'
$ link/exe=hub  /nouserlib/nomap   sys$input/opt,                -
        'here'simples.obj,args.obj,util.obj
MULTINET_COMMON_ROOT:[MULTINET]MULTINET_SOCKET_LIBRARY.EXE/share
sys$library:vaxcrtl.exe/share


***************************************************************************** */
#ifdef UNIXSYS
#  ifdef VAX
#	include "multinet_root:[multinet.include.sys]types.h"
#	include "multinet_root:[multinet.include]errno.h"
#	include "multinet_root:[multinet.include.sys]time.h"
#	include "multinet_root:[multinet.include.sys]socket.h"
#	include "multinet_root:[multinet.include.netinet]in.h"
#	include "multinet_root:[multinet.include]netdb.h"

#	define SOCKET unsigned int
#	define INVALID_SOCKET (0xffffffff) 
#	define CONN_obj PCONNOBJ
#	define closesocket socket_close
#	include signal
#	define Sleep(a)  vaxWait(a)
void vaxWait(int n) {
     float t;
     t = (float)n/1000.0;
     lib$wait(&t);
}
#  else



#	include <sys/time.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <unistd.h>
#	include <netdb.h>
#	include <netinet/in.h>
#	include <errno.h>
#	include <arpa/inet.h>
#	define SOCKET unsigned int
#	define INVALID_SOCKET (0xffffffff) 
#	define closesocket close 
//# define Sleep(a) sleep(a/1000)
#	define Sleep(a) usleep(a*1000)
#	define CONN_obj PCONNOBJ
#  endif
#else
# define CONN_obj PCONNOBJ
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <winsock2.h>
#endif

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef CONN_CODE
#include "utils.h"
#include "args.h"
#endif

typedef struct conn_obj {
	SOCKET sock1;	//main socket
	SOCKET sock2;	//listen socket
	int  port;      //listen port number
	int  listener;  //listener only
	char *buf1;		//buffer one
	char *buf2;		//buffer two
	int reuse;
	int	opt2;		//option for setting the server send/rec buffer sizes 
	int	opt3;
	int term;		//terminator char for records
	int bufsize;
	int error;
	int holder1[5];
	int holder2[5];
	char errorstr[200];

	char other_name[100];
	char other_namen[100];
	int  other_port;
	int original_port;	// holds a listen port number

} CONNOBJ, *PCONNOBJ;

typedef struct conn_obj_ref {	// object to place in a linked list to track all connections/listeners etc.
	PCONNOBJ cptr;
	int		 ctype;				// 1/2 for type of connection 1=connection, 2=listener
} CONNOBJREF, *PCONNOBJREF;

#include "tcp_library.h"

static char errorstr[200];
static int    error    = 0;
static char * inaddr;
static char * inaddrn;
static int    inport;

#ifdef HUBMAIN
static int net = 0; // flag, 0/1/control, input global to teprintf (only)  to set up logging to the net
#endif
static PCONNOBJREF conn_lh = 0; // list of connection references

#ifndef UNIXSYS
#define WIN_ERR(e) } else if (err == e) {return #e;
#define Static static
static char *erris(int err) {
    if (0) {
        WIN_ERR(WSAVERNOTSUPPORTED)         WIN_ERR(WSAEWOULDBLOCK)     WIN_ERR(WSAECONNRESET)
        WIN_ERR(WSANOTINITIALISED)          WIN_ERR(WSAEINPROGRESS)     WIN_ERR(WSAENOBUFS)
        WIN_ERR(WSAEDISCON)                 WIN_ERR(WSAEALREADY)        WIN_ERR(WSAEISCONN)
        WIN_ERR(WSAENOMORE)                 WIN_ERR(WSAENOTSOCK)        WIN_ERR(WSAENOTCONN)
        WIN_ERR(WSAECANCELLED)              WIN_ERR(WSAEDESTADDRREQ)    WIN_ERR(WSAESHUTDOWN)
        WIN_ERR(WSAEINVALIDPROCTABLE)       WIN_ERR(WSAEMSGSIZE)        WIN_ERR(WSAETOOMANYREFS)
        WIN_ERR(WSAEINVALIDPROVIDER)        WIN_ERR(WSAEPROTOTYPE)      WIN_ERR(WSAETIMEDOUT)
        WIN_ERR(WSAEPROVIDERFAILEDINIT)     WIN_ERR(WSAENOPROTOOPT)     WIN_ERR(WSAECONNREFUSED)
        WIN_ERR(WSASYSCALLFAILURE)          WIN_ERR(WSAEPROTONOSUPPORT) WIN_ERR(WSAELOOP)
        WIN_ERR(WSASERVICE_NOT_FOUND)       WIN_ERR(WSAESOCKTNOSUPPORT) WIN_ERR(WSAENAMETOOLONG)
        WIN_ERR(WSATYPE_NOT_FOUND)          WIN_ERR(WSAEOPNOTSUPP)      WIN_ERR(WSAEHOSTDOWN)
        WIN_ERR(WSA_E_NO_MORE)              WIN_ERR(WSAEPFNOSUPPORT)    WIN_ERR(WSAEHOSTUNREACH)
        WIN_ERR(WSA_E_CANCELLED)            WIN_ERR(WSAEAFNOSUPPORT)    WIN_ERR(WSAENOTEMPTY)
        WIN_ERR(WSAEREFUSED)                WIN_ERR(WSAEADDRINUSE)      WIN_ERR(WSAEPROCLIM)
        WIN_ERR(HOST_NOT_FOUND)             WIN_ERR(WSAEADDRNOTAVAIL)   WIN_ERR(WSAEUSERS)
        WIN_ERR(TRY_AGAIN)                  WIN_ERR(WSAENETDOWN)        WIN_ERR(WSAEDQUOT)
        WIN_ERR(NO_RECOVERY)                WIN_ERR(WSAENETUNREACH)     WIN_ERR(WSAESTALE)
        WIN_ERR(NO_DATA)                    WIN_ERR(WSAENETRESET)       WIN_ERR(WSAEREMOTE)
        WIN_ERR(NO_ADDRESS)                 WIN_ERR(WSAECONNABORTED)    WIN_ERR(WSASYSNOTREADY)
        WIN_ERR(WSAEINTR)                   WIN_ERR(WSAEBADF)           WIN_ERR(WSAEACCES)
        WIN_ERR(WSAEFAULT)                  WIN_ERR(WSAEINVAL)          WIN_ERR(WSAEMFILE)
    } else if(err == 0) {
		return "EOF or CLOSE";
    } else {
        return "";
    }
}
#endif
static char * xstrchr(char *p, int c) {
	for(;;) {
		if (*p == c) {
			return p;
		}
		p++;
	}
}
static int ibetween(long a,long b,long c){
        if (b <= a) {
           return a;
        } else if (b >= c) {
           return c;
        }
        return b;
}

// on windows, the symbol NDEBUG means release mode, memcpy generates a rep instruction
// and is faster. We assume the way our moveb works so that an overlapping buffer where
// the input is at a higher address than the output, but they overlap will result in a
// sliding down in memory of the buffer. memcpy on windows will do that,but we can't be
// sure otherwise, so we do the code ourselves on other systems

#ifdef NDEBUG
#define moveb(__src,__dest,__num) memcpy(__dest,__src,__num)
#endif

#ifndef moveb
static void moveb(char *f, char *t, int n) {
	register int i;
	for(i = 0 ; i <  n ; i++ ) {
		t[i] = f[i];	// seems faster
		// *t++ = *f++;
	}
}
#endif

#ifdef DEBUG_CODE
static void movebu(char *f, char *t, int n) {
	register int i;
	for(i = n ; i >0   ; i-- ) {
		t[i] = f[i];	
	}
}
#endif

static void mSleep(int n) {
	Sleep(n);
}
// -----------------------------------
TCP_STATIC void tcp2_close_socket(SOCKET s) {
    closesocket (s);
    return ;
}
// -----------------------------------
TCP_STATIC char *tcp2_get_error_mess() {
    return errorstr;
}
// -----------------------------------
TCP_STATIC int tcp2_get_error() {
    return error;
}
// -----------------------------------
TCP_STATIC char * tcp2_server_inaddr() {
    return inaddr;
}
// -----------------------------------
TCP_STATIC char * tcp2_server_inaddrn() {
    return inaddrn;
}
// -----------------------------------
TCP_STATIC int    tcp2_server_inport() {
    return inport;
}
// -----------------------------------
static int    tcp2_is_ready(SOCKET s,int a_read) {
    int n;
    fd_set rset;
    struct timeval t;
    t.tv_sec = 0;
    t.tv_usec= 0;
    FD_ZERO(&rset);
    FD_SET(s,&rset);
    if (a_read == 1) {
        n = select(s+1,&rset,NULL,NULL,&t);  // read
    } else {
        n = select(s+1,NULL,&rset,NULL,&t);  // write
    }
    //fprintf(stderr,"    s=%d n=%d\n",s,n);
    return n;
}
TCP_STATIC int tcp2_server_ready_read(SOCKET s) {
    return tcp2_is_ready(s,1);
}
TCP_STATIC int tcp2_client_ready_read(SOCKET s) {
    return tcp2_is_ready(s,1);
}
TCP_STATIC int tcp2_server_ready_write(SOCKET s) {
    return tcp2_is_ready(s,0);
}
TCP_STATIC int tcp2_client_ready_write(SOCKET s) {
    return tcp2_is_ready(s,0);
}
// -----------------------------------
// init a server connection and optionaly wait for inbound connection
//      port        port number to declare for this server
//      listens     pointer to a socket variable to return the server listen socket, user must zero initially
//      poll        0/1 as to whether to use polling mode, where this returns if no connection waiting
//      
// returns: either, 0, INVALID_SOCKET (which is 0xffffffff) or a valid non-zero socket handle
// INVALID_SOCKET is returned only if poll is 1 and no inbound is ready yet, but listens will be set
// if 0 returned, then the error can be determined by tcp2_server_error() or tcp2_get_error_mess()
// -----------------------------------
TCP_STATIC SOCKET tcp2_server_init(int port,SOCKET *listens,int poll, int reuse, int lookup,int buf_size) {

    //char Buffer[128];
    //char *interface= NULL;
    //int retval;
    //int i;
#ifdef UNIXSYS  
    int bn;
    socklen_t fromlen;
#endif  
    int socket_type;
    struct sockaddr_in local, from;
    SOCKET listen_socket, msgsock;
#ifndef UNIXSYS 
    WSADATA wsaData;
    int fromlen;
     inaddr =    "unknown";
     inport =    0;
    
    if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) {
        error = WSAGetLastError();
        sprintf(errorstr,"WSAStartup failed with error %d",WSAGetLastError());
        WSACleanup();
        return 0;
    }
#endif
    
    socket_type = SOCK_STREAM;

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY; 

    /* 
     * Port MUST be in Network Byte Order
     */
    local.sin_port = htons((unsigned short )port);
    if (listens && *listens == 0) {
        listen_socket = socket(AF_INET, socket_type,0); // TCP socket
#ifndef UNIXSYS
        if (listen_socket == INVALID_SOCKET){
            error = WSAGetLastError();
            sprintf(errorstr,"socket() failed with error %d %s",error,erris(error));
            WSACleanup();
            return 0;
        }
#else
        if(listen_socket == (SOCKET) -1 ){
            error = errno;
            sprintf(errorstr,"socket() failed with error %s",strerror(errno));
            return 0;
        }
#endif
        //
        // bind() associates a local address and port combination with the
        // socket just created. This is most useful when the application is a 
        // server that has a well-known port that clients know about in advance.
        //
//fprintf(stderr,"now bind\n");
		if (reuse) { int on,stat;  // set socket to reuse if asked to
			on = 1;
			stat = setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof (on)); 
#ifndef UNIXSYS
			if (stat == SOCKET_ERROR) {
				error = WSAGetLastError();
				sprintf(errorstr,"set socket SO_REUSEADDR: error %d %s",error,erris(error));
				WSACleanup();
				return 0;
			}
#else
			if(stat == -1 ){
				error = errno;
				sprintf(errorstr,"set socket SO_REUSEADDR failed with error %s",strerror(errno));
				return 0;
			}
#endif
		}


    if (buf_size > 0) 
    {
      int n=0,n2=0,bsize; //WSAEFAULT
//      int m=4;
		bsize = buf_size;
        n  = setsockopt(listen_socket, SOL_SOCKET, SO_SNDBUF,(char *)&bsize, sizeof(int));
        if(n==0) {
			bsize = buf_size;
			n2 = setsockopt(listen_socket, SOL_SOCKET, SO_RCVBUF,(char *)&bsize, sizeof(int));
		}
//		fprintf(stderr,"after setsockopt, buf_size is now %d n=%d n2=%d\n",buf_size,n,n2);
#ifndef UNIXSYS
        if (n == SOCKET_ERROR  || n2 == SOCKET_ERROR) {
            error = WSAGetLastError();
            sprintf(errorstr,"Server: Error setting server buffer size %d %s",error,erris(error));
            WSACleanup();
        }
#else
        if ( (n != 0)  ||  (n2 !=0)  ) {
			 error = errno;
			 sprintf(errorstr,"Server: Error setting server buffer size %s",strerror(errno) );
				close(listen_socket);
				return 0;
        }
#endif

    }







#ifndef UNIXSYS
        if (bind(listen_socket,(struct sockaddr*)&local,sizeof(local) ) == SOCKET_ERROR) {
            error = WSAGetLastError();
            if (error == WSAENETDOWN) {
                strcpy(errorstr,"bind() failed: The network subsystem has failed.");
            } else if (error == WSAEADDRINUSE) {
                strcpy(errorstr,"bind() failed: A process on the machine is already bound to the same fully-qualified address and no SO_REUSEADDR.");
            } else if (error == WSAEADDRNOTAVAIL) {
                strcpy(errorstr,"bind() failed: The specified address is not a valid address for this machine.");
            } else if (error == WSAEINVAL) {
                strcpy(errorstr,"bind() failed: The socket is already bound to an address.");
            } else if (error == WSAENOBUFS) {
                strcpy(errorstr,"bind() failed: Not enough buffers available, too many connections.");
            } else {
                sprintf(errorstr,"bind() failed: error %d %s",error,erris(error));
            }
            WSACleanup();
            return 0;
        }
#else
        bn = bind(listen_socket,(struct sockaddr*)&local,sizeof(local));
        if(bn == -1 ){
            error = errno;
            sprintf(errorstr,"bind() failed with error %s",strerror(errno));
            return 0;
        }
#endif
//fprintf(stderr,"go listen\n");

#ifndef UNIXSYS

        if (listen(listen_socket,20000) == SOCKET_ERROR) {
            error = WSAGetLastError();
            sprintf(errorstr,"listen() failed with error %d %s",error,erris(error));
            WSACleanup();
            return 0;
        }
#else
        if (listen(listen_socket,200) == -1) {
            error = errno;
            sprintf(errorstr,"listen() failed with error %s",strerror(errno));
            return 0;
        }
#endif
        if (listens) {
            *listens = listen_socket;
        }
		if (poll) {
			return INVALID_SOCKET; // first time in, just set the listen, indicate not ready yet
		}
    } else {
        listen_socket = *listens;
    }
    


//fprintf(stderr,"'Listening' on port %d\n",port);
    if (poll) {
        if (tcp2_server_ready_read(listen_socket) == 0) {
            return INVALID_SOCKET;
        }
    }
//fprintf(stderr,"ready to accept\n");
    fromlen =sizeof(from);
    msgsock = accept(listen_socket,(struct sockaddr*)&from, &fromlen);
    if (msgsock == INVALID_SOCKET) {
#ifndef UNIXSYS
        error = WSAGetLastError();
        sprintf(errorstr,"accept() error %d %s",error,erris(error));
        WSACleanup();
        return 0;
#else
        error = errno;
        sprintf(errorstr,"accept() error %s",strerror(errno));
        return 0;
#endif
    }
    //fprintf(stderr,"accepted connection from %s, port %d\n", inet_ntoa(from.sin_addr),htons(from.sin_port)) ;

     inaddr =    (char *)inet_ntoa(from.sin_addr);
	 inaddrn = inaddr;
     inport =    htons(from.sin_port) ;
	 if(lookup) { struct hostent *p;
		p = gethostbyaddr((char *)&from.sin_addr,4,AF_INET);
		if (p) {
			inaddr = p->h_name;
		}
		//fprintf(stderr,"p=%x\n",p);
	 }
        
    return msgsock; // return the socket to use for recv and send
}
// ----------------------------------- ???????? need to protect against output error
// write a record 
//      s			socket
//      buf         buffer to output
//      size        size of output buffer
//      holder[5]   array of indices must be zeroed by caller one time init 
//					[0..4] used only by this routine, do not modify
//      poll        0/1 whether or not to poll
//      
// returns: either, -1, 0, or >0
//
//			  0  means an error, see error/errorstr, if error == 0 then EOF (close on other end)
//			 -1 in poll mode means not finished yet and data is not ready to send, do work then come back
//			 >0 record completely written
// -----------------------------------
TCP_STATIC int tcp2_write_record(SOCKET s, char * buf, int  size, int holder[5], int poll) {
	int n,rem,ready;
#ifndef UNIXSYS
	int ecode;
#endif
	for(;;) {
		if (size <= 0) {
			return -1; // just in case, ignore a write of 0 bytes
		}
		if (poll) {
			ready = tcp2_is_ready(s,0);
			//fprintf(stderr,"                         ready = %d\n",ready);
			if (!ready) {
				return -1;
			}
		}
		if (holder[0] == 0) {
			n = send(s,buf,size,0);
#ifndef UNIXSYS
			if (n<0) {
				ecode = WSAGetLastError();
				if (ecode == WSAENOBUFS) { // on windows, we can get this error, but recover like a would block
					ecode = WSAEWOULDBLOCK; // so pretend it was a blocking error (select should not have said ready!, but it did!)
				}
			}
			if (n < 0 && ecode == WSAEWOULDBLOCK) {
				if(!poll) {
					mSleep(1000);
					continue;
				}
				return -1;
			}
#else
			if (n < 0 && errno == EWOULDBLOCK) {
				if(!poll) {
					Sleep(1000);
					continue;
				}
				return -1;
			}
#endif
			if (n < 0) {
#ifndef UNIXSYS
			error = WSAGetLastError();
			sprintf(errorstr,"send error %d %s",error,erris(error));
			WSACleanup();
			return 0;
#else
			error = errno;
			sprintf(errorstr,"send error %s",strerror(errno));
			return 0;
#endif
			}
			if (n < size) { // this code should allow for zero being sent
				holder[1] = size;
				holder[0] = n;
				if (!poll) {
					continue;
				}
				return -1;
			} else {
				return n;
			}
		} else {
			rem = size - holder[0];
			n = send(s,&buf[holder[0]],rem,0);
			if (n < rem) {
#ifndef UNIXSYS
				if (n<0) {
					ecode = WSAGetLastError();
					if (ecode == WSAENOBUFS) {
						ecode = WSAEWOULDBLOCK;
					}
				}
				if (n < 0 && ecode != WSAEWOULDBLOCK) {
					error = WSAGetLastError();
					sprintf(errorstr,"send error %d %s",error,erris(error));
					WSACleanup();
					return 0;
				} else if (n < 0 && ecode == WSAEWOULDBLOCK) {
					if(!poll) {
						mSleep(1000);
						continue;
					}
					return -1;
				}
#else
				if (n < 0 && errno != EWOULDBLOCK) {
					error = errno;
					sprintf(errorstr,"send error %s",strerror(errno));
					return 0;
				} else if (n < 0 && errno == EWOULDBLOCK) {
					return -1;
					if(!poll) {
						Sleep(1000);
						continue;
					}
				}
#endif
				if (n>=0) { // changed this to include 0 being sent, but never saw this so far, note: not
							// a problem, if n == 0, then holder[1]/[0] just compute to what they were before
							// this code only runs if we had already sent only a partial block, and if n==0 it
							// simply means we got no further this time. Should not happen though if select said do it.
						holder[1] = size;
						holder[0] = size - rem+n;
				}
				if (!poll) {
					continue;
				}
				return -1;
			} else {
				holder[1] = 0;
				holder[0] = 0;
				return size;
			}
		}
	}
	return 0;
}
// -----------------------------------
// read a record 
//      s			socket
//      buf         buffer to accumulate data MUST NOT MODIFY THIS BUFFER, copy or treat as read only
//      maxbuf      size of input buffer
//      holder[5]   array of indices must be zeroed by caller one time init 
//					[0] returned as the index where the current data is
//					[1..4] used only by this routine, do not modify
//      poll        0/1 whether or not to poll
//      term        byte to indicate end of record
//      
// returns: either, -1, 0, or >0
//
//			  0  means an error, see error/errorstr, if error == 0 then EOF (close on other end)
//			 -1 in poll mode means not finished yet and data is not ready to read, do work then come back
//			 >0 data or if return is maxbuf, data has no terminator
// if returns size = maxbuf, then no terminator was seen and data buffer is full
// -----------------------------------
TCP_STATIC int tcp2_read_record(SOCKET s, char * buf, int  pmaxbuf, int holder[5], int poll, int term) {
	int maxread=0,n=0,ready,kur=0,i,r0;
	char *p=NULL,*q;
	int maxbuf = pmaxbuf-1;
	static const int BRETURN = 0;  // where the returned data in buffer is, size is n
	static const int BRSIZE  = 1;  // return size of last call
	static const int BSIZE   = 2;  // amount of data in the entire buffer
	static const int BSAVE1   = 3;  // save area
	static const int BSAVE2   = 4;  // save area

	error = 0;
	errorstr[0] = '\0';
	r0=0;
	if (poll && holder[BSAVE2] > 0 ) {
		p		= &buf[	holder[BSAVE1]];
		maxread	= holder[BSAVE2];
		kur = holder[BSAVE1];
	} else {
		if (poll && holder[BSIZE] == 0) { // if polling and no data waiting in our buffer
			ready = tcp2_is_ready(s,1);
			if (!ready) {
				return -1;
			}
			r0 = 1; // indicate we did 1 ready test, and it was ready (saves calling it twice)
		}
		if (holder[BSIZE] == 0) { // fresh case, start with a read into entire buffer, no holdovers
			p = buf;
			maxread = maxbuf;
			kur = 0;
		} else if (holder[BSIZE] > 0) {
			p = &buf[holder[BRSIZE] + holder[BRETURN]];
			if (term == 0 && 0) {
				i = strlen(p)+1; // locate first terminator
			} else {
				q = xstrchr(p,term); // handle other than 0 for a terminator
				i = q-p;
				i++;
			}
			n = holder[BSIZE] -(p-buf); // size of remaining data held over 
			if (i > n) {  // filled buffer with no terminator
				moveb(p,buf,n+1);

				//buf[n+1] = (char)0xff;
				//moveb(&buf[n+1],&buf[n+2],maxbuf-n-1); // temp debug code

				p = &buf[n];
				maxread = maxbuf - n;
				kur = n;
			} else if (i == n) { // exactly one record in buffer
				holder[BRETURN] = p-buf;
				holder[BRSIZE]  = i;
				holder[BSIZE]   = 0;
				holder[BSAVE2] =  0;
				return i;
			} else { // multiple data in buffer (i < n)
				holder[BRETURN] = p-buf;
				holder[BRSIZE]  = i;
				holder[BSAVE2] =  0;
				return i;
			}
		}

	} // check for polling and restart
	for(;;) {
		if (poll) {
			if(r0) {
				ready = 1;
				r0 = 0;
			} else {
				ready = tcp2_is_ready(s,1);
			}
			if (!ready) {
					holder[BSAVE1] = p-buf;
					holder[BSAVE2] = maxread;
				return -1;
			}
			n = recv(s,p,maxread,0);  // read as much as possible into buffer &buf[0]
		} else {
			if (term == -1) {
				n = recv(s,p,maxread,0);  // read as much as possible into buffer &buf[0] as a stream
			} else {
				n = recv(s,p,1,0);        // read a single character if not polling and term not -1
			}
		}
		holder[BSAVE2] =  0;
		if (n == 0) {
			error = 0;
			strcpy(errorstr,"EOF or CLOSE");
			return 0;
		} else if (n < 0) {
			// handle errors
#ifndef UNIXSYS
			error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK) {
				mSleep(200);
				continue;
			}
			sprintf(errorstr,"recv error %d %s",error,erris(error));
			WSACleanup();
			return 0;
#else
			error = errno;
			if (error == EWOULDBLOCK) {
				Sleep(200);
				continue;
			}
			sprintf(errorstr,"recv error %s",strerror(errno));
			return 0;
#endif
		} else if (n > 0) {			// data in buffer, but not yet full to max
			// handle new data in buffer
			n = n + kur;			// compute current data size
			p = buf;				// which begins at beginning of buffer
			p[n] = (char)term;		// add a terminating char so search must succeed 

			if (term == 0 && 0) {
				i = strlen(p)+1; // locate first terminator
			} else if (term == -1) {
				q = &p[n]-1;
				i = q-p;
				i++;
			} else {
				q = xstrchr(p,term); // handle other than 0 for a terminator
				i = q-p;
				i++;
			}
			if (i > n) {  // filled buffer with no terminator
				maxread = maxbuf - n;
				kur = n;
				p = &buf[n];
				if (maxread) {
					continue;
				} else {
					holder[BRETURN] = 0;
					holder[BRSIZE] = 0;
					holder[BSIZE] = 0;
					holder[BSAVE1] =  0;
					holder[BSAVE2] =  0;
					return pmaxbuf;
				}
			} else if (i == n) { // exactly one record in buffer
				holder[BRETURN] = 0;
				holder[BRSIZE] = 0;
				holder[BSIZE] = 0;
				return n;
			} else { // multiple data in buffer (i < n)
				holder[BRETURN] = 0;
				holder[BRSIZE] = i;
				holder[BSIZE] = n;
				return i;
			}
		} else {
			// cannot happen
		}
		break;
	}
	return n;
}
// -----------------------------------
// parse a server name 
//      
//    format:    ipaddress:port[+offset]  
//    offset	 in,out,stat,control,alternate (1st char is sufficient)                   
//      
// returns: either, 0, or a port number, and server is null terminated  
// -----------------------------------
int tcp2_address(char *name,char *server,int offset) {
	char *p,*q;
	int pt=0,n=0;
	strcpy(server,name);
	p = strchr(&server[offset],':');
	if (p) {
		*p++ = 0;
		if (*p == ':') { // double colon
			p++;
		}
		q = strchr(p,'+');
		if (q) {
			*q++ = 0;
			if (q[0] == 'i'						 ) { // in
				n=1;
			} else if (q[0] == 'o'				 ) { // out
				n=0;
			} else if (q[0] == 'O'				 ) { // Out altern
				n=7;
			} else if (q[0] == 's' || q[0] == 'S') { // stat
				n=2;
			} else if (q[0] == 'c' || q[0] == 'C') { // control
				n=3;
			} else if (q[0] == 'I' || q[0] == 'a') { // alternate (In)
				n=4;
			} else if (q[0] == 'd'               ) { // duplex
				n=5;
			} else if (q[0] == 'D'               ) { // Duplex 2
				n=6;
			} else {
				return 0;;
			}
		}
		pt = atoi(p) + n;
	} else {
		return 0;
	}
	if (strlen(server)== 1 && server[0] == '.') {
		strcpy(server,"localhost");
	}
	return pt;

}

// -----------------------------------
// init a client connection 
//      kind          1,2,3 
//                          1 = a number
//                          2 = a name 
//                          3 = a name first, if it fails, try a number
//      ip          ip address or host name, depending on kind 
//      pport       port number to declare for this server
//      buf_size    sending buffer size, 0=use default
//      myport      port number of client
//      
// returns: either, 0, or a valid non-zero socket handle
// -----------------------------------
TCP_STATIC SOCKET tcp2_client_init(int kind, char * server_name, int port_number,int buf_size,int myport) {

    unsigned int addr;
	int len,j,aname=0;
	
    int socket_type = SOCK_STREAM;
    struct sockaddr_in server,local;
    struct hostent *hp;
    SOCKET  conn_socket;
    unsigned short port;
#ifndef UNIXSYS
    WSADATA wsaData;

    if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) {
        error = WSAGetLastError();
        sprintf(errorstr,"WSAStartup failed with error %d",error);
        WSACleanup();
        return 0;
    }
#endif  
    port =  (unsigned short)  port_number;
    
    //
    // Attempt to detect if we should call gethostbyname() or gethostbyaddr()
    // 
	len = strlen(server_name);
	for(j = 0 ; j < len  ; j++ ) {
		if (server_name[j] == ibetween('0',server_name[j],'9') || server_name[j] == '.') {
		} else {
			aname = 1;
			break;
		}
	}
    if (aname &&  ( (kind == 2)  ||  (kind == 3))   /*|| (kind == 0 && isalpha(server_name[0])) */  ) {   /* server address is a name */
        hp = gethostbyname(server_name);
        if (hp == NULL && kind == 3) {
            addr = inet_addr(server_name);
            hp = gethostbyaddr((char *)&addr,4,AF_INET);
        }

    } else  { 
    /* Convert nnn.nnn address to a usable one */
        addr = inet_addr(server_name);
        hp = gethostbyaddr((char *)&addr,4,AF_INET);
    }
    if (hp == NULL && aname) {
#ifndef UNIXSYS
        error = WSAGetLastError();
        sprintf(errorstr,"Client: Cannot resolve address [%s]:  %d %s",server_name,error,erris(error));
        WSACleanup();
#else
        error = errno;
        sprintf(errorstr,"Client: Cannot resolve address [%s]:  %s",server_name,strerror(errno));
#endif
        return 0;
    }
	memset(&server,0,sizeof(server));
	if (hp == NULL) {
		//
		// no domain server or not in a host table entry
		// so assume ipv4 32 bit value from inet_addr should work
		//
		memcpy(&(server.sin_addr),&addr,4);
		server.sin_family = AF_INET;

	} else {
		//
		// Copy the resolved information into the sockaddr_in structure
		//
		memcpy(&(server.sin_addr),hp->h_addr,hp->h_length);
		server.sin_family = hp->h_addrtype;
	}
		server.sin_port = htons(port);


    conn_socket = socket(AF_INET,socket_type,0); /* Open a socket */
    if ((int) conn_socket <0 ) {
#ifndef UNIXSYS
        error = WSAGetLastError();
        sprintf(errorstr,"Client: Error Opening socket:  %d %s",error,erris(error));
        WSACleanup();
#else
        error = errno;
        sprintf(errorstr,"Client: Error Opening socket:  %s",strerror(errno));
#endif
        return 0;
    }
    if (buf_size > 0) 
    {
      int n,n2=0,bsize; //WSAEFAULT
//      int m=4;
		bsize = buf_size; // on windows, they change bsize w/o telling you
        n  = setsockopt(conn_socket, SOL_SOCKET, SO_SNDBUF,(char *)&bsize, sizeof(int));
        if(n==0) {
			bsize = buf_size;
			n2 = setsockopt(conn_socket, SOL_SOCKET, SO_RCVBUF,(char *)&bsize, sizeof(int));
		}
    //  n = getsockopt(conn_socket, SOL_SOCKET, SO_SNDBUF,(char *)&buf_size, &m);
#ifndef UNIXSYS
        if (n == SOCKET_ERROR  || n2 == SOCKET_ERROR) {
            error = WSAGetLastError();
            sprintf(errorstr,"Client: Error setting client buffer size %d %s",error,erris(error));
            WSACleanup();
        }
#else
        if ( (n != 0)  ||  (n2 !=0) ) { 
			 error = errno;
			 sprintf(errorstr,"Client: Error setting client buffer size %s",strerror(errno));
				close(conn_socket);
				return 0;
        }
#endif

    }


	if(myport) {int bstat;
		
		local.sin_family		= AF_INET;
		local.sin_addr.s_addr	= INADDR_ANY; 

		/* 
		 * Port MUST be in Network Byte Order
		 */
		local.sin_port = htons((unsigned short )myport);
		if( (bstat = bind(conn_socket,(struct sockaddr*)&local,sizeof(local) ))  == 
#ifndef UNIXSYS
			SOCKET_ERROR
#else
			-1
#endif
			) {
#ifndef UNIXSYS
			error = WSAGetLastError();
			sprintf(errorstr,"Client: bind failed: %d %s",error,erris(error));
			WSACleanup();
#else
			error = errno;
			sprintf(errorstr,"Client: bind failed: %s",strerror(errno));
			close(conn_socket);
#endif
			return 0;
		}

	}


    //printf("Client connecting to: %s\n",hp->h_name);
#ifndef UNIXSYS
    if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR) {
        error = WSAGetLastError();
        sprintf(errorstr,"Client: connect to %s:%d failed:  %d %s",server_name,port,error,erris(error));
        WSACleanup();
        return 0;
    }
#else
    if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))== -1) {
        error = errno;
        sprintf(errorstr,"Client: connect to %s:%d failed:  %s",server_name,port,strerror(errno));
		close(conn_socket);
        return 0;
    }
#endif
    return conn_socket;
}
// ---------------------------
// An access list looks like
//
//   type ip_address [mask]			type is read,write, or none  ip_address is xxx.xxx.xxx.xxx or nnn.nnn.nnn.nnn
//
//   when ip_address is numerical, the values are decimal or 0xnn hex (only dec supported yet)
//   for now, only numerical ip addresses are supported
//
// ---------------------------
typedef struct a_list {			
	char a_type;						// type of access, read, write, none, stat, control
	int  a_ip[4];
	int	 a_mask[4];
} ALIST, *PALIST;


// -----------------------------------
//  open and read an access list file
//       create a PALIST list
// -----------------------------------
#ifdef HUB
PALIST con_access_list_read(char *file) {
	FILE *io;
	PALIST alh,p;
	extern int tokens();
	char buf[500],out[500],*av[500],*stat;
	char buf2[500],out2[200],*av2[50];
	int i,j,ac,ac2,l;
	io = fopen(file,"r");
	if (!io) {
		return NULL;
	}
	alh = list_newlh();
	for(i = 1 ; i <  99999 ; i++ ) {
		stat = fgets(buf,sizeof(buf)-1,io);
		if (stat == NULL) {
			break;
		}
		if (buf[0] == '#' || buf[0] == '!') {
			continue;
		}
		ac = tokens(buf,av,out);
		p = list_new(sizeof(ALIST),"alist");
		if (ac >= 1) {
			if (av[0][0] == 'r' || av[0][0] == 'R') {
				p->a_type = 'r';
			} else if (av[0][0] == 'w' || av[0][0] == 'W') {
				p->a_type = 'w';
			} else if (av[0][0] == 'n' || av[0][0] == 'N') {
				p->a_type = 'n';
			} else if (av[0][0] == 's' || av[0][0] == 'S') {
				p->a_type = 's';
			} else if (av[0][0] == 'c' || av[0][0] == 'C') {
				p->a_type = 'c';
			} else if (av[0][0] == 'a' || av[0][0] == 'A') {
				p->a_type = 'a';
			} else {
				fprintf(stderr,"unknown access type in access list line %d\n",i);
				continue; // error
			}
		}
		for(j = 0 ; j <  4 ; j++ ) {
			p->a_ip[j] = p->a_mask[j] = -1;
		}
		if (ac == 1) {
		} else if (ac == 2) {
			strcpy(buf2,av[1]);
			l = strlen(buf2);
			for(j = 0 ; j <  l ; j++ ) {
				if (buf2[j] == '.') {
					buf2[j] = ',';
				}
			}
			ac2 = tokens(buf2,av2,out2);
			if (ac2 == 4) {
				for(j = 0 ; j <  4 ; j++ ) {
					p->a_ip[j] = atoi(av2[j]);
				}
			} else {
				fprintf(stderr,"error in access list line %d '%s'\n",i,buf2);
				continue;// error
			}
		} else if (ac == 3) {
			strcpy(buf2,av[1]);
			l = strlen(buf2);
			for(j = 0 ; j <  l ; j++ ) {
				if (buf2[j] == '.') {
					buf2[j] = ',';
				}
			}
			ac2 = tokens(buf2,av2,out2);
			if (ac2 == 4) {
				for(j = 0 ; j <  4 ; j++ ) {
					p->a_ip[j] = atoi(av2[j]);
				}
			} else {
				fprintf(stderr,"error in access list line %d '%s'\n",i,buf2);
				continue;// error
			}
			strcpy(buf2,av[2]);
			l = strlen(buf2);
			for(j = 0 ; j <  l ; j++ ) {
				if (buf2[j] == '.') {
					buf2[j] = ',';
				}
			}
			ac2 = tokens(buf2,av2,out2);
			if (ac2 == 4) {
				for(j = 0 ; j <  4 ; j++ ) {
					p->a_mask[j] = atoi(av2[j]);
				}
			} else {
				fprintf(stderr,"error in access list line %d '%s'\n",i,buf2);
				continue;// error
			}
		} else {
			fprintf(stderr,"error in access list line %d too many fields\n",i);
			continue; // error
		}
		list_insert_tail(alh,p);

	}
	fclose(io);
	return alh;
}
#endif
// -----------------------------------
//  keep track of connections
// -----------------------------------
#ifdef CONN_CODE
static void conn_add_rem_connect(PCONNOBJ con, int add) {
	char *kind; int flag = 0;
	PCONNOBJREF p,q;
	if (!conn_lh) {
		conn_lh = (PCONNOBJREF) list_newlh();
	}
	if (add == 1) {
		kind = "add";
		for_list(CONNOBJREF,q,conn_lh) { // check for this one already in list, if so, don't add him
			if (q->cptr == con) {		 // this happens for single server connections, like hub stat/control
				flag = 1;				 // these do not allow more than one connection at a time on a socket
				break;					 // and close the connection before accepting any more
			}
		}
		if (!flag) {
			p = (PCONNOBJREF) list_new(sizeof(CONNOBJREF),"crefa");
			p->cptr = con;
			p->ctype = 1;
			list_insert_tail(conn_lh,p);
		}
	} else if (add == 0) {
		kind = "rem";
		for_list(CONNOBJREF,q,conn_lh) {
			if (q->cptr == con ) {
				if (q->ctype == 1) {
					list_remove(q);
					list_old(q);
					break;
				} else {
					break;
				}
				break;
			}
		}

	} else if (add == 2) {
		kind = "lis";
		p = (PCONNOBJREF) list_new(sizeof(CONNOBJREF),"crefl");
		p->cptr = con;
		p->ctype = 2;
		list_insert_tail(conn_lh,p);
	} else {
	}
	//	fprintf(stderr,"%d  %08x --- %s port %5d to %5d  sockets [main=%d lis=%d]\n",flag,con,kind,con->port, con->other_port,con->sock1,con->sock2);
	for_list(CONNOBJREF,q,conn_lh) {
		//fprintf(stderr,"+++%08x --- %d port %5d to %5d  sockets [main=%d lis=%d]\n",q->cptr,q->ctype,q->cptr->port, q->cptr->other_port,q->cptr->sock1,q->cptr->sock2);
	}
	
}
// ---------------------------
// connect layer, simplified 
// set of routines, text only, nl-terminators
// ---------------------------
void connect_init() {
	con_init();
	return;
}
// ---------------------------
// list connections on stderr 
// 
// ---------------------------
void connect_show_connections(char *text) {
	PCONNOBJREF q;
	if (text) {
		if (strlen(text) > 0) {
			fprintf(stderr,"%s\n",text);
		}
	}
	for_list(CONNOBJREF,q,conn_lh) {
#ifdef I64
		fprintf(stderr,"+++%08llx --- %d port %5d to %5d  sockets [main=%d lis=%d]\n",(unsigned IN_T)q->cptr,q->ctype,q->cptr->port, q->cptr->other_port,q->cptr->sock1,q->cptr->sock2);
#else
		fprintf(stderr,"+++%08x --- %d port %5d to %5d  sockets [main=%d lis=%d]\n",(unsigned int)q->cptr,q->ctype,q->cptr->port, q->cptr->other_port,q->cptr->sock1,q->cptr->sock2);
#endif
	}
}

// ---------------------------
// connect a  server 
// 
// ---------------------------
CONN_OBJ_PTR connect_new_server(int port) {
	PCONNOBJ portlis;
	int stat;
	
	portlis   = con_new_connection(0,0,1,0,0);			// no buffers, just create a connection to listen with
	stat      = con_server_init(portlis,port,1,NULL,0); // the NULL means we are only creating a listener, should poll if no buffers
	portlis->original_port = port;
	return (CONN_OBJ_PTR)portlis;
}
// ---------------------------
// create a connection object 
// 
// ---------------------------
CONN_OBJ_PTR connect_new_object_ref() {
	return (CONN_OBJ_PTR)con_new_connection(300000,'\n',1,0,0);
}
// ---------------------------
// check for a connection 
// 
// ---------------------------

int connect_new_arrival(CONN_OBJ_PTR server,CONN_OBJ_PTR object) {
		int stat;
		if (server->listener != 1 || object->listener != 0) {
			fprintf(stderr,"Fatal error - connect_new_arrival, bad parameters\n");
			exit(0);
		}
		stat = con_server_init((PCONNOBJ)object,0,1,(PCONNOBJ)server,0);
		return stat;
}
// ---------------------------
// close a connection 
// 
// ---------------------------

void connect_close_connection(CONN_OBJ_PTR object) {
	con_close_data_connection((PCONNOBJ)object,0);
	return;
}
// ---------------------------
// destroy a connection object 
// after closing the connection
// ---------------------------
void connect_destroy_object_ref(CONN_OBJ_PTR object) {
	list_old(object);
	return;
}
// ---------------------------
// read or write a single record 
// 
// ---------------------------
int connect_read_record(CONN_OBJ_PTR object, char *buffer, int size) {
	int stat;
	stat = con_read_record((PCONNOBJ)object, buffer,size,1); 
	return stat;
}
int connect_write_record(CONN_OBJ_PTR object, char *buffer, int size) {
	int stat;
	stat = con_write_record((PCONNOBJ)object, buffer,size,  0); // write is non-polling 
	return stat;
}

// ---------------------------
// connect wait 
// waits until one or more
// listeners or connections become
// ready for reading
// ---------------------------

int  connect_wait_for_ready(int timeout_sec,int timeout_usec, int *writers) {
	
	PCONNOBJREF q;
    fd_set rset1,rset2;
    struct timeval t;
	int hi = 1,x,r,w;
//	static int num = 0;

	for_list(CONNOBJREF,q,conn_lh) {
		if (q->ctype==1) {
			x = q->cptr->sock1;
		} else {
			x = q->cptr->sock2;
		}
		if (x > hi) {
			hi = x;
		}
	}

    FD_ZERO(&rset1);
    FD_ZERO(&rset2);
	//fprintf(stderr,"-----\n");
	for_list(CONNOBJREF,q,conn_lh) {
		//fprintf(stderr,"+++ %08x - %d ports %5d to %5d  sockets [main=%d lis=%d]\n",q->cptr,q->ctype,q->cptr->port, q->cptr->other_port,q->cptr->sock1,q->cptr->sock2);
		if (q->ctype==1) {
			FD_SET(q->cptr->sock1,&rset1);
			FD_SET(q->cptr->sock1,&rset2);
		} else {
			FD_SET(q->cptr->sock2,&rset1);
			FD_SET(q->cptr->sock2,&rset2);
		}
	}
    t.tv_sec = timeout_sec;  // in seconds
    t.tv_usec= timeout_usec; // in microseconds
	r = select(hi+1,&rset1,NULL,NULL,&t);  // readable or connections waiting
    t.tv_sec = 0;
    t.tv_usec= 0;
	w = select(hi+1,NULL,&rset2,NULL,&t);  // readable or connections waiting
	//fprintf(stderr,"%3d ----- %d %d\n",++num,r,w);
	if (writers) {
		*writers = w;
	}
	return r;
}


// -----------------------------------
//  Close, data, listen or both
//      object
//		free up memory option
// -----------------------------------
void con_close_data_connection(PCONNOBJ con, int kill) {
	if (!con) {
		return;
	}
	if(con->sock1 && (int)con->sock1 != -1) {
		//fprintf(stderr,"closing %d on port %d to %d from data_connection\n",con->sock1,con->port,con->other_port);
		conn_add_rem_connect(con,0);
		closesocket(con->sock1);
		con->sock1 = 0;
	}
	if (kill) {
		if(con->buf1)list_old(con->buf1) ;
		if(con->buf2)list_old(con->buf2) ;
		list_old(con);
	}
	return;

}
// -----------------------------------
void con_close_listen_connection(PCONNOBJ con, int kill) {
	if (!con) {
		return;
	}
	if(con->sock2) {
		//fprintf(stderr,"closing %d on port %d to %d from listen_connection\n",con->sock1,con->port,con->other_port);
		closesocket(con->sock2);
		conn_add_rem_connect(con,0);
		con->sock2 = 0;
	}
	if (kill) {
		if(con->buf1)list_old(con->buf1) ;
		if(con->buf2)list_old(con->buf2) ;
		list_old(con);
	}
	return;

}
// -----------------------------------
void con_close_connection(PCONNOBJ con, int kill) {
	if (!con) {
		return;
	}
	if(con->sock1 && (int)con->sock1 != -1) {
		//fprintf(stderr,"closing %d on port %d to %d from close 1\n",con->sock1,con->port,con->other_port);
		closesocket(con->sock1);
		conn_add_rem_connect(con,0);
		con->sock1=0;
	}
	if(con->sock2){
		//fprintf(stderr,"closing %d on port %d to %d from close 2\n",con->sock1,con->port,con->other_port);
		conn_add_rem_connect(con,0);
		closesocket(con->sock2);
		con->sock2=0;
	}

	if (kill) {
		if(con->buf1)list_old(con->buf1) ;
		if(con->buf2)list_old(con->buf2) ;
		list_old(con);
	}	
	return;

}
// -----------------------------------
// init a  connection object
//      bufsize     read/write buffer size, 0=no buffers allocated (used for listener only obj)
//      term		terminator for records
//		reuse		reuse option for server		      
//		option2		extra option (now used for setting server socket queue sizes)		      
//		option3		extra option 		      
//      
// returns: a connection handle
// -----------------------------------
CONN_obj con_new_connection(int bufsize,int term, int reuse, int option2, int option3) {
	PCONNOBJ p;
	int i;
	p = (PCONNOBJ) list_new(sizeof (CONNOBJ),"conobj");
	if (!p) {
		return NULL;
	}
	if(bufsize) {
		p->buf1 = (char *) list_new(bufsize,"con-buf1");
		p->buf2 = (char *) list_new(bufsize,"con-buf2");
		if (!p->buf1 || !p->buf2) {
			return NULL;
		}
		p->buf1[0] = 0;
		p->buf2[0] = 0;
	} else {
		p->buf1 = NULL;
		p->buf2 = NULL;
	}
	p->bufsize = bufsize;
	p->errorstr[0] = 0;
	p->error = 0;
	p->sock1 = 0;
	p->sock2 = 0;
	p->term  = term;
	p->reuse = reuse;
	p->opt2  = option2; // this is for the server send/recv buffer (queue) sizes
	p->opt3  = option3;
	p->listener = 0;
	p->port     = 0;
	for(i = 0 ; i <  5 ; i++ ) {
		p->holder1[i] = 0;
		p->holder2[i] = 0;
	}
	return p;
}
// -----------------------------------
// init a client connection 
//		con			connection object pointer
//      server_name ip address or host name
//      port_number port number to declare for this server
//      buf_size    sending buffer size, 0=use default
//      myport      client side port number (for identification only)
//      
// returns: either, 0, or a valid non-zero socket handle
// -----------------------------------
int con_client_init(PCONNOBJ con,  char * server_name, int port_number,int buf_size,int myport) {
	con->sock1 = tcp2_client_init(3, server_name,  port_number, buf_size, myport);
	if (con->sock1 == 0) {
		con->error = error;
		strncpy(con->errorstr,errorstr,sizeof(errorstr));
		return 0;
	}
#ifndef UNIXSYS
			{ unsigned int blk =1; int stat;
			    stat = ioctlsocket (con->sock1,FIONBIO,&blk);
				if (stat != 0) {
					error = WSAGetLastError();
					errorstr[0] = 0;
					return 0;
				}
			}
#endif
	strcpy(con->other_name,server_name);
	con->other_port = port_number;
	return 1;
}
// -----------------------------------
// accept a server connection 
//		con			connection object pointer
//      poll        0/1 on polling, if true, first connection only does a listen
//      listen      prior connection that is the listener connection
// returns: either, -1(poll not ready), 0 (error), or  non-zero (sucess)
// -----------------------------------
int con_server_accept(PCONNOBJ con, int poll, PCONNOBJ listen,int lookup,char *typ, void *paccess  ) {
	PALIST access = (PALIST) paccess;
	extern int tokens();
	char *av[50],buf[200],buf2[200];
	PALIST p;
	int ip[4];
	int stat,ac,l,j,ok=0,n=0;

	stat = con_server_init( con, 0,  poll,  listen ,lookup );
	if (stat != -1 && stat != 0 && access != NULL) {
		strcpy(buf2,con->other_namen);
		l = strlen(buf2);
		for(j = 0 ; j <  l ; j++ ) {
			if (buf2[j] == '.') {
				buf2[j] = ',';
			}
		}
		ac = tokens(buf2,av,buf);
		if (ac == 4) {
			for(j = 0 ; j <  4 ; j++ ) {
				ip[j] = atoi(av[j]);
			}
			for_list(ALIST,p,access) {
				n++;
				//fprintf(stderr,"-%c- %d %d %d %d    %d %d %d %d\n",p->a_type
				//	,p->a_ip[0],p->a_ip[1],p->a_ip[2],p->a_ip[3]
				//	,p->a_mask[0],p->a_mask[1],p->a_mask[2],p->a_mask[3]);
				if (	 (p->a_ip[0]  & p->a_mask[0])   ==  (ip[0] & p->a_mask[0])   && 
						 (p->a_ip[1]  & p->a_mask[1])   ==  (ip[1] & p->a_mask[1])   &&     
						 (p->a_ip[2]  & p->a_mask[2])   ==  (ip[2] & p->a_mask[2])   &&   
						 (p->a_ip[3]  & p->a_mask[3])   ==  (ip[3] & p->a_mask[3])
					) {
					if (p->a_type == 'n') {
						ok = 0;
						goto c1;
					} else if (p->a_type == 'a' || p->a_type == typ[0]) {
						ok = 1;
						goto c1;
					}
				}
			}

			n = 0;
		}
c1:
		if (ok) {
			con->error = n;
			return stat;
		} else {
			con_close_data_connection(con,0);
			con->error = 0;
			sprintf(con->errorstr,"No access: %s rule %d",con->other_namen,n);
			return 0;
		}

	}

	return stat;
}
// -----------------------------------
// init a server connection 
//		con			connection object pointer
//      port        port number to declare for this server
//      poll        0/1 on polling, if true, first connection only does a listen
//      listen      NULL or prior connection that is the listener connection
//		lookup		passed on to lower level, 0/1 for doing reverse name lookup (ip -> name)     
// returns: either, -1(poll not ready), 0 (error), or  non-zero (sucess) 
// -----------------------------------
int con_server_init(PCONNOBJ con, int port, int poll, PCONNOBJ listen, int lookup  ) {
	int flag = 0;
	if (listen) {
		con->sock2 = listen->sock2; // preload the listen socket
		con->port  = listen->port;
		con->listener = 0;
	} else {
		con->port = port;
		con->listener = 1;
		if (con->port != 0) {
			flag = 1;
		}
	}
	con->sock1 = tcp2_server_init( port,&con->sock2, poll,con->reuse,lookup,con->opt2);
	if (flag) {
		conn_add_rem_connect(con,2);
	}
	if ((int)con->sock1 > 0 && poll /*&& listen*/) {
#ifndef UNIXSYS
			{ unsigned int blk =1; int stat;
			    stat = ioctlsocket (con->sock1,FIONBIO,&blk);
				if (stat != 0) {
					error = WSAGetLastError();
					errorstr[0] = 0;
					return 0;
				}
			}
#endif
	}
	if (con->sock1 == 0) {
		con->error = error;
		strncpy(con->errorstr,errorstr,sizeof(errorstr));
		return 0;
	} else if ((int)con->sock1 == -1) {
		return -1;
	}
	strcpy(con->other_namen,tcp2_server_inaddrn());
	strcpy(con->other_name,tcp2_server_inaddr());
	con->other_port = tcp2_server_inport();
	//fprintf(stderr,"connection complete ports %d to %d\n",con->port,con->other_port);
	conn_add_rem_connect(con,1);
	return 1;
}
// -----------------------------------
// read a record on the connection with timeout
//      con			connection block
//		buf			buffer to copy record into
//		tout		if -n then timeout in 100'th seconds
//      
// returns: either, -1, 0, or >0
//
//			  0  means an error, see error/errorstr, if error == 0 then EOF (close on other end)
//			 -1 in poll mode means not finished yet and data is not ready to read, do work then come back
//			 >0 data bytes read, or if return is maxbuf, data has no terminator
// if returns size = maxbuf, then no terminator was seen and data buffer is full
// if user buffer is not large enough, 0 is returned, and the error is set to -1
// -----------------------------------
int con_read_record_with_timeout(PCONNOBJ con, char * buf,int maxbuf,int tout) {
	int n=0,i,j=0;
	if (tout <= 1) {
		tout = 1;
	}
	
	for(i = 0 ; i <  tout ; i++ ) {
		n =  tcp2_read_record(con->sock1, con->buf1,   con->bufsize, con->holder1 ,  1, con->term);
		if (n >= 0) {
			j=1;
			break;
		} else {
			mSleep(10);
		}
	}
	j = 1-j;    // toggle 1<-->0
	if (n > 0) {
		if (n > maxbuf) {
			con->buf1[0]= con->term;
			con->error = -1;
			strcpy(con->errorstr,"no room in buffer");
			return 0;
		} else if(n < maxbuf) {
			buf[n] = '\0';  // null terminate as well as the terminator is in the buffer (in case of nl terms)
		} else { // exactly bufsize, means no terminator

		}
		moveb(&con->buf1[con->holder1[0]],buf,n);
	} else {
		con->error = error;
		strcpy(con->errorstr,errorstr);
		if (j==1) {
			strcpy(con->errorstr,"timeout");
		}
	}
	return n;
}

// -----------------------------------
// read a record on the connection
//      con			connection block
//		buf			buffer to copy record into
//		poll		0/1 for polling, if -n then timeout in n seconds
//      
// returns: either, -1, 0, or >0
//
//			  0  means an error, see error/errorstr, if error == 0 then EOF (close on other end)
//			 -1 in poll mode means not finished yet and data is not ready to read, do work then come back
//			 >0 data bytes read, or if return is maxbuf, data has no terminator
// if returns size = maxbuf, then no terminator was seen and data buffer is full
// if user buffer is not large enough, 0 is returned, and the error is set to -1
// -----------------------------------
int con_read_record(PCONNOBJ con, char * buf,int maxbuf,int poll) {
	int n=0,i,j=0;
	if (poll < 0) {
		for(i = 0 ; i <  -poll ; i++ ) {
			n =  tcp2_read_record(con->sock1, con->buf1,   con->bufsize, con->holder1 ,  1, con->term);
			if (n >= 0) {
				j=1;
				break;
			} else {
				mSleep(1000);
			}
		}
		j = 1-j;    // toggle 1<-->0
	} else {
		n =  tcp2_read_record(con->sock1, con->buf1,   con->bufsize, con->holder1 ,  poll, con->term);
	}
	if (n > 0) {
		if (n > maxbuf) {
			con->buf1[0]= con->term;
			con->error = -1;
			strcpy(con->errorstr,"no room in buffer");
			return 0;
		} else if(n < maxbuf) {
			buf[n] = '\0';  // null terminate as well as the terminator is in the buffer (in case of nl terms)
		} else { // exactly bufsize, means no terminator
//			moveb(&con->buf1[con->holder1[0]],buf,n);
//			buf[n-1]= con->term;
//			return n;

		}
		moveb(&con->buf1[con->holder1[0]],buf,n);
	} else {
		con->error = error;
		strcpy(con->errorstr,errorstr);
		if (j==1) {
			strcpy(con->errorstr,"timeout");
		}
	}
	return n;
}
// -----------------------------------
// write a record on the connection
//      con			connection block
//		buf			buffer to copy record into
//		size		size of buf
//		poll		0/1 for polling
//      
// returns: either, -1, 0, or >0
//
//			  0  means an error, see error/errorstr, if error == 0 then EOF (close on other end)
//			 -1 in poll mode means not finished yet and data is not ready to read, do work then come back
//			 >0 data or if return is maxbuf, data has no terminator
// if returns size = maxbuf, then no terminator was seen and data buffer is full
// -----------------------------------
int con_write_record(PCONNOBJ con, char * buf,int size, int poll) {
	int n;
	n = tcp2_write_record(con->sock1,buf,size,con->holder2,poll);
	if (n == 0) {
		con->error = error;
		strcpy(con->errorstr,errorstr);
	}
	return n;
}
// -----------------------------------
int con_get_error(PCONNOBJ con ) {
	return con->error;
}
// -----------------------------------
char * con_get_errorstr(PCONNOBJ con ) {
	return con->errorstr;
}
// -----------------------------------
SOCKET con_get_sock1(PCONNOBJ con ) {
	return con->sock1;
}
// -----------------------------------
SOCKET con_get_sock2(PCONNOBJ con ) {
	return con->sock2;
}
// -----------------------------------
char * con_get_other_name(PCONNOBJ con ) {
	return con->other_name;
}
// -----------------------------------
int con_get_other_port(PCONNOBJ con ) {
	return con->other_port;
}
// -----------------------------------
//int con_get_(PCONNOBJ con ) {
//	return ;
//}


// --------------------------------------------------------------

#ifdef UNIXSYS
#include <signal.h>

typedef void Sigfunc(int);
static Sigfunc * con_signal(int signo, Sigfunc *func){

        struct sigaction        act, oact;

        act.sa_handler = func;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        if (signo == SIGALRM) {
#ifdef  SA_INTERRUPT
                act.sa_flags |= SA_INTERRUPT;   /* SunOS 4.x */
#endif
        } else {
#ifdef  SA_RESTART
                act.sa_flags |= SA_RESTART;             /* SVR4, 44BSD */
#endif
        }
        if (sigaction(signo, &act, &oact) < 0)
                return(SIG_ERR);
        return(oact.sa_handler);
}

// --------------------------------------------------------------
void con_init() { // unix version
	IN_T i;
	i = (IN_T)con_signal(SIGPIPE,SIG_IGN);
	if (i == -1) {
		fprintf(stderr,"error setting up SIGPIPE signal %s\n",strerror(errno));
		exit(1);
	}
}
#else
// --------------------------------------------------------------
void con_init() { // pc version
}
#endif
// --------------------------------------------------------------
int con_printf(PCONNOBJ con, char * format, ... ) {  /*returns # chars written*/
   char buffer[1000];
   int n,m;
   va_list argptr;

   va_start( argptr, format );     /* Initialize variable arguments. */
   n = vsprintf( buffer, format,  argptr );
   m = con_write_record(con,buffer,n,0);
   //printf("some prefix: (%s)\n",buffer);  /*do something with the formatted string*/
   if (m==0) {
	   n = 0;
	}
   return n;
}
// --------------------------------------------------------------



#endif // CONN_CODE

#ifdef TEP_CODE

#if 1
int teprintf(char *p_id,char * format, ... ) {  /*returns # chars written*/
   char buffer[1000],tstr[200]; char idstr[200], xid[200]; char *id;
   int n; 
   static PCONNOBJ obj = 0; 
   static char name[200],tname[200]			// name sent to the logging hub 
	   ,loghost[200]={""};
   char *str;
   va_list argptr;
        struct tm *newtime;
        time_t long_time;
        time( &long_time );                /* Get time as long integer. */
        newtime = localtime( &long_time ); /* Convert to local time. */
		id = p_id;							// id will point to either the original p_id or a copy in xid
		if (p_id) {
			strcpy(xid,p_id);
			id = xid;
		}
		if (net == 2) {				// code for changing from stderr/other net to new net
			if (1) {
				if(obj) {
					con_close_connection(obj,1);
				}// close down old net connection
				obj = 0;
				strcpy(name,format);
				net = -1;
				return 0;
			}
									// save 
		} else if (net == 3) {		// code for going back to stderr if net goes out
		} else {
		}

		if (obj && name[0] != 0) {
			strcpy(idstr,name);
		} else if (name[0] != 0) {
			strcpy(idstr,name);

		} else {
			if (id != NULL) {
				sprintf(idstr,"%s ",id);
			} else {
				idstr[0] = '\0';
			}
		}
		if (net <= 0 && id) {int aac; char *aav[20],aaout[200];
			if (net == 0) {
				name[0] = 0;
			} else if (net == -1) {
				strcpy(tname,name);
				name[0] = 0;
				id = tname;
			}
			n = 0;
			str =strchr(id,'=');
			if (str) {
				if (id != NULL) {
					sprintf(idstr,"%s ",str+1);
				}
				*str = 0;
				n = atoi(id)+1;
				strcpy(name,idstr);
			}
			str =strchr(id,':');
			if (str) {
				*str = ',';
				n = atoi(str+1);
				aac = tokens(id,aav,aaout);
				if (aac==2) {
					n = atoi(aav[1]) + 1;
					strncpy(loghost,aav[0],sizeof(loghost)-1);
				} else {
					fprintf(stderr,"Invalid -i[host:]port=name (%s) not 2 : separated items\n",id);
					return 0; //exit(0);
				}
			}

			net = 1;
			
			if (n) {int stat; char lh[150];
				obj  =	con_new_connection(400,'\n',0,0,0);      // create a connection, set the termintor character
				stat = gethostname (lh,150);					// note: this errors out on windows, since we did not call WSAStartup yet (and can't)
				if (stat != 0) {
					strcpy(lh,"localhost");						// so, if we got an error, then make lh be "localhost"
					stat = 0;									// and pretend we were sucessful
				}
				if (stat == 0) {			// localhost on windows is not a good idea!!! get actual name (not sure what this was all about)
					if (strlen(loghost)>0) {
						strncpy(lh,loghost,sizeof(lh)-1); // this is only the case when the user did not enter a hostname (5000=name)
					}
					stat =	con_client_init(obj,lh,n+3,0,0);  // connect as a client, sin/pin = server and port
				} else {
					stat = 0;
				}
				if (stat == 0) {
					fprintf(stderr,"--- cannot log to %s:%d use stderr --\n",lh,n-1);
				    freopen( "hubstderr.txt", "a", stderr );
					fprintf(stderr,"--- cannot log to %s:%d use stderr --\n",lh,n-1);
					con_close_connection(obj,1);
					obj = NULL;
					net = 1; // don't keep trying the network if it fails
				} else {
					fprintf(stderr,"--- closing down stderr, log to network %s:%d\n",lh,n-1);
					con_printf(obj,"log-%s\n",name);
					//con_printf(obj,"port was %d\n",n+4);
				}
			}
		}
		sprintf(tstr,"%s%d-%d:%02d:%02d",idstr,newtime->tm_mday,newtime->tm_hour,newtime->tm_min,newtime->tm_sec);

		va_start( argptr, format );     /* Initialize variable arguments. */
		n = vsprintf( buffer, format,  argptr );

		if (obj) {int n;
			n = con_printf(obj,"%s -%s",tstr,buffer);
			if (n == 0) {
				fprintf(stderr,"--- closing down or lost network log, use stderr\n");
				con_close_connection(obj,1);// lost connection, reset
				obj = 0;

				   /* Reassign "stderr" to "hubstderr.txt": */
				   freopen( "hubstderr.txt", "a", stderr );
				   fprintf(stderr,"--- closing down or lost network log, use stderr (hubstderr.txt)\n");

				goto serror;
									
			}
		} else {
serror:
			fprintf(stderr,"%s -%s",tstr,buffer);
			fflush(stderr);
		}




   //printf("some prefix: (%s)\n",buffer);  /*do something with the formatted string*/
   return n;
}
#else
static int teprintf(char *id,char * format, ... ) {  /*returns # chars written*/
   char buffer[1000],tstr[60]; char idstr[50];
   int n;
   va_list argptr;
        struct tm *newtime;
        time_t long_time;
        time( &long_time );                /* Get time as long integer. */
        newtime = localtime( &long_time ); /* Convert to local time. */
		if (id != NULL) {
			sprintf(idstr,"%s ",id);
		} else {
			idstr[0] = '\0';
		}
		sprintf(tstr,"%s%d-%d:%02d",idstr,newtime->tm_mday,newtime->tm_hour,newtime->tm_min);

   va_start( argptr, format );     /* Initialize variable arguments. */
   n = vsprintf( buffer, format,  argptr );
   fprintf(stderr,"%s -%s",tstr,buffer);
   //printf("some prefix: (%s)\n",buffer);  /*do something with the formatted string*/
   return n;
}
#endif
#endif // TEP_CODE

#ifdef DEBUG_CODE
static char * pstr(char *a) {
	if (a == NULL) {
		return "?";
	}
	if (a[0] == '\0') {
		return "?";
	}
	return a;
}
#endif // DEBUG_CODE


#ifdef DEBUG_CODE
static char * num_bytes1(char *buf, double d) {
	if (d < 1000.0) {
		sprintf(buf,"%-.0f %12.0f",d,d);
	} else if (d < 10000.0) {
		sprintf(buf,"%-4.2fK %12.0f",d/1000.0,d);
	} else if (d < 1000000.0) {
		sprintf(buf,"%-.1fK %12.0f", d/1000.0,d);
	} else if (d < 1000000000.0) {
		sprintf(buf,"%-4.2fM %12.0f",d/1000000.0,d);
	} else {
		sprintf(buf,"%-.2fG %12.0f", d/1000000000.0,d);
	}
	return buf;
}

static char * num_bytes0(char *buf, double d) {
	if (d < 1000.0) {
		sprintf(buf,"%-.0f",d);
	} else if (d < 10000.0) {
		sprintf(buf,"%-4.2fK",d/1000.0);
	} else if (d < 1000000.0) {
		sprintf(buf,"%-.1fK", d/1000.0);
	} else if (d < 1000000000.0) {
		sprintf(buf,"%-4.4fM",d/1000000.0);
	} else {
		sprintf(buf,"%-.6fG", d/1000000000.0);
	}
	return buf;
}
static char * num_bytes(char *buf, double d,int counts) {
	if (counts) {
		return num_bytes1(buf, d);
	} else {
		return num_bytes0(buf, d);
	}
}
#endif // DEBUG_CODE

#ifdef DEBUG_CODE
// --------------------------------------------------------------
static int list_count(void *lh) {
	int i = 0;
	void * p;
	for_list(void,p,lh) {
		i++;
	}
	return i;

}
#endif // DEBUG_CODE
#ifdef HUB
#define MAXNAME 120
typedef struct io_list {			//##
	int      io_id;                 // sequential id
	char     io_myname[MAXNAME];    // a name for this connection ,if provided by connection
	int		 io_wild;				// wild card receives all
	int		 io_hub;				// hub duplex port 4 type
	int		 io_private;			// private messaging set (# in id)
	int		 io_ngrp;				// number of groups, max 20
	char*	 io_grp[20];			// pointers to groups
	char*	 io_rgrp[20];			// pointer to group names to receive messages
	char	 io_rgbuf[MAXNAME];		// buffer to assemble group names
	int		 io_nrgrp;				// number of groups in receive (if 0, a malformed message, send to all)
	PCONNOBJ io_con;				//connection object
	char     io_me[100];			//my address
	int      io_port;				//my port
	int      io_active;				//waiting on an i/o
	time_t   io_retry;				//time to retry a connect (only on input server, id=1)
	int		 io_altout;				//alternalte output port, does not proceed until we get an input
	int      io_port2;				//indicates that this was an input port 2, and need to store name
	int      io_port3;				//indicates that this was an input port 3, so a full duplex channel
	int      io_port4;				//indicates that this was an input port 4, so a full duplex channel 2
	struct io_list *io_port3_other; //pointer to the reader or writer io_list so we can clean up on disconnect on read  or write side
	int		 io_size;				// size of current buffer
	int		 io_size2;				// size of buffer 2
	int		 io_count;				// stats
	int		 io_wait;				// stats - times we waited on this guy
	double	 io_bytes;				// stats
	int		 io_maxsize;			// max size of io_buffer
	char     *io_buffer;			//i/o buffer (can put into list)
	char     *io_buffer2;			//i/o buffer2 (can put into list)

} IOLIST, *PIOLIST;





//        struct tm *newtime;
//        time_t long_time;
//        time( &long_time );                /* Get time as long integer. */
//        newtime = localtime( &long_time ); /* Convert to local time. */

//## --------------
// 
// HUB:
// 
//	dispatch	0/1 dispatch to one at a time, instead of broadcast
//  block		0/1 blocking on/off
//  blockcount	number of required output connections or block
//  switchmode	0/1 mode where each input is round robin'd
//  ports		starting port number for the 5 consequtive ports
//  bufsize		buffer size
//  ptimeout	timeout parameter
//  term		terminator character
//  pinput_server name of input server (or NULL)
//  pinput_port port number of input server to connect to (upstream)
//  retry_secs	seconds to wait before retry of upstream server
//  alt			argv type array of alternate servers
//  nalt		number of alternates 
//  timeout2	control port timeout (0 = none) 
//  maxin
//  maxout
//	killout		not used
//	trace		verbose operation for debug
// --------------
static void old_iolist(PIOLIST p) {
	if (p->io_buffer) {
		list_old(p->io_buffer);
	}
	if (p->io_buffer2) {
		list_old(p->io_buffer2);
	}
	list_old(p);
}
static char * sys_id(PIOLIST p) {
	static char name[50]; int n;
	if (p->io_myname == 0 || strlen(p->io_myname) <= 0) {
		name[0] = '\0';
		return name;
	}
	name[0] = '(';
	n = strlen(p->io_myname);
	if (n > 45) {
		n = 45;
	}
	moveb(p->io_myname,&name[1],n);
	name[n+1] = ')';
	name[n+2] = ' ';
	name[n+3] = '\0';
	return name;

}

void hub_begin(char *id,int lookup, int dispatch,int block,int blockcount,int switchmode,int ports
			   , int bufsize, int ptimeout,int term,char *pinput_server
			   , int pinput_port, int retry_secs, char ** alt, int nalt, int upstream_duplex
			   ,int timeout2,int maxin,int maxout,int killout,int trace,int isprivate,char *alist,int sched
			   ,int killmax,int queue_size,int ports_on[10],int sync_kill,int full_counts, int nopoll) {

	PIOLIST		in_lh				// input ( publishers)  listhead
				,out_lh				// output (subscribers) listhead
				,work_lh			// work queue listhead, input connections with a read finished move here
				,pin=NULL			// pointer to elements on the input  queue
				,pout=NULL			// pointer to elements on the output queue
				,pwork=NULL			// pointer to elements on the work   queue
				,ptemp=NULL			// temp pointer
				,upstream=NULL		// pointer to the connection who is the upstream connection (we are a client to it)
				,switch_lh;
	PCONNOBJ	in_con				// connection block pointers, these hold the listen socket information
				,in2_con			// alternate for input port, allows first string to provide a name
				,in3_con			// duplex    for input port 
				,in4_con			// duplex    between hubs only 
				,out_con			// output port listener
				,out2_con			// output2 port listener, requires an input id
				,co;				// spare used to point to the current connection that just completed

	PCONNOBJ	hub_in_con=NULL;			//connect to other hub for first input connection
	PCONNOBJ	hub_stat_con=NULL;			//connect for status
	PCONNOBJ	hub_control_con=NULL;		//connect to send a command
	PCONNOBJ	iocon_con=NULL;				//used to connect inputs/outputs via the control connection
	int stat;						// used to get function results from connection calls
//	int dosleep = 0;				// whether or not to sleep (this is not actually used, we do a continue to avoid sleeping now)
	int didsleep = 0;				// did we sleep last loop? if we did, then this is a 1, else a 0
	int warnflag=0;							//used to remember we warned about slow output, so we can say "finished" later
	int done = 0;							// when true, we exit
	int timer=0;							// how long we have been waiting for this fan out to complete zero at read, increment on sleep
	int i=0,j=0,k=0;  // indices
	enum Mode {
		Read,
		Write
	};

// define class of connection codes
	enum Class {
	 Wild=	 0,
	 Private= 1,
	 Plain=	 2,
	 Dup2=	 3,
	 Dup1=	 4
	};

// define actions
	enum Actions {
		Unf,
		Format,
		Skip
	};

	enum Mode mode;							// read/write mode
	int step=0;								// step number of the write mode, 1=initial step in write, 2=actual writing in this step  
	int delay = 200;						// time we delay when nothing to do
	int timeout;							// number of ticks (converted from seconds) before slow output is killed
	unsigned int timecount= 0;						//  a count of times we slept
	unsigned int workcount= 0;				//  a count of times we went through main loop
	unsigned int lastsleep=0;				// set to workcount last time we slept
	time_t nowtime  = 0;					// get the current time in seconds since 1970
	time_t basetime  = 0;					// get the current time at program start
	int nextid=1;							// next id for connections
	char *input_server;						// ptr to input server
	char sinput_server[100];				// need to copy the parameter here so we can modify it (when we rotate in the alterates)
	int input_port;							// input server port
	int  next_server = 0;					// keep track of who is the next server in the alternate list
	int tdelay=0,tnmess=1;					// time delay for each read in 10ths of a second every n messages
	int tnext=0,tcount=0;					// time before we can proceed with i/o and time through counter
	char tbuf[50];
	int stats[11];
	int nin,nout;
	int killnow = 0;
	int throttle_mode = 0;					// if 0, throttle is in messages, if 1, its in kbytes
	PALIST a_lh = NULL;

// ---------------------------------------------
	if (alist != NULL) {
		a_lh = con_access_list_read(alist);
	}
	for(i = 0 ; i <  11 ; i++ ) {
		stats[i] = 0;
	}
	stats[8] = 999999; // smallest read (9 is largest)
	input_server = NULL;					// set up the initial input server (later we might rotate in the alts)
	if (pinput_server) {
		input_server = sinput_server;
		strcpy(sinput_server,pinput_server);
	}
	input_port = pinput_port;


	timeout = ptimeout *  (1000/delay) ;   // NOTE this assumes a polling delay of 200 ms or 1/5 second


	in_lh = list_newlh();					// create listheads
	out_lh = list_newlh();
	work_lh = list_newlh();
	switch_lh = list_newlh();

	con_init();

	time(&basetime);						//get starting time of the program
	nowtime = basetime;						//and set nowtime to it

	if (ports_on[0]) out_con			= con_new_connection(0  ,term,1,queue_size,0);
	if (ports_on[1]) in_con			= con_new_connection(0  ,term,1,queue_size,0);	//create all the needed connection blocks
	if (ports_on[2])hub_stat_con	= con_new_connection(0  ,'\n',1,0,0);
	if (ports_on[3])hub_control_con	= con_new_connection(100,'\n',1,0,0);
	if (ports_on[4])in2_con			= con_new_connection(0  ,term,1,queue_size,0);
	if (ports_on[5])in3_con			= con_new_connection(0  ,term,1,queue_size,0);
	if (ports_on[6])in4_con			= con_new_connection(0  ,term,1,queue_size,0);
	if (ports_on[7])out2_con		= con_new_connection(0  ,term,1,queue_size,0);

// ---------------------- init all ports --------------------
	if (ports_on[0]) {
		stat = con_server_init(out_con,ports+0,1,NULL,lookup);
		if (stat == 0) {
			teprintf(id,"hub: cannot create port %d %s\n",ports,con_get_errorstr(out_con));
			return;
		}
	}

	if (ports_on[1]) {
		stat = con_server_init(in_con ,ports+1  ,1,NULL,lookup);
		if (stat == 0) {
			teprintf(id,"hub: cannot create port %d\n",ports+1);
			return;
		}
	}

	if (ports_on[2]) {
		stat = con_server_init(hub_stat_con ,ports+2  ,1,NULL,lookup);
		if (stat == 0) {
			teprintf(id,"hub: cannot create port %d\n",ports+2);
			return;
		}

	}

	if (ports_on[3]) {
		stat = con_server_init(hub_control_con ,ports+3  ,1,NULL,lookup);
		if (stat == 0) {
			teprintf(id,"hub: cannot create port %d\n",ports+3);
			return;
		}
	}

	if (ports_on[4]) {
		stat = con_server_init(in2_con ,ports+4  ,1,NULL,lookup);
		if (stat == 0) {
			teprintf(id,"hub: cannot create port %d\n",ports+4);
			return;
		}
	}

	if (ports_on[5]) {
		stat = con_server_init(in3_con ,ports+5  ,1,NULL,lookup);
		if (stat == 0) {
			teprintf(id,"hub: cannot create port %d\n",ports+5);
			return;
		}
	}

	if (ports_on[6]) {
		stat = con_server_init(in4_con ,ports+6  ,1,NULL,lookup);
		if (stat == 0) {
			teprintf(id,"hub: cannot create port %d\n",ports+6);
			return;
		}
	}

	if (ports_on[7]) {
		stat = con_server_init(out2_con ,ports+7  ,1,NULL,lookup);
		if (stat == 0) {
			teprintf(id,"hub: cannot create port %d\n",ports+7);
			return;
		}
	}

	teprintf(id,"HUB v%s using ports  %d .. %d  - t/o %d secs t-byte= %d\n",VERS
		,ports,ports+7,ptimeout,term);
// ---------------------- input from upstream HUB server -----
	if (input_server) { int dopin = 0;
		hub_in_con = con_new_connection(bufsize,term,0,0,0);
		stat = con_client_init( hub_in_con,input_server,input_port,0,0);
		if (stat == 0 && retry_secs <= 0) {
			teprintf(id,"cannot connect to %s : %d\n%s\n",input_server,input_port,con_get_errorstr(hub_in_con));
			//if (retry_secs > 0) {
			//}
		} else { // either a sucess, or failure with retry set
			pin = list_new(sizeof(IOLIST),"iolist-in");
			{
				pin->io_buffer = list_new(bufsize,"io_buffer-in");
				pin->io_maxsize= bufsize;
				pin->io_id     = nextid++;
				pin->io_active = 0;
				pin->io_con    = hub_in_con;
				pin->io_port   = input_port;
				strcpy(pin->io_me,input_server);
				strcpy(pin->io_myname,"upstream");
				if (upstream_duplex) {
					pin->io_port3		= pin->io_id;

					pout = list_new(sizeof(IOLIST)+bufsize-bufsize,"iolist-out/buffer"); //??? do we need this extra
					pin->io_port3_other = pout; // save pointer to the writer in the reader structure
					{
						pout->io_port3_other= pin;// save pointer to the reader in the reader structure
						pout->io_port3		= pin->io_id;
						pout->io_id			= nextid++;
						pout->io_size		= 0;
						pout->io_buffer		= NULL;			// no buffer for output, use input buffers to send
						pout->io_maxsize	= 0;
						pout->io_myname[0]	= '\0';
						//??pout->io_active		= 1;
						pout->io_con		= pin->io_con;
						pout->io_port		= pin->io_port;
						strcpy(pout->io_me,pin->io_me);
						strcpy(pout->io_myname,pin->io_myname);
					}
					if (isprivate) { char *myid,*q;
						pin->io_port2	= pout->io_port2	= 1; // don't require an identifier as first input record
						pin->io_port4	= pout->io_port4	= 1;
						pin->io_buffer2	= list_new(bufsize,"io_buffer2");

						pin->io_private = pout->io_private = 1;
						pin->io_ngrp	= pout->io_ngrp = 1;
						pin->io_grp[0]	= pin->io_myname;
						pout->io_grp[0] = pout->io_myname;
						pin->io_wild	= pout->io_wild = 1;
						pin->io_hub		= pout->io_hub  = 1;
						myid = id;
						if (myid == NULL) {
							myid = "hub";
						} else {
							q = strchr(myid,'=');
							if (q) {
								myid = q+1;
							}
						}
						strcpy(pout->io_myname,"%upstream");
						strcpy(pin->io_myname,pout->io_myname);
						con_printf(pin->io_con,"#%%%s\n",myid);
						teprintf(id," sent out  (#%%%s)\n",myid);
					
					
					
					}


				}
			}
			upstream = pin;
			if (stat != 0) {
				list_insert_head(in_lh,pin); // ok, ready to go
				if (upstream_duplex) {
					list_insert_tail(out_lh,pout);
				}
			} else {
				teprintf(id,"cannot connect (yet) to %s:%d retry in %d seconds\n  %s\n"
					,input_server,input_port,retry_secs,con_get_errorstr(hub_in_con));
				pin->io_retry = nowtime + retry_secs;
			}
		}
	} else {
		nextid = 3; // skip over this one (special)
	}
// ---------------------- --------------------
	for_list(IOLIST,pin,in_lh) {
		teprintf(id,"connection in from %s:%d\n",pin->io_me,pin->io_port);
	}
	co = con_new_connection(bufsize,term,1,0,0);			// allocate a connection block for next connection
	mode = Read;
	pout = NULL;

//#define SCHED(nnnnn) if (!( (lastsleep==workcount) || (workcount%nnnnn == 0) )) {stat = -1;fprintf(stderr,"ls %d %d\n",workcount,lastsleep);} else
#define SCHED(nnnnn,l,w,s) if (!( (l==w) || (w%nnnnn == 0) )) {s = -1;} else
#define ISCHED(nnn) //fprintf(stderr,"LS %d %d - %d\n",workcount,lastsleep,nnn);
//## ---------------------- main loop -------------------- poll all ports every 1/5 second
	for(;;) {
		workcount++;
		if (workcount%300==0) { // get current time even if we are very busy (else get it when we sleep)
			time(&nowtime);// note, this is not intended to be precise, but rather don't call too often
			//fprintf(stderr,"---------- time is %d %d\n",nowtime,workcount);
		}
		if (switchmode == 0 || list_count(in_lh) == 0 ) { void *p;	// if not switching, all go to input queue now
			for(;;) {												// but if switching, we delay until all are done
				p = list_remove_head(switch_lh);	// move everyone from the switch queue back to the input queue
				if (!p) {
					break;
				}
				list_insert_tail(in_lh,p);
			}
		}
//		dosleep = 1;
//##    ----------------------------------------------------  STAT connections
		SCHED(sched,lastsleep,workcount,stat) {ISCHED(1) 
			if (ports_on[2]) {stat = con_server_accept( hub_stat_con, 1,  NULL ,lookup ,"s",a_lh );}
		}
		//fprintf(stderr,"connection stat = %d\n",stat);
		if (ports_on[2] == 0) {
			// ignore
		} else if (stat == 0) {
			teprintf(id,"connection (stat) port error %s\n",con_get_errorstr(hub_stat_con));
		} else if (stat != -1 && stat != 0) {char bf[30]; char name[200]; char *thtype,hname[150];
			if(trace)teprintf(id,"access allowed by rule %d\n",hub_stat_con->error);
		
			//fprintf(stderr,"connection (stat) from %s : %d\n",con_get_other_name(hub_stat_con)
			//	,con_get_other_port(hub_stat_con));
//			con_printf(hub_stat_con,"%s:%d m/s %d %d t/o %d tmr %d nid %d s/w/t %d %d %d\n"
			con_printf(hub_stat_con,"m/s %d %d tmo/rtry/tmr %d %d %d nid %d s/w/t %d %d %d %s\n"
//				,con_get_other_name(hub_stat_con),con_get_other_port(hub_stat_con)
				,mode,step,timeout/ (1000/delay),retry_secs, timer/ (1000/delay) 
				,nextid,timecount,workcount,nowtime-basetime,id ? id: "");
				{int nbytes,nalloc,nfree;
					list_stats(&nbytes,&nalloc,&nfree);
					thtype = "byt";
					if (throttle_mode==0) {
						thtype = "rec";
					}
					con_printf(hub_stat_con,"%s mi/mo %d %d sec/%s %d %d blk %d  byts/a/f/u/trm %d %d %d %d %d\n",VERS
						,maxin,maxout,thtype,tdelay,tdelay !=0 ? tnmess:0,blockcount,nbytes,nalloc,nfree,nalloc-nfree,term);
				}
#define HSIZE 19
			con_printf(hub_stat_con,"(0125) %d %d %d %d (1-5-10-30) %d %d %d %d [%d %d] bad=%d\n",
				stats[0],stats[1],stats[2],stats[3],stats[4],stats[5],stats[6],stats[7],stats[8]>100000? -1:stats[8],stats[9],stats[10]);
			if (upstream && upstream->io_retry > 0) {
				pin = upstream;
				strncpy(hname,pin->io_me,sizeof(hname)-1);hname[HSIZE] = '\0';
				con_printf(hub_stat_con,"%4d rtry %-19s : %5d %12s %c %d/%s #%-5d Rsecs %d\n"
					,pin->io_id,hname,pin->io_port,strlen(pin->io_myname)>0 ? pin->io_myname : "?",'.'
					,pin->io_count,num_bytes(tbuf,pin->io_bytes,full_counts) ,pin->io_size,pin->io_retry - nowtime);
			}
			if (mode == Write && step == 2) {
				moveb(pwork->io_buffer,bf,20); bf[20] = '\0';
				if (pwork->io_size <20) {
					bf[pwork->io_size] = '\0';
				}
				strncpy(hname,pwork->io_me,sizeof(hname)-1);hname[HSIZE] = '\0';
				con_printf(hub_stat_con,"%4d in-a %-19s : %5d %12s %c %d/%s #%-5d [%s]\n"
					,pwork->io_id,hname,pwork->io_port
					,pwork->io_myname,pwork->io_active?'A':'.',pwork->io_count
					, num_bytes(tbuf,pwork->io_bytes,full_counts),pwork->io_size,bf);
			}
			for_list(IOLIST,pin,work_lh) {
				strncpy(hname,pin->io_me,sizeof(hname)-1);hname[HSIZE] = '\0';
				con_printf(hub_stat_con,"%4d in-q %-19s : %5d %12s %c %d/%s #%-5d\n"
					,pin->io_id,hname,pin->io_port,strlen(pin->io_myname)>0 ? pin->io_myname : "?"
					,pin->io_active?'a':'.'
					,pin->io_count, num_bytes(tbuf,pin->io_bytes,full_counts),pin->io_size);
			}
			for_list(IOLIST,pin,in_lh) {char pending[20]; int pend,pend1,typ;
				if (strlen(pin->io_myname) > 0) {
					if (pin->io_private) {
						sprintf(name,"#%s",pin->io_myname);
					} else {
						strcpy(name,pin->io_myname);
					}
				} else {
					strcpy(name,"?");
				}
				pending[0] = 0;
				pend  = pin->io_con->holder1[3];
				pend1 = pin->io_con->holder1[2];
				if (pend1 > 0 ) {
					sprintf(pending," [%d]",pend1);
				} else if (pend > 0) {
					sprintf(pending," [%d...]",pend);
				}
				if (pin->io_port4  != 0) {
					typ = 'D' ;
				} else if (pin->io_port3  != 0) {
					typ = 'd' ;
				} else if (pin->io_port2  != 0) {
					typ = 'I' ;
				} else if (pin->io_private  != 0) {
					typ = '#' ;
				} else {
					typ = ' ' ;
				}
				strncpy(hname,pin->io_me,sizeof(hname)-1);hname[HSIZE] = '\0';
				con_printf(hub_stat_con,"%4d in%c  %-19s : %5d %12s %c %d/%s%s\n"
					,pin->io_id, typ
					,hname
					,pin->io_port,name,'.'
					,pin->io_count, num_bytes(tbuf,pin->io_bytes,full_counts),pending);
			}
			{int numout,tper,tch; double twait,tw; char *wt;
				twait = 0.0;
				numout = 0;
				for_list(IOLIST,pout,out_lh) {
					numout++;
					tw = (double)pout->io_wait;
					twait = twait + tw;
				}
				for_list(IOLIST,pout,out_lh) {int typ; 
					if (pout->io_altout == 2) {
						typ = 'i' ;
					} else if (pout->io_altout == 1) {
						typ = 'I' ;
					} else if (pout->io_port4  != 0) {
						typ = 'D' ;
					} else if (pout->io_port3  != 0) {
						typ = 'd' ;
					} else if (pout->io_port2  != 0) {
						typ = 'I' ;
					} else if (pout->io_private  != 0) {
						typ = '#' ;
					} else {
						typ = ' ' ;
					}
					if (strlen(pout->io_myname) > 0) {
						if (pout->io_private) {
							sprintf(name,"#%s",pout->io_myname);
						} else {
							strcpy(name,pout->io_myname);
						}
					} else {
						strcpy(name,"?");
						if (pout->io_port3 != 0) {
							if (pout->io_port3_other != NULL &&  strlen(pout->io_port3_other->io_myname) > 0 ) {
								sprintf(name,"(%s)",pout->io_port3_other->io_myname);
							}
						}
					}
					tw = pout->io_wait;
					if (twait < 1.0) {
						tw = 0.0;
					} else {
						tw = (tw / twait)*10.0;	// compute fraction of waits for this guy * 10
					}
					tch = '.';
					wt = "";
					if (pout->io_active) {
						wt = " busy";
					}
					if (tw < 1.0) {
						tch = '.';
					} else {
						if (tw > 9.0) {
							tch = '9';
						} else { char *tch0;
							tper = (int)tw;
							tch0 = ".123456789X";
							tch = tch0[tper];
						}
					}
					strncpy(hname,pout->io_me,sizeof(hname)-1);hname[HSIZE] = '\0';
					con_printf(hub_stat_con,"%4d out%c %-19s : %5d %12s %c %d/%s%s\n"
						,pout->io_id,  typ
						,hname,pout->io_port
						,name,tch
						,pout->io_count, num_bytes(tbuf,pout->io_bytes,full_counts),wt);
				}
			}
			for_list(IOLIST,pin,switch_lh) {
				strncpy(hname,pin->io_me,sizeof(hname)-1);hname[HSIZE] = '\0';
				con_printf(hub_stat_con,"%4d swit %-19s : %5d %12s %c %d/%s\n"
					,pin->io_id,hname,pin->io_port,strlen(pin->io_myname)>0 ? pin->io_myname : "?"
					,pin->io_active?'a':'.'
					,pin->io_count, num_bytes(tbuf,pin->io_bytes,full_counts));
			}
			if (trace) {PCONNOBJREF q;
				for_list(CONNOBJREF,q,conn_lh) {
					con_printf(hub_stat_con,"+++ %08x - %d ports %5d to %5d  sockets [main=%d lis=%d]\n",q->cptr,q->ctype,q->cptr->port, q->cptr->other_port,q->cptr->sock1,q->cptr->sock2);
				}
				
			}
			con_close_data_connection(hub_stat_con,0);
		}
//##      ---------------------------------------------------- CONTROL  connections
		SCHED(sched,lastsleep,workcount,stat) {ISCHED(2) 
			if (ports_on[3]) {stat = con_server_accept( hub_control_con, 1,  NULL,lookup ,"c",a_lh  ); }
		}
		//fprintf(stderr,"connection control stat = %d\n",stat);
		if (ports_on[3] == 0) {
			// ignore
		} else if (stat == 0) {
			teprintf(id,"connection (control) port error %s\n",con_get_errorstr(hub_control_con));
		} else if (stat != -1 && stat != 0) { char buf[100];
			teprintf(id,"connection (control) from %s : %d\n",con_get_other_name(hub_control_con)
				,con_get_other_port(hub_control_con));
			if(trace)teprintf(id,"access allowed by rule %d\n",hub_control_con->error);
			stat = con_read_record(hub_control_con,buf,100,timeout2 >0 ? -timeout2 : 0);
			if (stat > 0) {
				buf[stat] = '\0';
				if (buf[strlen(buf)-1] < ' ') {
					buf[strlen(buf)-1] = '\0';
				}
				if (buf[strlen(buf)-1] < ' ') {
					buf[strlen(buf)-1] = '\0';
				}
				teprintf(id,"command = %s\n",buf);
//              ---------------------------------------------- quit
				if (buf[0] == 'q') {
					done = 1;
					goto continue_wait;
//              ---------------------------------------------- access
				} else if (buf[0] == 'a') {
					if (alist != NULL) {
						a_lh = con_access_list_read(alist);
						teprintf(id,"re-read %s for access list\n",alist);
					}
//              ---------------------------------------------- reset counts
				} else if (buf[0] == 'Z') { PIOLIST p;
					teprintf(id,"Zero all record and byte counters\n");
					for_list(IOLIST,p,out_lh) {
						p->io_count = 0;
						p->io_bytes = 0.0;
					}
					for_list(IOLIST,p,work_lh) {
						p->io_count = 0;
						p->io_bytes = 0.0;
					}
					for_list(IOLIST,p,switch_lh) {
						p->io_count = 0;
						p->io_bytes = 0.0;
					}
					for_list(IOLIST,p,in_lh) {
						p->io_count = 0;
						p->io_bytes = 0.0;
					}
//              ---------------------------------------------- reset wait stats
				} else if (buf[0] == 'R') { PIOLIST p;
					teprintf(id,"Reset wait stats\n");
					for_list(IOLIST,p,out_lh) {
						p->io_wait = 0;
					}
//              ---------------------------------------------- force timeout
				} else if (buf[0] == 'x') {
					timer = timeout+1;
					teprintf(id,"force timeout\n");
//              ---------------------------------------------- change blockcount
				} else if (buf[0] == 'b') {
					blockcount = atoi(&buf[2]);
					teprintf(id,"changed blockcount to %d\n",blockcount);
					if (blockcount == 0) {
						block = 0;
					} else {
						block = 2;
					}
//              ---------------------------------------------- change max in/out
				} else if (buf[0] == 't') {
					timeout = atoi(&buf[2]);
					teprintf(id,"changed timeout to %d\n",timeout);
					timeout = timeout * (1000/delay);
				} else if (buf[0] == 'I') {
#ifndef LITE
					maxin = atoi(&buf[2]);
					teprintf(id,"changed max in to %d\n",maxin);
#endif
				} else if (buf[0] == 'O') {
#ifndef LITE
					maxout = atoi(&buf[2]);
					teprintf(id,"changed max out to %d\n",maxout);
#endif
//              ---------------------------------------------- change trace
				} else if (buf[0] == 'v') {
					trace = atoi(&buf[2]);
					teprintf(id,"changed trace to %d\n",trace);
//              ---------------------------------------------- change timeout
				} else if (buf[0] == 'r') {
					retry_secs = atoi(&buf[2]);
					teprintf(id,"changed retry timer to %d\n",retry_secs);
					if (upstream && upstream->io_retry > 0) {
						upstream->io_retry = nowtime + retry_secs;
					}
//              ---------------------------------------------- change delay parameters
				} else if (buf[0] == 'd') {int n;char *av[50]; char out[250]; 
					extern int tokens();
					throttle_mode = 0;
					n = tokens(buf,av,out);
					if (n == 3) {
						tnmess = atoi(av[1]);
						tdelay = atoi(av[2]);
						teprintf(id,"change throttle: at most %d messages in %d second(s)\n",tnmess,tdelay);
						tnext = nowtime; // cancel current wait if necessary
					} else {
						teprintf(id,"throttle: at most %d messages in %d second(s)\n",tnmess,tdelay);
					}
				} else if (buf[0] == 'D') {int n;char *av[50]; char out[250]; 
					extern int tokens();
					throttle_mode = 1;
					n = tokens(buf,av,out);
					if (n == 3) {
						tnmess = atoi(av[1]);
						tdelay = atoi(av[2]);
						teprintf(id,"change throttle: at most %d bytes in %d second(s)\n",tnmess,tdelay);
						tnext = nowtime; // cancel current wait if necessary
						tnmess = tnmess;
					} else {
						teprintf(id,"throttle: at most %d bytes in %d second(s)\n",tnmess,tdelay);
					}
//              ---------------------------------------------- connect logging nodes
				} else if (buf[0] == 'Q' ) {

					
					net = 2;
					if (id == NULL) {
						id = "hub";
					}

					teprintf(0,&buf[2]);
//              ---------------------------------------------- connect input/output nodes
				} else if (buf[0] == 'i' || buf[0] == 'o' || buf[0] == 'm'|| buf[0] == 'n') 
											{ char *p,*name; char *av[50]; char out[250]; 
											int i,n; int pt;char server[100];
					extern int tokens();
					n = tokens(buf,av,out);
					for(i = 1 ; i <  n ; i++ ) {
					//	fprintf(stderr,"arg %d = %s\n",i,av[i]);
						p = strchr(av[i],':');
						name = strchr(av[i],'=');
						if (name) {
							*name = '\0';
							name++;
						}
						pt = tcp2_address(av[i],server,0);
						if (pt) {
							if (buf[0] == 'o') {
								teprintf(id,"connect out to   node=%s port=%d\n",server,pt);
							} else if (buf[0] == 'm') {
								teprintf(id,"connect duplex  node=%s port=%d\n",server,pt);
							} else if (buf[0] == 'n') {
								teprintf(id,"connect duplex2  node=%s port=%d\n",server,pt);
							} else {
								teprintf(id,"connect in from  node=%s port=%d\n",server,pt);
							}

								iocon_con = con_new_connection(bufsize,term,0,0,0);
								stat = con_client_init( iocon_con,server,pt,bufsize,0);
								if (stat == 0) {
									teprintf(id,"cannot connect to %s:%d\n%s\n",server,pt,con_get_errorstr(iocon_con));
								} else {
									pin = list_new(sizeof(IOLIST),"iolist-pin");
									{

										pin->io_buffer = list_new(bufsize,"io_buffer-in");
										pin->io_maxsize= bufsize;
										if (name) {int k; char *n = name;
										    moveb(name,pin->io_myname,sizeof(pin->io_myname));
											strncpy(pin->io_myname,name,sizeof(pin->io_myname));
											pin->io_myname[sizeof(pin->io_myname)-1] = '\0';
											for(k = 0 ; k < (int) sizeof(pin->io_myname)-1 ; k++ ) {
												if (pin->io_myname[k] < ' ') {
													pin->io_myname[k] = '\0';
												}
												
											}
										} else {
											pin->io_myname[0] = '\0';
										}
										pin->io_id     = nextid++;
										pin->io_active = 0;
										pin->io_size   = 0;
										pin->io_con    = iocon_con;
										pin->io_port   = con_get_other_port(iocon_con);
										if (buf[0] == 'n') {
											pin->io_port4	= 1;
										}
										strcpy(pin->io_me,con_get_other_name(iocon_con));
									}
									if (buf[0] == 'o') {
										list_insert_head(out_lh,pin);
									} else if (buf[0] == 'm' || buf[0] == 'n') { // create the write connection as well
										list_insert_head(in_lh,pin);
										pout = list_new(sizeof(IOLIST)+bufsize-bufsize,"iolist-out/buf-m"); //??? do we need this?
										pin->io_port3_other = pout; // save pointer to the writer in the reader structure
										pout->io_port3_other= pin;// save pointer to the reader in the reader structure
										pout->io_port3		= pin->io_id;
										if (buf[0] == 'n') {
											pout->io_port4	= 1;
										}
										pout->io_id			= nextid++;
										pout->io_size		= 0;
										pout->io_buffer		= NULL;			// no buffer for output, use input buffers to send
										pout->io_maxsize	= 0;
										pout->io_myname[0]	= '\0';
										//??pout->io_active		= 1;
										pout->io_con		= iocon_con;
										pout->io_port		= con_get_other_port(iocon_con);
										strcpy(pout->io_me,con_get_other_name(iocon_con));
										pin->io_port3 = pout->io_id; // save id of 'out' in the 'in' and vica versa
										//pin->io_port4= 1;
										pin->io_buffer2		= list_new(bufsize,"io_buffer2n");

										list_insert_tail(out_lh,pout);
										{ char *pp,*pp2; char iname[200], iname2[200];
											if (id != NULL && strlen(id) > 0) {
												strcpy(iname,id);
											} else {
												strcpy(iname,"hub");
											}
											pp =  strchr(iname,'=');
											if (pp) {
												*pp = '\0';
												pp++;
											} else {
												pp = iname;
											}
											if (pout->io_port4) {
												if (name != NULL && strlen(name) > 0) {
													strcpy(iname2,name);
													pp2 =  strchr(iname2,'\n');
													if (pp2) {
														*pp2 = '\0';
														teprintf(id," nulling nl (%s)\n",iname2);
													}
													pp2 =  strchr(iname2,'\r');
													if (pp2) {
														*pp2 = '\0';
														teprintf(id," nulling  cr (%s)\n",iname2);
													}
													pp2 = iname2;
													if (*pp2 == '#') {
														*pp2 = '%';
													}
												} else {
													pp2 = "?";
												}
												strcpy(pout->io_myname,pp2);
												teprintf(id," made myname (%s)\n",pp2);
												moveb(pout->io_myname,pin->io_myname,sizeof(pin->io_myname));
												pin->io_private = pout->io_private = 1;
												pin->io_ngrp	= pout->io_ngrp = 1;
												pin->io_grp[0]	= pin->io_myname;
												pout->io_grp[0] = pout->io_myname;
												pin->io_wild	= pout->io_wild = 1;
												pin->io_hub		= pout->io_hub  = 1;
												if (pp != NULL && strlen(pp) > 0) {
													strcpy(iname2,pp);
													pp2 =  strchr(iname2,'\n');
													if (pp2) {
														*pp2 = '\0';
													}
													pp2 =  strchr(iname2,'\r');
													if (pp2) {
														*pp2 = '\0';
													}
													pp2 = iname2;
												} else {
													pp2 = "?";
												}
												con_printf(pin->io_con,"#%%%s\n",pp2);
												teprintf(id," sent out  (#%%%s)\n",pp2);
											}
										}

									} else { // this is the i command
										list_insert_head(in_lh,pin);
									}
								}

						} else {
							teprintf(id,"  invalid %s\n",av[i]);
							break;
						}
					}
//              ---------------------------------------------- kill ids
				} else if (buf[0] == 'k') {  char *av[50]; char out[450]; int i,n,s; int found=0;
					extern int tokens();
					n = tokens(buf,av,out);
					s = 0;
					for(i = 1 ; i <  n ; i++ ) {
						s = atoi(av[i]);
						//fprintf(stderr,"kill seq %d\n",s);
						if (s == 1 && upstream && upstream->io_retry > 0) {
							teprintf(id,"killing upstream connection attempt\n");
							upstream = NULL;
						}
						for_list(IOLIST,pin,work_lh) {
							if (s == pin->io_id) {
								teprintf(id,"killing seq %d from work queue\n",s);
								goto cont1;
							}
						}
						for_list(IOLIST,pin,in_lh) {
							if (s == pin->io_id) {
								teprintf(id,"killing seq %d from input queue\n",s);
								goto cont1;
							}
						}
						for_list(IOLIST,pin,out_lh) {
							if (s == pin->io_id) {
								teprintf(id,"killing seq %d from output queue\n",s);
								goto cont1;
							}
						}
						continue;
cont1:

						if (pin->io_port3) {// if full duplex port, note: pin and pout could be reversed, but it's symetric
							teprintf(id,"killing seq %d a full duplex guy\n",s);

							list_remove(pin);

							pout = pin->io_port3_other;

							list_remove(pout);

							con_close_data_connection(pin->io_con,1); // close and return memory
							//if(pin->io_buffer)list_old(pin->io_buffer);
							//if(pin->io_buffer2)list_old(pin->io_buffer2);
							//list_old(pin);	// both of these shared the same io_con block
							//if(pout->io_buffer)list_old(pout->io_buffer);
							//if(pout->io_buffer2)list_old(pout->io_buffer2);
							//list_old(pout);
							old_iolist(pin);
							old_iolist(pout);
						} else {
							list_remove(pin);
							con_close_data_connection(pin->io_con,1);
							//if(pin->io_buffer)list_old(pin->io_buffer);
							//if(pin->io_buffer2)list_old(pin->io_buffer2);
							//list_old(pin);
							old_iolist(pin);
						}
						found++;
					}
					if (!found) {
						teprintf(id,"No connection found with id %d\n",s);
					}
//              ---------------------------------------------- set identify for ids
				} else if (buf[0] == 'S') {  char *av[50]; char out[450]; int i,n,s;
					extern int tokens();
					for(i = 0 ; i <  50 ; i++ ) {
						av[i] = "";
					}
					for(i = 0 ; i <  (int)strlen(buf) ; i++ ) {
						if (buf[i] < ' ') {
							buf[i] = 0;
							break;
						}
					}
					n = tokens(buf,av,out);
					for(i = 1 ; i <  n ; i = i + 2 ) {
						s = atoi(av[i]);
						if (s < 0) {
							s = nextid + s;
						}
						for_list(IOLIST,pin,work_lh) {
							if (s == pin->io_id) {
								goto cont2;
							}
						}
						for_list(IOLIST,pin,in_lh) {
							if (s == pin->io_id) {
								goto cont2;
							}
						}
						for_list(IOLIST,pin,out_lh) {
							if (s == pin->io_id) {
								goto cont2;
							}
						}
						continue;
cont2:
						strncpy(pin->io_myname,av[i+1],sizeof(pin->io_myname)-1);
						pin->io_myname[sizeof(pin->io_myname)-1] = '\0';
						continue;
					}
//              ---------------------------------------------- set identify for ids
				} else if (buf[0] == 'P') {  char *av[50]; char out[450]; int i,n,s;
					extern int tokens();
					for(i = 0 ; i <  50 ; i++ ) {
						av[i] = "";
					}
					for(i = 0 ; i <  (int)strlen(buf) ; i++ ) {
						if (buf[i] < ' ') {
							buf[i] = 0;
							break;
						}
					}
					n = tokens(buf,av,out);
					for(i = 1 ; i <  n ; i = i + 2 ) {
						s = atoi(av[i]);
						for_list(IOLIST,pin,work_lh) {
							if (s == pin->io_port) {
								goto cont3;
							}
						}
						for_list(IOLIST,pin,in_lh) {
							if (s == pin->io_port) {
								goto cont3;
							}
						}
						for_list(IOLIST,pin,out_lh) {
							if (s == pin->io_port) {
								goto cont3;
							}
						}
						continue;
cont3:
						strncpy(pin->io_myname,av[i+1],sizeof(pin->io_myname)-1);
						pin->io_myname[sizeof(pin->io_myname)-1] = '\0';
						continue;
					}
				} else {
					teprintf(id,"unknown command, valid commands are:\n");
					teprintf(id,"v 0/1            - change trace to 0 or 1 (off/on)\n");
					teprintf(id,"q                - exit\n");
					teprintf(id,"a                - re-read access list if specified at startup\n");
					teprintf(id,"d 2  5           - change delay at most 2 messages in 5 seconds\n");
					teprintf(id,"D 3000  5        - change delay at most 3000 bytes in 5 seconds\n");
					teprintf(id,"t 999            - change timeout to 999\n");
					teprintf(id,"r 999            - change retry seconds (for upstream) to 999\n");
					teprintf(id,"b 3              - change block count to 3\n");
					teprintf(id,"I 3              - change maxin  to 3\n");
					teprintf(id,"O 3              - change maxout to 3\n");
					teprintf(id,"x                - force a timeout on slow writer\n");
					teprintf(id,"R                - Reset io wait counts on writers\n");
					teprintf(id,"Z                - Reset (zero) all record and byte counts\n");
					teprintf(id,"i node:port=name,  - connect node as input  client(s) to node\n");
					teprintf(id,"o node:port=name,  - connect node as output client(s) to node\n");
					teprintf(id,"m node:port=name,  - connect node as duplex client(s) to node\n");
					teprintf(id,"n node:port=name,  - connect node as duplex 2 client(s) to node\n");
					teprintf(id,"Q node:port=name,  - connect logging output\n");
					teprintf(id,"k seq,seq..      - kill connection(s) (seq from stat list)\n");
					teprintf(id,"S seq,name..     - set name given seq (-n for -1=last,-2=next to last etc.)\n");
					teprintf(id,"P port,name..    - set name given port\n");
				}
			} else {
					teprintf(id,"error reading control command = %s\n",con_get_errorstr(hub_control_con));
			}
			con_close_data_connection(hub_control_con,0);
		}
//##      ---------------------------------------------------- INPUT connection to upstream server (we are its client)

		if (upstream) { // 1000 linux:3000+out 10 localhost:4000+out mama:222+out   -- just a place to save a command line
			if (upstream->io_retry) {long int sss;
				sss =(int)(upstream->io_retry)-(int) nowtime;
				//fprintf(stderr,"upstream retry %d\n",upstream->io_retry-nowtime);
				if ( sss <= 0) {
					stat = con_client_init( upstream->io_con,input_server,input_port,0,0);
					if (stat == 0) {
						teprintf(id,"cannot connect to %s:%d retry in %d seconds\n",input_server,input_port,retry_secs);
						teprintf(id,"  %s\n",con_get_errorstr(upstream->io_con));
						upstream->io_retry = nowtime + retry_secs;
						if (nalt > 0) {
							for(;;) {
								if (next_server < nalt) {
									input_port = tcp2_address(alt[next_server],input_server,0);
									next_server++;
									if (isprivate) {char *p; int bad = 0;
										p = strchr(alt[next_server-1],'+');
										if (p) {
											if (p[1] != 'D') {
												bad = 1;
											}
										} else {
											bad = 1;
										}
										if (bad) {
											teprintf(id,"bypassing reconnect to %s  message Duplex port required\n",alt[next_server-1]);
											continue;
										}
									}
								} else  {
									strcpy(input_server,pinput_server);
									input_port = pinput_port;
									next_server = 0;
								}
								if (input_port == 0 && next_server != 0) {
									continue;
								}
								break;
							}
						}
					} else {
						teprintf(id,"(re)connected to %s:%d\n",input_server,input_port);
						upstream->io_port = input_port;
						list_insert_tail(in_lh,upstream);
						upstream->io_retry = 0;
						if (upstream_duplex) {
							upstream->io_port3_other->io_port = input_port;
							list_insert_tail(out_lh,upstream->io_port3_other);
							if (isprivate) {char *q,*myid;
								myid = id;
								if (myid == NULL) {
									myid = "hub";
								} else {
									q = strchr(myid,'=');
									if (q) {
										myid = q+1;
									}
								}
								con_printf(upstream->io_con,"#%%%s\n",myid);
								teprintf(id," sent out on reconnect  (#%%%s)\n",myid);
							}
						}
					}
				}
			}
		}
//      ---------------------------------------------------- COUNT connections in/out
		nin = list_count(in_lh)+list_count(work_lh)+list_count(switch_lh);
		if (mode == Write && step == 2) {
			nin++;
		}
		nout= list_count(out_lh);
//		if (nin > 0 && killout) {
//			killnow = 1;
//		}

		if (killnow ) {
			killnow = 0;
			for_list(IOLIST,pin,out_lh) {
				if (pin->io_port3) {// if full duplex port, note: pin and pout could be reversed, but it's symetric
					ptemp = list_prev(pin);
					teprintf(id,"killing seq %d a full duplex connection\n",pin->io_id);

					list_remove(pin);

					pout = pin->io_port3_other;

					list_remove(pout);

					con_close_data_connection(pin->io_con,1); // close and return memory

					//if(pin->io_buffer)list_old(pin->io_buffer);
					//if(pin->io_buffer2)list_old(pin->io_buffer2);
					//list_old(pin);	// both of these shared the same io_con block
					//if(pout->io_buffer)list_old(pout->io_buffer);
					//if(pout->io_buffer2)list_old(pout->io_buffer2);
					//list_old(pout);
					old_iolist(pin);
					old_iolist(pout);

					pin = ptemp;
				} else {
				//	teprintf(id,"killing seq %d from output queue\n",pin->io_id);
				//	list_remove(pin);
				//	con_close_data_connection(pin->io_con,1);
				//	if(pin->io_buffer)list_old(pin->io_buffer);
				//	list_old(pin);
				}
			}
		}

	SCHED(sched,lastsleep,workcount,stat) {ISCHED(3)
//##    ---------------------------------------------------- INPUT  connections normal
		if (ports_on[1] && (maxin == 0 ||  nin  < maxin || killmax) ) {
			stat = con_server_accept( co		  , 1,  in_con ,lookup,"w",a_lh); 
			if ( killmax && (stat > 0) && maxin > 0  ) {
				if (nin >= maxin) {
					teprintf(id,"%s : %d exceded in limit %d<%d  closing\n"
						,con_get_other_name(co),con_get_other_port(co),maxin,nin+1);
					con_close_data_connection(co,0);
					stat = -1;
				}
			}
			if (stat == 0) {
				teprintf(id,"connection (in) port error %s\n",con_get_errorstr(co));
			} else 	if (stat != -1) {
				teprintf(id,"connected an IN  socket from %s : %d\n",con_get_other_name(co),con_get_other_port(co));
				if(trace)teprintf(id,"access allowed by rule %d\n",co->error);
				pin = list_new(sizeof(IOLIST),"iolist-in");
				{
					pin->io_id     = nextid++;
					pin->io_size   = 0;
					pin->io_myname[0] = '\0';
					pin->io_buffer = list_new(bufsize,"io_buffer-in");
					pin->io_maxsize= bufsize;
					pin->io_active = 0;
					pin->io_con    = co;
					pin->io_port   = con_get_other_port(co);
					strcpy(pin->io_me,con_get_other_name(co));
				}
				nin++;
				list_insert_tail(in_lh,pin);
				co = con_new_connection(bufsize,term,1,0,0);
			}
		}
//      ---------------------------------------------------- INPUT  connections first time with name
		if (ports_on[4] && (maxin == 0 ||  nin  < maxin|| killmax)) {
			stat = con_server_accept( co		  , 1,  in2_con ,lookup,"w",a_lh); 
			if ( killmax && (stat > 0) && maxin > 0  ) {
				if (nin >= maxin) {
					teprintf(id,"%s : %d exceded in limit %d<%d  closing\n"
						,con_get_other_name(co),con_get_other_port(co),maxin,nin+1);
					con_close_data_connection(co,0);
					stat = -1;
				}
			}
			if (stat == 0) {
				teprintf(id,"connection (inI) port error %s\n",con_get_errorstr(co));
			} else 	if (stat != -1) {
				teprintf(id,"connected an inI socket from %s : %d\n",con_get_other_name(co),con_get_other_port(co));
				if(trace)teprintf(id,"access allowed by rule %d\n",co->error);
				pin = list_new(sizeof(IOLIST),"iolist-in");
				{
					pin->io_id     = nextid++;
					pin->io_size   = 0;
					pin->io_port2   = 2;
					pin->io_myname[0] = '\0';
					pin->io_buffer = list_new(bufsize,"io_buffer-in");
					pin->io_maxsize= bufsize;
					pin->io_active = 0;
					pin->io_con    = co;
					pin->io_port   = con_get_other_port(co);
					strcpy(pin->io_me,con_get_other_name(co));
				}
				nin++;
				list_insert_tail(in_lh,pin);
				co = con_new_connection(bufsize,term,1,0,0);
			}
		}
//##    ---------------------------------------------------- INPUT  connections duplex on one port
		if (ports_on[5] && (maxin == 0 ||  nin  < maxin|| killmax)) {
			stat = con_server_accept( co		  , 1,  in3_con ,lookup,"w",a_lh); 
			if ( killmax && (stat > 0) && maxin > 0  ) {
				if (nin >= maxin) {
					teprintf(id,"%s : %d exceded in limit %d<%d  closing\n"
						,con_get_other_name(co),con_get_other_port(co),maxin,nin+1);
					con_close_data_connection(co,0);
					stat = -1;
				}
			}
			if (stat == 0) {
				teprintf(id,"connection (in3) port error %s\n",con_get_errorstr(co));
			} else 	if (stat != -1) {
				teprintf(id,"connected an IN3 socket from %s : %d\n",con_get_other_name(co),con_get_other_port(co));
				if(trace)teprintf(id,"access allowed by rule %d\n",co->error);
				pin = list_new(sizeof(IOLIST),"iolist-in");
				{
					pin->io_id			= nextid++;
					pin->io_size		= 0;
					pin->io_port3		= pin->io_id;
					pin->io_myname[0]	= '\0';
					pin->io_buffer		= list_new(bufsize,"io_buffer");
					pin->io_maxsize		= bufsize;
					pin->io_active		= 0;
					pin->io_con			= co;
					pin->io_port		= con_get_other_port(co);
					strcpy(pin->io_me,con_get_other_name(co));
				}
				list_insert_tail(in_lh,pin);
//				co = con_new_connection(bufsize,term,1,0,0);
				pout = list_new(sizeof(IOLIST)+bufsize-bufsize,"iolist-out/buffer"); //?? do we need this extra???
				pin->io_port3_other = pout; // save pointer to the writer in the reader structure
				{
					pout->io_port3_other= pin;// save pointer to the reader in the reader structure
					pout->io_port3		= pin->io_id;
					pout->io_id			= nextid++;
					pout->io_size		= 0;
					pout->io_buffer		= NULL;			// no buffer for output, use input buffers to send
					pout->io_maxsize	= 0;
					pout->io_myname[0]	= '\0';
					//??pout->io_active		= 1;
					pout->io_con		= co;
					pout->io_port		= con_get_other_port(co);
					strcpy(pout->io_me,con_get_other_name(co));
				}
				nin++;
				list_insert_tail(out_lh,pout);
				co = con_new_connection(bufsize,term,1,0,0);
			}
		}
//##    ---------------------------------------------------- INPUT  connections duplex 2 on one port
		if (ports_on[6] && (maxin == 0 ||  nin  < maxin|| killmax)) {
			stat = con_server_accept( co		  , 1,  in4_con ,lookup,"w",a_lh); 
			if ( killmax && (stat > 0) && maxin > 0  ) {
				if (nin >= maxin) {
					teprintf(id,"%s : %d exceded in limit %d<%d  closing\n"
						,con_get_other_name(co),con_get_other_port(co),maxin,nin+1);
					con_close_data_connection(co,0);
					stat = -1;
				}
			}
			if (stat == 0) {
				teprintf(id,"connection (in4) port error %s\n",con_get_errorstr(co));
			} else 	if (stat != -1) {
				teprintf(id,"connected an IN4 socket from %s : %d\n",con_get_other_name(co),con_get_other_port(co));
				if(trace)teprintf(id,"access allowed by rule %d\n",co->error);
				pin = list_new(sizeof(IOLIST),"iolist-in");
				{
					pin->io_id			= nextid++;
					pin->io_size		= 0;
					pin->io_port2		= 2; // also require an identifier as first input record
					pin->io_port3		= pin->io_id;
					pin->io_port4		= 1;
					pin->io_myname[0]	= '\0';
					pin->io_buffer		= list_new(bufsize,"io_buffer");
					pin->io_buffer2		= list_new(bufsize,"io_buffer2");
					pin->io_maxsize		= bufsize;
					pin->io_active		= 0;
					pin->io_con			= co;
					pin->io_port		= con_get_other_port(co);
					strcpy(pin->io_me,con_get_other_name(co));
				}
				list_insert_tail(in_lh,pin);
//
				pout = list_new(sizeof(IOLIST)+bufsize-bufsize,"iolist-out/buffer"); //?? do we need this extra???
				pin->io_port3_other = pout; // save pointer to the writer in the reader structure
				{
					pout->io_port3_other= pin;// save pointer to the reader in the writer structure
					pout->io_port3		= pin->io_id;
					pout->io_port4		= 1;
					pout->io_id			= nextid++;
					pout->io_size		= 0;
					pout->io_buffer		= NULL;			// no buffer for output, use input buffers to send
					pout->io_maxsize	= 0;
					pout->io_myname[0]	= '\0';
					//??pout->io_active		= 1;
					pout->io_con		= co;
					pout->io_port		= con_get_other_port(co);
					strcpy(pout->io_me,con_get_other_name(co));
				}
				nin++;
				list_insert_tail(out_lh,pout);
				co = con_new_connection(bufsize,term,1,0,0);
			}
		}
//##    ---------------------------------------------------- OUTPUT connections
		if (ports_on[0] && (maxout == 0 ||  nout  < maxout|| killmax)) {
			stat = con_server_accept( co, 1,  out_con ,lookup,"r",a_lh); 
			if ( killmax && (stat > 0) && maxout > 0  ) {
				if (nout >= maxout) {
					teprintf(id,"%s : %d exceded out limit %d<%d  closing\n"
						,con_get_other_name(co),con_get_other_port(co),maxout,nout+1);
					con_close_data_connection(co,0);
					stat = -1;
				}
			}
			if (stat == 0) {
				teprintf(id,"connection (out) port error %s\n",con_get_errorstr(co));
			} else if (stat != -1) {
				teprintf(id,"connected an OUT socket from %s : %d\n",con_get_other_name(co),con_get_other_port(co));
				if(trace)teprintf(id,"access allowed by rule %d\n",co->error);
				pout = list_new(sizeof(IOLIST)+bufsize-bufsize,"iolist-out"); //??? extra???
				{
					pout->io_id     = nextid++;
					pout->io_size   = 0;
					pout->io_buffer = NULL;			// no buffer for output, use input buffers to send
					pout->io_maxsize= 0;
					pout->io_myname[0] = '\0';
					//??pout->io_active = 1;
					pout->io_con    = co;
					pout->io_port   = con_get_other_port(co);
					strcpy(pout->io_me,con_get_other_name(co));
				}
				nout++;
				list_insert_tail(out_lh,pout);
				co = con_new_connection(bufsize,term,1,0,0);
				lastsleep = workcount + 1;
				continue;
			}
		}

//##    ---------------------------------------------------- OUTPUT type 2 connections
		if (ports_on[7] && (maxout == 0 ||  nout  < maxout|| killmax)) {
			stat = con_server_accept( co, 1,  out2_con ,lookup,"r",a_lh); 
			if ( killmax && (stat > 0) && maxout > 0  ) {
				if (nout >= maxout) {
					teprintf(id,"%s : %d exceded out limit %d<%d  closing\n"
						,con_get_other_name(co),con_get_other_port(co),maxout,nout+1);
					con_close_data_connection(co,0);
					stat = -1;
				}
			}
			if (stat == 0) {
				teprintf(id,"connection (out-alt) port error %s\n",con_get_errorstr(co));
			} else if (stat != -1) {
				teprintf(id,"connected an OUT-alt socket from %s : %d\n",con_get_other_name(co),con_get_other_port(co));
				if(trace)teprintf(id,"access allowed by rule %d\n",co->error);
				pout = list_new(sizeof(IOLIST)+bufsize-bufsize,"iolist-out"); //??? extra???
				{
					pout->io_id     = nextid++;
					pout->io_size   = 0;
					pout->io_buffer = NULL;			// no buffer for output, use input buffers to send
					pout->io_maxsize= 0;
					pout->io_myname[0] = '\0';
					//??pout->io_active = 1;
					pout->io_altout = 2;			// indicate type and status 2= wait for input
					pout->io_con    = co;
					pout->io_port   = con_get_other_port(co);
					strcpy(pout->io_me,con_get_other_name(co));
				}
				nout++;
				list_insert_tail(out_lh,pout);
				co = con_new_connection(bufsize,term,1,0,0);
			}
		}
	} // end sched
//##    ---------------------------------------------------- CHECK  OUTPUT connections, sending data and/or going away
//		----------------------------------------------------               also input id on alt output type connections		

		
		
		if (workcount % 10 == 0) {PIOLIST old_pout; // check every 10'th time through here (10 = 2/second, unless busy)

			for_list(IOLIST,pout,out_lh) {char name[sizeof(pout->io_myname)];
				if (pout->io_port3 == 0) {	//check only if not a full duplex port
					stat = con_read_record(pout->io_con,name,sizeof(name),1); //  attempt to read to see if it is still there, or id input
					if (stat == 0) { // disconnect
						teprintf(id,"test output to [%s%s / %d] - connection lost: %s\n",sys_id(pout),pout->io_me,pout->io_port,pout->io_con->errorstr);
						//??pout->io_active = 0;

						old_pout = list_prev(pout); // trick for removing the current guy in the list
						list_remove(pout);			// remove the current (for loop guy) from the list
						con_close_data_connection(pout->io_con,1); // close down the socket and free memory
						//if(pout->io_buffer)list_old(pout->io_buffer);
						//if(pout->io_buffer2)list_old(pout->io_buffer2);
						//list_old(pout);				// free up memory for iolist
						old_iolist(pout);
						pout = old_pout;			// set for next in list to advance correctly;

					} else if (stat == -1) { // not ready
					} else if (stat > 0) { int k; char *n = name;  // an input

						if (name[0] == '#') {int lx;
							lx = stat;
							if (stat >= (int)sizeof(name)) {
								lx = sizeof(name) - 1;
							}
							name[sizeof(name)-1] ='\0'; 
							for(k = 0 ; k < (int) sizeof(pout->io_myname)-1 ; k++ ) {
								if (name[k] < ' ') {
									name[k] = '\0';
								}
								
							}
							pout->io_ngrp = tokens(&name[1],pout->io_grp,pout->io_myname);
							pout->io_private = 1;
							if (strcmp("*",pout->io_myname)==0) {
								pout->io_wild = 1;
							}

						//} else if () {
						} else {
							moveb(name,pout->io_myname,sizeof(pout->io_myname));
							strncpy(pout->io_myname,name,sizeof(pout->io_myname));
							pout->io_myname[sizeof(pout->io_myname)-1] = '\0';
							for(k = 0 ; k < (int) sizeof(pout->io_myname)-1 ; k++ ) {
								if (pout->io_myname[k] < ' ') {
									pout->io_myname[k] = '\0';
								}
								
							}
						}
						if (pout->io_altout == 2) {
							pout->io_altout = 1;
						}

					} else {
					}

				} // if

			} // for

		}
		
		
//##    ---------------------------------------------------- READ - mut excl with write
		if (mode == Read) {PIOLIST old_pin; int found_work = 0;
//      ---------------------------------------------------- 
			if (tdelay > 0 && tnmess > 0 && tcount <= 0) {  // implement throttle effect, restrict reading
				if (tnext > nowtime) {
					//fprintf(stderr,"waiting tdelay = %d, tnmess = %d, tcount = %d, tnext-base = %d\n",tdelay,tnmess,tcount,tnext-nowtime);
					goto continue_wait;		// just delay some more, but still check on connections etc
				}
				//fprintf(stderr,"ok, reset tcount to %d and tnext to now + %d\n",tnmess,tdelay);
				tcount = tnmess ; // times through here is once per input, so use input round robin if necessary
				tnext = nowtime + tdelay;	//set next time we can proceed
			} else if(tdelay <= 0) {
				tdelay = 0;
				tnext = 0;
				tcount = 0;
			}
			timer = 0;
			for_list(IOLIST,pin,in_lh) {
				stat = con_read_record(pin->io_con,pin->io_buffer,pin->io_maxsize ,1);
				if (stat > 0) {int process_it;	int badflag;					// ----------------------------- stat + data in
					badflag = 0;
					if (stat >= pin->io_maxsize) {
						stats[10]++;
						pin->io_buffer[stat-2] = term;
						pin->io_buffer[stat-1] = 0;
						stat--;
						badflag = 1;
					}
					if (stat > stats[9]) {
						stats[9] = stat;	// largest record
					}
					if (stat < stats[8]) {
						stats[8] = stat;	// smallest record
					}
					if		  (stat >= 30000) {
						stats[7]++;
					} else if (stat >= 10000) {
						stats[6]++;
					} else if (stat >= 5000) {
						stats[5]++;
					} else if (stat >= 1000) {
						stats[4]++;
					} else if (stat >= 500) {
						stats[3]++;
					} else if (stat >= 200) {
						stats[2]++;
					} else if (stat >= 100) {
						stats[1]++;
					} else {
						stats[0]++;
					}
					if (trace) {
						if (badflag) {
							teprintf(id,"in from id %d (%s) size %d (BAD: no terminator seen)\n",pin->io_id,pin->io_myname,stat);
						} else {
							teprintf(id,"in (%d) from id %d (%s) size %d\n",pin->io_count+1,pin->io_id,pin->io_myname,stat);
						}
					}
					process_it = 1;
															// -------------------------------------
					if (pin->io_port2 == 2) {int lx,k;		// -------------- port 2 alt input check
						if (stat > 2 && pin->io_buffer[0] == '@' && pin->io_buffer[1] == '\n') {
							pin->io_myname[0] = '@'; // special situation for streaming '@' in first char
							pin->io_myname[1] = '\0';// we use this to process html stuff
							pin->io_port2 = 1; // indicate we have got the name, but we WERE an in2 port (for stats)
							moveb(&pin->io_buffer[2],pin->io_buffer,stat-2);
							stat = stat - 2;
						} else if (stat > 2 && pin->io_buffer[0] == '#') {

							lx = stat;
							if (stat >= (int)sizeof(pin->io_myname)) {
								lx = sizeof(pin->io_myname) - 1;
							}
							pin->io_buffer[sizeof(pin->io_myname)-1] ='\0'; 
							for(k = 0 ; k < (int) sizeof(pin->io_myname)-1 ; k++ ) {
								if (pin->io_buffer[k] < ' ') {
									pin->io_buffer[k] = '\0';
								}
								
							}
							pin->io_buffer2 =  list_new(bufsize,"io_buffer2-in");
							pin->io_ngrp = tokens(&pin->io_buffer[1],pin->io_grp,pin->io_myname);
							pin->io_private = 1;
							if (strcmp("*",pin->io_myname)==0) {
								pin->io_wild = 1;
							}
							if (pin->io_myname[0] == '%') {
								pin->io_wild = 1;
								pin->io_hub  = 1;
							}
							pin->io_port2 = 1; // indicate we have got the name, but we WERE an in2 port (for stats)
							if (pin->io_port4) { // if also a duplex port, then fill in other half
								pout = pin->io_port3_other;
								moveb(pin->io_myname,pout->io_myname,sizeof(pin->io_myname));
								pout->io_private = 1;
								pout->io_ngrp = pin->io_ngrp ; //???? potential for disaster here, we copy the pointers but they point into the other structures myname, should point to ours, fix sometime!!!!
								moveb((char *)pin->io_grp,(char *)pout->io_grp,sizeof(pin->io_grp)*sizeof(char *));
								pout->io_wild = pin->io_wild;
								pout->io_hub  = pin->io_hub;
							}
							process_it = 0;
						} else {
							lx = stat;
							if (stat >= (int)sizeof(pin->io_myname)) {
								lx = sizeof(pin->io_myname) - 1;
							}
							moveb(pin->io_buffer,pin->io_myname,lx);
							for(k = 0 ; k < (int) sizeof(pin->io_myname)-1 ; k++ ) {
								if (pin->io_myname[k] < ' ') {
									pin->io_myname[k] = '\0';
								}
								
							}
							pin->io_myname[lx-1] = '\0';
							pin->io_port2 = 1; // indicate we have got the name, but we WERE an in2 port (for stats)
							process_it = 0;
						}
					}
					if(process_it) {char *p; int bad=0; int typ; char *styp;
						pin->io_buffer[stat] = 0;
						//fprintf(stderr,"data input [%s / %d] -  %d byts ------ %s",pin->io_me,pin->io_port,stat,pin->io_buffer);
						pin->io_size = stat;
						//??pin->io_active = 1;
						pin->io_count++;
						pin->io_bytes = pin->io_bytes + (double)stat;
						if (pin->io_private /*|| pin->io_port4 */) { // look for private messaging format setup buf2 if true ????? test code  the comment out of port4 check ???????
							pin->io_buffer[stat+1] = '/';
							p = xstrchr(pin->io_buffer,'/');
							if (p-pin->io_buffer+1 > stat || stat < 6 || p-pin->io_buffer+1 < 5) {
								bad = 1;// malformed
							} else if (pin->io_buffer[0] != 's') {
								bad = 1;// malformed
							} else {
								if (pin->io_buffer[1] == 'a') {
									typ = 1; styp = "all";
								} else if (pin->io_buffer[1] == 'o') {
									typ = 2; styp = "only";
								} else if (pin->io_buffer[1] == '1') {
									typ = 3; styp = "one";
								} else {
									bad = 1;// malformed
								}
								if (bad) {
									pin->io_nrgrp = 0;
								} else { int sid,shd,idata; char *who;
									*p = 0;
									//moveb(pin->io_buffer,pin->io_buffer2,stat);
									pin->io_nrgrp = tokens(&pin->io_buffer[3],pin->io_rgrp,pin->io_rgbuf);
									if (pin->io_nrgrp == 0) {
										bad = 1; goto Bad1;
									}
									idata = p-pin->io_buffer+1;
									who = pin->io_rgrp[0];
									sid = strlen(who);
									shd = strlen(pin->io_buffer);
									//if (sid <= shd) {
									//} else {
									//	movebu(p,&pin->io_buffer2[sid],stat-idata+1); // slide up the data
									//	strcpy(pin->io_buffer2,pin->io_myname); // copy in the id
									//}
									strcpy(pin->io_buffer2,who); // copy in the id
									if(sid != shd) {
										moveb(p+1,&pin->io_buffer2[sid+1],stat-idata); // slide down the data
									}
									pin->io_buffer2[sid] = '/'; // restore the slash
									pin->io_size2 = stat - (shd-sid);
									*p = '/';
									if (trace) {int k; char xbuf[300],ybuf[100]; 
										sprintf(xbuf,".private:(%s) sender(%s)  to:",styp,pin->io_rgrp[0]);
										for(k = 1 ; k <  pin->io_nrgrp ; k++ ) {
											sprintf(ybuf," (%s)",pin->io_rgrp[k]);strcat(xbuf,ybuf);
										}
										teprintf(id,"%s\n",xbuf);
									}
								}
								
							}
Bad1:
							if (bad) {char mess[40];
								moveb(pin->io_buffer,mess,sizeof(mess));
								mess[sizeof(mess)-1] = 0;
								teprintf(id,"bad message: %s",mess);
								//strcpy(pin->io_buffer2,"bad"); //bad messages are not echoed out out
								//pin->io_buffer2[3] = term;
								//pin->io_buffer2[4] = 0;
								//pin->io_size2 = 4;
							}
						}
						if(!bad) {								// if ok, queue for output, put into work queue
							old_pin = list_prev(pin);
							list_remove(pin);			// take out of this list
							list_insert_tail(work_lh,pin);  // put into the active output list
							pin = old_pin;				// set for next in list to advance correctly
							found_work = 1;
						}
					}
				} else if (stat == 0) {															// ----- stat 0 error
					teprintf(id,"connection closed  [%s%s / %d] %s\n",sys_id(pin),pin->io_me,pin->io_port,con_get_errorstr(pin->io_con));
					if (pin->io_myname[0] == '@') {
						killnow = 1;
					}
					pout = pin->io_port3_other; // get pointer to writer half if a full dup connection
					if (pout) {					// is our writer still active?
						teprintf(id,"closing other half [%s%s / %d] of full duplex channel\n",sys_id(pin),pin->io_me,pin->io_port);
						pout = pin->io_port3_other; // get pointer
						list_remove(pout);
						con_close_data_connection(pout->io_con,0);
						if ( !(pin->io_id == 1 && retry_secs > 0) ) { // if not upstream that retries, then give back memory
							//if (pout->io_buffer) {
							//	list_old(pout->io_buffer);
							//}
							//if (pout->io_buffer2) {
							//	list_old(pout->io_buffer2);
							//}
							//list_old(pout);
							old_iolist(pout);
						}
					}
					old_pin = list_prev(pin); // trick for removing the current guy in the list, save prev and restore
					list_remove(pin);
					if (pin->io_id == 1 && retry_secs > 0) { // id 1 is special, he is our upstream hub we are a client to
						con_close_connection(pin->io_con,0); // close the socket but save the connnection block
						pin->io_retry = nowtime + retry_secs;// indicate when to retry this connection
					} else {
						con_close_data_connection(pin->io_con,1);
						if (pin->io_id == 1) {	// if this is the upstream and we have no retries, this lossed connection is gone forever
							upstream = NULL;
						}
						//if (pin->io_buffer) {
						//	list_old(pin->io_buffer);
						//}
						//if (pin->io_buffer2) {
						//	list_old(pin->io_buffer2);
						//}
						//list_old(pin);
						old_iolist(pin);
					}
					pin = old_pin; // restore to continue in list loop
					if (sync_kill) {PIOLIST old_pinx,poutx,pinx; // kill all output connections since our input connection is gone

						for_list(IOLIST,pinx,out_lh) {
							old_pinx = list_prev(pinx); // trick for removing the current guy in the list, save prev and restore
							if (pinx->io_port3) {// if full duplex port, note: pinx and poutx could be reversed, but it's symetric
								teprintf(id,"disconnect duplex [%s %s / %d] from output queue (input EOF)\n",sys_id(pinx),pinx->io_me,pinx->io_port);
								list_remove(pinx);
								poutx = pinx->io_port3_other;
								list_remove(poutx);
								con_close_data_connection(pinx->io_con,1); // close and return memory
								old_iolist(pinx);
								old_iolist(poutx);
							} else {
								list_remove(pinx);
								teprintf(id,"disconnect [%s%s / %d] from output queue (input EOF)\n",sys_id(pinx),pinx->io_me,pinx->io_port);
								con_close_data_connection(pinx->io_con,1);
								old_iolist(pinx);
							}
							pinx = old_pinx; // restore to continue in list loop
						}


					}
				} else {															// stat < 0 ------------- no data yet
					//fprintf(stderr,"check connection  [%s / %d] - ",pin->io_me,pin->io_port);
					//fprintf(stderr,"no data %d\n",stat);
				}
			}
			if (found_work) {
				// note: stat still has the record size when we get here, and it must be > 0 since found_work is true
				if (throttle_mode == 0) {
					if (tcount > 0) { // count down records for delay option
						tcount--;
					}
				} else {
					if (tcount > 0) { // count down bytes for delay option
					//	fprintf(stderr,"count down bytes tcount = %d, stat = %d, new = %d\n",tcount,stat,tcount - stat);
						tcount = tcount - stat;
						if (tcount < 0) {
							tcount = 0;
						}
					}
				}
				mode = Write;
				step = 1;
				continue;
			}
//##    ---------------------------------------------------- WRITE
		} else if (mode == Write){ int wflag; PIOLIST old_pout;
//      -------------------------------------
			if (step == 1) {
//      ---------------------
				//teprintf(id,"Blockcount =%d block=%d count=%d\n",blockcount,block,list_count(out_lh));
				if (block && list_count(out_lh) < blockcount) {		// not enough output connections, so done right away
					//teprintf(id,"   Not enough\n");
				} else {
					pwork = list_remove_head(work_lh);
					warnflag = 0;
					if (pwork == NULL) {
						mode = Read;
						step = 0;							// no work, return to read mode, clear step for neatness
						continue;
					} else {
						if (dispatch) {						// if dispatching mode, then rotate output list if not empty
							pout = list_remove_head(out_lh);	// get head of list 
							if (pout) {							// if any on list then
								list_insert_tail(out_lh,pout);	// insert back on tail
							}
						}
						for_list(IOLIST,pout,out_lh) {
							pout->io_active = 0;			// set all to be non active if any
						}
						for_list(IOLIST,pout,out_lh) {
							if (pout->io_altout != 2) {		// if alternate output, wait till it id's itself with a write to us (a read here)
								pout->io_active = 1;		// set all (if any) to be actively awaiting i/o
							}
							if (dispatch) {					// if dispatch mode, then break after setting just one
								break;
							}
						}
						// now we just move this check to step 2 so it's only done once
						//if (list_count(out_lh) == 0) {		// no output connections, so done right away
						//	list_insert_tail(in_lh,pwork);	// put this guy back onto the input list
						//	pwork->io_active = 0;
						//	step = 1;						// go back and see if more work to do
						//	continue;
						//}// start an output i/o on each in out list, set active flag if not completed immediatly
					}
					step = 2; // set next step, leave, check and come back here
					continue;
				}
//      -------------------------
			} else if (step == 2) { // keep checking for the stragglers  tnext tcount
//      -------------------------
				//teprintf(id,"blockcount =%d block=%d count=%d\n",blockcount,block,list_count(out_lh));
				if (block && list_count(out_lh) < blockcount) {		// not enough output connections, so done right away
					//teprintf(id,"   not enough\n");
				} else if (list_count(out_lh) == 0) {		// no output connections, so done right away
					//teprintf(id,"   zero\n");
						list_insert_tail(switch_lh,pwork);	// put this guy back onto the input list (first goes onto switch queue)
						pwork->io_active = 0;
						step = 1;						// go back and see if more work to do
						continue;
				} else {							// check em all
					//teprintf(id,"   other\n");
					wflag = 0;
					for_list(IOLIST,pout,out_lh) {
						if (pout->io_port3 && pout->io_port3 == pwork->io_id) { // is this us? don't echo back to us
							pout->io_active = 0;
							continue;
						}
						if (pout->io_active) {int rstat; int stat_count, stat_bytes;
							rstat = 1;
							if (dispatch && pout->io_port3 == 0) {char abuf[10];// if we are in dispatch mode, and not duplex attempt to read to see 
								rstat = con_read_record(pout->io_con,abuf,10,1);// if it is still there, before we do a write
							}
							stat_count = 1;
							stat_bytes = 0;
							if (rstat != 0) { enum Class pw,po;
								static enum Actions action[5][5] = {
								//	wild	private		plain	dup2	dup				table of actions based on class of in/out
									{Unf,	Format,		Skip,	Unf,	Skip},	//wild
									{Unf,	Format,		Skip,	Unf,	Skip},	//private
									{Unf,	Skip,		Unf,	Unf,	Unf},	//plain
									{Unf,	Format,		Skip,	Unf,	Skip},	//dup2
									{Unf,	Skip,		Unf,	Skip,	Unf}		//dup
								};

//## ---------------------		compute matrix location of pout vs. pwork
								if (pout->io_wild) {
									po = Wild;
								} else if (pout->io_private) {
									po = Private;
								} else if (pout->io_port4) {
									po =  Dup2;
								} else if (pout->io_port3) {
									po =  Dup1;
								} else {
									po =  Plain;
								}

								if (pwork->io_wild) {
									pw = Wild;
								} else if (pwork->io_private) {
									pw = Private;
								} else if (pwork->io_port4) {
									pw =  Dup2;
								} else if (pwork->io_port3) {
									pw =  Dup1;
								} else {
									pw =  Plain;
								}


								if (action[pw][po] == Format) { int sendit = 0; int j,k;
									for(k = 1 ; k <  pwork->io_nrgrp ; k++ ) {
										if (sendit) {
											break;
										}
										for(j = 0 ; j <  pout->io_ngrp ; j++ ) {
											if (strcmp(pwork->io_rgrp[k],pout->io_grp[j])== 0) {
												sendit = 1;
												break;
											}
										}
									}
									if (sendit) {
										if (trace) {char buf[50]; int i;
											moveb(pwork->io_buffer2,buf,49);
											buf[ibetween(0,pwork->io_size2-1,49)] = '\0';
											for(i = 0 ; i <  3 ; i++ ) {
												if (buf[strlen(buf)-1] == '\n' || buf[strlen(buf)-1] == '\r' ) {
													buf[strlen(buf)-1] = '\0';
												}
											}
											teprintf(id,"..send message  from %s as(%s) -> %s '%s'\n"
												,pstr(pwork->io_myname),pstr(pwork->io_rgrp[0]),pstr(pout->io_myname),buf);
										}
										stat = con_write_record(pout->io_con,pwork->io_buffer2,pwork->io_size2,1); // its a fellow private, format the output
										stat_bytes = pwork->io_size2;
									} else {
										stat = 1;
										stat_count = 0;
										stat_bytes = 0;
									}
								} else if (action[pw][po]== Unf) {int i;
										if (trace) {char buf[50];
											moveb(pwork->io_buffer,buf,49);
											buf[ibetween(0,pwork->io_size-1,49)] = '\0';
											for(i = 0 ; i <  3 ; i++ ) {
												if (buf[strlen(buf)-1] == '\n' || buf[strlen(buf)-1] == '\r' ) {
													buf[strlen(buf)-1] = '\0';
												}
											}
											teprintf(id,"..send raw data from %s -> %s '%s'\n"
												,pstr(pwork->io_myname),pstr(pout->io_myname),buf);
										}
									stat = con_write_record(pout->io_con,pwork->io_buffer,pwork->io_size,1);   // its our special guy *, gets it all unformatted
									stat_bytes = pwork->io_size;
								} else { //skip
									stat = 1;
									stat_count = 0;
									stat_bytes = 0;
								}



							} else {
								stat = 0;
							}



							if (stat == 0) { // disconnect
								teprintf(id,"try output to [%s%s / %d] size = %d - connection lost (%s)\n",sys_id(pout),pout->io_me,pout->io_port,pwork->io_size,pout->io_con->errorstr);
								pout->io_active = 0;
								
								old_pout = list_prev(pout); // trick for removing the current guy in the list
								list_remove(pout);			// remove the current (for loop guy) from the list
								if (pout->io_port3) {		// if full duplex port
									pout->io_port3_other->io_port3_other = NULL; // let him know we did the cleanup on a write, the read will clean itself up soon
									con_close_data_connection(pout->io_con,0); // close down the socket and keep memory till read side cleans it up
								} else {
									con_close_data_connection(pout->io_con,1); // close down the socket and free memory
								}
								if (pout->io_id != 2) { // as long as not a full dup upstream connection (note if not retrying, we don't bother to return the memory)
									old_iolist(pout);
								}
								pout = old_pout;			// set for next in list to advance correctly;
								if (dispatch) {
									if (list_count(out_lh) > 0) {		// if now we have at least one in the output list
										old_pout = list_next(out_lh);	// then this is the new first in the list
										old_pout->io_active = 1;		// so, make this the new active guy ???? note this is in conflict with other rules making someone active
									}									// must eventually fix, actually, means cannot have dispatch mode with all these other things
									wflag = 1;			// we want to recheck after a wait
									break;				// and go back and check again
								}
								
							} else if (stat == -1) { // not ready
								if (timer > timeout && timeout > 0) {
									//fprintf(stderr,"did output to  %s port %d size = %d - timeout %d\n",pout->io_me,pout->io_port,pwork->io_size,timer);
									teprintf(id,"TIMEOUT %s%s : %d timeout %d secs, disconnected!\n"
										,sys_id(pout),pout->io_me,pout->io_port,timer / (1000/delay) );

									old_pout = list_prev(pout);  // now we handle the duplex condition too
									list_remove(pout);

									if (pout->io_port3) {		// if full duplex port
										pout->io_port3_other->io_port3_other = NULL; // let him know we did the cleanup on a write, the read will clean itself up soon
										con_close_data_connection(pout->io_con,0); // close down the socket and keep memory till read side cleans it up
									} else {
										con_close_data_connection(pout->io_con,1); // close down the socket and free memory
									}
									if (pout->io_id != 2) { // as long as not a full dup upstream connection (note if not retrying, we don't bother to return the memory)
										old_iolist(pout);
									}

									pout = old_pout;			// set for next in list to advance correctly;
								} else {int freq;
									freq = 40;
									if ((timer/ (1000/delay)) > 20) {
										freq = (freq * 75)/20; // once a 30 secs after the first 4 warnings
									}
									if(timer%freq == 0 &&  (timer/ (1000/delay))  > 4) {
										teprintf(id,"stalled output to  %s%s: %d  size= %d  timer %d / %d\n"
											,sys_id(pout),pout->io_me,pout->io_port,pwork->io_size,timer/ (1000/delay)
											,timeout<=0 ? 0 : (timeout-timer) / (1000/delay));
										warnflag = 1;
									}
									pout->io_active = 1;
									pout->io_wait++;
									wflag = 1;       // still want to wait some more for this one
								}
							} else if (stat > 0) {   // done
								pout->io_active = 0;
								pout->io_count =pout->io_count + stat_count;
								pout->io_bytes = pout->io_bytes + (double)stat_bytes;
								//fprintf(stderr,"did output to  %s port %d size = %d - done\n",pout->io_me,pout->io_port,pwork->io_size);
							} else {
							}
						}


					}
					if (wflag == 0) { //  finished 
						//fprintf(stderr,"finn...\n");
						list_insert_tail(switch_lh,pwork);	// put this guy back onto the input list eventually, switch q first
						pwork = NULL;						// clear so someday we can test pwork to see if any work in progress (not now used)
						step = 1;// go back and see if more work to do
						if(block==1)block=0;
						if(warnflag) {
							teprintf(id,"un-stalled at %d secs\n",(timer) / (1000/delay));
						}
						continue;
					} // not finished, drops through and sleeps
				}
			}
		}
//##    ---------------------------------------------------- scopy -D200 -M30,30000 -M100,5000 -M1000,1000 -llocalhost:5000=scp -ieric -IERIC  .:5400+Out .:5500+In
continue_wait:
		if (done) {
			break;
		}
//		if (dosleep) {
			//fprintf(stderr,"%3d waiting...%d\n",timecount++,timer++);
			
			if (mode == Write || nopoll == 0) {	// if writing, we must wait since we don't know who we are waiting for
				mSleep(delay);					// and this means some straggler has a full write buffer
			} else {
				connect_wait_for_ready(0,200000,NULL); // if mode is Read, we can wake up w/o polling
			}
			
			timer++;
			timecount++;
			time(&nowtime); // get current time
			didsleep = 1;
			if (workcount > 100000000 || timecount > 100000000 ) {
				workcount = 0;
				timecount = 0;
			}
			lastsleep=workcount+1;
			//fprintf(stderr,"SLP        time is %d %d\n",nowtime,workcount);

//		}
	}
// ---------------------- end main loop --------------------
//	con_close_connection(hub_in_con,1); // close down client
	con_close_connection(in_con,1);     // close down listener
	con_close_connection(out_con,1);    // close down listener
}
#endif

#ifdef HUBMAIN
int main(int argc, char **argv) {
	int port;
	int term=10;
	int timeout=0;
	int timeout2=0;
	char *serv;
	char aaa[100];
	int pt;
	int isprivate=0;
	int retry=0;
	int i,j;
	int lookup=1;
	int block=0;
	int duplex = 0;
	int blockcount = 0;
	int dispatchmode = 0;
	int switchmode =0;
	char *hubid = NULL;
	char *alist = NULL;
	int maxin=0,maxout=0;
	int maxbuf = 0xffff;
	int killout = 0;
	int killmax = 0;
	int sync_kill = 0;
	int trace = 0;
	int nopoll = 0;
	int sched = 300;
	int queue_size = 0;
	int scopy(int,char**);
	int ports_on[10];
	int full_counts = 0;
	char *ports_name[10];
#ifdef LITE
	maxin = 3 ; maxout = 3;
#endif
	if (argc > 1 &&  ( (strcmp(argv[1],"copy") == 0) || (strcmp(argv[1],"scopy") == 0) ) ) {
		return scopy(argc-1,&argv[1]);
	}
	ports_name[0] = "out     ";
	ports_name[1] = "in      ";
	ports_name[2] = "status  ";
	ports_name[3] = "control ";
	ports_name[4] = "in2/id  ";
	ports_name[5] = "in3/d   ";
	ports_name[6] = "in4/d/id";
	ports_name[7] = "out/id  ";
	serv = NULL;
	for(i = 0 ; i <  10 ; i++ ) {
		ports_on[i] = 1;
	}
    for(i = 1 ;  ; i++ ) { int j=0;
        if (i >= argc) {
            break;
        } else {
            if		 (argv[i][0] == '-' && argv[i][1] == 'b' ) { // block if no writers 
                block = 1; blockcount = atoi(&argv[i][2]);
				fprintf(stderr,"blocking with # = %d\n",blockcount);
            }else if (argv[i][0] == '-' && argv[i][1] == 'B') { // block if no writers
				block = 2; blockcount = atoi(&argv[i][2]);
				fprintf(stderr,"Blocking with # = %d\n",blockcount);
            }else if (argv[i][0] == '-' && argv[i][1] == 'I') { // max in
#ifndef LITE
				maxin = atoi(&argv[i][2]);
				fprintf(stderr,"Max input connections %d\n",maxin);
#endif
            }else if (argv[i][0] == '-' && argv[i][1] == 'O') { // max out
#ifndef LITE
				maxout = atoi(&argv[i][2]);
				fprintf(stderr,"Max output connections %d\n",maxout);
#endif
            }else if (argv[i][0] == '-' && argv[i][1] == 'm') { // 
				maxbuf =  atoi(&argv[i][2]);
				maxbuf = ibetween(MAXNAME+40,maxbuf,0xffffff);
				fprintf(stderr,"max buffer size  = %d\n",maxbuf);
            }else if (argv[i][0] == '-' && argv[i][1] == 'r') { // 
				term =  atoi(&argv[i][2]);
				fprintf(stderr,"record terminator  = %d\n",term);
            }else if (argv[i][0] == '-' && argv[i][1] == 't') { // 
				timeout = atoi(&argv[i][2]);
				fprintf(stderr,"timeout = %d\n",timeout);
            }else if (argv[i][0] == '-' && argv[i][1] == 'q') { // 
				queue_size = atoi(&argv[i][2]);
				fprintf(stderr,"queue size = %d\n",queue_size);
            }else if (argv[i][0] == '-' && argv[i][1] == 'T') { // 
				timeout2 = atoi(&argv[i][2]);
				fprintf(stderr,"Control timeout = %d\n",timeout2);
            }else if (argv[i][0] == '-' && argv[i][1] == 'S') { // 
				sched = atoi(&argv[i][2]);
				fprintf(stderr,"Schedule factor = %d\n",sched);
//          }else if (argv[i][0] == '-' && argv[i][1] == 'k') { // 
//				killout = 1;
//				fprintf(stderr,"kill output on no input = on\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 's') { // 
				switchmode = 1;
				fprintf(stderr,"switching = on\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 'K') { // 
				killmax = 1;
				fprintf(stderr,"killmax = on\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 'c') { // 
				sync_kill = 1;
				fprintf(stderr,"sync_kill = on\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 'd') { // 
				dispatchmode = 1;
				fprintf(stderr,"dispatch = on\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 'f') { //
				lookup = 0;
				fprintf(stderr,"lookup = off\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 'F') { // 
				lookup = 1;
				fprintf(stderr,"lookup = on\n");
            }else if (argv[i][0] == '+' && argv[i][1] == 'f') { // 
				full_counts = 1;
				fprintf(stderr,"full counts = on\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 'v') { // 
				trace = 1;
				fprintf(stderr,"trace = on\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 'P') { // 
				nopoll = 1;
				fprintf(stderr,"Less Polling = on\n");
            }else if (argv[i][0] == '-' && argv[i][1] == 'i') { // 
				hubid = &argv[i][2];
            }else if (argv[i][0] == '-' && argv[i][1] == 'p') { // ports off
				for(j = 2 ; ; j++ ) { int c;
					c = argv[i][j];
					if (c == 0) {
						break;
					}
					if (c == ibetween('0',c,'9')) {
						ports_on[c-'0'] = 0;
						fprintf(stderr,"port offset %d (%s) is off\n",c-'0',ports_name[c-'0']);
					}
				}
            }else if (argv[i][0] == '-' && argv[i][1] == 'a') { // 
				alist = &argv[i][2];
				fprintf(stderr,"access list = %s\n",alist);
           // }else if (argv[i][0] == '-' && argv[i][1] == 'b') { // 
				//size = atoi(&argv[i][2]);
			} else {
				break;
			} // if block
			
		}  // if
	} // for
    for(j = 1 ; j <  i ; j++ ) {
		argc--; argv++;
	}
    if (argc < 2) {
        fprintf(stderr,"usage: hub [-opts] portno  [server:port retry-secs alt:ports...] %s\n",VERS);
        fprintf(stderr,"usage: hub scopy [scopy args] with no args output scopy usage\n\n");
        fprintf(stderr,"    -r#    message terminator byte 0..255 (-1=stream) (default = 10 or newline)\n");
        fprintf(stderr,"    -t#    timeout (default = 0 or none)\n");
        fprintf(stderr,"    -b#    block if less than # writers at startup\n");
        fprintf(stderr,"    -B#    block if less than # writers always\n");
        fprintf(stderr,"    -I#    maximum number of input connections\n");
        fprintf(stderr,"    -O#    maximum number of output connections\n");
        fprintf(stderr,"    -K     kill connection on max exceeded\n");
        fprintf(stderr,"    -m#    maximum buffer size (default = 65000)\n");
        fprintf(stderr,"    -afile Access list file\n");
        fprintf(stderr,"    -c     close mode   (close out readers when writer closes connection)\n");
        fprintf(stderr,"    -s     switch mode   (inputs round robin)\n");
        fprintf(stderr,"    -d     dispatch mode (outputs round robin)\n");
        fprintf(stderr,"    -f     faster connection, no reverse lookup\n");
        fprintf(stderr,"    -F     cancel -f\n");
        fprintf(stderr,"    -v     Trace inputs\n");
        fprintf(stderr,"    -P     avoid polling on reads and connects (experimental)\n");
        fprintf(stderr,"    +f     output full counts on stats\n");
//        fprintf(stderr,"    -K     Kill output connections when input go to zero\n");
        fprintf(stderr,"    -S#    schedule factor (def=300)\n");
        fprintf(stderr,"    -T#    control port timeout in seconds\n");
        fprintf(stderr,"    -innn  id string for this hub\n");
        fprintf(stderr,"    -qnnn  server queue sizes, default=system default\n");
        fprintf(stderr,"    -p012..port offsets not to enable\n");
       // fprintf(stderr,"\n");
        exit(0);
    }
	if (argc >= 3) { char *q;
		pt = tcp2_address(argv[2],aaa,0);
		if (pt) {
			serv = aaa;
//		strcpy(aaa,argv[4]);
//		if ((p=strchr(&aaa[2],':')) != NULL) {
//			*p = '\0';
//			pt = atoi(p+1);
//			serv = aaa;
		}else {
			fprintf(stderr,"invalid server:port %s\n",argv[2]);
			exit(0);
		}
		q = strchr(argv[2],':');
		if (q && q[1] == ':') {
			duplex++;
			q= strchr(argv[2],'+');
			if (!q) {
badserver:
				fprintf(stderr,"invalid server::port %s\n(requires +dup or +Dup for all alternates - and the same for each)\n",argv[2]);
				exit(0);
			}
			if (q[1] == 'd') {
					isprivate = 0;
			} else if (q[1] == 'D') {
					isprivate = 1;
			} else {
				goto badserver;
			}
		}

		
	}

	port = atoi(argv[1]);
	if (argc >= 4) {
		retry = atoi(argv[3]);
	}
	if (port != ibetween(1,port,0x7fff)) {
		fprintf(stderr,"invalid port number %d (1..65000)\n",port);
		exit(0);
	}
#ifdef LITE
	fprintf(stderr,"Lite version, maximum input connects = %d, out = %d\n",maxin,maxout);
#endif

	hub_begin(hubid
				,lookup	
				,dispatchmode					/* */
				,block							/* */
				,blockcount						/* */
				,switchmode						/* */
				,port							/* */
				,maxbuf							/* buffer size - max 16 bits */
				,timeout						/* */
				,term							/* */
				,serv							/* */
				,pt								/* */
				,retry							/* */
				,&argv[4]						/* */
				,argc > 4? argc-4: 0			/* */
				,duplex							/* */
				,timeout2						/* control port timeout */
				,maxin
				,maxout
				,killout						/* kill output when no input - not implemented*/
				,trace
				,isprivate
				,alist							/* access list file or NULL */
				,sched							/* scheduling parameter */
				,killmax
				,queue_size						/* server queue sizes (send/rec) */
				,ports_on						/* array of port numbers (offsets) to enable/not enable 0=no*/
				,sync_kill 						/* kill all output connections after input connection closes */
				,full_counts					/* full counters */
				,nopoll							/* no polling mode */
		);
	return 0;
}
#endif

#ifdef TESTING

int main(int argc, char **argv) {

    char buffer[128];                   // character buffer for i/o         
    unsigned short port=5001;           // port number to create as server      
    int retval;                         // for recv/send count      
    SOCKET  msgsock,listens=0;          // two sockets for server, listens must be zero initially         
    int poller = 0;                     // if an argc > 1 then don't poll       
    int cur    = 0;                     // use with read record       
	int b[5];
	static holder[5] = {0,0,0,0,0};
    if(argc >2) {
        poller = 1;
    }
	
	msgsock = tcp2_client_init( 3, "127.0.0.1", 5000, 0, 0 );
	retval  = tcp2_read_record( msgsock, buffer, 128, holder, 1, 10 );

    if (argc < 2) {
        fprintf(stderr,"usage: server     portno   [poll]\n");
        fprintf(stderr,"usage: client     0 server portno\n");
        fprintf(stderr,"usage: conserver  1 server portno\n");
        exit(0);
    }
    port = atoi(argv[1]);

    if (port > 10) {
        for(;;) {
            
// -- create server, and wait or poll for connection (this example polls)
            
            fprintf(stderr,"waiting on port %d\n",port) ;
            for(;;) {
                msgsock = tcp2_server_init(port,&listens,poller,1,0,0);  // must init listens to 0 as above, returns 2 sockets, msgsock is for send/recv
                if (msgsock != INVALID_SOCKET) {
                    break; // inbound connection (or error)
                } else {
                    fprintf(stderr,"waiting for socket connection...%d\n",msgsock);
                    Sleep(1000); // wait then continue to poll
                }
            }
// test for server creation failure and print error on fail

            if (!msgsock) {
                fprintf(stderr,"tcp error: %s\n", tcp2_get_error_mess());
                break; // quit program
            }

// poll until data is ready on msgsock

            fprintf(stderr,"accepted connection from %s, port %d\n",tcp2_server_inaddr(), tcp2_server_inport()) ;
            for(;;) {
                if (tcp2_server_ready_read(msgsock)) {
                    break;
                }
                fprintf(stderr,"waiting for socket read ready...\n");
                Sleep(1000);
            }

            
// receive data

            retval = recv(msgsock,buffer,sizeof (buffer),0 );
#ifndef UNIXSYS         
            if (retval == SOCKET_ERROR) {
                fprintf(stderr,"recv() failed: error %d\n",WSAGetLastError());
                closesocket (msgsock);
                closesocket (listens);
                listens = 0;
                continue;
            }
#else
            if (retval == -1) {
                fprintf(stderr,"recv() failed: error %s\n",strerror(errno));
                closesocket (msgsock);
                closesocket (listens);
                listens = 0;
                continue;
            }
#endif      
            if (retval == 0) {
                printf("Client closed connection\n");
                closesocket (msgsock);
                closesocket (listens);
                listens = 0;
                continue;
            }

// insure null at end of buffer but also send back number of bytes that were received

            buffer[retval] = '\0';
            fprintf(stderr,"Received %d bytes, data [%s] from client\n",retval,buffer);

            fprintf(stderr,"Echoing same data back to client\n");
            retval = send(msgsock,buffer,retval,0);
#ifndef UNIXSYS         
            if (retval == SOCKET_ERROR) {
                fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
            }
#else
            if (retval == -1) {
                fprintf(stderr,"send() failed: error %s\n",strerror(errno));
            }
#endif      

// close ONLY the data socket, the listener is still alive, go back and wait/poll for next connection

            fprintf(stderr,"Terminating connection, keeping listener socket alive\n");
            //closesocket (listens);
            //listens = 0;
            closesocket (msgsock);
        }
    } else if (port == 2) { int n=0;
#ifdef HUB
			if (argc >=4) {
				n = atoi(argv[3]);
				fprintf(stderr,"hub using terminator %d\n",n);
			}
			hub_begin(atoi(argv[2]),50000, 50,n,"linux",6666,1);
#endif
    } else if (port == 1) { PCONNOBJ portlis,co; int stat,k; int nc=0; PCONNOBJ ccs[10]; char buf[500]; int ppp=0; 
							int count=0; 
							//char *p;

//			p = list_new(4);
//			strcpy(p,"hello");
//			list_old(p);
			
			port = atoi(argv[2]);
			con_init();
			portlis   = con_new_connection(0,0,1,0,0);		  // no buffers, just create a connection to listen with
			stat      = con_server_init(portlis,port,1,NULL,0); // the NULL means we are only creating a listener, should poll if no buffers
			if (stat == -1) {
				fprintf(stderr,"port %d listening %d\n",port,portlis->sock2);
			}

			co        = con_new_connection(50000,0,1,0,0);

			for(;;) { 
				stat = con_server_init(co,port,1,portlis,0);
				if (stat> 0) {
					ccs[nc++] = co;
					fprintf(stderr,"created connection socket=%d %d\n",co->sock1,co->sock2);
					co   = con_new_connection(50000,0,1,0,0);
					continue;
				}
				fprintf(stderr,"polling %d\n",ppp++);
				for(k = 0 ; k <  nc ; k++ ) { int nnn,mmm=0;
					if (!ccs[k]) {
						continue;
					}
					stat = con_read_record(ccs[k], buf,1,1);
					fprintf(stderr,"   checking connection %d socket %d stat= %d\n",k,ccs[k]->sock1,stat);
					if (stat > 0) {
						fprintf(stderr,"   read [%s] n=%d\n",buf,stat);
						for(;;) {
							sprintf(buf,"This-is-a-test-string %06d\n",count);
							nnn = con_write_record(ccs[k], buf,strlen(buf)+1,  1) ;

							if (nnn > 0) {
								//fprintf(stderr,"   write ok nnn=%d  %d\n",nnn,mmm);
								mmm++;
								count++;
								continue; //break;
							} else if(nnn == 0) {
								goto out;
							} else if(nnn == -1) {
								fprintf(stderr,"   write poll %d count=%d\n",mmm,count);
								mmm++;
								Sleep(1000);
								continue; //break;
							} else {
								continue;
							}
						}
						break;
					}
					if (stat == 0) {
						fprintf(stderr,"   error = %d\n",ccs[k]->error);
						ccs[k] = 0;
					}
				}
				Sleep(1000);
			}
out:
            fprintf(stderr,"finished\n");
			Sleep(1500);
    } else{ 
        char *server;
        int n;
        SOCKET  sock;
        char buff[500];

        port = atoi(argv[3]);
        server = argv[2];

        sock = tcp2_client_init(3,server,port,40000,0);
        if (!sock) {
            fprintf(stderr,"client error %s\n",tcp2_get_error_mess());
            fprintf(stderr,"client error %d\n",tcp2_get_error());
            exit(0);
        }
#ifdef UNIXSYS
//#define SO_SNDLOWAT    19
//      {int val,len,stat;
//      stat = getsockopt(sock,SOL_SOCKET,SO_SNDLOWAT,(char *)&val,&len);
//      fprintf(stderr,"val=%d len=%d stat=%d\n",val,len,stat);
//      }
#endif

        strcpy(buff,"hello there-line 2-and three\n");
		buff[11] = '\0';
		buff[18] = '\0';
        n = send(sock,buff,29,0);
#ifndef UNIXSYS         
        if (n == SOCKET_ERROR) {
            fprintf(stderr,"send() failed: error %d %s\n",WSAGetLastError(),erris(WSAGetLastError()));
        }
#else
        if (n == -1) {
            fprintf(stderr,"send() failed: error %s\n",strerror(errno));
        }
#endif      
        fprintf(stderr,"n on send = %d\n",n);
		b[0] = b[1] = b[2] = b[3] = b[4]= 0;
		buff[0] = (unsigned char)0xff;
		moveb(buff,&buff[1],sizeof(buff)-1);
		n=0;
		{int x=1; int rec,siz,rsize; int errs=0;
			for(;;) {int n5; char *p; char bbb[300]; 
				n5 = tcp2_read_record(sock,buff,sizeof(buff),b,01,'\n');
				if (n5 == 0) {
					if (tcp2_get_error() == 0) {
						fprintf(stderr,"socket closed\n");
					}
					break;
				} else if (n5 < 0) {
					fprintf(stderr,"                                         polling %d errs=%d x = %d\n",n++,errs,x);
					Sleep(1000);
					continue;
				} else if (n5 == sizeof(buff)) {
					fprintf(stderr,"buffer overflow with no record terminator\n");
					continue;
				}
				p = &buff[b[0]];
				moveb(p,bbb,n5);
				bbb[n5] = '\0';
				sscanf(bbb,"%d %d",&rec,&siz);
				rsize = strlen(bbb);
				if (x != rec || rsize != 10+10*siz || x%100 == 0) {
					if (x == rec && rsize == 10+10*siz) {
				        fprintf(stderr,"rec=%d siz=%d rsize=%d  %d  ok\n",rec,siz,rsize,10+10*siz);
					} else {
				        fprintf(stderr,"rec=%d siz=%d rsize=%d  %d\n",rec,siz,rsize,10+10*siz);
						errs++;
					}
					x = rec;
				}
				x++;
				if(x > 300000) {
					break;
					Sleep(1000000);
				}
				//fprintf(stderr,"[%s]%d\n",bbb,n5);
			}
		}
        tcp2_close_socket(sock);

    }
    return 0;
}
#endif
