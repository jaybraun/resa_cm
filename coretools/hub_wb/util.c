#ifdef WIN32
#pragma warning( disable : 4100 )   /*  un refed parameter  */
#pragma warning( disable : 4001 )   /*  single line comment */
#pragma warning( disable : 4505 )   /*  unrefed function    */
#endif


#ifdef I64
#define IN_T long long
#else
#define IN_T int
#endif

#ifdef I64
#else
#endif
 
/* HEADER **********************************************************************
--------------------------------------------------------------------------------
MODULE    UTIL.C

PURPOSE   Utilities for managing hash tables and linked lists.


OPTIONS
          TEST_MAIN          set if doing unit test 
          LL_CLOBBER             Implement clobber checking (off top end)
          LL_VERBOSE         use sprintf to format some errors 
          LL_INSERT          check calls to insert             
          LL_REMOVE          check calls to removee            
          LL_OLDLH           check calls to oldlh              
          LL_OLD             check calls to old                
          LL_INSERT_TAIL     check calls to insert_tail
          LL_INSERT_HEAD     check calls to insert_head
          LL_REMOVE_TAIL     check calls to remove_tail
          LL_REMOVE_HEAD     check calls to remover_head
		  LL_STATS			 compute stats
		  LL_TRACE			 trace allocs and deallocs - requires LL_ID to be defined also
		  LL_ID				 store id of memory block

#define TEST_MAIN   
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
             
/*#define LL_DEBUG */
/**/
//#define LL_TRACE
#ifdef LL_DEBUGMODE  
#define LL_TRACKALL
#define LL_ID
#define LL_VERBOSE   
#define LL_CLOBBER
#define LL_INSERT    
#define LL_REMOVE    
#define LL_OLDLH     
#define LL_OLD       
#define LL_INSERT_TAIL  
#define LL_INSERT_HEAD  
#define LL_REMOVE_TAIL  
#define LL_REMOVE_HEAD  
#define LL_STATS
#endif
#ifdef LL_TRACKALL // if tracking, must have id set too
	#ifndef LL_ID
		#define LL_ID
	#endif
#endif

#define LL_MAX_NEW 0x40000000L  /* arbitrary limit to size of memory allocation */

#define REG register
#define z if(1==0)

#include <stdio.h>
#include <string.h>
/*#include <malloc.h>*/
#include <stdlib.h>
#define UTIL_OPAQUE 1
#include "utils.h"

#ifdef LL_TRACKALL
static struct atom list_track_lh = {0}; 
#endif

static FPC error_routine = NULL;    /* user error routine if not zero */
                                    /* what this returns is passed on */
//static char ebuf[80];               /* general purpose error buffer */
//static char insert_mess[] = "Bad insert: ";
//static char remove_mess[] = "Bad remove: ";

                                    /* which kind of allocation, only 1 time to be set */
//static IN_T allocator = 0;       
static IN_T once      = 1;
static  int string_hash(IN_T size,char *s,IN_T alg);

#ifdef DEBUG_CODE
static IN_T is_valid();
#endif

static void errwindow(char *str, char *parm);


