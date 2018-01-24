
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
short dmed_open(char *file);
short dmed_read(char *DMED_record);
short dmed_close();

/* global variables */
static FILE *fin;
static short nofile=0;

short dmed_open( char *file)
{
	if( (fin=fopen(file,"rb") )==NULL) // r=read; b=binary
		nofile=1;  //flag indicates file could not be opened
	else
			nofile=0; /* flat indicates file sucessfully opened*/
	return(nofile);
}

short dmed_close()
{
	if (nofile == 0)
		fclose(fin);
	return(0);
}
short dmed_read( char *DMED_record)
{
	static char file[33]="/run/media/resa/TCD_DTED138/dmed.";
	/*                    12345678901234567890123456789012345678901234 */
	char dat[394];
	static long firste=0;
	static long lenrcd=394;
	static long seekpos=0;
	long i,lon,lat,lonsec,latsec,latindex;

	long seek,len;

	seek=fseek(fin,seekpos,SEEK_SET);
	// seekpos is the byte offset to the desired column of data *
	seekpos = seekpos + lenrcd;
	// read one rcd of data
	len=fread(DMED_record,sizeof(char) ,394,fin);
	//on PC must swap the bytes */
	//swab( (char*)dat,(char*)DMED_record,sizeof(dat));
	return(0);
}

		

