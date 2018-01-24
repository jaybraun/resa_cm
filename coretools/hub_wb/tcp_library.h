#ifdef __cplusplus
extern "C" {
#endif
#ifdef UNIXSYS
# ifndef SOCKET
# define  SOCKET unsigned int 
# endif
#else
#  define WIN32_LEAN_AND_MEAN
#  include <winsock2.h>
#endif
#ifndef CONN_obj
#define CONN_obj void *
#endif
typedef struct connection_obj {
	SOCKET sock1;	//main socket
	SOCKET sock2;	//listen socket
	int  port;      //listen port number, temp
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

} CONN_OBJ, *CONN_OBJ_PTR;

/* ---------------------------- tcp layer ---------------------------------------------------*/
char *	tcp2_get_error_mess();
int		tcp2_get_error();
char *	tcp2_server_inaddr();
int		tcp2_server_inport();
void	tcp2_close_socket(SOCKET s);
		
int		tcp2_server_ready_read (SOCKET s);
int		tcp2_client_ready_read (SOCKET s);
int		tcp2_server_ready_write(SOCKET s) ;
int		tcp2_client_ready_write(SOCKET s) ;

SOCKET	tcp2_server_init(int port,SOCKET *listens,int poll,int reuse,int lookup,int buf_size);
SOCKET	tcp2_client_init(int kind, char * server_name, int port_number,int buf_size, int myport);

int		tcp2_read_record(SOCKET s, char * buf, int  pmaxbuf, int holder[5], int poll, int term);
int		tcp2_write_record(SOCKET s, char * buf, int  size, int holder[5], int poll);
int		tcp2_address(char *name,char *server,int offset);

/* ----------------------------- connection layer -------------------------------------------*/
void		con_init() ;

CONN_obj	con_new_connection(int bufsize,int term, int reuse, int option2, int option3);

int			con_client_init(CONN_obj con,  char * server_name, int port_number,int buf_size,int myport);
int			con_server_init(CONN_obj con, int port, int poll ,CONN_obj listen,int lookup) ;
int			con_read_record(CONN_obj con, char * buf,int maxbuf,int poll);
int			con_read_record_with_timeout(CONN_obj con, char * buf,int maxbuf,int tout);
int			con_write_record(CONN_obj con, char * buf,int size, int poll);
int			con_server_accept(CONN_obj con, int poll, CONN_obj listen,int lookup ,char *typ,void * access   );

int			con_printf(CONN_obj con, char * format, ... ) ;

int			con_get_error(CONN_obj con ) ;
char*		con_get_errorstr(CONN_obj con ) ;

SOCKET		con_get_sock1(CONN_obj con );
SOCKET		con_get_sock2(CONN_obj con );
char*		con_get_other_name(CONN_obj con );
int			con_get_other_port(CONN_obj con );

void		con_close_connection(CONN_obj con, int kill);
void		con_close_data_connection(CONN_obj con, int kill);

void		hub_begin(char *id,int lookup, int dispatchmode, int block,int blockcount,int switchmode,int ports, int bufsize
					  , int ptimeout,int term,char *input_server
					  , int input_port, int drop,char **alt
					  , int nalt, int upsteam_duplex,int timeout2
					  ,int maxin,int maxout,int killout,int trace,int isprivate,char *alist,int sched,int killmax
					  ,int queue_size,int ports_on[10],int sync_kill,int full_counts, int nopoll);

/*

  The tcp2_* routines are a layer below the con_* routines. There is no need
  to go below the con_* layer.(with the possible exception of the tcp2_address routine).

  These routines overlay a record structure on a stream based tcp/ip network.
  A record is an array of bytes with a reserved terminator character, such as
  a newline or a null. The user must abide by these record definitions, but data
  content is otherwise not mandated and is up to the user.
  
	Two modes are supported, polling and blocking.  Polling mode does not function for a client
	connection (yet). It either fails in a short time or is sucessful. Inbound connections
	to a server can be periodically checked in a master loop where no blocking occurs, 
	and the user is careful to sleep if no activity occured during a cycle in the master loop.

	This technique can be used to develop sophisticated control loops without using threads
	or signals, all of which are still not (as of jan 2000) easily portable due to 
	errors and omissions in the more advanced methods. It is also somewhat simpler to 
	manage because no locking or synchronization needs to be performed. But, really huge
	stuff can get tricky. This library was created to facilitate a hub program.

  The con layer uses CONN_obj objects, created by con_new_connection. For example:

  	CONN_obj obj1=0;  // a reference pointer

	obj    =	con_new_connection(50000,term,0,0,0);      // create a connection, set the termintor character
	listen =	con_new_connection(50000,term,0,0,0);      // create a connection, set the termintor character

	stat =	con_client_init(obj,sin,pin,0,myport);     // connect as a client, sin/pin = server and port

	stat = con_server_init(con,port,0,NULL);			// server connection that blocks and accepts a single connection
														// first call sets up the listener, returns -1, then call
														// again to get a connection.
	
	stat = con_server_init(listen,port,1,NULL);			// server connection that polls, when polling, the
														// call only sets up the listener, returns -1, 
														// then call con_server_accept
	stat = con_server_accept(obj, 1,  listen);			// accept connection with resued listen socket object


	stat =	con_read_record(obj1,buffer,0);				// read a record 
	stat =  con_write_record(obj2,buffer,n,0);			// write n bytes
			con_close_connection(obj1,1);				// close socket(s)and optionally free all memory



  A CONN_obj houses 2 sockets. For a client only one is used. For a server, there is
  a listen socket and a connection socket. A CONN_obj can also be created that is used
  as a listening socket object only. It is created with a buffer size of 0. It is then
  used to accept multiple connections on the same port.

  A single CONN_obj used as a server will only create the listening socket once, and
  can be reused in either blocking or polling mode after a close provided the memory
  is not freed. The con_server_accept takes 2 connection objects where the port number of the
  listening object is used for the new input connection (the object has its own socket for
  data transfers, but shares the listening socket).

  In polling mode, the value of 'stat' will be -1, 0 or a positive value. -1 always means
  that the connection has no activity at present. 0 is an error, usually just the connection
  is closed. The various get routines can be used for more info. An error code that is also 
  a  0 means an EOF or close. A value > 0 indicates that an operation was completed, and 
  often 'stat' is the character count.

  When reading records, the con_read_record will not report completion until it sees the
  terminator, though it may have actually performed a lower level socket recv data call. 

  When writing records, the return value of -1 means that this write has not yet completed, but
  there is nothing to be done at present. This permits the polling loop to perform other work.

  Note in either case where a read or write returns -1, it MUST be called again with the SAME
  arguments as before. Nothing can be changed or unpredictable results will occur.

*/


/*
connect api, used for simple multi-connection server

*/


void			connect_init				();
CONN_OBJ_PTR	connect_new_object_ref		();
CONN_OBJ_PTR	connect_new_server			(int port);

int				connect_new_arrival			(CONN_OBJ_PTR server,CONN_OBJ_PTR object);
void			connect_show_connections	(char *text);
int				connect_wait_for_ready		(int timeout_sec,int timeout_usec, int *writers);
void			connect_close_connection	(CONN_OBJ_PTR object);
void			connect_destroy_object_ref	(CONN_OBJ_PTR object);
int				connect_read_record			(CONN_OBJ_PTR object, char *buffer, int size);
int				connect_write_record		(CONN_OBJ_PTR object, char *buffer, int size);

#ifdef __cplusplus
}
#endif