static void sdelsym(char *a, Ptr b, IN_T i, Ptr bl, Ptr stab){
       hash_sdelete(stab,a);
       return;
}
#define staticx
staticx IN_T list_nalloc = 0;
staticx IN_T list_nfree  = 0;
staticx IN_T list_ninuse = 0;
#ifdef LL_CLOBBER
static void clobber_init(void *p, IN_T size) {
	unsigned char *i = (unsigned char *)((unsigned char *)p + size );
	*i++     = 0xfe;
	*i++     = 0xca;
	*i++     = 0xef;
	*i       = 0xbe;
}
/* HEADER **********************************************************************
--------------------------------------------------------------------------------

PURPOSE   checking utilities

RETURNS   None
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
static clobber_add_size(){
	return sizeof(double)*2;
}


// ******************************************************

static void clobber_check(void *p, IN_T size,struct atom *a){
	unsigned char *i = (unsigned char *)((unsigned char *)p + size );
	if (i[0] != 0xfe || i[1] != 0xca || i[2] != 0xef || i[3] != 0xbe) {
#ifdef LL_VERBOSE
#ifdef LL_ID
		 a->name[sizeof(a->name)-1] = '\0';
#ifdef I64
         sprintf(ebuf,"loc %16lx size: %04lx(hex) %ld(dec) beefcafe != %08lx (%s)",p,size,size,* (( IN_T *)i) ,a->name);
#else
         sprintf(ebuf,"loc %08x size: %04x(hex) %d(dec) beefcafe != %08x (%s)",p,size,size,* (( IN_T *)i) ,a->name);
#endif
#else
         sprintf(ebuf,"loc %08x size: %04x(hex) %d(dec) %08x",p,size,size,*(( IN_T *)i));
#endif
             errwindow("clobber.",ebuf);

#else
             errwindow("clobber.",NULL);
#endif
	}
}
#endif
// ******************************************************
void list_clobber_check(struct atom *obj) {
#ifdef LL_CLOBBER
	register  struct atom     *a;
	a = obj - 1;
	if (!is_valid(a)) {
		errwindow("notvalid.",NULL);
	}
	clobber_check(obj,a->size,a);
#endif
}
// ******************************************************

void list_clobber_check_all(IN_T flag, PFI routine, void * parm) {
#ifdef LL_TRACKALL
	struct atom *p,*q;
	p = q = &list_track_lh;
	for (;;) {
		p = p->all_right;
		if (p == q) {
			break;
		}
		if (flag == 2) {
			if (routine) {
				(*routine)(p,parm);
			}
			
		} else if (flag == 1) {
			list_clobber_check(p+1);
#ifdef I64
			fprintf(stderr,"%16lx) [%16lx %16lx]%5ld-b %-16s(%s %s)\n"
				,p,p->left , p->right , p->size , p->name, p->left->name ,p->right->name );
#else
			fprintf(stderr,"%08x) [%08x %08x]%5d-b %-16s(%s %s)\n"
				,p,p->left , p->right , p->size , p->name, p->left->name ,p->right->name );
#endif
		} else {
			list_clobber_check(p+1);
		}
	}
#endif
}

/* HEADER **********************************************************************
--------------------------------------------------------------------------------

PURPOSE   Error utilities

RETURNS   None
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

static void errtraceback(char *s)  {
        char c;
        fprintf(stderr,"errtraceback\n");
        c = *s;  /* force an access violation */

}
static void ewindow(char *a,char *b){
	fprintf(stderr,"%s: %s\n",a,b==NULL? "<null>":b);
  errtraceback(NULL);
}

static void errwindow(char *str, char *parm){
/*  char *xx = NULL; */
	FILE *io;
	io = fopen("error999.txt","w");
	if (io) {
		  fprintf(io,"%s\n",str);
		  if(parm) {
			fprintf(io,"%s\n",parm);
		  }
		  fclose(io);
	}
  fprintf(stderr,"%s\n",str);
  if(parm) {
	fprintf(stderr,"%s\n",parm);
  }
  errtraceback(NULL);
}

#ifdef DEBUG_CODE
static void erdwindow(char *str, IN_T parm){
  fprintf(stderr,"%s\n",str);
#ifdef I64
  fprintf(stderr,"%ld\n",parm);
#else
  fprintf(stderr,"%d\n",parm);
#endif
  errtraceback(NULL);
}
#endif

/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  char *xstrcpy(s1, s2)

PURPOSE   Copy string s2 to s1.  s1 must be large enough.

RETURNS   s1
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

static char *xstrcpy(char *s1, char *s2){
        register char *os1;

        os1 = s1;
        while( (*s1++ = *s2++) != 0)
                ;
        return(os1);
}



/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  sdelete(stable,symbol)

PURPOSE   This routine deletes the symbol from the  table. If symbol is 0/1
          then all entries in the stable are removed and the stable block is
          effectively deleted (for symbol=0). no symbol found (only entries
          deleted if symbol=1)

RETURNS   1 = OK
          0 = no symbol found
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

IN_T hash_sdelete(REG PSYMTABLE table, REG char *sym)  {
   REG Ptr kill, p;
   REG PSYMBLOCK *array,list_head;
   if (table->s_entries <= 0) {
      ewindow("Bad call to sdelete, table block corrupted",NULL);
      return 0;
   }

   if (sym == 0 || sym == (char *) 1L) {
       hash_sdump(table,sdelsym);
     if (sym == 0)  {           /* delete this too */
       table->s_entries = 0;    /* in case user tries this on us after */
       list_old(table);              /* we deleted it */
     }
   } else {

     p = hash_slookup(table,sym);    /* try to find it and set pointers to it */



     if (p==(char *)0 ) {
     
     
     
       return 0;        /* nada to delete here */
     } else {

        kill =(Ptr) table->s_block;    /* the one to remove - set by lookup */
        list_old(list_remove(kill));            /* get rid of it                     */
        array = table->s_hash;         /* addr of the array                 */
        list_head = array[table->s_hval];  /* see if last node to delete    */
        if ((Ptr)list_prev(list_head) == (Ptr)list_head ) {
           list_oldlh(list_head);           /* delete the listhead too       */              
           array[table->s_hval] = NULL;    /* indicate no nodes hash here   */
        }
     }
   }
     return 1;
}




