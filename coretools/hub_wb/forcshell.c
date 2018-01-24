#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "tcp_library.h"
#include <ctype.h>
#include <wctype.h>
#define WAIT 0
#define POLL 1
#define WHATEVER 3
#define SOCKBUFFSIZE 20000
static void moveb(char *f, char *t, int n) {
        int i;
        for(i = 0 ; i <  n ; i++ ) {
                *t++ = *f++;
        }
}
void fortran_millisleep( int *millisec )
{
        int nummilli;
        nummilli=*millisec;
        usleep(nummilli*1000);
}
void fortran_tcp_client_init( int *port, SOCKET *sockreturn )
{
        SOCKET sock;
        sock
        = tcp2_client_init( WHATEVER, "127.0.0.1", *port, SOCKBUFFSIZE, 0 );
        *sockreturn = sock;
}
void fortran_tcp_close_socket( int *sock )
{
        tcp2_close_socket( *sock );
}
void fortran_tcp_write_record( int *sock, int *mylen, char *buff, int forsize )
{
        int len, result;
        len=*mylen;
        result = send(*sock, buff, len, 0);
}
void fortran_tcp_nonblocking_read_record( int *sock
                            , char *buff
                            , char *working_buff
                            , int eric[]
                            , int *returned_len 
//next 2 args are appended by Fortran as lengths of char strings
                            , int forsize1 
                            , int forsize2)
{
        int len, return_value;
   for (;;) { //would loop only if len==0
        len = tcp2_read_record( *sock, working_buff, SOCKBUFFSIZE
                              , eric, POLL, 10 );
        if ( len >= SOCKBUFFSIZE )
        {
           return_value = -len;
           *returned_len = return_value;
           return;
        }
        if ( len >= 0 )
        {
           moveb ( &working_buff[eric[0]], buff, len );
           len--; //this will allow caller to clobber \n with \0
           return_value = len;
           *returned_len = return_value;
           return;

        }
        if ( len == -1 ) //nothing, caller should wait or take other action
        {
           return_value = -1;
           *returned_len = return_value;
           return;
        }
        //finaly, error if len==0
        return_value = 0;
        *returned_len = return_value;
        return;
   }
}
void fortran_tcp_blocking_read_record( int *sock
                            , char *buff
                            , char *working_buff
                            , int eric[]
                            , int *returned_len
//next 2 args are appended by Fortran as lengths of char strings
                            , int forsize1
                            , int forsize2)
{
        int len, return_value;
        len = tcp2_read_record( *sock, working_buff, SOCKBUFFSIZE
                              , eric, WAIT, 10 );
        if ( len >= SOCKBUFFSIZE )
        {
           return_value = -len;
           *returned_len = return_value;
           return;
        }
        if ( len >= 0 )
        {
           moveb ( &working_buff[eric[0]], buff, len );
           len--; //this will allow caller to clobber \n with \0
           return_value = len;
           *returned_len = return_value;
           return;

        }
        if ( len == -1 ) //nothing, caller should wait or take other action
        {
           return_value = -1;
           *returned_len = return_value;
           return;
        }
        //finaly, error if len==0, but should not occur on blocking read
        return_value = 0;
        *returned_len = return_value;
        return;
}

