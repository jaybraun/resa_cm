/********************************************************************
out.h
***********************************************************************
07/01/96 - SJL - changed STDERR to STDOUT (as part of cleaning up
                 diagnostics used to test JCM 1047).
********************************************************************/

#ifndef OUT_H
#define OUT_H

#include <stdio.h>

/*****************************************************************
definitions
*****************************************************************/
#define OUT_STDOUT     1  /* enable output to stdout */
#define OUT_LOG        2  /* enable output to Out_file */
#define OUT_FLUSH      4  /* flush Out_file after each call to Out */
#define OUT_NEWLINE    8  /* add '\n' to end */
#define OUT_OVERWRITE 16  /* add '\r' to end */
#define OUT
extern int Out_flags;       /* controls Out processing */
extern FILE *Out_file;      /* log file */

/*****************************************************************
prototypes
*****************************************************************/
void Out (char *string, ...);
int Open_Out (char *fname);
void Close_Out (void);

#endif