/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  PSYMTABLE stabinit(entries)     

PURPOSE   initializes a symbol table and returns a pointer to an internal
          table to be used for all subsequent calls. The value returned will be
          denoted here as the stable.  Entries is the  number of cells in the
          direct table. Colisions are handled by a linked list. 

RETURNS   returns pointer to sym table 

--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

PSYMTABLE hash_stabinit(IN_T entries){

    PSYMTABLE t;
    IN_T i;

    t = (PSYMTABLE) list_new(((IN_T) (sizeof(Ptr)*entries)+sizeof(SYMTABLE)),"hash_init" );
    t->s_nins = 0;
    t->s_colls= 0;
    t->s_ncmps= 0;
    t->s_look= 0;
    t->s_nfnd= 0;
	t->s_hash_alg = 0;

    t->s_entries=entries;
    for (i=0 ; i<entries ; i++) {
            t->s_hash[i] = NULL;
        }
    return t;
        }


static IN_T int_hash();

/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  PSYMBLOCK sinstall(stable,sym,p)

PURPOSE   This routine takes the stable pointer to a symbol into the symbol
          table.  The symbol is assumed to not be there already.  It hashes the
          symbol, sees if there is a colision, and if so links this symbol in
          with any preceding one.  The hash location is a longword pointer to a
          list of doubly linked symbol blocks.  

RETURNS   P is a non-zero datum associated with sym; it is returned via a
          lookup

--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

PSYMBLOCK hash_sinstall(PSYMTABLE table,char *sym,void *p){
    REG IN_T table_size,hval;
    REG PSYMBLOCK pt,*array;

    if(p == NULL) {
         ewindow("Bad call to sinstall, value was NULL for",sym);
         return NULL;
    }
    table_size = table->s_entries;
    if(table_size <= 0) {
         ewindow("Bad call to sinstall, table block corrupted",NULL);
         return NULL;
    }
    array = table->s_hash;  /* addr of the array */
    hval = string_hash(table_size,sym,table->s_hash_alg);
    pt = (PSYMBLOCK) list_new((IN_T) (sizeof(SYMBLOCK)+strlen(sym)+2),"hash_install"); /* build sym block */
    pt->b_value = (char *) p;                /* install the value */
    pt->b_refs  = 0;                /* clear   the number of refs  */
    xstrcpy(&(pt->sym_type.b_symbol[0]),sym);   /* install the symbol */
    (table->s_nins)++;              /* statistics */



    if (array[hval] == NULL ) {
        array[hval] = (PSYMBLOCK) list_newlh();/* no colision get list head */
		list_insert_tail(array[hval],pt);     /* add to list at end        */
    } else {
        (table->s_colls)++;               /* stats # of collisions     */
		list_insert_tail(array[hval],pt);     /* add to list at end        */
/*
		{PSYMBLOCK p;
			printf("\n");
			for_list(SYMBLOCK,p,array[hval]) {
					printf("%d = %s\n",hval,&(p->sym_type.b_symbol[0]));
			}
		}
		
*/
    }



    return pt;
}

/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  char *slookup(stable,symbol)    

PURPOSE   This function returns the data value contained in the data portion of
          the symbol block.  If the symbol is not in the table, then NULL is
          returned.  Note that install will not permit a value of NULL to be
          stored since that value could then corrupt the symbol  table 
          mechanism.  It is assumed that a valid address (pointer) will always
          be there. 

RETURNS   Install will error exit if a NULL value is to be inserted.

--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

Ptr hash_slookup(PSYMTABLE table,char *sym){
    REG IN_T table_size,hval;
    REG PSYMBLOCK lht,t,*array;

    table_size = table->s_entries;
    if (table_size <= 0) {
         ewindow("Bad call to slookup, table block corrupted",NULL);
         return NULL;
    }
    array = (PSYMBLOCK *) table->s_hash;  /* addr of the array */
    table->s_hval = hval = string_hash(table_size,sym,table->s_hash_alg);
    table->s_block = NULL;
    (table->s_look)++;

        if( (t = array[hval]) != NULL) {
            lht = t;          /* save pointer to lh */
        t = (PSYMBLOCK) list_next(t);  /* start at first node */
        for (;;) {
            (table->s_ncmps)++;
              /*                    fprintf(stderr,"lookup compares with t=%x\n",t); */
            if (strcmp(t->sym_type.b_symbol,sym) == 0) {
				table->s_block = t;
				(t->b_refs)++;
				return (t->b_value);    /* found it */
            }
            t = (PSYMBLOCK) list_next(t);
            if ( t == lht) {
				break;                  /* no more here */
            }
        }

        }
       (table->s_nfnd)++;
            return NULL;
}


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  sdump(stable,routine)           

