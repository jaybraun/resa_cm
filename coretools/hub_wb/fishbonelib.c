#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifdef UNIXSYS
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define Sleep(a) usleep(a*1000)
#define slash      "/" 
char *Compile_Date=COMPILE_DATE;

#else
#define slash      "\\"
char *WIN_VERS = WINDOWS_VERSION;
#endif

#include "tcp_library.h"
#include "utils.h"

#define MAX_CONFED_SIZE      32
#define MAX_CBS_MSG     1000000
#define MAX_MSG_SIZE    1000000

long debug_flag = 0;
long pid        = 0;
char *wb_name = "wishbone.exe";

char adaptor_msg[MAX_MSG_SIZE+10];
char cbs_msg[MAX_MSG_SIZE+10];
char log_msg[MAX_MSG_SIZE+10];

char terminator = 10;
char cr         = 13;
char lf         = 13;

long hub_base_port, read_port, write_port, log_port;
long cmnd_port;
char *hub_ip;
char *adaptor_ip;

SOCKET	read_sock      = 0;  // program reads Hub data from this socket (hub writes to it)
SOCKET	write_sock     = 0;  // program writes data going to Hub to this socket  (hub reads from it)
SOCKET	log_sock       = 0;  // program reads data from Hub and saves to a log file

long create_connection(char *ip, long port, SOCKET *sock, char *name,int limit);
long connect_to_cbs(char *ip, long read_port, long write_port, long log_port,
                    SOCKET *read_sock, SOCKET *write_socket, SOCKET *log_sock);
long read_from_cbs(char **msg, long max_msg, SOCKET sock);
long read_log_data(char **msg, long max_msg, SOCKET sock);
long send_to_cbs(char *msg, long msg_len, SOCKET sock);
long send_to_adaptor(char *msg, long msg_len, SOCKET sock);
char *get_current_time (void);
void INFOMSG (char *fmt, ...);
void write_to_log_file(char *fmt, ...);
static int sock_ready(SOCKET s,int read);
long wait_for_data(SOCKET sock1, SOCKET sock2, SOCKET sock3);
long check_for_command( long con_flag );
long save_log_data(char *msg);
long process_running();
long check_proc_name(char *name);
long process_cmnd( CONN_OBJ_PTR q, char *cmnd_buf, long stat_client, 
                                                                long con_flag);
int getpid();

int fishbone(int argc, char **argv)
{
   long read_offset;
   long write_offset;
   long log_offset;
   long confed_number;
   long actor_number;
   long n,ndata=0;

   if ( argc < 10 )
   {
     save_log_data("\n**** Process stopping\n");
     INFOMSG("**** Missing arguments:\n");
     // We should indicate correct usage when we can
     exit(0);
   }

   if ( !check_proc_name(argv[0]) )
     INFOMSG("\n**** WARNING: running %s as %s may cause problems\n",
                                                       wb_name, argv[0]);
#ifdef UNIXSYS
   INFOMSG("\n%s\n",COMPILE_DATE);
#else
   INFOMSG("Compiled - %s %s - %s\n", __DATE__, __TIME__, WIN_VERS);
#endif

   pid = getpid();

   hub_base_port = read_port = write_port = log_port = cmnd_port = 0;

   adaptor_msg[0] = cbs_msg[0] = log_msg[0] = 0;

   hub_ip            = argv[1];
   hub_base_port     = atoi(argv[2]);
   read_offset       = atoi(argv[3]);
   write_offset      = atoi(argv[4]);
   log_offset        = atoi(argv[5]);
   confed_number     = atoi(argv[6]);
   actor_number      = atoi(argv[7]);
   adaptor_ip        = argv[8];
   // adaptor base port is not required for a federate
   if ( argc > 10 )
     debug_flag = atoi(argv[10]);

   if ( (n = process_running()) )
   {
     INFOMSG("Process is already running (pid = %d)\n",n);
     exit(0);
   }

   if ( hub_base_port )
   {
     read_port    = hub_base_port + read_offset;
     write_port   = hub_base_port + write_offset;
     log_port     = hub_base_port + log_offset;
     cmnd_port    = (hub_base_port / 100)*100 + 99;
   }

   connect_to_cbs (hub_ip, read_port, write_port, log_port, 
                           &read_sock, &write_sock, &log_sock);

   for ( ; ; )
   {
     char *temp = cbs_msg;
     n = read_from_cbs(&temp, MAX_MSG_SIZE, read_sock);
     if ( n > 0 )
     {
       if ( debug_flag )
         write_to_log_file("Received from Hub:\n   (%s)\n",temp);
       //send_to_adaptor(temp, n, adaptor_sock);
       ndata += n;
     }
     else if ( n == 0 )
     {
       read_sock = 0;
       connect_to_cbs(hub_ip, read_port, write_port, log_port, 
                              &read_sock, &write_sock, &log_sock);
     }

     temp = log_msg;
     if ( (n = read_log_data(&temp,MAX_MSG_SIZE,log_sock)) > 0 )
       ndata += n;

     check_for_command(0);
     if ( !ndata )
       Sleep( 10 );
     ndata = 0;
   }
}
//------------------------------------------------------------------------------

