#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cbuf.h"
#include "utils.h"

static void bugcheck(char *mess);

#define FIRST(NNN,SSS)	(  (( (NNN)% base->c_size + (SSS) )  > base->c_size)  ? base->c_size -   ((NNN)% base->c_size)   :  (SSS)  ) 
#define SECOND(NNN,SSS) (  (( (NNN)% base->c_size + (SSS) )  > base->c_size)  ?   ( ((NNN)% base->c_size)   +  (SSS))   % base->c_size  :  0  ) 
#define MODULO (base->c_size)

#ifdef TESTMODE
#define TESTSIZE 10000000
static PCBUF buf;
static unsigned char eric[TESTSIZE];

// ---------------------------------------- verify code
static int	cb0(int nnn) {

	PCBLOCK p;  int i; int n,s,sum;
	i = 0;
	sum = 0;
	if(nnn)printf("-------- flag = %d\n",buf->c_flag);
	for_list(CBLOCK,p,buf->c_lh) {
		if (i==0) {
			n = p->b_start;
			s = p->b_size;
		} else {
			if (p->b_start !=  ( (n+s)%buf->c_size )  ) {
				printf("invalid - %d != %d (%d+%d)\n",p->b_start, (n+s) %buf->c_size,n,s);
				bugcheck("cb1");
			}
			n = p->b_start;
			s = p->b_size;
			if (s <= 0) {
				bugcheck("cb2");
			}
		}
		if(nnn)printf("%3d) [%4d %4d] %d\n",i,p->b_start, p->b_size, p->b_type);
		sum = sum+p->b_size;
		i++;
	}
	if (sum != buf->c_size ) {
		printf("invalid sum - %d != %d\n",sum,buf->c_size);
		bugcheck("cb3");
	}
	return 0;
}
static int cb() {
	cb0(0);
	return 0;
}
// ----------------------------------------
int main(int argc, char **argv) {
	PCBUF base; int size; int i; int n = 2; int k,flag;
	unsigned char *p;
	base = buf = Cbuf_init(TESTSIZE,eric,1);
	cb();
//	printf("first  0,100  = %d\n",FIRST(0,100));
//	printf("first  10,100 = %d\n",FIRST(10,100));
//	printf("first  50,60  = %d\n",FIRST(50,60));
//	printf("first  90,5   = %d\n",FIRST(90,5));
//	printf("first  100,100= %d\n\n",FIRST(100,100));

//	printf("SECOND 0,100  = %d\n",SECOND(0,100));
//	printf("SECOND 10,100 = %d\n",SECOND(10,100));
//	printf("SECOND 50,60  = %d\n",SECOND(50,60));
//	printf("SECOND 90,5   = %d\n",SECOND(90,5));
//	printf("SECOND 100,100= %d\n",SECOND(100,100));

	p = Cbuf_get_free_space(base,50);
	if (p) {
		printf("got free space = %d, split back to 20\n",p);
		Cbuf_split_free(base,20);
	}
	cb();
	p = Cbuf_get_data_block(base,&size);
	printf("got data block = %d size = %d\n",p,size);
	cb();
	Cbuf_release(base);
	printf("did a release\n",p,size);
	cb();
	p = Cbuf_get_data_block(base,&size);
	printf("got data block = %d size = %d\n",p,size);
	cb();


	p = Cbuf_get_free_space(base,50);
	if (p) {
		printf("got free space = %d, split back to 20\n",p);
		Cbuf_split_free(base,20);
	}
	cb();
	p = Cbuf_get_free_space(base,50);
	if (p) {
		printf("got free space = %d, split back to 30\n",p);
		Cbuf_split_free(base,30);
	}
	cb();
	p = Cbuf_get_free_space(base,50);
	if (p) {
		printf("got free space = %d, split back to 40\n",p);
		Cbuf_split_free(base,40);
	} else {
		printf("NO  free space = %d\n",p);
	}
	cb();
	p = Cbuf_get_free_space(base,10);
	if (p) {
		printf("got free space = %d, split back to 5\n",p);
		Cbuf_split_free(base,5);
	} else {
		printf("NO  free space = %d\n",p);
	}
	cb();


	p=Cbuf_get_data_block(base,&size);
	printf("got data block = %d size = %d\n",p,size);
	cb();

	Cbuf_release(base);
	printf("did a release\n",p,size);
	cb();

	p = Cbuf_get_free_space(base,15);
	if (p) {
		printf("got free space = %d, split back to 1\n",p);
		Cbuf_split_free(base,1);
	} else {
		printf("NO  free space = %d\n",p);
	}
	cb();

	for(i = 0 ; i <  4 ; i++ ) {
		p = Cbuf_get_data_block(base,&size);
		printf("got data block(%d) = %d size = %d\n",i,p,size);
		if (p) {
			Cbuf_release(base);
			printf("did a release\n",p,size);
		} else {
			printf("NO data block yet\n");
		}
		cb();
	}
// ----------------------------------------


		p = Cbuf_get_free_space(base,50);
		*p = 1; k = 1;
		Cbuf_split_free(base,10);
		cb();
		flag = 0;
{ int no1=0, no2=0;unsigned char *p1, *p2; int mod=100,prio=55; int tsize,niq,used,nav;
	p1 = NULL; p2 = NULL;
	for(i = 0 ; /*i <  150000000*/ ; i++ ) { int kkk; 
		kkk = rand();
		if ( (kkk %mod)  > prio) {
			flag = 0;
		} else {
			flag = 1;
		}
		if (i%100 == 0) {
			cb();
		}
		if (i%1000000 == 0 ) {
			//cb0(1);
			nav = Cbuf_avail(base,&niq,&tsize,&used);
			printf("%3d) nf=%8d ndat=%8d p=%d avl=%7d us=%7d nq=%5d [%d]\n"
				,i/1000000,no1,no2,prio,nav,used,niq,tsize);
			//if(no2>5 && no1 > 1)break;
			if (no1 > no2) {
				prio = 45;
			} else {
				prio = 55;
			}
		}
		
		if (flag != 0) {
			if (p1 == NULL) {
				p1 = Cbuf_get_free_space(base,1000);
				if (p1 == NULL) {
					no1++;
					//printf("NO  free space = %d %d\n",p1,i);
				//	if (mod==3) {
				//		mod=2;
				//	} else {
				//		mod=3;
				//	}
				} else {
					//printf("got free space = %d (%d)\n",p1,p1-eric);
				}
			} else { int i;
				for(i = 0 ; i <  1000 ; i++ ) { // overwrite entire free area requested
					p1[i] = n;
				}
				*p1 = n++;
				kkk = rand();
				Cbuf_split_free(base,kkk%1000 ? kkk%1000 : 1);
				//printf("split back to %d set to value %d\n",kkk%100 ? kkk%100 : 1,n-1);
				p1 = NULL;
			}
			
		} else {
			if (p2 == NULL) {
				p2=Cbuf_get_data_block(base,&size);
				if (p2 == NULL) {
					no2++;
				//	if (mod==3) {
				//		mod=2;
				//	} else {
				//		mod=3;
				//	}
					//printf("NO data block yet %d\n",i);
				} else {
					//printf("got data block = %d (%d) size = %d value=%d should be %d\n"
					//	,p2,p2-eric,size,(unsigned int)*p2,k);
				}

			} else {int i,j;
				j = 0;
				for(i = 0 ; i <  size ; i++ ) {
					if (p2[i]%256 != k%256) {
						j=1;
						break;
					}
				}
				if (*p2%256 != k%256 || j == 1) {
					if (k != 1) {
						printf("got wrong value %d vs %d (at %d %d)\n",*p2,k, i,&p2[i]-eric);
					}
				}
				k++;
				Cbuf_release(base);
				//printf("did a release\n",p,size);
				p2 = NULL;
			}
		}
	}
}
cb();
	return 0;
}