PURPOSE   This calls routine for each symbol in the table.  The call to routine
          is with the address of the symbol string and the data value cell.

RETURNS   None
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
static Ptr slisting();
void hash_sdump(PSYMTABLE stable,PFI routine) {
     IN_T i;  PSYMBLOCK t,tsav,lht;
    if (routine == NULL) {
        routine = (PFI) slisting;
    }
    for (i=0 ; i < stable->s_entries ; i++) {
        if( (t = stable->s_hash[i]) != NULL) {
        lht = t;
        t = (PSYMBLOCK) list_next(t); /* start at first node */
        for (;;) {
            tsav = (PSYMBLOCK) list_next(t);
                                           /* this routine could delete ``t'' */
            (*routine)( (&t->sym_type.b_symbol), t->b_value,i,t,stable);
                                /*  fprintf(stderr,"in sdump t=%x tsav=%x\n",t,tsav); */

            if (tsav == t || stable->s_hash[i] == NULL) {
            break;
            }
            t = tsav;
            if ( t == lht) {
            break;
            }
        }
        }
        }
        return;
}

static Ptr slisting(char *a,Ptr b,IN_T i,PSYMBLOCK bl,Ptr stab){
       fprintf(stderr,"%-20s ptr= %08lx hash=%5ld refs=%ld\n",a,(unsigned long)b,(long)i,bl->b_refs);
       return NULL;
}


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  string_hash routines   

PURPOSE   Hash into the list head

RETURNS   index to the list head array
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
static int sed_hash1a(char *v, IN_T M) {
	IN_T h=0, a=127;
	for (; *v != '\0'; v++) {
		h = (a*h + *v);
	}
	h = h %M;
    if ( h < 0 ) {
      h = -h;
    }
	return h;
}
static int sed_hash1(char *v, IN_T M) {
	IN_T h=0, a=127;
	for (; *v != '\0'; v++) {
		h = (a*h + *v) % M;
	}
	return h;
}
static int sed_hash2(char *v, IN_T M) {
	IN_T h=0, a=31415, b=27183;
	for (; *v != '\0'; v++,a=a*b%(M-1)) {
		h = (a*h + *v) % M;
	}
	return h;
}

static  int string_hash(IN_T size,char *s,IN_T alg){
        IN_T hashval=0;
		static unsigned char t[]={7,11,13,17,23,31,37};

		if (alg == 0) {
			return sed_hash1(s,size); // sedgwick's alg 1 = default
		} else if (alg == 1) {
			return sed_hash1a(s,size);// sedgwick's alg 1 modified for speed
		} else if (alg == 2) {
			return sed_hash2(s,size);// sedgwick's alg 2
		} else if (alg == 3) {		// hack 1
			for (hashval = 0; *s != '\0'; ) {
                                 int i = (int)(*s++);
				 hashval += t[i%7] * i;
			}
			return(int_hash(size,hashval));
		} else if (alg == 4) {		// hack 2
			hashval = 0;		
			for ( ; *s != '\0'; ) {
				hashval =   ((hashval*7)  + *s++)  % size;
			}
			if (hashval < 0) {
				hashval = -hashval;
			}
			hashval= hashval % size;
			return hashval;
		} else {					
			return sed_hash1(s,size); // sedgwick's alg 1 = default if bad alg value
		}
}

static IN_T int_hash(IN_T size,IN_T n){
        n  *= 47;
        if ( n < 0 ) {
          n = -n;
        }
        return n % size;
}




/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  struct atom *next (obj) 

PURPOSE   Go to next in chain

RETURNS   Address of data part

--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
#ifndef LL_INLINE

void *list_next (void    *obj) {
#ifdef LL_CLOBBER
		list_clobber_check(obj);
#endif
        return (void *) (((struct atom    *)obj-1)->right+ 1);
}
#endif

/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  struct atom *prev (obj) 

PURPOSE   Go to previous in chain

