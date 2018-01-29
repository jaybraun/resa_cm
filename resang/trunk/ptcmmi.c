/*#################################################################
#$%  PTCMMI.C
#$%
#$% 
#$%   1.2) If Button 1 is depressed then either:
#$%      1.2.1) If there was previous point was in the same place on 
#$%             the screen then the LAT/LONG will be deleted from  
#$%             the display.
#$%      1.2.2) If there was no previous point displayed then the 
#$%             LAT/LONG of the current point the will be displayed 
#$%             on the terminal screen.
#$%      1.2.3) If there was a previous LAT/LONG actively displayed
#$%             at a different point on the screen then a range and
#$%             bearing between the two points will be calculated 
#$%             and displayed; also the old LAT/LONG will be delete 
#$%             and the new LAT/LONG will be displayed.
#$%     1.4) If button 2 is depressed then :
#$%         1.3.1) If the point on the screen does not represent the
#$%                position of one of the ships/aircraft/etc then 
#$%                any previously hooked data is deleted. 
#$%         1.3.2) If the point on the screen does represent the 
#$%                position of one of the hookable units then the 
#$%                units ID,COURSE,SPEED,ALTITUDE will be displayed
#$%                in the upper right hand side of the screen.
#$%
###################################################################*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include <math.h>
//#include <X11/Xlib.h> -- included by ptcdraw.h
//#include <X11/Xutil.h>
//#include <X11/Xatom.h>
#include <X11/Xos.h>
//#include <X11/cursorfont.h>
//#include <X11/Xcursor/Xcursor.h>
//#include <X11/Xproto.h>

//include "ascii.inc"         
//include "macros.inc"         
#include "ptcolor.h"         
#include "ptgraph.h"         
#include "ptwand.h"//FALSE,TRUE
#include "ptvext.h"//contains EXT_ definition
//include "secdef.inc"        
#include "ptdraw.h" //contains global variables 

#define RAD2DEG 57.29577951
/*DEG2RAD defined in dwdraw.h*/

#define MAX_ARROWHEAD_POINTS 10

typedef struct {
  int ac_style;        // 1
  int ac_thickness;           // Pixels
  int ac_head_length;  // 10 / # Pixels
  int ac_line_length;  // 25 / # Pixels
  int ac_head_offset;  // 6 /  # Distance From Point To Tip Of Arrow
  int ac_tail_offset;  // 6 /  # Distance From Butt To Computed Point
  int ac_head_x;              // Initial X/Y Location Pointed To
  int ac_head_y;
  int ac_tail_x;              // Computed X/Y Location Pointed From
  int ac_tail_y;
  int ac_pattern;      // 6 /  # code
  int ac_pixmap;       // 0 /   
} arrow_desc;

arrow_desc ac = { 1,0,10,25,6,6,0,0,0,0,6,0 }; //initialize struct

//following defines used in get_arrowhead_points
#define ARROW_1 1 /*$arrow$1*/
#define ARROW_2 2
#define ARROW_3 3
#define ARROW_4 4
#define ARROW_5 5 
#define HALF_ARROW_DEGREES 25.0

typedef struct {
  int num_points;
  XPoint pnts[MAX_ARROWHEAD_POINTS];
  XPoint tail_pnt;
} arrow_pts;

arrow_pts arrow;

typedef struct {
  int prev_point;  //YES or NO
  int prev_ix, prev_iy;
  float rad_lat_prev;
  float rad_long_prev;
} prev_point;

prev_point prev_pt;


extern void draw_text_c();
extern void draw_textblock_init_c();
extern void draw_textblock_c();
extern int the_elevation(); //int fxn to obtain elevation <lbelevation.rat>
extern void puck_position_2_ll ();
extern void rll2rb();
extern void set_color();
extern void smovll();
extern void smov();
extern void smovib();
extern void smovv();
extern void ang_to_pi(float);  //cdwdraw.c

void puck_position_request_c(int,int); 
void draw_arrow_init_c(char *,int);/*moved from cdwdraw.c*/
void draw_arrow_get_tail_c(int *,int *);/*moved to cdwdraw.c*/
void check_for_user_input_c(void);
void timer_handler(void);
void do_Xevent_buffer_c(void);/*event handling*/
Bool predproc_c();
void draw_AST_button_c();
/*draw_ast_button_info*/
void draw_AST_expose_c(void);
void draw_AST_configure(void);
int draw_arrow_xy_c(int,int,int,int);
Atom wmDeleteMessage;
XEvent anevent;
int running;
void get_arrowhead_points_c(int, int, float, int, int, int, int, int, int);


