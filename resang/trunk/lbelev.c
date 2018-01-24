void resa_worldwide_elevation(float*, float*, int*);
void modify_resa_worldwide_elevation(int*,int*,float[],float[]);
signed short int loc_elev(float, float);
void set_loc_elev(float,float,short int);
int pnpoly(int, float*, float*, float, float);

//These defines can be changed for each app
#define ELEVDIR "./"
#define ARCSEC 3
//The next define should always be 3600 / ARCSEC + 1
#define NUMSAMPLES 1201

//At about 1000 tiles, we reach max virtual address space of about 3GB 
#define MAXTILES 600

#define UNAVAILABLE -32768

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int last_latfloor, last_lonfloor, last_fd, last_empty_latfloor, last_empty_lonfloor;
int numtiles;
char *already_mapped[180][360];
signed short int last_available_elev;
const float feet_per_meter=3.2808399;

//The wrapper for RESA:

void resa_worldwide_elevation(float *latp, float *lonp, int *feet) {

   float lat=*latp;
   float lon=*lonp;

   *feet=(int)((float)loc_elev(lat,lon)*feet_per_meter);

}

//The function loc_elev() takes lat/lon as its arguments, and returns a result
//in meters.  It can be used by any application.

signed short int loc_elev(float lat, float lon) {

   int latfloor = (int) lat;
   int lonfloor = (int) lon;
   int latcells = (int) ((lat - (float) latfloor) * 3600.0 / (float)ARCSEC);
   int loncells = (int) ((lon - (float) lonfloor) * 3600.0 / (float)ARCSEC);
   int cellnum=0;
   int altcellnum=0;
   int offset=0;
   int result;
   int i,j;
   char latchar, lonchar;
   char str[100], filename[100];
   int fd;
   signed short int elev=0;
   union {
      char ch[2];
      short n;
   } char2short;
   char *addr;
   void *unmapaddr;

   if(lat>0.0) {
      latchar='N';
      latcells= 1200-latcells; //north first
   } else {
      latchar='S';
      latcells=abs(latcells);
      latfloor=abs(latfloor)+1; 
   }

   if(lon>0.0) {
      lonchar='E';
   } else {
      lonchar='W';
      loncells=abs(loncells);
      lonfloor=abs(lonfloor)+1; //closer to 0
      loncells=1200-loncells; //west first
   }

   if (last_empty_latfloor == latfloor && last_empty_lonfloor == lonfloor) {
      return 0;
   }

   if (last_latfloor != latfloor || last_lonfloor != lonfloor) {

      last_latfloor=latfloor;
      last_lonfloor=lonfloor;

      filename[0]='\0';
      sprintf(str,ELEVDIR);
      strcat(filename,str);
      sprintf(str,"%c",latchar);
      strcat(filename,str);
      sprintf(str,"%d",latfloor);
      strcat(filename,str);
      sprintf(str,"%c",lonchar);
      strcat(filename,str);
      if (lonfloor<100) { //add leading 0
         sprintf(str,"0");
         strcat(filename,str);
      }
      sprintf(str,"%d",lonfloor);
      strcat(filename,str);
      sprintf(str,".hgt");
      strcat(filename,str);

      if (access(filename, F_OK) == -1) { 
         last_empty_latfloor = latfloor;
         last_empty_lonfloor = lonfloor;
         return 0; //no file
      }

      if (already_mapped[latfloor+90][lonfloor+180]==0) {

         //This is where we check if we should free up memory
         if (numtiles >= MAXTILES) {

         /* Best to just leave the elevations that we have.  It is very easy
          * to "bounce" the workstation or ptmain itself and start again if
          * the user has really jumped to a different area or consistenty
          * needs a huge radius.
          *  for (i=0;i<180;i++) {
          *    for (j=0;j<360;j++) {
          *       if(already_mapped[i][j]) {
          *          unmapaddr=already_mapped[i][j];
          *          result=munmap(unmapaddr, NUMSAMPLES*NUMSAMPLES*2);
          *          already_mapped[i][j]=0;
          *       }
          *    }
          * }
          *
          *  numtiles=0; //start again
          */

            return 0;
         }
      
         fd=open(filename, O_RDWR);

         addr=mmap((caddr_t)0, NUMSAMPLES*NUMSAMPLES*2, 
                   PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

         if (addr==MAP_FAILED || !addr) {
            return 0;
         }

         //One more tiles has been mapped into memory
         numtiles++;

         already_mapped[latfloor+90][lonfloor+180]=addr;

      } else { 

         addr=already_mapped[latfloor+90][lonfloor+180];

      }

   } else {

     addr=already_mapped[latfloor+90][lonfloor+180];

   } //on test for new file or existing one (or no file)

   if (!addr) {
      return 0;
   }

   cellnum=latcells*NUMSAMPLES+loncells;

   offset=cellnum*2;

   char2short.ch[1]=*(addr+offset);
   offset++;
   char2short.ch[0]=*(addr+offset);
   elev=char2short.n;

   if (elev==UNAVAILABLE) {
      elev=1; //for now
   }

   return elev;

}

void modify_resa_worldwide_elevation(int *feetp, int *np, float lats[], float lons[]) {

   int feet=*feetp, n=*np, i;
   float fmeters = (float)feet/feet_per_meter;
   short int meters = (short int) fmeters;
   float minlat, maxlat, minlon, maxlon;
   float thisx, thisy;
   const float step = (float) ARCSEC / 3600.0;

   minlat=lats[0];
   maxlat=minlat; 
   minlon=lons[0];
   maxlon=minlon;
   for (i=1; i<n; i++) { //we start at 1 because we already know first elements
      if (lats[i]<minlat) minlat=lats[i];
      if (lats[i]>maxlat) maxlat=lats[i];
      if (lons[i]<minlon) minlon=lons[i];
      if (lons[i]>maxlon) maxlon=lons[i];
   }

   //Advance through the points in a "rectangle"
   thisy=minlat;
   thisx=minlon;
   while (thisx<maxlon) {
      while(thisy<maxlat) {

         if(pnpoly(n,lons,lats,thisx,thisy)) {
            set_loc_elev(thisy,thisx,meters);
if (loc_elev(thisy,thisx)>0) printf("%f %f %d\n", thisy, thisx, loc_elev(thisy,thisx));
         }

         thisy=thisy+step;
      }
      thisy=minlat;
      thisx=thisx+step;
   }
   
} 


void set_loc_elev(float lat, float lon, short int meters) {

   int latfloor = (int) lat;
   int lonfloor = (int) lon;
   int latcells = (int) ((lat - (float) latfloor) * 3600.0 / (float)ARCSEC);
   int loncells = (int) ((lon - (float) lonfloor) * 3600.0 / (float)ARCSEC);
   int cellnum=0;
   int altcellnum=0;
   int offset=0;
   int result;
   int i,j;
   char latchar, lonchar;
   char str[100], filename[100];
   int fd;
   signed short int elev=0;
   union {
      char ch[2];
      short n;
   } char2short;
   char *addr;
   void *unmapaddr;

   if(lat>0.0) {
      latchar='N';
      latcells= 1200-latcells; //north first
   } else {
      latchar='S';
      latcells=abs(latcells);
      latfloor=abs(latfloor)+1; 
   }

   if(lon>0.0) {
      lonchar='E';
   } else {
      lonchar='W';
      loncells=abs(loncells);
      lonfloor=abs(lonfloor)+1; //closer to 0
      loncells=1200-loncells; //west first
   }

   if (last_empty_latfloor == latfloor && last_empty_lonfloor == lonfloor) {
      return;
   }

   if (last_latfloor != latfloor || last_lonfloor != lonfloor) {

      last_latfloor=latfloor;
      last_lonfloor=lonfloor;

      filename[0]='\0';
      sprintf(str,ELEVDIR);
      strcat(filename,str);
      sprintf(str,"%c",latchar);
      strcat(filename,str);
      sprintf(str,"%d",latfloor);
      strcat(filename,str);
      sprintf(str,"%c",lonchar);
      strcat(filename,str);
      if (lonfloor<100) { //add leading 0
         sprintf(str,"0");
         strcat(filename,str);
      }
      sprintf(str,"%d",lonfloor);
      strcat(filename,str);
      sprintf(str,".hgt");
      strcat(filename,str);

      if (access(filename, F_OK) == -1) { 
         last_empty_latfloor = latfloor;
         last_empty_lonfloor = lonfloor;
         return; //no file
      }

      if (already_mapped[latfloor+90][lonfloor+180]==0) {

         //This is where we check if we should free up memory
         if (numtiles >= MAXTILES) {

            for (i=0;i<180;i++) {
               for (j=0;j<360;j++) {
                  if(already_mapped[i][j]) {
                     unmapaddr=already_mapped[i][j];
                     result=munmap(unmapaddr, NUMSAMPLES*NUMSAMPLES*2);
                     already_mapped[i][j]=0;
                  }
               }
            }

            numtiles=0; //start again

         }
      
         fd=open(filename, O_RDWR);

         addr=mmap((caddr_t)0, NUMSAMPLES*NUMSAMPLES*2, 
                   PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

         if (addr==MAP_FAILED || !addr) {
            return;
         }

         //One more tiles has been mapped into memory
         numtiles++;

         already_mapped[latfloor+90][lonfloor+180]=addr;

      } else { 

         addr=already_mapped[latfloor+90][lonfloor+180];

      }

   } else {

     addr=already_mapped[latfloor+90][lonfloor+180];

   } //on test for new file or existing one (or no file)

   if (!addr) {
      return;
   }

   cellnum=latcells*NUMSAMPLES+loncells;

   offset=cellnum*2;

   char2short.n=elev;
   *(addr+offset)=char2short.ch[1];
   offset++;
   *(addr+offset)=char2short.ch[0];

}


//The following algorithm was found on the internet.
//Credit goes to W. Randolph Franklin, Professor in the Electrical, Computer &
//Systems Engineering Deprtment at Rensselaer Polytechnic Institute,
//who explains this elegant algorithm at:
//     http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html

int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
         (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}