RETURNS   Address of data

--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
#ifndef LL_INLINE

void *list_prev (void    *obj) {
        return (void *) (((struct atom    *)obj-1)->left+1);
}
#endif
void list_stats(IN_T *bytes, IN_T *alloc, IN_T *fre) {
	*bytes = list_ninuse;
	*alloc = list_nalloc;
	*fre   = list_nfree;
}
/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  struct atom *new (item_size) 

PURPOSE   Allocate storage for node plus hidden linkage

RETURNS   Pointer to node structure
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

void *list_new (IN_T item_size,char *item_name) {
        struct atom     *p; char *etemp;
        IN_T extra = 0;

        once = 0;
#ifdef LL_CLOBBER
        extra       = clobber_add_size();
#endif
        if (item_size > LL_MAX_NEW || item_size < 0) {
           goto bad_allocate;
        }
        p = (struct atom *) calloc ((IN_T)(item_size + extra + sizeof (struct atom)),sizeof(char));
        if (p == NULL ) {
           if ( error_routine != NULL) {
              etemp = (*error_routine)(1L,item_size);
              p = (struct atom *) etemp;
              return p;
           } else {
bad_allocate:
#ifdef LL_VERBOSE
#ifdef I64
         sprintf(ebuf,"Request size: %04lx(hex) %ld(dec)",item_size,item_size);
#else
         sprintf(ebuf,"Request size: %04x(hex) %d(dec)",item_size,item_size);
#endif
             errwindow("No memory.",ebuf);

#else
             errwindow("No memory.",NULL);
#endif
         return NULL;
       }
        }
#ifdef LL_CLOBBER
        clobber_init(p+1,item_size);
#endif
#ifdef LL_ID
		{ IN_T i;  char c;
			for(i = 0 ; i <  16 ; i++ ) {
				c = item_name[i];
				p->name[i] = c;
				if (c == '\0') {
					break;
				}
				
			}
			p->name[15] = '\0';
#ifdef LL_TRACE
#ifdef I64
			fprintf(stderr,"new %16s %6ld bytes at %16lx\n",p->name,item_size,p+1);
#else
			fprintf(stderr,"new %16s %6d bytes at %08x\n",p->name,item_size,p+1);
#endif
#endif
		}
#endif

        p->size = item_size;
        p->left = p; /* init to singleton status */
        p->right = p;
#ifdef LL_STATS
		list_nalloc++;
		list_ninuse = list_ninuse + p->size;
#endif

		
#ifdef LL_TRACKALL
	if (!list_track_lh.all_left ) {
		list_track_lh.all_left = list_track_lh.all_right = &list_track_lh;
	}
	{ struct atom *entr, *pred;
		entr = p; 
		pred = &list_track_lh;
		p->all_left = p;
		p->all_right= p;


	    entr ->all_right			= pred->all_right; /* forw of entry */
        entr ->all_left				= pred;        /* back of entry */
        (pred->all_right)->all_left = entr ;       /* back of succ  */
        pred->all_right				= entr ;       /* for of pred */
	}

#endif

        return (void *)(p+1);
}


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  VOID old (obj) 

PURPOSE   Deallocate storage for node and extra linkage

RETURNS   Pointer to previous node
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

void list_old (void    *obj) {
       register  struct atom     *a;
        
        a = (struct atom    *)obj - 1;
#ifdef LL_OLD
        if (a->left != a->right || a->left != a) { /* verify not still  */
           errwindow("bad old: not singleton ",NULL);    /* connected to list */
           return;                                    /* or already free   */
        }
#endif
        a->right = NULL;         /* clobber pointer to detect twice old */
                   /* not 100% able to detect this        */
#ifdef LL_CLOBBER
        clobber_check(obj,a->size,a);
#endif
#ifdef LL_STATS
		list_nfree++;
		list_ninuse = list_ninuse - a->size;
#endif
#ifdef LL_ID
#ifdef LL_TRACE
#ifdef I64
		fprintf(stderr,"old %16s %6ld bytes at %16lx\n",a->name,a->size,obj);
#else
		fprintf(stderr,"old %16s %6d bytes at %08x\n",a->name,a->size,obj);
#endif
#endif
#endif
#ifdef LL_TRACKALL
	{ struct atom *entr;
		entr = a; 

		(entr->all_left) ->all_right = entr->all_right; /* forw link of pred */
        (entr->all_right)->all_left  = entr->all_left;  /* back link of succ */
       // entr->all_left = entr;                  /* return to singleton status */
       // entr->all_right= entr;
	}
#endif

        free(a);
}


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  oldlh(obj)