void puck_position_request_c (int x_in,int y_in)
{
  int   curr_ix,curr_iy;
  int   prev_ix;
  int   prev_iy;
  int   latlon_len;
  int  kk;
  int  ilen=0;
  int  prev_point; 
  int  irange, ibearing;
  float range, bearing;

  float deg_lat_curr, deg_long_curr;
  float rad_lat_curr, rad_long_curr;
  float cos_rad_lat_prev;
  float cos_rad_lat_curr;
  float curr_radius;

  int font_w, font_h;
  int old_mode, angle;
  int tail_x, tail_y;
  int text_width, box_style; 
  int fg_color, bg_color;   
  int text_style=0;
  int arrow_style, font_size;
  bool upper,left;
  int alt_at_point=0;
  int msglen, brglen, rnglen, altlen;

  char msg[ARB], bearing_char[3], range_char[5];
  char lat_msg[7], lon_msg[8], alt_msg[8];
  int linenum=0, lon_len=0, alt_len=0;
  int colornum;

  memset(msg,0,ARB);

  curr_ix = x_in;
  curr_iy = y_in;

  if (prev_pt.prev_point == YES) // If old exists then delete it
    {       
     colornum = COLOR_TEXT;

     // Copy Pixmap to window to "erase" old DDD-MM-SSH text

     if (WinC.dw_live_pixmap != WinC.dw_window[DW_TACTICAL])
        {
	 XCopyArea (dw_display, WinC.dw_live_pixmap, 
                    WinC.dw_window[DW_TACTICAL],
                    ta_win_GC.dw_GC, 0, 0, 
                    WinC.dw_current_w, WinC.dw_current_h, 
		    0, 0);
        }
    }/* end if there is a previous point*/

  if ((abs(curr_ix-prev_pt.prev_ix)+abs(curr_iy-prev_pt.prev_iy))<4) 
    {/*if prev_point is approx the same as this point
       then  call the qio for the Tablet and return*/
     prev_pt.prev_point=NO;   // reset pointer 
     prev_pt.prev_ix=0;       // make sure it will light next time
     prev_pt.prev_iy=0;       // thru the loop
    }
  else // We need to draw the specified point
    {
     //converts window pixels to lat/lon degrees
     puck_position_2_ll (&curr_ix, &curr_iy, 
			 &deg_lat_curr, &deg_long_curr,
			 &EXT_.wvxa, &EXT_.wvxm,
			 &EXT_.wvya, &EXT_.wvym);

     rad_lat_curr  = deg_lat_curr * DEG2RAD;
     rad_long_curr = deg_long_curr * DEG2RAD;

     curr_radius = WinC.dw_radius;
     colornum = COLOR_BOX_YELLOW;
     linenum = LINE_POS_1; 
     strcpy(msg,"Cursor was at:");
     draw_text_c (&linenum,msg,&colornum);

     ilen=0;                  // reset pointer
     strcpy(msg,"              ");
     strcpy(lon_msg,"        ");
     ilen = 1;
     if (curr_radius > 60.001)   // format lat/long as DDD-MMH
        {
	 smovll (&rad_lat_curr,"NS",&msg,&ilen);
         strcpy(lat_msg,msg);
 	 smov   (" ",&msg,&ilen);
         kk = strlen(msg);
         smovll (&rad_long_curr,"EW",&msg,&ilen);
         smovll (&rad_long_curr,"EW",&lon_msg,&lon_len);
        }
     else                          // format lat/long as DDD-MM-SSH
        {
	 smovlls (&rad_lat_curr,"NS",&msg,&ilen);
         strcpy(lat_msg,msg);
	 smov    (" ",&msg,&ilen);
 	 //kk = ilen;             // save the position of the slash
         kk = strlen(msg);
	 smovlls (&rad_long_curr,"EW",&msg,&ilen);
         // tmb 03-2017 show seconds of long, not just ddd-mm
         smovlls (&rad_long_curr,"EW",&lon_msg,&lon_len);
        }

     smov    (" ",&msg,&ilen);
     latlon_len = ilen;

     set_plot_input(&msg, &ilen); 	// store position in BB 

    alt_at_point = the_elevation (&deg_lat_curr, &deg_long_curr);
    //alt_at_point=750;
    strcpy(alt_msg,"       ");
    //strcat(msg," ");//space btwn long and alt
    if (alt_at_point > 0)
        {
	 altlen=5;
         smovib (&alt_at_point,&altlen,&msg,&ilen);
         smovib (&alt_at_point,&altlen,&alt_msg,&alt_len);
	 /*smovv ("ft",&msg,&ilen);*/
         strcat(msg,"ft");
         strcat(alt_msg,"ft");
        }

    // at this point, we have a string with the Lat/Long/elev

    old_mode = NO;
    if (old_mode == NO)
      {
      //setup arrow
       draw_arrow_init_c("pixmap",WinC.dw_window[DW_TACTICAL]);
       draw_arrow_init_c("style",      5);
       draw_arrow_init_c("thickness" , 1);
       draw_arrow_init_c("headoffset", 0);
       draw_arrow_init_c("tailoffset", 0);

       //draw the arrow
       upper = curr_iy <= (WinC.dw_current_h / 2);
       left  = curr_ix <= (WinC.dw_current_w / 2);

       if      (upper &   left) angle = 7 * 45;
       else if (upper & ! left) angle = 1 * 45;
       else if (          left) angle = 5 * 45;
       else                     angle = 3 * 45;

       colornum = COLOR_BOX_YELLOW;
       draw_arrow_xy_c (curr_ix, curr_iy, angle, colornum);

       draw_arrow_init_c("pixmap",     0);// return to default
       draw_arrow_init_c("headoffset", 6);
       draw_arrow_init_c("tailoffset", 6);

       // setup textblock
       font_size  = 3;        // Large
       text_width = 11;       // Display on two/three lines
       /*      text_style = mod(text_style+1,3);*/
       text_style = ((text_style+1)%3);
       box_style  = 4;        // shadow
       fg_color   = COLOR_BOX_YELLOW;
       bg_color   = COLOR_BACKGROUND;

       draw_arrow_get_tail_c(&tail_x,&tail_y);//get text attach point

       draw_textblock_init_c("pixmap",WinC.dw_window[DW_TACTICAL]);
       draw_textblock_init_c("font",font_size);//1==small,3=Large
       draw_textblock_init_c("boxcolor",COLOR_BOX_YELLOW);

       // draw the textblock
       
       draw_textblock_c (tail_x, /*Display Block Of Text At X/Y*/
	      tail_y,       /*As Returned From Arrow (pixels)*/
              angle,        /* As Passed To Arrow (Deg)*/
	      msg,       /*With $LineFeed between lines*/
	      text_width,      /*Text Width (To Wrap) Or Zero*/
	      text_style,      /*Left/Center/Right (0..2)*/
	      box_style,/*Transparent/Opaque/Rect/Shadow (0..3)*/
	      fg_color,/*Color Of Text (normally $color$Text)*/
	      bg_color,/*Color Of Fill (normally $color$background)*/
	      0);       /*Used In Hooking (0 or unique value)*/

       //8/20/14       draw_textblock_init_c("pixmap",  0);
      } /*end if old_mode = NO*/
    else /* old_mode != NO*/
      {
	set_background (COLOR_BACKGROUND); /*change current color*/
        set_color (COLOR_BOX_YELLOW); /*change current color*/
	set_font (DW_PICK_FONT); /*select the font*/

        font_h = WinC.dw_font_h[DW_PICK_FONT];
        font_w = WinC.dw_font_w[DW_PICK_FONT];

        /*this first part is without background...  (the +)*/
        msglen = strlen(msg);
        XDrawString (dw_display, 
	             WinC.dw_window[DW_TACTICAL], 
		     //WinC.dw_map_pixmap,//SMM 8/19/14
	             ta_win_GC.dw_GC, 
                     curr_ix-font_w/2, 
	             curr_iy+font_h/2, 
	             msg,msglen);

       /*this next part is with default (white/black) background...*/
        //msglen = 0;
        XDrawImageString (dw_display, 
	                  WinC.dw_window[DW_TACTICAL],
                          //WinC.dw_map_pixmap,//SMM 8/19/14 
	                  ta_win_GC.dw_GC, 
                          curr_ix+font_w*2, 
	                  curr_iy+font_h/2, 
	                  msg,msglen);
	                //msg[3],msglen);
        XFlush(dw_display);

      } /*end if old_mode !=NO*/

    /* update the Tactical Data Window*/

      colornum = COLOR_BOX_YELLOW;
      linenum = LINE_POS_2;
      draw_text_c(&linenum,lat_msg,&colornum); // Latitude
      linenum = LINE_POS_3; 
      draw_text_c(&linenum,&lon_msg,&colornum);// Longitude
      linenum = LINE_POS_4; 
      draw_text_c(&linenum,&alt_msg,&colornum);// altitude
          

      if (prev_pt.prev_point == YES) //Do we need Range/Bearing
        {/* Rhumb line Range/Bearing*/
	 cos_rad_lat_prev = cos(prev_pt.rad_lat_prev);
	 cos_rad_lat_curr = cos(rad_lat_curr);

	 rll2rb(&prev_pt.rad_lat_prev,      /* lat of 1 st point*/
	        &prev_pt.rad_long_prev,     /* long of 1 st point*/
	        &cos_rad_lat_prev,  /* cos of lat of 1 st point*/
	        &rad_lat_curr,      /* lat of second point*/
	        &rad_long_curr,     /* long of second point*/
	        &cos_rad_lat_curr,  /* cos of lat of second point*/
	        &range,             /* range between the objects*/
	        &bearing);          /*bearing between the objects*/

	 ibearing = bearing * RAD2DEG;  //make bearing integer

         ilen = 0;
sprintf(bearing_char,"%i",ibearing);/*convert radius to char*/ 
	 strcpy(msg,bearing_char);
         strcat(msg,"  BRG         ");
	 linenum = LINE_BEARING;
	 colornum = COLOR_BOX_YELLOW;
	 draw_text_c(&linenum,&msg,&colornum);

	 ilen = 0;
	 if (curr_radius > 60.001) // Format range as 9999
            {
	     irange = range + 0.5; // make range integer
	     sprintf(range_char,"%i",irange);
           }
	 else if (curr_radius > 30.001)// Format range as 99.9
            {
	     sprintf(range_char,"%4.1f",range);
           }
	 else                     // Format range as 99.99
            {
	     sprintf(range_char,"%5.2f",range);
            }
	 strcpy(msg,range_char);
         strcat(msg,"  RNG         ");
	colornum = COLOR_BOX_YELLOW;
        linenum = LINE_RANGE;
        draw_text_c (&linenum,&msg,&colornum);
      } /* end if Previous point*/

    prev_pt.prev_point = YES;  // flag that we have a point
    prev_pt.rad_lat_prev = rad_lat_curr;   // save curr lat as prev
    prev_pt.rad_long_prev = rad_long_curr; // save curr lat as prev
    prev_pt.prev_ix = curr_ix;             // save curr x screen coor 
    prev_pt.prev_iy = curr_iy;             // save curr y screen coor 

   } // end IF (We need to draw the specified point)

 draw_text_done();// refresh Text window
//call x$sync (DW_Display, .false.)
}/*end of puck_position_request_c */




