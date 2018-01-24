// ptcvext.c

#include <math.h>
#include <stdio.h>
#include "ptwand.h"         
#include "ptvext.h"  /* EXT_ structure definition */       
#include "ptdraw.h"         

/* following routines found in dwtext.c */
/*arc*/
/*ccross*//*moved to dwdraw.rat*/
int clpmsk_wrap(float *, float *);
int clpmsk(float,float);
/*envlop*//*moved to dwdraw.rat*/
int inbox(float, float, float, float);
int inpnt(short int, short int);
/*linedg*/
/*md*/
int mx_wrap(float *);
int mx(float);
int my_wrap(float *);
int my(float);
void proj(short int);
/*satoi*//*moved to dwdraw.rat*/
/*smv*//*moved to dwdraw.rat*/
/*smvhn*//*moved to dwdraw.rat*/
/*smvib*//*moved to dwdraw.rat*/
void viewpt (short int,short int,short int,short int);
void window (float,float,float,float);
void winview(void);

int clpmsk_wrap(float *xarg, float *yarg)
{
	float x, y;
	int irc;
	x = *xarg;
	y = *yarg;
    irc = clpmsk(x, y);
	return(irc);
}

int clpmsk(float x,float y)    
{

/*#####################################################################
#
# Arguments
#       x      A single precision real variable containing a x value.
#       y      A single precision real variable containing a y value.
#
#####################################################################*/

float xt;      /* float x,y input in routine call */
short int c;

/* initialize c */
c = 0;

xt = x;

/* if mercator projection */
if (EXT_.projty == MERCATOR)
  {
   if (abs(EXT_.wxr) > 180.) /* Crossing date line */
     {
       if ((xt < EXT_.wxl) & ((EXT_.wxl - xt) > 180.))
        {
	 xt = 360. + xt;
        }
     }
  }
 
 if      (xt < EXT_.wxl) c = c|8;     /* Left of left edge */
 else if (xt > EXT_.wxr) c = c|4;      /* Right of right edge*/

 if      (y < EXT_.wybt) c = c|2;      /* Below bottom edge */
 else if (y > EXT_.wytt) c = c|1;      /* Above top edge */

 /* return clip mask code (see box above under "Purpose")*/
 return (c);

} /* end of clpmsk */



int inbox(float xl,float yb,float xr,float yt)
{ 
 int input_flag;

 if (xl < EXT_.vxl | yb > EXT_.vyb | 
     xr > EXT_.vxr | yt < EXT_.vyt) 
      input_flag = 0;
 else input_flag = 1;

 return(input_flag);
} /*end of inbox*/


int inpnt(short int x,short int y)  
{
/*if coordinate is too far left, bottom, right or top to be in view
  window, set flag to zero 
*/
 int inpnt_flag;

 if (x < EXT_.vxl | x > EXT_.vxr | y > EXT_.vyb | y < EXT_.vyt) 
      inpnt_flag = 0;
 else inpnt_flag = 1;

 return(inpnt_flag); /* end of inpnt*/
}

int mx_wrap(float *xarg)
{
	float x;
	int xt;
	x = *xarg;
	xt = mx(x);
	return(xt);
}
int mx(float x)
{

float xt;

xt = x;

/* if mercator projection */
if (EXT_.projty == MERCATOR)
  {
   if (abs(EXT_.wxr) > 180.)  /* Crossing date line */
     {
      if (xt < EXT_.wxl & (EXT_.wxl - xt) > 180.)
        {
	 xt = 360. + xt;
        }
     }
  }

xt = EXT_.wvxm * xt + EXT_.wvxa; 

return((int)xt);
} /* end of function mx */

int my_wrap(float *yarg)
{
	float y;
	int yt;
	y = *yarg;
	yt = my(y);
	return(yt);
}

int my(float y)
{

float yt, t_1, t_2;

yt = y;

/* if mercator projection */
if (EXT_.projty == MERCATOR) 
  {
    /* convert longitiude to mercator */
/*   yt = alog(tan(PI/4. + yt * PI/360.)) */

   t_1 =(M_PI/4. + yt * M_PI/360.);
   t_2 = tanf(t_1);
   yt = logf(t_2);
  }

yt = EXT_.wvym * yt + EXT_.wvya;

 return((int)yt);
}/* end of my*/

 void proj(short int type)
{

 EXT_.projty = type;
 } /* end of proj*/


void viewpt (short int ixl,short int iyb,short int ixr,short int iyt)
{

/* define rectangle to be used in the view */
 EXT_.vxl = ixl;
 EXT_.vxr = ixr;
 EXT_.vyb = iyb;
 EXT_.vyt = iyt;

 /* display the viewport */
 winview();

} /* end of viewpt */



void window (float xl,float yb,float xr,float yt)
{
  float quarter_pi = M_PI/4.;
  float full_circle_pi = M_PI/360.;
/* get viewport values */
 EXT_.wxl = xl;
 EXT_.wxr = xr;
 EXT_.wybt = yb;
 EXT_.wytt = yt;

/*if regular rectangular projection */
 if (EXT_.projty == NOPROJ)
  {
   EXT_.wyb = yb;
   EXT_.wyt = yt;
  }
/* else if mercator projection */
 else if (EXT_.projty == MERCATOR)
  {
   if (yb <= -90.) yb = -89.;
   if (yt >=  90.) yt =  89.;
  /* compare these log values to alpha alog values*/ 
   EXT_.wyb = log(tan(quarter_pi + yb * full_circle_pi));
   EXT_.wyt = log(tan(quarter_pi + yt * full_circle_pi));
  }

/* display viewport */
 winview();

} /* end of window*/



void get_window (float *xl,float *yb,float *xr,float *yt)
{
	*xl = EXT_.wxl;
	*xr = EXT_.wxr;
	*yb = EXT_.wybt;
	*yt = EXT_.wytt;
} /* end of get_window*/



void winview(void){

 float wxdiff;

 wxdiff = EXT_.wxr - EXT_.wxl;  /*range of degrees on x axis*/
 if (EXT_.projty == MERCATOR)
   {
     if (wxdiff < 0.) /*if the range of degrees is neg,make it pos*/
      {
       wxdiff = 360. - wxdiff;
      }
   }

 /*change in x pixels over change in x degrees */
 EXT_.wvxm = (EXT_.vxr - EXT_.vxl) / wxdiff;
 /*pixel range on x axis */
 EXT_.wvxa = EXT_.vxl - EXT_.wxl * EXT_.wvxm;

 /*change in y pixels over change in y degrees */
 EXT_.wvym = (EXT_.vyt - EXT_.vyb) / (EXT_.wyt - EXT_.wyb);
 /* wvym = (vyb - vyt) / (wyt - wyb)*/

 /* pixel range on y axis */
 EXT_.wvya = EXT_.vyb - EXT_.wyb * EXT_.wvym;
 /* wvya = vyt - wyt * wvym*/
 
}/* end of winview */