long wait_for_data(SOCKET sock1, SOCKET sock2, SOCKET sock3)
{
  int i;
  fd_set rset;

  FD_ZERO(&rset);
  FD_SET(sock1,&rset);
  FD_SET(sock2,&rset);
  FD_SET(sock3,&rset);
  i = select (FD_SETSIZE, &rset, NULL, NULL, NULL);
  return ( i );
}

//------------------------------------------------------------------------------
void convert_cr(char *buf)
{
  char *cp;

  if ( (cr != 13) || (lf != 10) )
  {
    cp = buf;
    for ( ; *cp ; cp++ )
      if ( (*cp == cr) )    //&& (*(cp+1) == lf) )
      {
//        *cp++ = 13;
        *cp   = 10;
      }
  }
}

void remove_nl(char *buf)
{
  char *cp;

  if ( (cr != 13) || (lf != 10) )
  {
    cp = buf;
    for ( ; *cp ; cp++ )
      if ( (*cp == 13) && (*(cp+1) == 10) )
      {
        *cp++ = cr;
        *cp   = lf;
      }
  }
}

//------------------------------------------------------------------------------
long create_connection(char *ip, long port, SOCKET *sock, char *name ,int limit)
{
  INFOMSG("%s Connecting to port %d (%s)\n", get_current_time(), port, ip);
  for ( ; ; )
  {
    *sock = tcp2_client_init (3, ip, port, 0,0);
    if ( *sock )
      break;
    INFOMSG("client error %s\n",tcp2_get_error_mess()); 
    if ( limit )
      break;
    
check_for_command(1);

    Sleep( 2000 );
  }
  if ( !*sock )
    return ( 0 );
  INFOMSG("%s Connection completed\n", get_current_time());
  if ( name && *name )
  {
    send (*sock, name, strlen(name), 0);
    send (*sock, &terminator, 1, 0);
  }
  return ( 1 );
}

//------------------------------------------------------------------------------
long disconnect_socket(SOCKET *sock)
{
  if ( sock && *sock )
  {
    tcp2_close_socket(*sock);
    *sock = 0;
  }
  return ( 1 );
}

long disconnect_cbs(SOCKET *read_sock, SOCKET *write_sock, SOCKET *log_sock)
{
  INFOMSG("%s Disconnecting from Hub\n", get_current_time());
  disconnect_socket(read_sock);
  disconnect_socket(write_sock);
  disconnect_socket(log_sock);
  return ( 1 );
}