void draw_arrow_init_c(char *argument, int value)
{
 if      (strcmp(argument,"style")==0) ac.ac_style = value;
 else if (strcmp(argument,"thickness")==0) ac.ac_thickness = value;
 else if (strcmp(argument,"headlength")==0)ac.ac_head_length = value;
 else if (strcmp(argument,"linelength")==0)ac.ac_line_length = value;
 else if (strcmp(argument,"headoffset")==0)ac.ac_head_offset = value;
 else if (strcmp(argument,"tailoffset")==0)ac.ac_tail_offset = value;
 else if (strcmp(argument,"pattern")==0)ac.ac_pattern = value;
 else if (strcmp(argument,"pixmap")==0) ac.ac_pixmap = value;

}/* end of draw_arrow_init_c */


void draw_arrow_get_tail_c (int *x,int *y)
{
  *x = ac.ac_tail_x;
  *y = ac.ac_tail_y;

} /*end of draw_arrow_get_tail_c */

void check_for_user_input_c(void)
{
  struct sigaction sa;
  struct itimerval timer;
  int number_pending = 0;

  running=1;

  do_Xevent_buffer_c();/*check once */
  number_pending = XEventsQueued(dw_display,QueuedAlready);
  if (number_pending > 0){  
     while (running== 1) {   
       do_Xevent_buffer_c();
       number_pending = XEventsQueued(dw_display,QueuedAlready);
       if (number_pending == 0) running = 0;
       // printf (" number pending %d----\n",number_pending);
    } //end while running
  } // end if number_pending
}/* end of check_for_user_input */

