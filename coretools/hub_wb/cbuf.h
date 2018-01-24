#ifndef ET_CBUF_H
#define ET_CBUF_H
#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

typedef void *POINTER;
//typedef char *PCHAR;    /* pointer to char */
//typedef PCHAR (*FPC)(); /* FPC is a pointer to a func that returns char ptrs */

//typedef int (*PFI) ();               /* pointer to func returning int */

#define TUSER 0
#define TF1   1
#define TF2   2

typedef struct cblock {

	int		b_start;	// start index of this block
	int		b_size;		// size of this block
	int		b_type;		// 0..2

    } CBLOCK, *PCBLOCK;

typedef struct cbuf {

	POINTER	c_lh;			// main list 
	POINTER	c_freelh;		// free node listhead 
	PCBLOCK	c_f1;			// pointer
	
	int		c_size;			// size of data area 
	int		c_flag;			// 0/1 0= no get data block called, 1 = called
	
	char *	c_data;			// pointer to data area, we don't touch this area
	int		c_num;			// number of entries in queue
	int		c_bytes; 		// number of bytes in queue
	int		c_request;		// size asked for during the last get_free_space

    } CBUF, *PCBUF;






struct cbuf *Cbuf_init(int size,POINTER data, int stain);	// create circular buffer object

void*		Cbuf_get_free_space(	PCBUF base, int minsize);
void		Cbuf_split_free(		PCBUF base, int truesize);
void		Cbuf_split_f1(			PCBUF base, int truesize, int type);
void*		Cbuf_get_data_block(	PCBUF base, int *size);
void		Cbuf_release(			PCBUF base);
int			Cbuf_avail(				PCBUF base, int *numinqueue, int *totalsize, int *usedbytes); // returns number in queue

#endif
