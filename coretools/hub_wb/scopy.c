//#pragma warning( disable : 4201 )  
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tcp_library.h"
#include "utils.h"
#include "cbuf.h"
#ifdef UNIXSYS
# define Sleep(a) usleep(a*1000)
# include <errno.h>
#endif

typedef struct a_buffer {
	int  b_queue;		// queue number 1..3
	int  b_group;		// group number 0..N-1
	int  b_len;			// length of current data
	int  b_size;		// maximum size of buf
	char b_buf[1];		// pointer to buffer
} ABUF, *PABUF;

	extern int tokens(),tty_get_char(),tty_setup(),tty_reset_in();
int teprintf(char *p_id,char * format, ... );
int last_time = 0;	// time in seconds that we last dumped buffer data

// --------------------------------------------
static void moveb(char *f, char *t, int n) {
	memcpy(t,f,n);
}
// --------------------------------------------









// ----------------------------------------------------------------
// is ready, test for file descriptor ready, only used on unix, simulate on windows
// ----------------------------------------------------------------
static int    is_ready(int s,int a_read) {
#ifdef UNIXSYS
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
    //fprintf(stderr,"select---s=%d n=%d\n",s,n);
    return n;
#else
	static int n = 0;

	n++;
	if (n%3 == 0) {
		return 1;
	}
	return 0;
#endif
}
// ----------------------------------------------------------------
// fread with a 10 second timeout but only if stdin is the input stream
// ----------------------------------------------------------------

static size_t xfread( void *pbuffer, size_t size, size_t count, FILE *stream ) {
	int bytes,pbytes,ntimes=0,i=0,m=0,n=0,total=0;
	char *buffer;

	static maxtimes = 500; // 10 seconds at most, then we flush what we have, if anything at all

	bytes = size * count;
	pbytes = bytes;
	buffer = (char *)pbuffer;



#ifdef UNIXSYS
	if (stream == stdin) {
		ntimes = 0;
		total = 0;
		for(i=0;;i++) {
			n = is_ready(0,1); // can we read fd 0 w/o blocking
			if (n > 0) {
				m = read( 0, buffer, bytes);
				//buffer[m] = 0;fprintf(stderr,"read---m=%d (%s)\n",m,buffer);
				//fprintf(stderr,"read---m=%d i = %d\n",m,i);

				if (m > 0) {
					total = total + m; // accumulate total read so far
					if (m == bytes) { // if true, we are done, having read everything we wanted to read
						//fprintf(stderr,"  --- return total, complete %d\n",total);
						return total;
					} else {
						bytes = pbytes - total; // remaining amount to read
						buffer = buffer + m;    // place to read into
					}
				} else if (m == 0) {
					return 0;
				} else if (m == -1) {
					if (errno != EINTR) {
						//fprintf(stderr,"  --- return total, zero     %d\n",total);
						return 0;
					}
				} else {
					return m;
				}
			} else {
				if (total > 0) {				// only count time after we read something
					ntimes++;
					if ( ntimes > maxtimes) {
						//fprintf(stderr,"  --- return total, short    %d\n",total);
						return total;
					}
				}
				//fprintf(stderr,"sleep\n");
				Sleep(20);						// wait for something to read
			}
		}

	} else {
		return fread(buffer,size,count,stream);
	}
#else
	return fread(buffer,size,count,stream);
#endif
}
// ----------------------------------------------------------------
// ----------------------------------------------------------------
