void timer_handler (void)
{
  static int count = 0;
} /*end of timer_handler */

/*do_Xevent_buffer_c (int *running)*/
void do_Xevent_buffer_c (void) {
   // Poll for the next event

   static char *stuff = "do this or that";
   int b_num, ix, iy, e_win;
   int new_w, new_h;
   int window_num;
   
   //Note: If no buffered event, e struct is not cleared.  
   //Check returns false if no event.
   if (XCheckIfEvent(dw_display, &anevent, predproc_c, stuff)){
   switch (anevent.type)  {
   case Expose:
      /* Redraw contents of windows; note that we can't
       * use switch because window IDs are not constant */
      break;
   case ButtonPress:
      /* Respond to buttonpress, probably depending on
       * which window is reported in anevent.xbutton.window */
      b_num = anevent.xbutton.button;
      ix = anevent.xbutton.x;
      iy = anevent.xbutton.y;
      window_num = WinC.dw_window[DW_TACTICAL];
      e_win = anevent.xbutton.window;
      if (e_win == window_num)
        {
	 draw_AST_button_c(ix, iy, b_num);
         break;
	 }
   case ConfigureNotify:
      /* Window was resized, moved, or restacked or border
       * width was changed; reset application variables
       * so Expose branch will scale graphics properly */
      WinC.dw_new_w = anevent.xconfigure.width;
      WinC.dw_new_h = anevent.xconfigure.height;
      draw_AST_configure();
      break;
   case MappingNotify:
      /* Keyboard or Pointer mapping was changed by another
       * client; if keyboard, should call XRefreshKeyboardMapping,
       * unless keyboard events are not used */
      break;
   case ClientMessage:
      /* Primarily used for transferring selection data,
       * also might be used in a private interclient
       * protocol; otherwise, not needed in event loop */
      if ((Atom)anevent.xclient.data.l[0] == wmDeleteMessage)
	 running = 0;
      break;
   case SelectionClear:
      /* If this application previously called
       * XSetSelectionOwner, it may get this event;
       * otherwise, you don't need it in your
       * event loop */
      break;
   case SelectionNotify:
      /* If this application calls XConvertSelection,
       * it will get this event; otherwise, you don't
       * need it in your event loop */
      break;
   case SelectionRequest:
      /* If this application previously called
       * XSetSelectionOwner, it may get this event;
       * otherwise, you don't need it in your
       * event loop */
      break;
   case GraphicsExpose:
      /* Fall through into NoExpose */
   case NoExpose:
      /* If this application calls XCopyArea or XCopyPlane
       * and the graphics_exposures member of the GC is
       * True and the source is a window, these events may
       * be generated; handle GraphicsExpose like Expose */
   default:
      /* All *Notify events except ConfigureNotify will
       * be thrown away; they are not needed by most
       * applications but are sent because ConfigureNotify
       * can't be selected independently */
      break;
   }  /* End switch on event type */
   } /*end if XCheckIfEvent */
} /* end do_Xevent_buffer_c */