PURPOSE   Delete a list head

RETURNS   If o.k. return the object
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

void list_oldlh(struct atom *obj){ 
#ifdef LL_OLDLH
      register struct atom *a;
      a = obj -1;
          if (! is_valid(a) || a->size != 0L ) {
              errwindow("bad oldlh: not listhead",NULL);  
              return;
          }
#endif
         list_old(obj);             /* just do simple for now */
} 


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  struct atom *newlh () 

PURPOSE   allocate storage for listhead size=0

RETURNS   Pointer to the storage location
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

void *list_newlh () {
        return (list_new(0,"listhead"));      /* just use this for now  */
}


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  VOID insert_tail (lh, obj) 

PURPOSE   Insert an object into a list tail

RETURNS   Null
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
#ifndef LL_INLINE

void list_insert_tail (void   *lh,void   * obj) {
#ifdef LL_INSERT_TAIL
      register struct atom *a;
      a =  ((struct atom *)lh)  -1;
          if (! is_valid(a) || a->size != 0L ) {
              errwindow("bad ins_tail: not listhead",NULL);  
              return;
          }
#endif
        list_insert(list_prev(lh),obj);
}
#endif


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  VOID insert_head (lh, obj)

PURPOSE   Insert an object into a list head

RETURNS   None
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
#ifndef LL_INLINE

void list_insert_head (struct atom   *lh,struct atom   * obj){
#ifdef LL_INSERT_HEAD
		register struct atom *a;
		a = lh -1;
		if (! is_valid(a) || a->size != 0L ) {
		  errwindow("bad ins_head: not listhead",NULL);  
		  return;
		}
#endif
		list_insert(lh,obj);
}
#endif


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  struct atom *remove_tail (lh) 

PURPOSE   Remove a list tail

RETURNS   if node not empty return node
          if node is empty  return NULL
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

void *list_remove_tail (struct atom *lh) {
#ifdef LL_REMOVE_TAIL
		register struct atom *a;
		a = lh -1;
		if (! is_valid(a) || a->size != 0L ) {
		  errwindow("bad rem_tail: not listhead",NULL);  
		  return NULL;
		}
#endif
		if (list_next(lh) != lh) {       /* spec says this may be interlocked */
		  return list_remove(list_prev(lh));    /* at some time in future            */
		} else {   
		  return NULL;
		}
}


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  struct atom *remove_head (lh) 

PURPOSE   Remove a list head

RETURNS   if node not empty return node
          if node is empty  return NULL
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

void  *list_remove_head (struct atom *lh) {
#ifdef LL_REMOVE_TAIL
	register struct atom *a;
	a = lh -1;
	if (! is_valid(a) || a->size != 0L ) {
	  errwindow("bad rem_head: not listhead",NULL);  
	  return NULL;
	}
#endif
	if (list_next(lh) != lh) {       /* spec says this may be interlocked */
	  return list_remove(list_next(lh));    /* at some time in future            */
	} else {   
	  return NULL;
	}
}


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  VOID insert (list, obj) 

PURPOSE   Insert an object in the list

RETURNS   None
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
void list_insert (struct atom *list,struct atom * obj) {
        register struct atom     *pred, *entr ;

        entr                = obj  - 1;
        pred                = list - 1;    /* alg stolen from vax arch book */
#ifdef LL_INSERT
    if (list == NULL || obj == NULL) {
#ifdef LL_VERBOSE
         sprintf(ebuf,"NULL arg(s) (%08lx,%08lx)",list,obj);
             errwindow(insert_mess,ebuf);
#else
             errwindow(insert_mess,"NULL arg(s)");
#endif             
             return;
    }

    if ( ! is_valid(pred) ) {
#ifdef LL_VERBOSE
         sprintf(ebuf,"Invalid Node arg1 %08lx",list);
             errwindow(insert_mess,ebuf);
#else
             errwindow(insert_mess,"Invalid Node arg1");
#endif           
             return;
    }
    
    if ( ! is_valid(entr) ) {
#ifdef LL_VERBOSE
         sprintf(ebuf,"Invalid Node arg2 %08lx",obj);
             errwindow(insert_mess,ebuf);
#else
             errwindow(insert_mess,"Invalid Node arg2");
#endif           
             return;
    }
#endif
#ifdef LL_DEBUG
    fprintf(stderr,"in %08lx<%08lx %08lx %04lx> %08lx<%08lx %08lx %04lx>\n",
        pred,pred->left,pred->right,pred->size,
        entr,entr->left,entr->right,entr->size);
#endif
#ifdef LL_INSERT

    if ( entr->size == 0L) {    
       errwindow(insert_mess,"arg2 may not be listhead");
       return;
    }
    if (entr->right != entr) {
       errwindow(insert_mess,"arg2 must be singleton");
           return;
    }
    if ((pred->right == pred) && (pred->size != 0L) ) {
       errwindow(insert_mess,"singleton arg1 must be listhead");
           return;
    }
#endif
        entr ->right        = pred->right; /* forw of entry */
        entr ->left         = pred;        /* back of entry */
        (pred->right)->left = entr ;       /* back of succ  */
        pred->right         = entr ;       /* for of pred */
}