//------------------------------------------------------------------------------
long connect_to_cbs(char *ip, long read_port, long write_port, long log_port, 
                       SOCKET *read_sock, SOCKET *write_sock, SOCKET *log_sock)
{
  disconnect_cbs(read_sock,write_sock,log_sock);

  if ( read_port )
  {
    INFOMSG("Attempting to connect to hub (read) (%s,%d)\n", ip, read_port);
    create_connection (ip, read_port, read_sock, "WB", 0);
  }
  if ( write_port )
  {
    INFOMSG("Attempting to connect to hub (write) (%s,%d)\n", ip, write_port);
    create_connection (ip, write_port, write_sock, "WB", 0);
  }
  if ( log_port )
  {
    INFOMSG("Attempting to connect to hub (log) (%s,%d)\n", ip, log_port);
    create_connection (ip, log_port, log_sock, "", 0);
  }
  return ( 1 );
}

//------------------------------------------------------------------------------
long read_from_cbs(char **msg, long max_msg, SOCKET sock)
{
  long n;
  static int b[5] = { 0, 0, 0, 0, 0 };

  if ( !sock )
    return ( 0 );

  n = tcp2_read_record(sock,*msg,max_msg,b,1,(int) terminator);
  if ( n > 0 )
  {
    *msg = *msg + b[0];
    if ( n == max_msg )
      INFOMSG("Message received from Hub is too large (%100.100s)\n",*msg);
    (*msg)[--n] = 0;
    convert_cr(*msg);
  }
  return ( n );
}

//------------------------------------------------------------------------------
long send_to_cbs(char *msg, long msg_len, SOCKET sock)
{
  long i, stat=0;
  static long n = 0;

  if ( !sock )
    return ( 0 );

  if (msg_len > MAX_CBS_MSG)
    INFOMSG("Hub message too large: (%d)(%100.100s)\n",msg_len,msg);
  else
  {
    if ( !sock_ready(sock,0) )
    {
      if ( (++n % 10) == 0 )
        INFOMSG("Message delayed: attempting to send message to Hub (%d)\n",n);
    }
    else
    {
      n = 0;
      remove_nl(msg);
      i = send (sock, msg, msg_len, 0);
      if ( i == -1 )
        INFOMSG("send() to Hub failed\n");
      else
      {
        i = send (sock, &terminator, 1, 0);
        if ( i == -1 )
          INFOMSG("send() to Hub failed (terminator)\n");
        else
          stat = 1;
      }
    }
  }
  return ( stat );
}
		
//------------------------------------------------------------------------------
long save_log_data(char *msg)
{
  long current_time;
  struct tm *timestruct;
  static long file_size = 1000001;
  static FILE *fd = 0;
  static char file_name[500];

  if ( (file_size > 1000000) || !msg )
  {
    file_size = 0;
    current_time = time (NULL);		// seconds since 1Jan70 (30 bits)
    timestruct = localtime( &current_time ); 
    sprintf (file_name, "ALSP_LOG.%02d%02d%02d%02d%02d",	//YYMMDDhhmm
                                   timestruct->tm_year % 100,
                                   timestruct->tm_mon + 1,
                                   timestruct->tm_mday,
                                   timestruct->tm_hour,
                                   timestruct->tm_min);
    if ( fd )
      fclose(fd);
    if ( (fd = fopen (file_name, "a")) == NULL )
      INFOMSG("Unable to open  (%s)\n", file_name);
    else
      file_size += fprintf (fd, "Opening new log file: %s\n", file_name);
  }
  if ( fd )
  {
    if ( msg )
      file_size += fprintf (fd, "%s\n", msg);
    fflush (fd);
  }
  return ( 1 );
}

long read_log_data(char **msg, long max_msg, SOCKET sock)
{
  long n;
  static int b[5] = { 0, 0, 0, 0, 0 };

  if ( !sock )
    return ( 0 );

  n = tcp2_read_record(sock,*msg,max_msg,b,1,(int) terminator);
  if ( n > 0 )
  {
    *msg = *msg + b[0];
    if ( n == max_msg )
      INFOMSG("Log message received from Hub is too large (%100.100s)\n",*msg);
    (*msg)[--n] = 0;
    convert_cr(*msg);
    save_log_data(*msg);
    if ( **msg == '!' )
    {
      Sleep (1000);                      // # seconds  
      process_cmnd(0, *msg+1, 1, 0);
    }
  }
  return ( n );
}