Bool predproc_c(display, event, arg)
Display *display;
XEvent *event;
char *arg;
{
        //printf("The arg is %s\n", arg);
        switch (event->type) {
                case Expose:
                case MotionNotify:
                case ConfigureNotify:
                case KeyPress:
                        return(True);
                        break;
                case ButtonPress:
                case ButtonRelease:
                        return(True);
                        break;
                default:
                        return(True);
        }
} /*end predproc_c */

void draw_AST_button_c (int ix, int iy, int b_num)
{


 if (b_num == 1)  //display lat/lon of position
    {
     puck_position_request_c(ix,iy);//cdwmmi.c
    }
 else if (b_num == 2) //find nearest unit
    {
     puck_hook_request(&ix,&iy);//lbplot.rat
    }

 } /*end of draw_AST_button_c*/
void draw_AST_expose_c (void)
{

           if (anevent.xexpose.count == 0)
              {
		if (WinC.dw_live_pixmap!=WinC.dw_window[DW_TACTICAL])
	          {
                   XCopyArea (dw_display,WinC.dw_live_pixmap,
                              WinC.dw_window[DW_TACTICAL],
                              ta_win_GC.dw_GC, 0, 0,
                              WinC.dw_current_w, 
	     	              WinC.dw_current_h, 
			      0, 0);
		   draw_text_done();   // refresh Text window

                  } /*endif live_pixmap!=tactical window*/
              }/*if there is just one event*/
}/*end of draw_AST_expose_c*/ 

void draw_AST_configure (void)
{
 int mask;
 bool found, max_min_error;
 XWindowChanges xwc;

 max_min_error = FALSE; //initialize

 if (WinC.dw_new_w > WinC.dw_maximum_w)
    {//if new width is greater than max, set new width to max
     max_min_error = TRUE;
     WinC.dw_new_w = WinC.dw_maximum_w;
    }
 else if (WinC.dw_new_w < DW_MINIMUM_W)
   {//if new width is less than min, set new width to min
     max_min_error = TRUE; 
     WinC.dw_new_w = DW_MINIMUM_W;
    }

 if (WinC.dw_new_h > WinC.dw_maximum_h)
    {//if the new height is greater than max, set new height to max
     max_min_error = TRUE;
     WinC.dw_new_h = WinC.dw_maximum_h;
    }
 else if (WinC.dw_new_h < DW_MINIMUM_H)
   {//if the new height is less than min, set new height to min
     max_min_error = TRUE;
     WinC.dw_new_h = DW_MINIMUM_H;
    }

 if (max_min_error)
    {
     mask = CWWidth | CWHeight;
     xwc.width  = WinC.dw_new_w;
     xwc.height = WinC.dw_new_h;


     XConfigureWindow (dw_display, WinC.dw_window[DW_TACTICAL],
                       mask, &xwc);
    }//end if there is a min or max error

 mask = CWX | CWY;
 xwc.x = WinC.dw_new_w - DW_INFO_W;
 xwc.y = 0; // DW_New_H - $DW$Info_H - 50

 XConfigureWindow (dw_display, WinC.dw_window[DW_TEXTINFO],
		   mask, &xwc);

 WinC.dw_changed_hw = YES;

} /*end draw_AST_configure */