//static IN_T is_valid();

/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  struct atom *removee (obj) 

PURPOSE   general purpose removee

RETURNS   if sucessful return a pointer to the object removed
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

void *list_remove (struct atom *obj) {
        register struct atom *entr;
        entr = obj - 1;
#ifdef LL_DEBUG
    fprintf(stderr,"rm %08lx<%08lx %08lx %04lx>\n",entr,entr->left,entr->right,entr->size);
#endif
#ifdef LL_REMOVE

    if ( ! is_valid(entr) || obj == NULL) {
#ifdef LL_VERBOSE
         sprintf(ebuf,"Invalid Node arg %08lx",obj);
             errwindow(remove_mess,ebuf);
#else
             errwindow(remove_mess,"Invalid Node");
#endif           
             return NULL;
    }
    if ( entr->size == 0L) {    
       errwindow(remove_mess,"cannot remove listhead");
       return NULL;
    }
    if (entr->right == entr) {
       errwindow(remove_mess,"node already singleton");
           return NULL;
    }
#endif
       
        (entr->left) ->right = entr->right; /* forw link of pred */
        (entr->right)->left  = entr->left;  /* back link of succ */
        entr->left = entr;                  /* return to singleton status */
        entr->right= entr;
        return (void *)obj;
}



/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  IN_T ishead(entr)

PURPOSE   routine to determine a list head or element

RETURNS   if a head    return 1
          else         return 0
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

IN_T list_ishead(struct atom *obj){
	register struct atom *entr;
	entr = obj - 1;
	if (entr->size == 0) {
		return 1;
	}
    return 0;
}
/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  static IN_T is_valid(entr)

PURPOSE   Internal list routine to check for a valid pointer

RETURNS   if invalid    return 0
          if o.k.   return 1
--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

#ifdef DEBUG_CODE
static IN_T is_valid(struct atom *entr){
    register struct atom *p; register IN_T j;
    j = (IN_T) entr;
    if (j != (j & -2L)) {  /* check for odd address */
       return 0;
    }
    p = entr->right;
    j = (IN_T) p;
    if (j != (j & -2L)) {  /* check for odd address */
       return 0;
    }
    if ( p->left != entr) { /* check for pointing back */
             return 0;
    }
    return 1;
}
#endif


/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  IN_T between(a,b,c)

PURPOSE   Compares the b to the range a to c

--------------------------------------------------------------------------------
** END HEADER *****************************************************************/
#if 0
static IN_T between(IN_T a,IN_T b,IN_T c){
        if (b <= a) {
           return a;
        } else if (b >= c) {
           return c;
        }
        return b;
}
#endif




/* HEADER **********************************************************************
--------------------------------------------------------------------------------
FUNCTION  main()

PURPOSE   Driver for testing functions


             - - - - - - - - - -
              Hash table format
                     - - - - - - - - - -
               (Note: spare cells not shown)


      Hash Table (stable)
     +--------------------+
     |stats, sizes, etc.  | 1-k
     |                    |
     +--------------------+

             ...                LH    symbol block
     +--------------------+     ++  +--------------------+
     |loc for this sym    | n-> ||->| data value Ptr/IN_T | <--> next symbol
     +--------------------+     ||  +--------------------+ 
                ||  | stats, misc info   |
         ...                ||  +--------------------+ 
                                ||  | string containing  |
     +--------------------+     ||  +-                  -+
     |                    |last ||  | symbol (symbol)    |
     +--------------------+     ++  +--------------------+


--------------------------------------------------------------------------------
** END HEADER *****************************************************************/