//------------------------------------------------------------------------------
long send_to_adaptor(char *msg, long msg_len, SOCKET sock)
{
  long i=0;
  union {
	long x;
	char y[4];
  } tst;

  if ( !sock )
    return ( 0 );

	//********** Is Socket ready for message
  while ( !tcp2_client_ready_write(sock) )
  {
    if ( i++ )
      INFOMSG("Adaptor socket not ready for write\n");
    Sleep (1000);  //# 1 sec	  
  }

	//********** Send the message length **********
  tst.x = ntohl (msg_len + 1);
  i = send (sock, tst.y, sizeof(int), 0);
  if ( i == -1 )
  {
    INFOMSG ("Unable to send message length to Adaptor (%100.100s)", msg);
    return ( 0 );
  }

	//********** Send the message text **********
  i = send (sock, msg, msg_len+1, 0);
  if ( i == -1 )
  {
    INFOMSG ("Unable to send message to Adaptor (%100.100s)", msg);
    return ( 0 );
  }

  return ( 1 );
}

//------------------------------------------------------------------------------
void write_to_log_file(char *fmt, ...)
{
  va_list pArgs;
  FILE *fd;

  va_start (pArgs,fmt);

  if ( (fd = fopen("zwb.log","a")) )
  {
    fprintf(fd,"**** %s\n",get_current_time());
    vfprintf(fd,fmt,pArgs);
    fprintf(fd,"\n\n");
    fclose(fd);
  }
 
  va_end (pArgs);
}
//------------------------------------------------------------------------------
void OUTMSG (char *fmt)		
{
  printf (fmt);
}