int draw_arrow_xy_c (/* Display arrow at X,Y*/
                     int x, int y, 
		     int relative_angle,
                     int color)
{
  float line_orientation;
  int ix, iy, my_pixmap; //num_points;
  //XPoint poly_points[MAX_ARROWHEAD_POINTS];

  //struct arrow_desc ac in this file - ac_head_x,y, ac_style,
  //ac_head_length, ac_line_length, ac_thickness, ac_head_offset,
  //ac_tail_offset, ac_tail_x,y

  //struct arrow_pts arr_pts in this file - num_points, pnts array, 
  //tail_pnt

  ix = x;
  iy = y;

  ac.ac_head_x = ix;
  ac.ac_head_y = iy;

  /*direction in radians that the arrow is pointing*/
  line_orientation = DEG2RAD * relative_angle;

  get_arrowhead_points_c (ix,  /* in, plot coord*/
                       iy,  /* in, plot coord*/
                       line_orientation, /* in, radians*/
                       ac.ac_style,      /* in, code*/
                       ac.ac_head_length,/* in, pixels*/
                       ac.ac_line_length,/* in, pixels*/
                       ac.ac_thickness,  /* in, pixels*/
                       //num_points,       /* out, num of points*/
                       //poly_points,      /* out, x,y points*/
                       ac.ac_head_offset,/* in, pixels, dist from pt*/
		       ac.ac_tail_offset);/* in, pixels, dist from tail*/
  // ac.ac_tail_x,y in struct so no need to pass as argument 
		      //ac.ac_tail_x,  /*pixels, tail insert pnt x*/
		      //ac.ac_tail_y); /*pixels, tail insert pnt y*/

    set_color (color);

    if (ac.ac_pixmap == 0) my_pixmap = WinC.dw_live_pixmap;
    else my_pixmap = ac.ac_pixmap;

    XFillPolygon (dw_display, my_pixmap, ta_win_GC.dw_GC,
                  arrow.pnts, arrow.num_points,
                  Nonconvex, CoordModeOrigin);
    XDrawLine(dw_display, my_pixmap, ta_win_GC.dw_GC,
	      ix, iy, ac.ac_tail_x, ac.ac_tail_y);
    //    XDrawPoint (dw_display, my_pixmap, ta_win_GC.dw_GC, 
    //		ac.ac_tail_x, ac.ac_tail_y);

    return (YES);
}/*end of draw_arrow_xy_c*/

void get_arrowhead_points_c(
     int center_x, /* in, plot coord, pixel*/
     int center_y, /* in, plot coord, pixel*/
     float line_orientation,/* in, radians*/
     int arrow_style, /* in, code*/
     int head_length, /* in, pixels*/
     int line_length, /* in, pixels*/
     int line_width, /* in, pixels*/
     //int num_points,--out, number of pnts - in arrow struct
     //xpoints,       --out, x,y pnts         in arrow struct
     int point_offset,/* in, pixels*/
     int tail_offset) /* in, pixels*/
     //int tail_x, --out, pixels - in ac struct
     //int tail_y) --out, pixels - in ac struct
			  