#endif


// ----------------------------------------
// ------------- free space manager -------
// ----------------------------------------



// ----------------------------------------
static PCBLOCK		Cbuf_alloc(PCBUF base){
// ----------------------------------------
	PCBLOCK p;
	p = list_remove_tail(base->c_freelh);
	if (p) {
		return p;
	}
	p = list_new(sizeof(CBLOCK),"cblock");
	return p;
}



// ----------------------------------------
PCBUF		Cbuf_init(int psize,POINTER data, int stain){
// ----------------------------------------
	PCBUF base; 
	
	base = list_new(sizeof(CBUF),"cbuf");

	base->c_freelh	= list_newlh();
	base->c_lh		= list_newlh();
	base->c_flag	= 0;
	base->c_size	= psize;
	base->c_data	= data;
	base->c_f1		= Cbuf_alloc(base);
	list_insert_tail(base->c_lh,base->c_f1);

	base->c_f1->b_start = 0;
	base->c_f1->b_size = psize;
	base->c_f1->b_type = TF1;
	base->c_bytes		= 0;
	base->c_num			= 0;
	return base;
}






// ----------------------------------------
void*		Cbuf_get_free_space(	PCBUF base, int minsize){
// ----------------------------------------
	PCBLOCK f1; int s,n,fir,sec; char *p;

	f1 = base->c_f1;
	n = f1->b_start;
	s = f1->b_size;

	fir = FIRST(n,s);
	sec = SECOND(n,s);

	p = base->c_data;
	base->c_request = minsize;
	if (fir >= minsize) {
		return p + n;

	} else if (sec >= minsize) {
		Cbuf_split_f1(base,fir,TF2);
		return p;
	}
	base->c_request = 0;
	return NULL;
}
// ----------------------------------------
void		Cbuf_split_free(		PCBUF base, int truesize) {
// ----------------------------------------
		if (truesize > base->c_request) { // did user lie about this?
			bugcheck("split1");
		}
		Cbuf_split_f1(base,truesize,TUSER);
		base->c_request = 0;
}