void INFOMSG (char *fmt, ...)
{
  char out_s [1000000];
  va_list pArgs;

  va_start (pArgs,fmt);            // Init to begining of variable-arguments
  vsprintf(out_s,fmt,pArgs);
  va_end (pArgs);
  OUTMSG(out_s);
  write_to_log_file(out_s);
  save_log_data(out_s);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
char *get_current_time (void)
{
	static char	time_s [28];
	time_t	current_time = time (NULL);		// seconds since 1Jan70 (30 bits)
	strcpy (time_s, ctime(&current_time));
	time_s[1] = time_s[8];
	time_s[2] = time_s[9];
	time_s[3] = 
	time_s[7] = '-';
	time_s[8] = time_s[22];
	time_s[9] = time_s[23];
	time_s[19]= 0;
	return  &time_s[1];		// dd-MMM-yy hh:mm:ss
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static int sock_ready(SOCKET s,int read) 
{
    int n;
    fd_set rset;
    struct timeval t;
    t.tv_sec = 0;
    t.tv_usec= 0;
    FD_ZERO(&rset);
    FD_SET(s,&rset);
    if (read == 1) {
        n = select(s+1,&rset,NULL,NULL,&t);  // read
    } else {
        n = select(s+1,NULL,&rset,NULL,&t);  // write
    }
	if ( !s )
	  return ( 0 );
    return n;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int send_forth_cmnd(char *msg)
{
  static char forth_hdr[] = "0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 0";
  int len;
  char buf[500];
  SOCKET fsock;

  if ( !msg || !*msg )
    return ( 0 );
  if ( strlen(msg) > 400 )
  {
    INFOMSG("Forth command too long - %s",msg);
    return ( 0 );
  }
  create_connection (hub_ip,hub_base_port+1 ,&fsock, "", 1);
  if ( !fsock )
  {
    INFOMSG("Unable to send Forth command");
    return ( 0 );
  }
  len = sprintf(buf,"%s %s",forth_hdr,msg);
  send_to_cbs(buf,len,fsock);
  disconnect_socket(&fsock);
  return ( 1 );
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
long process_cmnd( CONN_OBJ_PTR q, char *cmnd_buf, long stat_client, 
                                                                long con_flag )
{
  char cmnd[500];
  char *msg;

  sscanf(cmnd_buf,"%s",cmnd);
  msg = strchr(cmnd_buf,' ');
  if ( msg && *msg )
  {
    int i;
    for ( i=strlen(msg)-1 ; msg[i] < ' ' ; i-- )
      msg[i] = 0;
  }
  if ( strstr(cmnd,"STATUS") )
  {
    if ( q )
    {
      connect_write_record(q,cmnd,strlen(cmnd)); 
    }
  }
  else if ( strstr(cmnd,"NEW_LOG") )
  {
    save_log_data(NULL);
  }
  else
  {
    INFOMSG("\nUnknown command: (%s)\n", cmnd_buf);
  }
  return ( 1 );
}

long check_for_command( long con_flag )
{
  typedef struct connection_list {
       CONN_OBJ_PTR connlist_ptr;    // the connection pointer
       void *connlist_data_ptr;      // data associated with this connection
       int stat_client;
  } CONNLIST, *CONNLIST_PTR;

  static long first = 1;
  static char cmnd_buf[5000];
  static CONN_OBJ_PTR cs_server1    = 0;
  static CONN_OBJ_PTR cs_client1    = 0;
  static CONNLIST_PTR cs_client1_lh = 0;
  int stat,found;
  CONN_OBJ_PTR q;
  CONNLIST_PTR cl_q, cl_p, cl_temp;

  if ( first )
  {
    first = 0;
    cs_client1_lh = (CONNLIST_PTR) list_newlh();
    connect_init();
    cs_server1 = connect_new_server(cmnd_port);
    cs_client1 = connect_new_object_ref();
  }
 
  if ( !cs_server1 )
    return ( 1 );

  stat = connect_new_arrival(cs_server1,cs_client1); // check for connection arrival
  if ( stat > 0 ) 
  {
    cl_q = (CONNLIST_PTR) list_new(sizeof(CONNLIST),"conn_list");  // allocate a small object to save in a list
    cl_q->connlist_ptr = cs_client1;                // this object houses a connection pointer
    cl_q->connlist_data_ptr = NULL;                 // and a pointer to data associated with this connection
    found = 0;
    for_list(CONNLIST,cl_temp,cs_client1_lh) 
    {
      if ( cl_temp->stat_client )
      {
        found = 1;
        break;
      }
    }
    cl_q->stat_client = !found;
    list_insert_tail(cs_client1_lh,cl_q);           // place into list of connections
    INFOMSG("create cmnd connection at port: %d (%d)\n",
                                      cs_client1->port,cs_client1->other_port);
    cs_client1 = connect_new_object_ref();          // create a new connection object, for next connection
    connect_show_connections("1---");
  }
  for_list(CONNLIST,cl_q,cs_client1_lh) 
  {
    q = cl_q->connlist_ptr;
    stat = connect_read_record(q, cmnd_buf,sizeof(cmnd_buf)-1); // check for input on each
    if ( stat == 0 )
    {                                  // error or connection lost
      found = cl_q->stat_client;
      connect_close_connection(q);     // close the connection
      cl_p = (CONNLIST_PTR) list_prev(cl_q);          // to remove during a loop, use this technique
      list_remove(cl_q);               // save previous in list, remove current,
      INFOMSG("destroy connection server 1 at port: %d\n",q->other_port);
      connect_destroy_object_ref(q);   // destroy the current connection object
      list_old(cl_q);                  // and free up the list object as well
      cl_q = cl_p;                     // then set current to previous, at loop top, will cycle to next
      if ( found )
      {
        for_list(CONNLIST,cl_temp,cs_client1_lh) 
        {
          cl_temp->stat_client = 1;
          break;
        }
      }
    } 
    else if ( stat == -1 )
    {                    // nothing here now
    } 
    else 
    {                       // record in, size = stat
      stat = process_cmnd(q,cmnd_buf,cl_q->stat_client,con_flag);
    }
  }
  return ( 1 );
}

//------------------------------------------------------------------------------
long get_ip(char *name, char *ip)
{
  char *cp;
  struct sockaddr_in inaddr;
  int len_inaddr = sizeof(inaddr);
  SOCKET sock;

  *ip = 0;
  if ( (sock = tcp2_client_init(3,name,22,0,0)) )
  {
    if ( !getpeername((int)sock, (struct sockaddr *)&inaddr, (socklen_t *)&len_inaddr) )
    {
      if ( (cp = (char *)inet_ntoa(inaddr.sin_addr)) )
      {
        strcpy(ip,cp);
        return ( 1 );
      }
    }
    tcp2_close_socket(sock);
  }
  return ( 0 );
}

long process_running()
{
  int  local;
  char host_name[500];
  char host_ip[50];
  char cur_ip[50];
  char cmnd_buf[500];
  FILE *fd;

  if ( gethostname(host_name,sizeof(host_name)) < 0 )
    host_name[0] = 0;

  get_ip(host_name, host_ip);
  get_ip(hub_ip,cur_ip);

  local = ( !strcmp(cur_ip,"127.0.0.1") || !strcmp(cur_ip,host_ip) );


#ifdef UNIXSYS

  sprintf(cmnd_buf,"pgrep -x %s -d,>.zwb_sys", wb_name);
  system(cmnd_buf);

  if ( (fd = fopen(".zwb_sys","r")) )
  {
    char buf[1000];
    if ( fgets(buf,sizeof(buf)-1,fd) )
    {
      char *c;
      char *p = buf;
      fclose(fd);
      while ( (c = strtok(p,",")) )
      {
        int id = atoi(c);
        p = NULL;
        if ( id != pid )
        {
          char cmnd[100];
          sprintf(cmnd,"ps -o args --no-heading -p %d > .zwb_sys", id);
          system(cmnd);
          if ( (fd = fopen(".zwb_sys","r")) )
          {
            char temp_buf[500];
            while ( fgets(temp_buf,sizeof(temp_buf)-1,fd) )
            {
              int  g_port;
              char g_host[500];
              if ( sscanf(temp_buf,"%*s %s %d", g_host, &g_port) == 2 )
              {
                if ( g_port == hub_base_port )
                {
                  char ip[50];
                  get_ip(g_host,ip);
                  
                  if ( local && 
                         (!strcmp(ip,"127.0.0.1") || !strcmp(ip,host_ip)) )
                    return ( id );
                  if ( !strcmp(cur_ip,ip) )
                    return ( id );
                }
              }
            }
          }
          fclose(fd);
        }
      }
    }
  }
  return ( 0 );

#else

  sprintf(cmnd_buf,"netstat /b /n >.zwb_sys");
  system(cmnd_buf);

  if ( fd = fopen(".zwb_sys","r") )
  {
    int  id = 0;
    char buf[1000], temp_buf[1000];

    while ( fgets(buf,sizeof(buf)-1,fd) )
    {
      int  i, g_port;
      char c[500], ip[50], g_host[500];
      if ( strstr(buf,wb_name) )
      {
        sscanf(temp_buf,"%*s%*s%s%*s%d",c,&i);
		if ( i != pid )
		{
          strcpy(g_host,strtok(c,":"));
          g_port = (atoi(strtok(NULL,":")) / 1000) * 1000;
          if ( g_port == hub_base_port )
		  { 
            get_ip(g_host,ip);
            if ( (local && 
                   (!strcmp(ip,"127.0.0.1") || !strcmp(ip,host_ip))) ||
                      !strcmp(cur_ip,ip) )
			{
              id = i;
			  break;
			}
		  }
		}
      }
      else
        strcpy(temp_buf,buf);
    }
	fclose(fd);
	return ( id);
  }
  else
	INFOMSG("Unable to open zwb_sys. Wishbone may already be running");
  return ( 0 );

#endif
}

long check_proc_name(char *name)
{
  char *c;

  if ( (c = strrchr(name,*slash)) )
    c++;
  else
    c = name;
  return ( !strcmp(c,wb_name) );
}
