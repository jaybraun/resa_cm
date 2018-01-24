

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
short dted_open(char *file,
                 short *lon_interval,
                 short *lon_rcds,
                 short *lat_interval,
                 short *lat_points);
short dted_read(short *column, short *lon_rcd);
short dted_close();

static long lonspc,latspc;

/* global variables */
static FILE *fin;
static short nofile=0,nodata=-9999;

short dted_open(char *file,
                 short *lon_interval,
                 short *lon_rcds,
                 short *lat_interval,
                 short *lat_points)
{
	if( (fin=fopen(file,"rb") )==NULL) // r=read; b=binary
		nofile=1;  //flag indicates file could not be opened
	else
	{
		nofile=0; /* flat indicates file sucessfully opened*/
    	// read and decoas the lon and lat spacing (lonspc,latspc) from file header
		fseek(fin,(long) 20,SEEK_SET);  // SEEK_SET means offset is from file beg
		fscanf(fin,"%4d%4d",&lonspc,&latspc);  // read lon and lat interval 1/10 sec
		lonspc=lonspc/10;
		latspc=latspc/10;
		*lon_interval = lonspc;
		*lat_interval = latspc;
		*lon_rcds = 1201;
		*lat_points = 1201;
	}
	return(nofile);
}

short dted_close()
{
	if (nofile == 0)
		fclose(fin);
	return(0);
}

short dted_read(short *column, short *lon_rcd)
{


short dat[1201];
static long firste=3436;
//static long firste=3428;
//static long firste=3584;
//static long firste=3516;
static long lencol=2414;
long recnum;
long i,lon,lat,lonsec,latsec,latindex;
static int lastlon=9999,lastlat=9999;
static int first=1;
char lonc[4],latc[3],drive[1];
long seek,len;

// if the needed file could not be opened, return the nodata value
if (nofile==1) return nodata;

// recnum is the byte offset to the desired column of data *
recnum= firste + *column*lencol;
seek=fseek(fin,recnum,SEEK_SET);
// read one column of data
//len=fread(&lon_rcd,sizeof(short) ,1201,fin);

//on IBM must swap the bytes */
len=fread(dat,sizeof(short) ,1201,fin);
swab( (char*)dat,(char*)lon_rcd,sizeof(dat));

return (0);
}