// ----------------------------------------
void		Cbuf_split_f1(			PCBUF base, int truesize, int type) {
// ----------------------------------------
	PCBLOCK p,f1;
	if (truesize <= 0) {
		bugcheck("split1");
	}
	p = Cbuf_alloc(base);
	f1=base->c_f1;
	p->b_start = f1->b_start % MODULO;
	p->b_size  = truesize;
	p->b_type  = type;
	list_insert(list_prev(f1),p);

	f1->b_start = (f1->b_start + truesize)  % MODULO ;
	f1->b_size  = f1->b_size - truesize;

	base->c_bytes =base->c_bytes  + truesize;
	base->c_num++;
}




// ----------------------------------------
void*		Cbuf_get_data_block(	PCBUF base, int *size){
// ----------------------------------------
	PCBLOCK p,f1; int start;
	f1=base->c_f1;

	if (base->c_flag) {
		bugcheck("get_data_block flag already set");
	}
	*size = 0;
	p = list_next(base->c_lh);
	if (p->b_type == TUSER) {
		*size = p->b_size;
		start = p->b_start;
		base->c_flag = 1; // indicate we returned a data block, waiting for a release of it
		return start + base->c_data;
	} else if (p->b_type == TF2) { // if f2, then recombine with f1
		f1->b_size = f1->b_size + p->b_size; // add in to size of f1
		list_remove(p);						 // remove from queue
		list_insert_tail(base->c_freelh,p);	 // back on free list
		base->c_bytes = base->c_bytes - p->b_size; // keep track of storage
		base->c_num--;
		return Cbuf_get_data_block(base,size);
	} else {
		return NULL; // if f1, then there is nothing on queue
	}
}





// ----------------------------------------
void		Cbuf_release(			PCBUF base) {
// ----------------------------------------
	PCBLOCK p,f1;
	if (base->c_flag == 0) {
		bugcheck("release flag not set");
	}
	f1=base->c_f1;
	base->c_flag = 0; // clear
	p = list_next(base->c_lh);
	if (p->b_type != TUSER) {
		bugcheck("release not user block first on queue");
	}
	f1->b_size = f1->b_size + p->b_size; // add in to size of f1
	list_remove(p);						 // remove from queue
	list_insert_tail(base->c_freelh,p);	 // back on free list

// can't adjust start of f1 w/o locking - so don't enable unless locking installed
//	if (list_next(base->c_lh) == list_prev(base->c_lh) && list_next(base->c_lh) == f1) { // if only f1 on list
//		f1->b_start = 0;	// reset to beginning of memory, so we tend to reuse same memory when queue is empty
//	}
	base->c_bytes = base->c_bytes - p->b_size; // keep track of storage
	base->c_num--;
}







// ----------------------------------------
int			Cbuf_avail	(PCBUF base, int *numinqueue, int *totalsize, int *usedbytes) {// returns number in queue
// ----------------------------------------
	if(numinqueue)*numinqueue = base->c_num;
	if(usedbytes) *usedbytes  = base->c_bytes;
	if(totalsize) *totalsize  = base->c_size ;
	return base->c_size - base->c_bytes;
}





// ----------------------------------------
static void bugcheck(char *mess) {
// ----------------------------------------
	char *s = 0;
	fprintf(stderr,"error = %s\n",mess);
	*s = 0;
}