#ifdef TEST_MAIN
int tokens(char *str, char **av, char *out);
main(IN_T argc,char **argv) {

    IN_T ecode = 0;
    PSYMTABLE t;


	void *lh; IN_T i;
	char *a,*b,*c,*d,*e,*p;
	struct atom *foo;

	lh = list_newlh();
	a = (char *)list_new(50,"a"); 
								strcpy(a,"aaaa");
	b = (char *)list_new(50,"b");
								strcpy(b,"bbbb");
	c = (char *)list_new(50,"c"); 
								strcpy(c,"cccc");
	d = (char *)list_new(8, "d"); 
								strcpy(d,"1234567");  // this should fit, with the null at the end
	e = (char *)list_new(8, "e"); 
//								strcpy(e,"12345678"); //This should clobber the last byte with a null
	foo = (void *)a;
	foo = foo -1;
//	foo->right = 0;				// this is code to clobber a link on purpose

	list_clobber_check_all(1,0,0); // print a trace of all allocated memory
	list_clobber_check_all(0,0,0); // check all allocated memory for corruption

	list_insert_tail(lh,a);
	list_insert_tail(lh,b);
	list_insert_tail(lh,c);


	list_clobber_check_all(1,0,0);

	i = 0;
	for_list(char, p , lh) {
#ifdef I64
		fprintf(stderr,"fitem %ld = '%s'\n",i++,p);
#else
		fprintf(stderr,"fitem %d = '%s'\n",i++,p);
#endif
	}

	i = 0;


	for_list_rev(char, p , lh) {
#ifdef I64
		fprintf(stderr,"ritem %ld = '%s'\n",i++,p);
#else
		fprintf(stderr,"ritem %d = '%s'\n",i++,p);
#endif
	}

	for(;;) {
		p = list_remove_head(lh);
		if (p) {
			list_old(p);
		} else {
			break;
		}
	}


    t = hash_stabinit(71);	/* set up small size symbol table       */
						/* so we will be sure to get collisions */

    hash_sinstall(t,"one",(void *)1L);     /* put some items into the symbol table */
    hash_sinstall(t,"two",(void *)2L);      /* the data value is small integers     */
    hash_sinstall(t,"three",(void *)3L);    /* but could be structure pointers etc. */
    hash_sinstall(t,"four",(void *)4L);     /* or any 32 bit data items             */
    hash_sinstall(t,"five",(void *)5L);
    hash_sinstall(t,"six",(void *)6L);
    hash_sinstall(t,"seven",(void *)7L);
    hash_sinstall(t,"eight",(void *)8L);
    hash_sinstall(t,"nine",(void *)9L);
    hash_sinstall(t,"ten",(void *)10L);
    hash_sinstall(t,"eleven",(void *)11L);
    hash_sinstall(t,"twelve",(void *)12L);
    hash_sinstall(t,"thirteen",(void *)13L);
    hash_sinstall(t,"fourteen",(void *)14L);
    hash_sinstall(t,"fifteen",(void *)15L);

    fprintf(stderr,"look for ( 7)  %ld\n",hash_slookup(t,"seven")); /* find item */
    fprintf(stderr,"look for ( ?)  %ld\n",hash_slookup(t,"xxxxx")); /* not found */
    fprintf(stderr,"inserts=%ld lookups=%ld collisions=%ld compares=%ld notfound=%ld\n",
            t->s_nins,t->s_look,t->s_colls,t->s_ncmps,t->s_nfnd);

    hash_sdump(t,0L);				/* dump the symbol table to stdout */
    hash_sdelete(t,"four");			/* delete one symbol               */
    fprintf(stderr,"got rid of four\n");
    hash_sdump(t,0L);				/* dump the symbol table again     */
    hash_sdelete(t,(void *)1L);				/* delete all symbols in table     */
    fprintf(stderr,"deleted all items\n");
    hash_sdump(t,(void *)0L);				/* dump it again                   */
    hash_sinstall(t,"ffffff",(void *)5L);	/*  add back one  symbol           */
    fprintf(stderr,"added ffffff\n");
    hash_sdump(t,(void *)0L);				/* once more                       */
    hash_sdelete(t,(void *)0L);				/* now delete all items AND table  */


	{


		char *text = "aaa bbb,ccc,ddd \"one two\" last ";
		char temp[250];
		int ac;
		char *av[100];

		ac = tokens(text,av,temp);

		for(i = 0 ; i <  ac ; i++ ) {
#ifdef I64
			fprintf(stderr,"%ld '%s'\n",i,av[i]);
#else
			fprintf(stderr,"%d '%s'\n",i,av[i]);
#endif
		}

	}

    exit(ecode);
}
#endif