#ifdef HUB
scopy
#else
main 
#endif

		(int argc, char **argv)
{
	int i,j,n,k=0;
	int retries = 0xffffff;   // very large number
	int delay   = 5;
	int myport = 0;
	int binary = 0;
	int maxbuf = 2000000;
	int mp1 = 0;
	int mp2 = 0;
	int stat;
	int quiet=0;
	int hex=0;
	int fill=0;
	int term ;
	int term2;
	int nodata = 0;  // on . for stdin, if nodata is 1, then auto eof (use with -ixxx)
	int retried1 = 0;
	int retried2 = 0;
	int bsize = 0;
	int flsh = 1;
	int size = 2000000; // record size on input file
	int in   = 0;  // 0 file, 1,2 socket
	int out  = 0;
	char sin[100]; int pin=0;
	char sout[100]; int pout=0;
	char me[100] = "";
	char me2[100] = "";
	char logstr[100] = "";
	char *p;
	int nbuffers=1,nbuffers2=0,nbuffers3=0;
	int debug=0;
	int buffering = 0;
	int cbuffering = 0;
	int crecsize = 65000;
	PCBUF base = 0;
	char *cdata = 0;
	int progress = 0;
	int recsize = 65000;
	int recsize2 = 0;
	int recsize3 = 0;
	int seq=0,rcopied=0,rread=0;
	int okk=0;
	int no_close = 0;
#define NSIZEMAX  50
	int ngroup2[NSIZEMAX],ngroup3[NSIZEMAX];
	int nsize2[NSIZEMAX],nsize3[NSIZEMAX];
	int nsizes  = 1; // number of queues for each of 2nd and 3rd queue groups
	int nequal  = 0; // if 1, then equal size counts of buffers in queue groups
	int nqueues = 0; // max 3 queue sizes
	char *cin=0,*cout=0;
	int coutsize;
	int debugcount = 0;
	PABUF work_lh=0,free_lh=0,pbuf=0,curr_in=0,curr_out=0, q=0
		, free_lh2[NSIZEMAX]={0},free_lh3[NSIZEMAX]={0};
static	char *buffer,*buffer2;

	FILE *io1=0,*io2=0;

	CONN_obj obj1=0;
	CONN_obj obj2=0;
	
	term = '\n';
	term2 = '\n';
	if (argc <= 1) {
foo:
        fprintf(stderr,"Usage: scopy [opts] from to  (version 2.5d)\n");
        fprintf(stderr,"       -0   = use null terminators  (zero)      (one of these 4)\n");
        fprintf(stderr,"       -n   = use nl   terminators\n");
        fprintf(stderr,"       -0n  = convert null to newline\n");
        fprintf(stderr,"       -n0  = convert nl   to null\n\n");
        fprintf(stderr,"       -S   = stream (no terminators)\n");
        fprintf(stderr,"       -e   = no data (eof only) on stdin\n");
        fprintf(stderr,"       +Z   = no close on end of file, just go to sleep\n");
#ifdef WIN32
        fprintf(stderr,"       -b   = binary mode (windows only)\n\n");
#else
		fprintf(stderr,"       -F   = fill input if a pipe, in 10 seconds (unix only)\n\n");
#endif
        fprintf(stderr,"       -q   = quiet (no reconnect messages) (can be issued twice)\n");
        fprintf(stderr,"       -H   = convert binary file to simple hex records\n");
        fprintf(stderr,"       -ixxx= id in first record sent as xxx to input & output connection\n");
        fprintf(stderr,"       -jxxx= same as -e -q -q -r0 -ixxx to input & output  connection\n");
        fprintf(stderr,"       -Ixxx= id in first record sent as xxx to output connection (use after -i)\n");
        fprintf(stderr,"       -Jxxx= same as -e -q -q -r0 -ixxx to output connection (use after -j)\n");
        fprintf(stderr,"       -lxxx= log name, can also be host:port=name to log to a hub\n");
        fprintf(stderr,"       -m#  = maximum tcp record size (default=2000000 bytes)\n");
        fprintf(stderr,"       -s#  = size of reads when input is not tcp default= -m setting\n");
        fprintf(stderr,"       -B#  = snd/rcv queue size for sockets\n");
        fprintf(stderr,"       -r#  = retry on connect default = forever\n");
        fprintf(stderr,"       -d#  = delay in secs between retries default=5\n");
        fprintf(stderr,"       -p#  = my port number as a client\n");
        fprintf(stderr,"       -C#  = circular buffering size=# in megabytes\n");
        fprintf(stderr,"       -D#  = debug for buffering\n");
        fprintf(stderr,"       -f   = don't flush output on each write\n");
        fprintf(stderr,"       -E#  = 1=equal size of queue groups\n");
        fprintf(stderr,"       -N#  = number of buffer sizes in each group of size 2 and size 3\n");
        fprintf(stderr,"       -M#,#= number,size of buffers for tcp-tcp connections def=1,65000\n");
        fprintf(stderr,"              can be repeated <= 3 buffers and 3 sizes 1st must be largest\n");
        fprintf(stderr,"       file.ext\n");
        fprintf(stderr,"       ip:port\n");
        fprintf(stderr,"         .  = stdin or stdout\n");
		return 0;
	}

    for(i = 1 ;  ; i++ ) { //int j=0;
        if (i >= argc) {
            break;
        } else {
            if		 (argv[i][0] == '-' && argv[i][1] == '0'&& argv[i][2] == '\0'      ) { //  null terminated
                term = 0; term2 = 0;
            }else if (argv[i][0] == '-' && argv[i][1] == 'o'&& argv[i][2] == '\0'      ) { //  null terminated
                term = 0; term2 = 0;

            }else if (argv[i][0] == '-' && argv[i][1] == 'e') { // eof (no data)
				nodata = 1;  
            }else if (argv[i][0] == '-' && argv[i][1] == 's') { // size
				size = atoi(&argv[i][2]);
            }else if (argv[i][0] == '-' && argv[i][1] == 'm') { // record size
				maxbuf = atoi(&argv[i][2]);
				size = maxbuf;

            }else if (argv[i][0] == '-' && argv[i][1] == 'B') { // buffer size
				bsize = atoi(&argv[i][2]);

            }else if (argv[i][0] == '-' && argv[i][1] == 'd') { // delay
				delay = atoi(&argv[i][2]);

            }else if (argv[i][0] == '-' && argv[i][1] == 'i') { // id
				strcpy(me,&argv[i][2]);
				strcpy(me2,me);
            }else if (argv[i][0] == '-' && argv[i][1] == 'j') { // id with extras
				strcpy(me,&argv[i][2]);
				strcpy(me2,me);
				nodata = 1;
				quiet  = 2;
				retries= 0;

            }else if (argv[i][0] == '-' && argv[i][1] == 'l') { // log string (or log to hub)
				strcpy(logstr,&argv[i][2]);
            }else if (argv[i][0] == '-' && argv[i][1] == 'I') { // id
				strcpy(me2,&argv[i][2]);
            }else if (argv[i][0] == '-' && argv[i][1] == 'J') { // id with extras
				strcpy(me2,&argv[i][2]);
				nodata = 1;
				quiet  = 2;
				retries= 0;

            }else if (argv[i][0] == '-' && argv[i][1] == 'r') { // retries
				retries = atoi(&argv[i][2]);
            }else if (argv[i][0] == '-' && argv[i][1] == 'E') { // number of queue sizes per group
				nequal = atoi(&argv[i][2]);
            }else if (argv[i][0] == '-' && argv[i][1] == 'C') { // number of megabytes for C buffering
				cbuffering = atoi(&argv[i][2]) * 1000000;;
            }else if (argv[i][0] == '-' && argv[i][1] == 'N') { // number of queue sizes per group
				nsizes = atoi(&argv[i][2]);
				if (nsizes > NSIZEMAX) {
					fprintf(stderr,"invalid message queue group number -Nnumber > %d\n",NSIZEMAX);
					goto foo;
				}
            }else if (argv[i][0] == '-' && argv[i][1] == 'M') {int qac; char *qav[50],qout[200]; // number of buffers
				nqueues++;
				qac = tokens(&argv[i][2],qav,qout);
				if (qac != 2) {
					fprintf(stderr,"invalid message queue -Mnumber,size\n");
					goto foo;
				}
				if (nqueues > 3) {
					fprintf(stderr,"Maximum of 3 queues\n");
					goto foo;
				}
				if (nqueues == 1) {
					nbuffers = atoi(qav[0]);
					recsize = atoi(qav[1]); 
				} else if (nqueues == 2) {
					nbuffers2 = atoi(qav[0]);
					recsize2 = atoi(qav[1]); 
				} else {
					nbuffers3 = atoi(qav[0]);
					recsize3 = atoi(qav[1]); 
				}

            }else if (argv[i][0] == '+' && argv[i][1] == 'Z') { // no close
				no_close = 1;
            }else if (argv[i][0] == '-' && argv[i][1] == 'q') { // quiet
				quiet++;
            }else if (argv[i][0] == '-' && argv[i][1] == 'b') { // binary
				binary = 1;
            }else if (argv[i][0] == '-' && argv[i][1] == 'S') { // stream
				term = -1; term2 = -1;
            }else if (argv[i][0] == '-' && argv[i][1] == 'H') { // hex convert
				term = -1; term2 = -1;
				hex = 1;
				binary = 1;
				size = 40; // can be overridden

            }else if (argv[i][0] == '-' && argv[i][1] == 'F') { // fill
				fill = 1;
            }else if (argv[i][0] == '-' && argv[i][1] == 'f') { // flush
				flsh = 0;
            }else if (argv[i][0] == '-' && argv[i][1] == 'D') { // debug
				if (strlen(&argv[i][2]) == 0) {
					fprintf(stderr,"-D Debug value missing\n");
					exit(0);
				}
				debug =atoi(&argv[i][2]);
				fprintf(stderr,"Debug value = %d\n",debug);
				if (debug < 0) {
					debug = -debug;
					tty_setup();
				}
				

            }else if (argv[i][0] == '-' && argv[i][1] == 'p') { // client port
				myport = atoi(&argv[i][2]);

            }else if (argv[i][0] == '-' && argv[i][1] == 'P') { // client port in hex
																char * stopstring;
				myport = strtoul(&argv[i][2], &stopstring, 16); 

            }else if (argv[i][0] == '-' && argv[i][1] == '0'&& argv[i][2] == 'n') { // convert
                term = 0; term2 = 10;
            }else if (argv[i][0] == '-' && argv[i][1] == 'o'&& argv[i][2] == 'n') { // 
                term = 0; term2 = 10;


            }else if (argv[i][0] == '-' && argv[i][1] == 'n'&& argv[i][2] == '0') { // 
                term = 10; term2 = 0;
            }else if (argv[i][0] == '-' && argv[i][1] == 'n'&& argv[i][2] == 'o') { // 
                term = 10; term2 = 0;

            }else if (argv[i][0] == '-' && argv[i][1] == 'n') { // just -n for completeness, the default anyway

            }else if (argv[i][0] == '-' && argv[i][1] == 'r'&& argv[i][2] == 'r') { // 
            }else if (argv[i][0] == '-' && argv[i][1] == 'r'&& argv[i][2] == 'r') { // 
                                                                
			} else {
				break;
			} // if block
			
		}  // if
	} // for
	
	if (argc < i+1) {
		goto foo;
	}
	if (recsize2 > recsize || recsize3 > recsize2) {
		fprintf(stderr,"buffer record sizes must be largest to smallest = %d %d %d\n",recsize,recsize2,recsize3);
		goto foo;
	}
	if ((cbuffering > 0) && (cbuffering <= (maxbuf*2+5000))) {
		int j = ((maxbuf*2+5000)/1000000)+1;
		int k = j * 1000000;
		fprintf(stderr,"-C%d too small for given -m%d  - increased to -C%d = %d bytes\n", cbuffering/1000000, maxbuf, j, k );
		cbuffering = k ;
	}

	con_init();
	strcpy(sin,argv[i++]);
	if(argc >i) {
		strcpy(sout,argv[i++]);
	} else {
		strcpy(sout,".");
	}
// -------------------------------------
	for(i = 0 ; i <  NSIZEMAX ; i++ ) {
		ngroup2[i] = ngroup3[i] = 0;
		nsize2[i] = nsize3[i] = 0;
	}

	if (nbuffers3 > 0) {int i,j=0,k=0,nb;
		nb = recsize3/nsizes;
		
		for(i = 0 ; i <  nsizes ; i++ ) {
			nsize3[i] = recsize3 - (i)*nb;
			if (nequal == 1) {
				ngroup3[i]= nbuffers3 ;
			} else {
				ngroup3[i]=  (int)   (( (float)  recsize3 /  (float)  nsize3[i])   *   (float)  nbuffers3) ;
			}
		}
	}
	if (nbuffers2 > 0) {int i,j=0,k=0,nb;
		nb =  (recsize2-recsize3) /nsizes;
		
		for(i = 0 ; i <  nsizes ; i++ ) {
			nsize2[i] = recsize2 - (i)*nb;
			if (nequal == 1) {
				ngroup2[i]=   nbuffers2 ;
			} else {
				ngroup2[i]=  (int)   (( (float)  recsize2 /  (float)  nsize2[i])   *   (float)  nbuffers2) ;
			}
		}
	}

	work_lh = list_newlh();					// create listheads
	free_lh = list_newlh();					// for work and free list
	for(i = 0 ; i <  nbuffers ; i++ ) {
		pbuf = list_new(sizeof(ABUF)+recsize,"buffer");
		pbuf->b_size = recsize;
		pbuf->b_len = 0;
		pbuf->b_queue = 1;
		list_insert_tail(free_lh,pbuf);
	}
	for(j = 0 ; j <  nsizes ; j++ ) {
		free_lh2[j] = list_newlh();					// for work and free list
		free_lh3[j] = list_newlh();					// for work and free list
		for(i = 0 ; i <  ngroup2[j] ; i++ ) {
			pbuf = list_new(sizeof(ABUF)+nsize2[j],"buffer2");
			pbuf->b_size = nsize2[j];
			pbuf->b_len = 0;
			pbuf->b_queue = 2;
			pbuf->b_group = j;
			list_insert_tail(free_lh2[j],pbuf);
		}
		for(i = 0 ; i <  ngroup3[j] ; i++ ) {
			pbuf = list_new(sizeof(ABUF)+nsize3[j],"buffer3");
			pbuf->b_size = nsize3[j];
			pbuf->b_len = 0;
			pbuf->b_queue = 3;
			pbuf->b_group = j;
			list_insert_tail(free_lh3[j],pbuf);
		}
	}
	
// -------------------------------------
	if (cbuffering) {
		cdata = list_new(cbuffering,"cbuffer");
		base = Cbuf_init(cbuffering,cdata,0);
		cin = cout = NULL;
	}
// -------------------------------------
	if (   (strlen(sin)>2 &&  (   p=strchr(&sin[2],':')   ) != NULL)  || (sin[0] == '.' && sin[1] == ':')) {
		pin = tcp2_address(sin,sin,1);
		if (!pin) {
			fprintf(stderr,"invalid server:port = %s\n",sin);
			return 0;
		}
		in = 1;
	} else if (strlen(sin) == 1 && sin[0] == '.') {
		io1 = stdin;
	} else {
#ifdef WIN32
		if (binary) {
			io1 = fopen(sin,"rb");
		} else {
			io1 = fopen(sin,"r");
		}
#else
		io1 = fopen(sin,"r");
#endif
		if (!io1) {
			fprintf(stderr,"cannot open %s for input\n",sin);
			return 0;
		}
	}
// -------------------------------------
	if ((strlen(sout)>2 &&  (   p=strchr(&sout[2],':')   ) != NULL)|| (sout[0] == '.' && sout[1] == ':')) {
		pout = tcp2_address(sout,sout,1);
		if (!pout) {
			fprintf(stderr,"invalid server:port = %s\n",sout);
			return 0;
		}
		out = 1;
	} else if (strlen(sout) == 1 && sout[0] == '.') {
		io2 = stdout;
	} else {
#ifdef WIN32
		if (binary) {
			io2 = fopen(sout,"wb");
		} else {
			io2 = fopen(sout,"w");
		}
#else
		io2 = fopen(sout,"r");
#endif
		if (!io2) {
			fprintf(stderr,"cannot open %s for output\n",sout);
			return 0;
		}
	}
	if (myport) {
		mp1 = myport;
		mp2 = myport;
		if (in && out) {
			mp2++;
		}
	} else {
		mp1 = 0;
		mp2 = 0;
	}
	if (in && out &&  (nbuffers > 1) ) {int k,tot=0;
		buffering = 1;
		if(quiet==0)teprintf(logstr,"scopy bufs [#%d @ %d B]  = %d KB\n"
			,nbuffers,recsize

			,(nbuffers*recsize)/1000);
			tot = tot + (nbuffers*recsize)/1000;
		for(k = 0 ; k <  nsizes ; k++ ) {
			if(quiet==0)teprintf(logstr,"scopy bufs[%d] [%d @ %d B] [%d @ %d B] = %d KB\n",k
				,ngroup2[k],nsize2[k]
				,ngroup3[k],nsize3[k]

				,(ngroup2[k]*nsize2[k]
				+ngroup3[k]*nsize3[k])/1000);

			tot = tot + (ngroup2[k]*nsize2[k]+ngroup3[k]*nsize3[k])/1000;
		}
		if(quiet==0)teprintf(logstr,"scopy tot = %d KB\n",tot);

			
	}
//	teprintf(logstr,"this is a test of teprintf\n");
//	teprintf("localhost:5000=foobar2","this is a test of teprintf\n");
	buffer  = list_new(maxbuf+10,"Buffer");
	if (hex) {
		buffer2 = list_new(maxbuf+10,"Buffer2");
	} else {
		buffer2 = 0;
	}
	for(;;) {
// -------------------------------------
		if (in == 1) {
			stat = 0;
			if(!obj1)obj1 = con_new_connection(maxbuf,term,0,0,0);
			for(k = 0 ; k <  retries+1 ; k++ ) {
				stat = con_client_init(obj1,sin,pin,bsize,mp1);
				if (stat == 0) {
					if(quiet==0)teprintf(logstr,"trying... %s:%d for input %s\n",sin,pin,con_get_errorstr(obj1));
					con_close_data_connection(obj1,0);
					Sleep(1000 * delay); retried1++;
				} else {
					if (retried1) {
						if(quiet==0)teprintf(logstr,"input (re)connected\n");
					}
					break;
				}
			}
			if (stat == 0) {
				teprintf(logstr,"cannot open'%s' %d for input: \n%s\n",sin,pin,con_get_errorstr(obj1));
				return 0;
			}
			if (strlen(me) > 0) {
				if (term == '\n') {
					con_printf(obj1,"%s\n",me);   
				} else {
					con_write_record(obj1,me,strlen(me)+1,0); // include null in the ouput
				}

			}
			in = 2;
		}
// -------------------------------------
		if (out == 1) {
			stat = 0;
			if(!obj2)obj2 = con_new_connection(maxbuf,term2,0,0,0);
			for(k = 0 ; k <  retries+1 ; k++ ) {
				stat = con_client_init(obj2,sout,pout,bsize,mp2);
				if (stat == 0) {
					if(quiet==0)teprintf(logstr,"trying... %s:%d for output %s\n",sout,pout,con_get_errorstr(obj2));
					con_close_data_connection(obj2,0);
					Sleep(1000 * delay);retried2++;
				} else {
					if (retried2) {
						if(quiet==0)teprintf(logstr,"output (re)connected\n");
					}
					break;
				}
			}
			if (stat == 0) {
				teprintf(logstr,"cannot open '%s' %d for output:\n%s\n",sout,pout,con_get_errorstr(obj2));
				return 0;
			}
			if (strlen(me2) > 0) {
				if (term2 == '\n' || hex) {
					con_printf(obj2,"%s\n",me2);   
				} else {
					con_write_record(obj2,me2,strlen(me2)+1,0);
				}
			}
			out = 2;
		}
// ------------------------------------- loop
		if (cbuffering) { //													---------- C buffering ----------
			progress = 0;
			//if (seq > 200)exit(1);
			if (cin == NULL) {
				cin = Cbuf_get_free_space(base,maxbuf+10000);
			}
			if (cin) { 
				stat = con_read_record(obj1,cin,maxbuf+5000,1); // was crecsize
				if        (stat >   0) {
					progress = 1;
					rread++;
					if (term == term2) { // handle line terminator conversions
					} else if (term == 10 ) {
						for(k = 0 ; k <  stat ; k++ ) {
							if (cin[k] == '\n') {
								cin[k] = 0;
							} else if (cin[k] == 0){
								//
							}
						}
					} else if (term == 0) {
						for(k = 0 ; k <  stat ; k++ ) {
							if        (cin[k] == 0) {
								cin[k] = '\n';
							} else if (cin[k] == '\n'){
								//
							}
						}
					}
					Cbuf_split_free(base,stat);
					cin = 0; // indicate no longer in use, need another one
				} else if (stat == -1) {
				} else if (stat ==  0) {
					con_close_data_connection(obj1,0);
					if(quiet<2)teprintf(logstr,"input connection lost/closed\n");
					if(retries==0)break;
					Sleep(1000 * delay);
					in = 1;retried1++;
					continue;
				}
			}
	//      --------------
			if (cout == 0) {
				cout = Cbuf_get_data_block(base,&coutsize);
			}
			if (cout) { // anything to do?
				n = con_write_record(obj2,cout,coutsize,1);
				if (n == 0) { // connection closed
					if(quiet<2)teprintf(logstr,"output connection lost\n");
					con_close_data_connection(obj2,0);
					if(retries==0)break;
					Sleep(1000 * delay);
					out = 1;retried2++;
					continue;
				} else if (n == -1) { // no progress yet
				} else if (n >   0){  // finished
					progress = 1;
					rcopied++;
					Cbuf_release(base);
					cout = 0; // indicate no longer in use
				}
			} else { // nothing yet
			}


			// ------------------- logging -------------------
			if (logstr[0] != '\0' && debug) {int i,j,i2,j2,i3,j3,tb;  time_t nowtime;
				i = 0; j=0;
				i2 = 0; j2=0;
				i3 = 0; j3=0;
				seq++;

				time(&nowtime);
				if (last_time+debug <= nowtime || last_time == 0) {int numinqueue,totalsize,usedbytes,avail;
					last_time = nowtime;

					tb = 0;
					avail = Cbuf_avail(base,&numinqueue,&totalsize,&usedbytes);
					teprintf(logstr,"cstats %d r-w[%d %d %d] UFT[%d %d %d] q %d %% %d\n"
						,seq,rread,rcopied,rread-rcopied
						,usedbytes,avail/1000,totalsize/1000,numinqueue, (usedbytes) /(totalsize/100));
				}
			} else if (debug) {int i,j,i2,j2,i3,j3; 
				i = 0; j=0;
				i2 = 0; j2=0;
				i3 = 0; j3=0;
				seq++;
				if ( (seq % 50)  == 0 ) {int ch;
					ch = tty_get_char();
					if (ch == '0' || ch == 'z') {
						seq = 0;
						rread = 0;
						rcopied = 0;
						okk=0;
					} else if (ch == '1') {
						debug = 1;
					} else if (ch == '2') {
						debug = 10;
					} else if (ch == '3') {
						debug = 25;
					} else if (ch == '4') {
						debug = 100;
					} else if (ch == '5') {
						debug = 200;
					} else if (ch == '6') {
						debug = 1000;
					} else if (ch == 'Q') {
						tty_reset_in();
						return 0;
					} else {
					}
				}
				if ( (debug == 1 ) ||  ( (seq % debug)  == 0)  ) {int numinqueue,totalsize,usedbytes,avail;

					avail = Cbuf_avail(base,&numinqueue,&totalsize,&usedbytes);
					fprintf(stderr,"%d) r-w[%d %d %d] UFT[%d %d %d] q %d %% %d --\r"
						,seq,rread,rcopied,rread-rcopied
						,usedbytes,avail/1000,totalsize/1000,numinqueue, (usedbytes) /(totalsize/100));
					fflush(stderr);
				}
				
			}

			if (progress == 0) {
				Sleep(150);
			}
		// if c buffering




//		--------------------------



		} else if (buffering) { //													---------- buffering ----------
			progress = 0;
			if (curr_in == 0) { int k;
				curr_in = list_remove_head(free_lh);
				if (curr_in == 0) { PABUF p,px,pq,pn; int a2=0,a3=0;// we are now stuck can we adjust?
					
					for(k = 0 ; k <  nsizes ; k++ ) {
						if (!list_ishead(list_next(free_lh2[k]))) { // see if any of these are non-empty
							a2=1; //if the next on the list is not the listhead, then it's not empty
							break;
						}
					}
					for(k = 0 ; k <  nsizes ; k++ ) {
						if (!list_ishead(list_next(free_lh3[k]))) {
							a3=1;
							break;
						}
					}
					//a2= list_ishead(list_next(free_lh2[0]));
					//a3= list_ishead(list_next(free_lh3[0]));
					if (a2 || a3 ) { int nn=0; // if either one is not empty, might find something
						for_list(ABUF,p,work_lh) { // look down the work queue for something we can move off the largest
							nn++;
							if (p->b_queue == 1  ) { // is it on queue 1? if yes, then we try to free it
								for(k = nsizes-1 ; k >=  0 ; k-- ) {
									if (p->b_len < nsize3[k] ) {
										pq = list_remove_head(free_lh3[k]); // try for a free one
										if (pq ) {					 
											px = list_prev(p);				 // save position of the guy before us?
											pn = list_next(p);
											pq->b_len = p->b_len;			 // copy length
											moveb(p->b_buf,pq->b_buf,p->b_len); // copy data to smaller buffer
											list_clobber_check(p);list_clobber_check(pq);

											list_insert(px,pq); // insert new smaller guy in place of big one
											list_remove(p);     // remove the big one, so we can use it to proceed
											list_insert_head(free_lh,p); // put back onto largest free list
											//fprintf(stderr,"adjusted 3  [%d] = %d   \n",k,p->b_len);
											p = list_next(px);	// reset p to new guy in list
											//okk++; ;
											goto continue_search1;
										}
									}
								} // for


								for(k = nsizes-1 ; k >=  0 ; k-- ) {
									if (p->b_len < nsize2[k] ) {
										//fprintf(stderr,"maybe adjust 2  \n");
										pq = list_remove_head(free_lh2[k]); 
										if (pq) {					 
											px = list_prev(p);				 // save position of the guy before us?
											pn = list_next(p);
											pq->b_len = p->b_len;			 // copy length
											moveb(p->b_buf,pq->b_buf,p->b_len); // copy data to smaller buffer
											list_clobber_check(p);list_clobber_check(pq);

											list_insert(px,pq); // insert new smaller guy in place of big one
											list_remove(p);     // remove the big one, so we can use it to proceed
											list_insert_head(free_lh,p); // put back onto largest free list
											//fprintf(stderr,"adjusted 2 [%d]  = %d     \n",k,p->b_len);
											p = list_next(px);	// reset p to new guy in list
											//okk++;;
											goto continue_search1;
										}
									}

								} // for


							} // end on queue 1
continue_search1:
							continue;
						} // end for_list
					} // if a2 or a3 set
				} // if queue 1 empty (curr_in ==0)
			}// curr_in == 0
			if (curr_in) { int got_one;
				stat = con_read_record(obj1,curr_in->b_buf,recsize,1);
				if        (stat >   0) {
					progress = 1;
					rread++;
					if (term == term2) { // handle line terminator conversions
					} else if (term == 10 ) {
						for(k = 0 ; k <  stat ; k++ ) {
							if (curr_in->b_buf[k] == '\n') {
								curr_in->b_buf[k] = 0;
							} else if (curr_in->b_buf[k] == 0){
								//curr_in->b_buf[k] = '\n';
							}
						}
					} else if (term == 0) {
						for(k = 0 ; k <  stat ; k++ ) {
							if        (curr_in->b_buf[k] == 0) {
								curr_in->b_buf[k] = '\n';
							} else if (curr_in->b_buf[k] == '\n'){
								//curr_in->b_buf[k] = 0;
							}
						}
					}
					curr_in->b_len = stat;
					got_one = 0; // try to find a smaller buffer to use
					if (curr_in->b_len < recsize3) {int k;

						for(k = nsizes-1 ; k >=  0 ; k-- ) {
							if (curr_in->b_len < nsize3[k]) {
								q = list_remove_head(free_lh3[k]);
								if (q) {
									got_one = 1;
									q->b_len = curr_in->b_len;
									moveb(curr_in->b_buf,q->b_buf,q->b_len);
									list_clobber_check(curr_in);list_clobber_check(q);
									list_insert_tail(free_lh,curr_in);
									curr_in = q;
									break;
								}
							}
						}



					}
					if (got_one == 0 && curr_in->b_len < recsize2) {
						for(k = nsizes-1 ; k >=  0 ; k-- ) {
							if (curr_in->b_len < nsize2[k]) {
								q = list_remove_head(free_lh2[k]);
								if (q) {
									got_one = 1;
									q->b_len = curr_in->b_len;
									moveb(curr_in->b_buf,q->b_buf,q->b_len);
									list_clobber_check(curr_in);list_clobber_check(q);
									list_insert_tail(free_lh,curr_in);
									curr_in = q;
									break;
								}
							}
						}
					}
					list_insert_tail(work_lh,curr_in);
					curr_in = 0; // indicate no longer in use, need another one
				} else if (stat == -1) {
				} else if (stat ==  0) {
					con_close_data_connection(obj1,0);
					if(quiet<2)teprintf(logstr,"input connection lost/closed\n");
					if(retries==0)break;
					Sleep(1000 * delay);
					in = 1;retried1++;
					continue;
				}
			}
	//      --------------
			if (curr_out == 0) {
				curr_out = list_remove_head(work_lh);
			}
			if (curr_out) { // anything to do?
				n = con_write_record(obj2,curr_out->b_buf,curr_out->b_len,1);
				if (n == 0) { // connection closed
					if(quiet<2)teprintf(logstr,"output connection lost\n");
					con_close_data_connection(obj2,0);
					if(retries==0)break;
					Sleep(1000 * delay);
					out = 1;retried2++;
					continue;
				} else if (n == -1) { // no progress yet
				} else if (n >   0){  // finished
					progress = 1;
					rcopied++;
					if (curr_out->b_queue == 1) {
						list_insert_head(free_lh,curr_out); // recycle this
					} else if (curr_out->b_queue == 2) {
						list_insert_head(free_lh2[curr_out->b_group],curr_out); // recycle this
					} else if (curr_out->b_queue == 3) {
						list_insert_head(free_lh3[curr_out->b_group],curr_out); // recycle this
					} else {
						okk = 5;
					}
					curr_out->b_len = 0;       // clear this for debug use
					curr_out->b_buf[0] = '\0'; // this too
					curr_out = 0; // indicate no longer in use
				}
			} else { // nothing yet
			}
			if (logstr[0] != '\0' && debug) {int i,j,i2,j2,i3,j3,tb; PABUF p; time_t nowtime;
				i = 0; j=0;
				i2 = 0; j2=0;
				i3 = 0; j3=0;
				seq++;

				time(&nowtime);
				if (last_time+debug <= nowtime || last_time == 0) {int k;
					last_time = nowtime;
					for_list(ABUF,p,free_lh) {
						if (p->b_queue != 1) {
							okk = 1;
						}
						i++;
					}
					for(k = 0 ; k <  nsizes ; k++ ) {


						for_list(ABUF,p,free_lh2[k]) {
							if (p->b_queue != 2) {
								okk = 2;
							}
							i2++;
						}
						for_list(ABUF,p,free_lh3[k]) {
							if (p->b_queue != 3) {
								okk = 3;
							}
							i3++;
						}

					}

					tb = 0;
					for_list(ABUF,p,work_lh) {
						tb = tb + p->b_len;
						if (p->b_queue == 1) {
							j++;
						} else if (p->b_queue == 2) {
							j2++;
						} else if (p->b_queue == 3) {
							j3++;
						} else {
							okk = 4;
						}
					}
					teprintf(logstr,"stats %d in/out/o-i  %d %d %d  free  %d %d %d  work  %d %d %d  kb_in_q  %d\n"
						,seq

						,rread
						,rcopied
						,rread-rcopied 

						,i		// 3 sizes in the free queue
						,i2
						,i3

						,j
						,j2
						,j3

						//,okk
						,tb/1000
						);
				}
			} else if (debug) {int i,j,i2,j2,i3,j3,tb; PABUF p;
				i = 0; j=0;
				i2 = 0; j2=0;
				i3 = 0; j3=0;
				seq++;
				if ( (seq % 50)  == 0 ) {int ch;
					ch = tty_get_char();
					if (ch == '0' || ch == 'z') {
						seq = 0;
						rread = 0;
						rcopied = 0;
						okk=0;
					} else if (ch == '1') {
						debug = 1;
					} else if (ch == '2') {
						debug = 10;
					} else if (ch == '3') {
						debug = 25;
					} else if (ch == '4') {
						debug = 100;
					} else if (ch == '5') {
						debug = 200;
					} else if (ch == '6') {
						debug = 1000;
					} else if (ch == 'Q') {
						tty_reset_in();
						return 0;
					} else {
					}
				}
				if ( (debug == 1 ) ||  ( (seq % debug)  == 0)  ) {int k;
					for_list(ABUF,p,free_lh) {
						if (p->b_queue != 1) {
							okk = 1;
						}
						i++;
					}

					for(k = 0 ; k <  nsizes ; k++ ) {

					
						for_list(ABUF,p,free_lh2[k]) {
							if (p->b_queue != 2) {
								okk = 2;
							}
							i2++;
						}
						for_list(ABUF,p,free_lh3[k]) {
							if (p->b_queue != 3) {
								okk = 3;
							}
							i3++;
						}

					
					}


					tb = 0;
					for_list(ABUF,p,work_lh) {
						tb = tb + p->b_len;
						if (p->b_queue == 1) {
							j++;
						} else if (p->b_queue == 2) {
							j2++;
						} else if (p->b_queue == 3) {
							j3++;
						} else {
							okk = 4;
						}
					}
					fprintf(stderr,"%d [%d %d %d] %c f[%3d/%4d/%4d] w[%3d/%4d/%4d]%d %d --\r"
					,seq,rread,rcopied, rread-rcopied,progress ? 'P' : '-' ,i,i2,i3,j,j2,j3,okk,tb/1000)
					;fflush(stderr);
				}
				
			}

			if (progress == 0) {
				Sleep(150);
			}
		} else { //                                                ------------no buffering-----------------
			if (in) { //int n=0;
				for(;;) {
					stat = con_read_record(obj1,buffer,maxbuf,1);
					if (stat != -1) {
						//fprintf(stderr,"read %d\n",stat);
						break;
					}
					Sleep(50);
					//fprintf(stderr,"sleeping %d\n",++n);
				}
				//fprintf(stderr,"stat = %d\n",stat);
				if (stat == 0) {
					con_close_data_connection(obj1,0);
					if(quiet<2)teprintf(logstr,"Input connection lost/closed\n");
					if(retries==0)break;
					Sleep(1000 * delay);
					in = 1;retried1++;
					continue;
				}
			} else {
				if (nodata == 1) {
					stat = 0;
				} else {
					if (fill) {
						stat = xfread(buffer,1,size,io1); 
					} else {
						stat = fread(buffer,1,size,io1); 
					}
					k++;
				}
				//fprintf(stderr,"stat = %d %d\n",stat,++k);
				if (stat <= 0) {
					break;
				}
			}
	//      --------------
	//		if (in && out) {
				if (term == term2) {
				} else if (term == 10 ) {
					for(k = 0 ; k <  stat ; k++ ) {
						if (buffer[k] == '\n') {
							buffer[k] = 0;
						} else if (buffer[k] == 0){
							//buffer[k] = '\n';
						}
					}
				} else if (term == 0) {
					for(k = 0 ; k <  stat ; k++ ) {
						if        (buffer[k] == 0) {
							buffer[k] = '\n';
						} else if (buffer[k] == '\n'){
							//buffer[k] = 0;
						}
					}
				}
	//		}
	//      --------------
			if (hex) { int i,j; char xxx[10];
				moveb(buffer,buffer2,stat+1);
				j = 0;
				for(i = 0 ; i <  stat ; i++ ) {
					sprintf(xxx,"%02x",(int)buffer2[i] & 0xff);
					buffer[j++] = xxx[0];
					buffer[j++] = xxx[1];
				}
				buffer[j++] = '\n';
				stat = j;
				buffer[j++] = '\0';
				
			}
	//      --------------
			if (out) {
				n = con_write_record(obj2,buffer,stat,0);
				if (n == 0) {
					if(quiet<2)teprintf(logstr,"output connection lost\n");
					con_close_data_connection(obj2,0);
					if(retries==0)break;
					Sleep(1000 * delay);
					out = 1;retried2++;
					continue;
				}
			} else {
				n = fwrite(buffer,1,stat,io2);
				if (flsh) {
					fflush(io2);
				}
				if (n != stat) {
					teprintf(logstr,"error writing output file\n");
					break;
				}
			}

		}
		{//int nbytes=0,nalloc=0,nfree=0;
//			list_stats(&nbytes,&nalloc,&nfree);
			//fprintf(stderr,"nbytes = %d nalloc = %d  nfree = %d (%d)\n",nbytes,nalloc,nfree,nalloc-nfree);
		}
	}
	if(no_close) {
		Sleep(1000000);
	}
	if (in) {
		con_close_connection(obj1,1);
	}
	if (out) {
		con_close_connection(obj2,1);
	}
	//Sleep(2000);
	return 0;
}