{
 float angle_1;
 int arrow_head_length;
 float H_1;
 int half_line_width;
 int L_1;
 float line_angle;
 int point_x;
 int point_y;
 float radius;
 int sign_flag;
 float W_1;
 int combined_offset;
 int return_x, return_y;

 return_x = 0;
 return_y = 0;
 arrow.num_points = 0;//num_points

 arrow_head_length = head_length;

 /* adjust the start point so that it is not sitting right on 
    top of the center point*/
 point_x = center_x - (int)(point_offset * sin(line_orientation));
 point_y = center_y + (int)(point_offset * cos(line_orientation));

 // get x,y point, using offset from tail
 combined_offset = point_offset + tail_offset + line_length;
 ac.ac_tail_x = center_x-(int)((combined_offset)*sin(line_orientation));
 ac.ac_tail_y = center_y+(int)((combined_offset)*cos(line_orientation));

 half_line_width = (int)(line_width / 2.0);

 // get reciprocal of line orientation
 line_angle = line_orientation + M_PI;

 ang_to_pi(line_angle);

 //XPoints [ 1 ].X$W_GPNT_X = point_x
 //XPoints [ 1 ].X$W_GPNT_Y = point_y
 arrow.pnts[0].x = point_x;
 arrow.pnts[0].y = point_y;

 // if arrow is drawn with seven points
 if (arrow_style == ARROW_2 ||
     arrow_style == ARROW_4 ||
     arrow_style == ARROW_5 )
    {
     arrow.num_points = 7;

    // if wide head arrow
    if (arrow_style == ARROW_2 ||
        line_width >= 8)
        {
	// set the width of the arrow head
	 L_1 = arrow_head_length;
        }
    else //else if narrow head arrow
        {
	arrow_head_length = head_length * 1.6;

        radius = arrow_head_length/cos((M_PI*HALF_ARROW_DEGREES)/180);

        // get the width of the arrow head for narrow arrow head
        L_1 =(int)(sqrt((radius*radius)-(arrow_head_length*arrow_head_length)));
        }

    sign_flag = -1;

    get_points(&arrow_head_length,/*in,length of arrow head,pixels*/
    	       &L_1,  /*in,width of arrow head,pixels*/
               &line_angle, /*in,angle of line*/
               &point_x, /*in, pixel relative position*/
    	       &point_y, /*in, pixel relative position*/
               &sign_flag, /*in, sign indicator*/
               &return_x,/*out*/
               &return_y);/*out*/
    arrow.pnts[1].x = return_x;
    arrow.pnts[1].y = return_y;

    sign_flag = 1;

    get_points(&arrow_head_length,/*in, length in one plane*/
    	       &L_1,/* in, length in perpendicular plane*/
               &line_angle, /*in, angle of line*/
    	       &point_x,/*in, pixel relative position*/
               &point_y,/*in, pixel relative position*/
               &sign_flag,/* in, sign indicator*/
               &return_x,/*out*/
               &return_y);/*out*/
    arrow.pnts[6].x = return_x;
    arrow.pnts[6].y = return_y;

    if (arrow_style == ARROW_5)
        {
	 L_1 = 0.66 * arrow_head_length;
        }
    else
        {
	 L_1 = arrow_head_length;
        }

    sign_flag = -1;

    get_points (&L_1, /* in, length in one plane*/
                &half_line_width,/*in, length in perpendicular plane*/
                &line_angle,/* in, angle of line*/
    	        &point_x, /* in, pixel relative position*/
    		&point_y,/* in, pixel relative position*/
                &sign_flag, /* in, sign indicator*/
                &return_x,/*out*/
                &return_y);/*out*/
    arrow.pnts[2].x = return_x;
    arrow.pnts[2].y = return_y;

    sign_flag = 1;

    get_points (&L_1, /*in, length in one plane*/
                &half_line_width,/*in,length in perpendicular plane*/
                &line_angle,/* in, angle of line*/
                &point_x,/*in, pixel relative position*/
                &point_y,/*in, pixel relative position*/
                &sign_flag,/*in, sign indicator*/
                &return_x,/*out*/
                &return_y);/*out*/
    arrow.pnts[5].x = return_x;
    arrow.pnts[5].y = return_y;

    L_1 = line_length;

    sign_flag = -1;

    get_points(&L_1,/*in, length in one plane*/
               &half_line_width,/*in,length in perpendicular plane*/
               &line_angle,/*in, angle of line*/
               &point_x,/*in, pixel relative position*/
               &point_y,/*in, pixel relative position*/
               &sign_flag,/*in, sign indicator*/
               &return_x,/*out*/
               &return_y);/*out*/
    arrow.pnts[3].x = return_x;
    arrow.pnts[3].y = return_y;

    sign_flag = 1;

    get_points(&L_1, /*in, length in one plane*/
               &half_line_width,/*in,length in perpendicular plane*/
               &line_angle, /*in, angle of line*/
               &point_x,/*in, pixel relative position*/
               &point_y,/*in, pixel relative position*/
               &sign_flag,/*in, sign indicator*/               
               &return_x,/*out*/
               &return_y);/*out*/
    arrow.pnts[4].x = return_x;
    arrow.pnts[4].y = return_y;
 
    } //end if arrow is drawn with seven points
 // else if arrow is drawn with nine points
else if (arrow_style == ARROW_1 ||
         arrow_style == ARROW_3 )
    {
     arrow.num_points = 9;

     // if wide head arrow
     if ( arrow_style == ARROW_1 ||
         line_width >= 8 )
       {
	// set the width of the arrow head
	L_1 = arrow_head_length;
       }
     else // else if narrow head arrow
       {
	arrow_head_length = head_length * 1.6;

	radius = arrow_head_length/cos((M_PI*HALF_ARROW_DEGREES)/180);

	// get the width of the arrow head for narrow arrow head
	L_1 = (int)(sqrt((radius*radius) -
			(arrow_head_length*arrow_head_length )));
       }

     sign_flag = -1;

     get_points(&arrow_head_length,/*in,length of arrow head,pixels*/
                &L_1,/*in, width of arrow head, pixels*/
     	        &line_angle,/*in, angle of line*/
     	        &point_x,/*in, pixel relative position*/
     	        &point_y,/*in, pixel relative position*/
	        &sign_flag,/*in, sign indicator*/
		&return_x,/*out*/
	        &return_y);/*out*/
     arrow.pnts[1].x = return_x;
     arrow.pnts[1].y = return_y;
     //XPoints [ 2 ].X$W_GPNT_X,     # out
     //XPoints [ 2 ].X$W_GPNT_Y)     # out

     sign_flag = 1;

     get_points(&arrow_head_length,/*in, length in one plane*/
     		&L_1,/*in,length in perpendicular plane*/
                &line_angle,/*in, angle of line*/
     		&point_x,/*in, pixel relative position*/
                &point_y,/*in, pixel relative position*/
     		&sign_flag,/*in, sign indicator*/
                &return_x,/*out*/
                &return_y);/*out*/
     arrow.pnts[8].x = return_x;
     arrow.pnts[8].y = return_y;
     //XPoints [ 9 ].X$W_GPNT_X,     # out
     //XPoints [ 9 ].X$W_GPNT_Y)     # out

     if (arrow_style == ARROW_1)
        {
	 angle_1 = line_angle + M_PI/4;
        }
     else if (arrow_style == ARROW_3)
        {
	 angle_1 = line_angle + (M_PI/2)-((M_PI*HALF_ARROW_DEGREES)/180);
        }

     ang_to_pi(angle_1);

     arrow.pnts[2].x = arrow.pnts[1].x+(int)(line_width*sin(angle_1));
/*XPoints [ 3 ].X$W_GPNT_X = XPoints [ 2 ].X$W_GPNT_X +
  NINT(line_width * SIN (angle_1))*/
     arrow.pnts[2].y = arrow.pnts[1].y+(int)(line_width*cos(angle_1));
/*XPoints [ 3 ].X$W_GPNT_Y = XPoints [ 2 ].X$W_GPNT_Y -
  NINT(line_width * COS (angle_1))*/

     if (arrow_style == ARROW_1)
        {
	 angle_1=line_angle - M_PI/4;
        }
     else if (arrow_style == ARROW_3)
        {
	 angle_1=line_angle-((M_PI/2)-((M_PI*HALF_ARROW_DEGREES)/180));
        }

     ang_to_pi(angle_1);

     arrow.pnts[7].x = arrow.pnts[8].x+(int)(line_width*sin(angle_1));
/*XPoints [ 8 ].X$W_GPNT_X = XPoints [ 9 ].X$W_GPNT_X + 
  NINT(line_width * SIN (angle_1))*/
     arrow.pnts[7].y = arrow.pnts[8].y+(int)(line_width*sin(angle_1));
 /*XPoints [ 8 ].X$W_GPNT_Y = XPoints [ 9 ].X$W_GPNT_Y -
   NINT(line_width * COS (angle_1))*/

     if (arrow_style == ARROW_1)
        {
	 angle_1 = M_PI/4;
        }
     else if (arrow_style == ARROW_3)
        {
	 angle_1 = (M_PI*HALF_ARROW_DEGREES)/180.0;
        }

     W_1 = line_width * tan(angle_1);

     H_1 = sqrt((W_1 * W_1) + (line_width * line_width));

     W_1 = line_width / 2.0 + H_1;

     L_1 = (int)(W_1 / tan(angle_1));

     sign_flag = -1;

     get_points(&L_1, /*in, length in one plane*/
               &half_line_width,/*in,length in perpendicular plane*/
               &line_angle, /*in, angle of line*/
    	       &point_x,/*in, pixel relative position*/
               &point_y,/*in, pixel relative position*/
               &sign_flag,/*in, sign indicator*/
               &return_x,/*out*/
               &return_y);/*out*/
     arrow.pnts[3].x = return_x;
     arrow.pnts[3].y = return_y;

     sign_flag = 1;

     get_points(&L_1, /*in, length in one plane*/
               &half_line_width,/*in,length in perpendicular plane*/
               &line_angle, /*in, angle of line*/
    	       &point_x,/*in, pixel relative position*/
               &point_y,/*in, pixel relative position*/
               &sign_flag,/*in, sign indicator*/
               &return_x,/*out*/
               &return_y);/*out*/
     arrow.pnts[6].x = return_x;
     arrow.pnts[6].y = return_y;

     L_1 = line_length;

     sign_flag = -1;

     get_points(&L_1, /*in, length in one plane*/
               &half_line_width,/*in,length in perpendicular plane*/
    	       &line_angle,/*in, angle of line*/
               &point_x, /*in, pixel relative position*/
	       &point_y, /*in, pixel relative position*/
    	       &sign_flag, /*in, sign indicator*/
               &return_x,/*out*/
	       &return_y);/*out*/
     arrow.pnts[4].x = return_x;
     arrow.pnts[4].y = return_y;

     sign_flag = 1;

     get_points(&L_1,/*in, length in one plane*/
               &half_line_width,/*in,length in perpendicular plane*/
               &line_angle,/*in, angle of line*/
               &point_x, /*in, pixel relative position*/
    	       &point_y,/*in, pixel relative position*/
               &sign_flag,/*in, sign indicator*/
               &return_x,/*out*/
               &return_y);/*out*/
     arrow.pnts[5].x = return_x;
     arrow.pnts[5].y = return_y;
    }//end if arrow drawn with 9 points 

}/*end of get_arrowhead_points_c*/
