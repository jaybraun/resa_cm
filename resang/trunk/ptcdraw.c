/*#####FILE:      ptcdraw.c
/* compile using "gcc -g -c ptcdraw.c"
/* link using  "ptmain.lnk"
/* run using "ptmain.run"

# Key functions within this file:
#
# draw_init_c    Initialize Megatek MEGX
# draw_center_c  Define center LAT/LON/MAP/RADIUS/TIME
# draw_done      Indicate end of cycle
# draw_circle_c  Display Circle at LAT/LON/RADIUS/COLOR
# draw_xmark_c   Display Xmark at LAT/LON/COLOR
# draw_ntds_c    Display NTDS symbol at LAT/LON/COLOR/NAME1/NAME2
# draw_UNIT_c    Display NTDS symbol at ID/LAT/LON/NAME1/NAME2/COLOR/
#                COURSE/SPEED/TYPE/x1/x2/x3/INFO
# draw_line_c    Draw a line from LAT1/LON1 to LAT2/LON2 in COLOR
# draw_bearing_c Draw a line from ID/LAT/LON thru ID2/BEARING/NAME1
#                in COLOR
# draw_vector_c  Draw a speed vector from ID/LAT/LON for 
#                LENGTH/BEARING
# draw_text_c    Draw a LINE of TEXT (in COLOR) at lower right of 
#                display (lines are numbered from bottom-up)
# arc_c          Draws an arc of the given circle definition
#
# Lat/Lon arguments are in REAL*4 (float) Degrees(Deg <==> Radians*(180/M_PI))
#
#__________________________________________________________
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
//#include <X11/Xlib.h> -- included by ptdraw.h
//#include <X11/Xutil.h>
#include <X11/Xatom.h>
//#include <X11/Xos.h>
#include <X11/cursorfont.h>
//#include <X11/Xcursor/Xcursor.h>
//#include <X11/Xproto.h>

/*the following 4 definitions are also defined in ptmain.inc */
#define MAP_RIVERS 1
#define MAP_BOUNDARIES 2
#define MAP_TEXTSIZE 3
#define MAP_TERRAIN 4

#define TWOPI M_PI*2.0

#define EXCLAIM 33 /*double check this value for LINUX*/

#define NSIDES 72 /*used in routine arc*/

#include "ptcolor.h"
#include "ptgraph.h" 
#include "ptwand.h" /*false,true*/
#include "ptvext.h" /*contains EXT_ definition */
#include "ptdraw.h"  /*contains global variables*/

/*extern EXT_common EXT_;*/ /* EXT_common defined in ptvext.h */
extern WinCommon WinC; 
extern WinCommonSWDA ta_win_swda; 
extern WinCommonGC ta_win_GC; 
extern WinCommonPoints WinComPts; 
       GPH_common gph; /* ptgraph.h */
extern GPH2_common gph2;  /* ptgraph.h */
extern NTDSCommon NTDSsymDef; /*Ntds_common - dwdraw.h */
extern WinCommonPoints ar_pts; /*get_arrow_points - dwdraw.h*/

int mm_wide;

extern void rrb2ll();
extern void clear_hooks();
extern void puck_hook_request();
extern void add_hook();
extern void add_hook_box();
extern void add_hook_line();
extern void tb_find_lines();
extern void tb_find_corners();
extern void puck_position_request_c(int,int);
extern void check_for_user_input_c(); 

extern int clpmsk(float,float);
extern int inbox(float, float, float, float);
extern int inpnt(short int, short int);
extern void linedg(int,int,int,int,int *,int *);
/*md*/
extern int mx(float);
extern int my(float);
extern void proj(int);
extern void viewpt (short int,short int,short int,short int);
extern void window (float,float,float,float);
extern void winview(void);

/* following routines found in cdwdraw.c */
int draw_init_c(char *,int *,char *,int *); 
void setcolors(void);
void revisecolors(void);
void set_color_wrap(int *);
void set_color (int);
void set_background (int);
void setfonts(void);
void set_font (int *);
void set_dash_c (int *);
void symopn(void); /* open file ptsymbols.dat */
void symbol(int, int, int);
int draw_center_c(float *,float *,int *,int *,char *);
void bndry(float,float,float/*,float,float,float,float*/);
void map(int *);
void skip_map(void);
void grid(void);
void draw_done(void);
int draw_circle_c(float *,float *,short int *,int *, int *);
int draw_xmark_c(float *, float *, int *);
int draw_ntds_c(int *,float *,float *,int *,int *,char *,char *);
int get_font_height(int*);
int get_font_width(int*);
void draw_string(int *, int *, char *);
void draw_line(int *, int *, int *, int *);
int draw_vector_c(int *,float *,float *,float *,float *);
int draw_line_c(float *,float *,float *,float *,int *);
int draw_bearing_c(int *,float *,float *,int *,float *,char *,
		 int *,int *);
void draw_text_c (int *, char *, int *);/* partially done*/
void draw_text_done(void);
int draw_polygon_c(int *,float *,float *,int *,int *,int *);
int draw_unit_c(int *, int *, float *, float *, int*,char *, char *,int *, 
	      float *, int *, int *, int *, int *, int *, int *);
void draw_textblock_init_c(char *,int);
int draw_textblock_c(int,int,int,char *,int,int,int,int,int,int);
void tb_find_lines_c();//using tb_find_lines in dwdraw.rat
void tb_find_corners_c(int, int, int, int, int, int *, int *, int *,int *);
void resize_dw_pixmaps(int,int);
int draw_nrt_n(float, float, int, int);
int draw_local_l(float, float, int);
int alloc_color_name(Display *,Screen *,Visual *,
	    int, char color_name[], int rgb[]);
void arc_c(short int,short int,short int,float,float);
void x_draw_lines();
void ang_to_pi(float);

/* Initialize X-Window, called by a fortran routine*/
int draw_init_c(char meg_name[],
	       int *ef_num,
	       char window_name[],
	       int *ista) 
{

 unsigned long display_background, display_border;
 int window_x, window_y, window_h, window_w;
 int tparam;
 Window win;
 XEvent ev;
 char *display_name = NULL;
 float rparam;
 Display *temp_display;
 int n, yesno;
 float inches_wide;

 WinC.dw_wakeup_ef = *ef_num;     /* GLOBALS DEFINED IN dwdraw.h */ 
 WinC.dw_window_name_len = strlen(window_name);
 strncpy(WinC.dw_window_name, window_name, WinC.dw_window_name_len);
 temp_display = dw_display;

 /* Initialize display id by opening a connection*/
 /* swapped window_name with NULL */
 if ((dw_display = XOpenDisplay(NULL)) == NULL) 
   {
    (void) fprintf(stderr, "draw_init_c: cannot connect to X server %s\n",XDisplayName(display_name));
    exit(-1);
   }

 /* return the default screen structure of display opened above */
 dw_screen = DefaultScreenOfDisplay(dw_display);
 /* return the int value of display opened above */
 thescreen = DefaultScreen(dw_display);  

 /* get the width, height, depth and width in mm of default screen*/
 WinC.dw_screen_w = WidthOfScreen(dw_screen)-60;//was 0         
 WinC.dw_screen_h = HeightOfScreen(dw_screen)-100;//was 40 
 WinC.dw_depth = DefaultDepth(dw_display,thescreen);    
 mm_wide = WidthMMOfScreen(dw_screen); 

 if (WinC.dw_screen_w == 1280) mm_wide = 343;

 inches_wide = mm_wide/25.4;
 WinC.dw_pix_per_inch = WinC.dw_screen_w / inches_wide;
  
 dw_visual = DefaultVisualOfScreen (dw_screen);
 
 thecolormap = DefaultColormap(dw_display,thescreen);
 
 setcolors();  /* Set VLT to custom colors */
 if (dw_visual->class != TrueColor)
   revisecolors();  /* Revise VLT to custom colors*/

 WinC.dw_inverse = NO;          /* Draw Black on White */
 /* Set to custom fonts- check to see what is on the system by
 using Linux command xlsfonts then make changes in dwdraw.h*/
 setfonts();         /* Set to custom fonts */

 /* Load special Cursors */
 WinC.dw_cursor[0]=XCreateFontCursor(dw_display,DW_CURSOR0_NAME);
 WinC.dw_cursor[1]=XCreateFontCursor(dw_display,DW_CURSOR1_NAME);
 WinC.dw_cursor[2]=XCreateFontCursor(dw_display,DW_CURSOR2_NAME);

 /* Set window attributes and open Primary Tactical Window */ 
 ta_win_swda.w_swda_mask = CWEventMask   | \
                           CWBackPixel   | \
                           CWBorderPixel | \
                           CWCursor      | \
                           CWBitGravity;
 
 ta_win_swda.w_swda.bit_gravity = NorthWestGravity; /*(0,0)default*/
 
 ta_win_swda.w_swda.event_mask  = ExposureMask | \
                                  ButtonPressMask | \
                                  StructureNotifyMask;
 
 ta_win_swda.w_swda.background_pixel = WinC.dw_color[0][COLOR_BACKGROUND];
 ta_win_swda.w_swda.border_pixel = WinC.dw_color[0][COLOR_BORDER];

 ta_win_swda.w_swda.cursor = DW_CURSOR_CROSSHAIR;

 if (DW_MAX_W > WinC.dw_screen_w - 4) 
   WinC.dw_maximum_w = WinC.dw_screen_w - 4;
 else WinC.dw_maximum_w = DW_MAX_W;
 if (DW_MAX_H > WinC.dw_screen_h - 24)
   WinC.dw_maximum_h = WinC.dw_screen_h - 24;
 else WinC.dw_maximum_h = DW_MAX_H;

 window_x = 0;
 window_y = 0;
 
 if (WinC.dw_screen_w > 2 * WinC.dw_screen_h)  /* DEW 1/92 */
    {
     window_w = DW_DEFAULT_W;/*In the case of very wide screens.*/
     window_h = DW_DEFAULT_H;/* use default W/H */
    }
 else
    {
     window_w = WinC.dw_maximum_w;/* Or else use the Max */
     window_h = WinC.dw_maximum_h;
    }
 window_w = window_w-40;/*9/4/14*/
 window_h = window_h-60;

 WinC.dw_changed_hw = YES;
 WinC.dw_new_w = window_w;
 WinC.dw_new_h = window_h;

 WinC.dw_current_x = window_x;
 WinC.dw_current_y = window_y;
 WinC.dw_current_w = window_w;
 WinC.dw_current_h = window_h;

 WinC.dw_window[DW_TACTICAL] = XCreateWindow(dw_display,
                        RootWindowOfScreen(dw_screen),
			window_x, window_y, 
			window_w, window_h, 
			4, WinC.dw_depth, 
			InputOutput, dw_visual,
			ta_win_swda.w_swda_mask, 
			&ta_win_swda.w_swda);
  
 XStoreName(dw_display, WinC.dw_window[DW_TACTICAL],
            WinC.dw_window_name);
 
 /* Create graphics context, and clear the window */
 ta_win_GC.GC_values.foreground = WinC.dw_color[0][COLOR_TEXT];
 ta_win_GC.GC_values.background = WinC.dw_color[0][COLOR_BACKGROUND];
 ta_win_GC.GC_values.font = dw_font[WinC.dw_tactical_font];
 
 ta_win_GC.dw_GC_mask = GCForeground | \
                        GCBackground | \
                        GCFont;

 ta_win_GC.dw_GC = XCreateGC(dw_display, 
			    WinC.dw_window[DW_TACTICAL],
			    ta_win_GC.dw_GC_mask, 
                            &ta_win_GC.GC_values);

for (n = 0; n < DW_PIXMAPS; n++) 
   {
//    if (n > 0) 
//        WinC.dw_pixmap[n] = WinC.dw_pixmap[0];
//    else 
        WinC.dw_pixmap[n] = XCreatePixmap (dw_display, 
                                       WinC.dw_window[DW_TACTICAL],
                                       WinC.dw_current_w, 
                                       WinC.dw_current_h, 
                                       WinC.dw_depth);
   } /* end of loop thru DW_PIXMAPS*/

 WinC.dw_map_pixmap  = WinC.dw_pixmap[0];
 WinC.dw_live_pixmap = WinC.dw_pixmap[1];
 WinC.dw_next_pixmap = WinC.dw_pixmap[1];

 yesno = NO;
 if (yesno == YES)      /* Temp for debug... */
    {
     WinC.dw_map_pixmap  = WinC.dw_window[DW_TACTICAL];
     WinC.dw_live_pixmap = WinC.dw_window[DW_TACTICAL];
     WinC.dw_next_pixmap = WinC.dw_window[DW_TACTICAL];
    }

/* Need to set up AST handlers for assorted events*/
 XSelectInput(dw_display,WinC.dw_window[DW_TACTICAL],
              ButtonPress | ExposureMask | StructureNotifyMask);
 do_Xevent_buffer_c(); /*check for events*/

 XMapWindow(dw_display, WinC.dw_window[DW_TACTICAL]);

 /* open Text Information Window */   
 ta_win_swda.w_swda_mask = CWEventMask   | \
                           CWBackPixel   | \
                           CWCursor      | \
                           CWBorderPixel | \
                           CWBitGravity;
	       
 ta_win_swda.w_swda.bit_gravity = NorthWestGravity;

 ta_win_swda.w_swda.event_mask  = ExposureMask | \
                                  ButtonPressMask;
 
 ta_win_swda.w_swda.background_pixel = WinC.dw_color[0][COLOR_BACKGROUND];
 ta_win_swda.w_swda.border_pixel = WinC.dw_color[0][COLOR_BORDER];
 ta_win_swda.w_swda.cursor = DW_CURSOR_ARROW;
 
 window_w = DW_INFO_W;
 window_h = DW_INFO_H;
 window_x = (WinC.dw_current_x + WinC.dw_current_w)-window_w;//8/26
 window_y = WinC.dw_current_y; //try this 8/26

 strcpy(window_name,"TEXT INFO WINDOW \n");
 WinC.dw_window_name_len = strlen(window_name);
 strncpy(WinC.dw_window_name, window_name, WinC.dw_window_name_len);

 WinC.dw_window[DW_TEXTINFO] = XCreateWindow(dw_display,
                            WinC.dw_window[DW_TACTICAL],
		            window_x, window_y, 
                            window_w, window_h,
			    4, WinC.dw_depth, 
			    InputOutput, dw_visual,
                            ta_win_swda.w_swda_mask, 
			    &ta_win_swda.w_swda);
 
 XMapWindow(dw_display, WinC.dw_window[DW_TEXTINFO]);
 XFlush(dw_display);
symopn ();   /* Build NTDS symbols */
 proj(MERCATOR);  /* Mercator projection */

 window(-180.,-75.,180.,75.); /* Whole world */
/* WINDOW is re-called for each center point or radius change*/

/*         (lower left)    (upper right)*/
 viewpt (0, WinC.dw_current_h, WinC.dw_current_w, 0);
/*VIEWPT should be re-called for each change in dimens of the window*/

return (YES);  /* end of draw_init_c */
}

void setcolors(void)   /* Set VLT to custom colors*/
{

 int   r, g, b, n, kk, length, len, rgb[3];
 char  temp[16], wh_colors[16][16], bl_colors[16][16];
 XColor customcolor;
 r = 0;
 g = 0;
 b = 0;
 /* WinCommon WinC;*/

 /* load inverse set of colors into wh_colors array*/
 len = strlen(INVCOLOR1_NAME);
 snprintf(wh_colors[1],len+1,"%s",INVCOLOR1_NAME);/*White*/

 len = strlen(INVCOLOR2_NAME);
 snprintf(wh_colors[2],len+1,"%s",INVCOLOR2_NAME);/*Coral*/

 len = strlen(INVCOLOR3_NAME);
 snprintf(wh_colors[3],len+1,"%s",INVCOLOR3_NAME);/*Grey6*/

 len = strlen(INVCOLOR4_NAME);
 snprintf(wh_colors[4],len+1,"%s",INVCOLOR4_NAME);/*Magenta*/

 len = strlen(INVCOLOR5_NAME);
 snprintf(wh_colors[5],len+1,"%s",INVCOLOR5_NAME);/*LightGrey*/

 len = strlen(INVCOLOR6_NAME);
 snprintf(wh_colors[6],len+1,"%s",INVCOLOR6_NAME);/*LimeGreen*/

 len = strlen(INVCOLOR7_NAME);
 snprintf(wh_colors[7],len+1,"%s",INVCOLOR7_NAME);/*NavyBlue*/

 len = strlen(INVCOLOR8_NAME);
 snprintf(wh_colors[8],len+1,"%s",INVCOLOR8_NAME);/*DarkGreen*/

 len = strlen(INVCOLOR9_NAME);
 snprintf(wh_colors[9],len+1,"%s",INVCOLOR9_NAME);/*DarkSlateBlue*/

 len = strlen(INVCOLOR10_NAME);
 snprintf(wh_colors[10],len+1,"%s",INVCOLOR10_NAME);/*blue*/

 len = strlen(INVCOLOR11_NAME);
 snprintf(wh_colors[11],len+1,"%s",INVCOLOR11_NAME);/*DarkOrchid*/

 len = strlen(INVCOLOR12_NAME);
 snprintf(wh_colors[12],len+1,"%s",INVCOLOR12_NAME);/*orchid*/

 len = strlen(INVCOLOR13_NAME);
 snprintf(wh_colors[13],len+1,"%s",INVCOLOR13_NAME);/*Black*/

 len = strlen(INVCOLOR14_NAME);
 snprintf(wh_colors[14],len+1,"%s",INVCOLOR14_NAME);/*OrangeRed*/

 len = strlen(INVCOLOR15_NAME);
 snprintf(wh_colors[15],len+1,"%s",INVCOLOR15_NAME);/*VioletRed*/

 len = strlen(INVCOLOR16_NAME);
 snprintf(wh_colors[16],len+1,"%s",INVCOLOR16_NAME);/*grey48*/

 /* load color names into bl_colors array */
 len = strlen(COLOR1_NAME);
 snprintf(bl_colors[1],len+1,"%s",COLOR1_NAME);/*grey12*/

 len = strlen(COLOR2_NAME);
 snprintf(bl_colors[2],len+1,"%s",COLOR2_NAME);/*Coral*/

 len = strlen(COLOR3_NAME);
 snprintf(bl_colors[3],len+1,"%s",COLOR3_NAME);/*sandybrown*/

 len = strlen(COLOR4_NAME);
 snprintf(bl_colors[4],len+1,"%s",COLOR4_NAME);/*yellow*/

 len = strlen(COLOR5_NAME);
 snprintf(bl_colors[5],len+1,"%s",COLOR5_NAME);/*LightGrey*/

 len = strlen(COLOR6_NAME);
 snprintf(bl_colors[6],len+1,"%s",COLOR6_NAME);/*LimeGreen*/

 len = strlen(COLOR7_NAME);
 snprintf(bl_colors[7],len+1,"%s",COLOR7_NAME);/*NavyBlue*/

 len = strlen(COLOR8_NAME);
 snprintf(bl_colors[8],len+1,"%s",COLOR8_NAME);/*khaki*/

 len = strlen(COLOR9_NAME);
 snprintf(bl_colors[9],len+1,"%s",COLOR9_NAME);/*DECWBlue*/

 len = strlen(COLOR10_NAME);
 snprintf(bl_colors[10],len+1,"%s",COLOR10_NAME);/*cyan*/

 len = strlen(COLOR11_NAME);
 snprintf(bl_colors[11],len+1,"%s",COLOR11_NAME);/*VioletRed*/

 len = strlen(COLOR12_NAME);
 snprintf(bl_colors[12],len+1,"%s",COLOR12_NAME);/*Magenta*/

 len = strlen(COLOR13_NAME);
 snprintf(bl_colors[13],len+1,"%s",COLOR13_NAME);/*White*/

 len = strlen(COLOR14_NAME);
 snprintf(bl_colors[14],len+1,"%s",COLOR14_NAME);/*Red*/

 len = strlen(COLOR15_NAME);
 snprintf(bl_colors[15],len+1,"%s",COLOR15_NAME);/*firebrick*/

 len = strlen(COLOR16_NAME);
 snprintf(bl_colors[16],len+1,"%s",COLOR16_NAME);/*Wheat*/

/*#-------------------------------------------------------------------
  # Normal set (Black Background)*/
for (n = 1; n <= 16; n = n + 1) {

    strcpy(temp, bl_colors[n]);
   /* send the color name in temp and return window red,green,blue*/
    WinC.dw_color[0][n] = alloc_color_name(dw_display,
                                           dw_screen,
                                           dw_visual,
                                           n, temp, rgb);
    WinC.dw_rgb[0][n][0] = rgb[0];
    WinC.dw_rgb[0][n][1] = rgb[1];
    WinC.dw_rgb[0][n][2] = rgb[2];
   } /* end for loop through bl_colors */

/*#-----------------------------------------------------------------
  # Inverse set (White Background)  */
 
 for (n = 1; n <= 16; n = n + 1) {

    strcpy(temp,wh_colors[n]);

    WinC.dw_color[1][n] = alloc_color_name(dw_display,
                                           dw_screen,
					   dw_visual,
                                           n, temp, rgb);
    WinC.dw_rgb[1][n][0] = rgb[0];
    WinC.dw_rgb[1][n][1] = rgb[1];
    WinC.dw_rgb[1][n][2] = rgb[2]; 
   } /* for loop */
} /* end of setcolors */


void revisecolors(void)       /*Revise VLT to custom colors*/
{
  int set_num, index, color_num;
  int beg_index, end_index, color_name_len;
  int n_red, n_green, n_blue, n_rgb[3];
  char *line;
  char *line_ptr;
  char command;
  char color_name[16];
  size_t len=76;
  ssize_t length_of_line;
  FILE *file_pointer;

  line = (char *)malloc(len+1);
  file_pointer = fopen("xcolors.dat","r");
  if (file_pointer == NULL) {
    printf("Couldn't find or open xcolors.dat for reading.\n");
    exit(0);
  }

  /* read a line from the file*/
  while ((length_of_line = getline(&line,&len,file_pointer)) != -1) {
    index=0;
    color_name[0]='\0';
   
    /* inverse,0,white          # White (background)*/
    /* normal,1,Black           # Black (background)*/
    command = line[index];
 /* what color set is it, inverse or normal? */
    switch(command) {
     case (('i') | ('I')) : 
         set_num = 1;
         index = index + 7;
         break;
    case (('n') | ('N')): 
         set_num = 0;
	 index = index + 6;
         break;
      default:
         break;
    } /* end of switch */
    command = line[index];
    if (command == ','){ 
       index++;
      }
/* get the color number */
    line_ptr = &line[index];
    color_num = strtol(line_ptr,&line_ptr,10);
/* the color_number has to be between 1 and 16 */
    if (color_num >= 0 || color_num <= 15) {
	//printf ("The color_number is %i.\n",color_num);
       }  /*color_number is between 0 and 15 */
    /* the color_number is wrong*/
    else {
	printf ("Bad color_number: %i.\n",color_num);
        break; /* skip over this one */
       }  /*color_number is outside the range */
/* get the color name (trim preceeding/trailing blanks)*/
    index++;
    command = line[index];
    /* this should be the comma before the color name */
    if (command == ','){
       index++;   
       command = line[index]; /* first letter of color name */
       beg_index = index;     /* save the beg index */
       while (command != ' '){ /* inc index until blank */
          index++;
          command = line[index];
          }
       end_index = index;
       }
    color_name_len = (end_index-beg_index);
    strncpy(color_name,&line[beg_index],color_name_len);
    WinC.dw_color[set_num][color_num]=alloc_color_name(
			 dw_display,
			 dw_screen, 
			 dw_visual,
                         color_num, color_name,
                         n_rgb);

    /*WinC.dw_color[set_num] [color_num] = N;*/
    WinC.dw_rgb[set_num][color_num][0] = n_rgb[0];
    WinC.dw_rgb[set_num][color_num][1] = n_rgb[1];
    WinC.dw_rgb[set_num][color_num][2] = n_rgb[2];
    } 
  free(line);
} /* end of revisecolors */

void set_color_wrap (int *color)
{
	int icolor;
	icolor = *color;
	set_color (icolor);
}
void set_color (int icolor)
{
  Display *display_info;
  GC tact_win_GC;
  unsigned long foreground_color;

  display_info = dw_display;
  tact_win_GC = ta_win_GC.dw_GC;
  foreground_color = WinC.dw_color[0][icolor];

  /* only change the foreground if it is different */
  if (ta_win_GC.GC_values.foreground != WinC.dw_color[0][icolor])
    {
     ta_win_GC.GC_values.foreground = WinC.dw_color[0][icolor];

     XSetForeground(dw_display,ta_win_GC.dw_GC, 
                    WinC.dw_color[0][icolor]);
    }
}/* end of set_color */

void set_background (int icolor) 
{

  ta_win_GC.GC_values.background = WinC.dw_color[0][icolor];

  XSetBackground(dw_display, ta_win_GC.dw_GC, 
                 ta_win_GC.GC_values.background);
} /* end of set_background */
 
void setfonts(void)
{  

 XFontStruct *font_struct = 0; 
 Display *temp_display;
 /*WinCommon WinC;*/
 Font temp_font;/*[DW_FONTS+1];*/
 char font_names[DW_FONTS][ARB];
 char *test_font_name;
 unsigned long value=0;
 bool status=false;
 int n, len;

 temp_display = dw_display;

 /* load the font names into array*/
 len = strlen(DW_FONT0_NAME);
 /*printf("font name len is %i\n",len);*/
 snprintf(font_names[0],len+1,"%s",DW_FONT0_NAME);

 len = strlen(DW_FONT1_NAME);
 /*printf("font name len is %i\n",len);*/
 snprintf(font_names[1],len+1,"%s",DW_FONT1_NAME);

 len = strlen(DW_FONT2_NAME);
 /*printf("font name len is %i\n",len);*/
 snprintf(font_names[2],len+1,"%s",DW_FONT2_NAME);

 len = strlen(DW_FONT3_NAME);
 snprintf(font_names[3],len+1,"%s",DW_FONT3_NAME);
   /*printf("font name is %s \n",font_names[3]);*/

 len = strlen(DW_FONT4_NAME);
 /*printf("font name len is %i\n",len);*/
 snprintf(font_names[4],len+1,"%s",DW_FONT4_NAME);

 len = strlen(DW_FONT5_NAME);
 /*printf("font name len is %i\n",len);*/
 snprintf(font_names[5],len+1,"%s",DW_FONT5_NAME);
 
 for (n=0; n < DW_FONTS; n=n+1) /*load w_font array*/
   { 
    dw_font[n] = XLoadFont(dw_display,font_names[n]);
    /* dw_font[n] = temp_font;*/
    font_struct = 0;
    font_struct = XQueryFont (dw_display, dw_font[n]);
 
    if (font_struct == NULL)
      printf(" Cannot open font %s \n",font_names[n]);
    /* else  
       //printf (" We have a winner %s !!\n",font_names[n]);*/
   
    WinC.dw_font_h[n] = 0;
    WinC.dw_font_w[n] = 0;

    status = XGetFontProperty(font_struct,XA_CAP_HEIGHT,&value);
    if (status == true)
      {
       WinC.dw_font_h[n]=value;
       /*  printf (" Value of XA_CAP_HEIGHT %d\n",WinC.dw_font_h[n]);*/
      }
    else printf (" Can't get XA_CAP_HEIGHT index value\n ");
    status = false;

    status = XGetFontProperty(font_struct,XA_QUAD_WIDTH,&value);
    if (status == true)
      {
       WinC.dw_font_w[n] = value;
       /* printf (" Value of XA_QUAD_WIDTH %i\n",WinC.dw_font_w[n]);*/
      }
    else printf (" Can't get XA_QUAD_WIDTH index value \n");
    if (WinC.dw_font_h[n] == 0)
      { /* Fix problem when window manager fails # E2142 DEW 9/99 */
       if     (n == DW_SMALL_TACTICAL_FONT) WinC.dw_font_h[n] = 9;
       else if (n == DW_MEDIUM_TACTICAL_FONT)WinC.dw_font_h[n]=11;
       else if (n == DW_BIG_TACTICAL_FONT)   WinC.dw_font_h[n]=14; 
       else                                  WinC.dw_font_h[n]= 6;
      }

    if (WinC.dw_font_w[n] == 0)
      {/* round up to the nearest integer */
	WinC.dw_font_w[n] = (int)((WinC.dw_font_h[n] * 0.8) + 0.5);   
      }
   } /*end of for loop through fonts */
 
 WinC.dw_tactical_font = DW_BIG_TACTICAL_FONT;
 WinC.dw_info_font     = DW_INFO_FONT;
 } /* end of setfonts */
 

void set_font (int *afont)
{
  Display *display_info;
  GC tact_win_GC;
  Font incoming_font;

  display_info = dw_display;
  tact_win_GC = ta_win_GC.dw_GC;
  incoming_font = dw_font[*afont];

 XSetFont(dw_display, ta_win_GC.dw_GC, dw_font[*afont]);

} /*end of set_font */


void set_dash_c (int *flagp)
{
  int dash_len, flag;
unsigned char dash_list[6];

 flag = *flagp;

if (WinC.dw_dashed_line != flag)
   {
    WinC.dw_dashed_line = flag;

    if (flag == 0)
       {
	dash_list[0] = 10;
	dash_len = 1;

        XSetLineAttributes (dw_display, ta_win_GC.dw_GC, 0,
			    LineSolid, CapRound, JoinRound);
       }
    else if (flag > 0)
       {
	dash_list[0] = 8 * abs(flag);
	dash_list[1] = 4 * abs(flag);
	dash_list[2] = 2;
	dash_list[3] = 4 * abs(flag);
	dash_len = 4;

        XSetDashes (dw_display, ta_win_GC.dw_GC, 0, 
                    dash_list, dash_len);
        XSetLineAttributes (dw_display, ta_win_GC.dw_GC, 0,
			    LineDoubleDash, CapRound, JoinRound);
       }
    else /* (flag <  0)*/
       {
        dash_list[0] = abs(flag);
        dash_list[1] = abs(flag);
        dash_len = 2;

        XSetDashes (dw_display,ta_win_GC.dw_GC, 0, 
                    dash_list, dash_len);
        XSetLineAttributes (dw_display, ta_win_GC.dw_GC, 0,
			    LineDoubleDash, CapRound, JoinRound);
       }
    }
} /* end of set_dash_c */

void symopn(void) /* only called to open ptsymbols.dat */
{

  long int dx, dy;
  /*  NTDSCommon NTDSsymDef;*/
  char *line;
  char *line_ptr;
  char command;
  size_t len=76;
  ssize_t length_of_line;
  FILE *file_pointer;
  int comment_only;
/*WinCommon not sure what these are used for yet*/

  line = (char *)malloc(len+1);

  file_pointer = fopen("ptsymbols.dat","r");
  if (file_pointer == NULL) {
    printf("Couldn't find or open ptsymbols.dat for reading.\n");
    exit(0);
  }

  NTDSsymDef.ntds_count = 1;
  NTDSsymDef.ntds_used  = 1;
  NTDSsymDef.ntds_syms[NTDSsymDef.ntds_count] = NTDSsymDef.ntds_used;

  /* get the character set */
  while ((length_of_line = getline(&line,&len,file_pointer)) != -1) {
    comment_only = NO;
    command = line[0];
    switch(command) {
    case 'C':  /* beginning of symbol definition */
           comment_only = YES;
	   /*break out of switch/go to next line in file*/
	   break;
    case 'E': /* end */
           dx = 0;
	   dy = 0;
	   /*printf(" X & Y = %ld & %ld\n",dx,dy);*/
	   break;
   case 'D': /* draw */
	   line_ptr = &line[2];
	   dx = strtol(line_ptr,&line_ptr,10);
	   dy = (strtol(line_ptr,&line_ptr,10))*-1;
	   /*printf(" X & Y = %ld & %ld\n",dx,dy);*/
	   break;
    case 'M': /* move */
	   line_ptr = &line[2];
	   dx = strtol(line_ptr,&line_ptr,10);
	   dy = (strtol(line_ptr,&line_ptr,10))*-1;
	   /*printf(" X & Y = %ld & %ld\n",dx,dy);*/
	   break;
    } /* end switch through possible commands */

    if (comment_only == NO)
      {
       NTDSsymDef.ntds_x[NTDSsymDef.ntds_used] = dx;
       NTDSsymDef.ntds_y[NTDSsymDef.ntds_used] = dy;
       NTDSsymDef.ntds_pen[NTDSsymDef.ntds_used] = command;

       NTDSsymDef.ntds_used++;

       if (command == 'E')
         {
          NTDSsymDef.ntds_count++;
          NTDSsymDef.ntds_syms[NTDSsymDef.ntds_count] = NTDSsymDef.ntds_used;
         }
       if (NTDSsymDef.ntds_used >= NTDS_MAX_POINTS) break;
      } /* end if not a comment line */
  } /* end of while loop through file */

  NTDSsymDef.ntds_x  [NTDSsymDef.ntds_used] = 0;
  NTDSsymDef.ntds_y  [NTDSsymDef.ntds_used] = 0;
  NTDSsymDef.ntds_pen[NTDSsymDef.ntds_used] = 'E';

  free(line);
} /* end of symopn */


void symbol (int isymb,int ix,int iy)
{
 int LETE = 69;
 int LETM = 77;
 int dx, dy, cx, cy, ipnt, single_point;

 cx = ix;  /* use these for keeping absolute X/Y positions */
 cy = iy;

 WinComPts.dw_pnt_count = 1;
 WinComPts.dw_pnt[WinComPts.dw_pnt_count-1].x = cx;
 WinComPts.dw_pnt[WinComPts.dw_pnt_count-1].y = cy;

 for (ipnt = NTDSsymDef.ntds_syms[isymb]; 
      NTDSsymDef.ntds_pen[ipnt] != LETE; 
      ipnt=ipnt+1)
   {
    single_point = NO;
    dx = NTDSsymDef.ntds_x[ipnt];
    dy = NTDSsymDef.ntds_y[ipnt];

    if (dx == 0 & dy == 0)   /*draw a single point*/
       {
	XDrawPoint (dw_display, 
		    WinC.dw_live_pixmap,
		    /*WinC.dw_window[DW_TACTICAL],*/ 
		    ta_win_GC.dw_GC, 
		    cx, cy);
	/*XFlush(dw_display);*//* put this here to test SMM */
        single_point = YES;
       }
    
    if (single_point != YES)
      {
    /* display symbology at 1/4 inch across (ie 80 units per 1/4 inch)
     *                                      (  320 per inch )
     */
	cx = cx + (int)(dx / 5.0);
	cy = cy + (int)(dy / 5.0);

       if (NTDSsymDef.ntds_pen[ipnt] == LETM)
         {
          if (WinComPts.dw_pnt_count > 1)
            {
	     XDrawLines (dw_display,
			 WinC.dw_live_pixmap,
			 /* WinC.dw_window[DW_TACTICAL],*/ 
			  ta_win_GC.dw_GC,
                          WinComPts.dw_pnt, 
			  WinComPts.dw_pnt_count, 
			  CoordModeOrigin);
	     /*XFlush(dw_display);*//* put this here to test SMM */
            }
	  
          WinComPts.dw_pnt_count = 1;
          WinComPts.dw_pnt[WinComPts.dw_pnt_count-1].x = cx;
          WinComPts.dw_pnt[WinComPts.dw_pnt_count-1].y = cy;
         }
       else
         {
          WinComPts.dw_pnt_count = WinComPts.dw_pnt_count + 1;
          WinComPts.dw_pnt[WinComPts.dw_pnt_count-1].x = cx;
          WinComPts.dw_pnt[WinComPts.dw_pnt_count-1].y = cy;
         }
      }/* end if not a single point */
   } /* for loop through points */
if (WinComPts.dw_pnt_count > 1)
    {
      XDrawLines (dw_display,WinC.dw_live_pixmap,
		  /*WinC.dw_window[DW_TACTICAL],*/
		 ta_win_GC.dw_GC,
                 WinComPts.dw_pnt, WinComPts.dw_pnt_count, 
		 CoordModeOrigin);
      /*XFlush(dw_display);*//* put this here to test SMM */
    }

} /*end of symbol*/

/* Define center LAT/LON/MAP/RADIUS/TIME */
int draw_center_c (float *lat1ptr,float *lon1ptr, int *imapptr,
                 int *radiusptr,char time[])
{
 float lat1,lon1,rad,lat,lon;
 float temp_xl,temp_xr,temp_yb,temp_yt;
 int imap,radius;
 int standard_entry, need_map, last_imap, mapflg;
 int max_radius;
 int top, bottom, left, right;
 int kk, time_len, len, i;
 int linenum, colornum;
 char buf[ARB],radius_char[4];

 lat1 = *lat1ptr;
 lon1 = *lon1ptr;
 imap = *imapptr;
 radius = *radiusptr;
 
 memset(buf,0,ARB);

XDefineCursor(dw_display, 
	      WinC.dw_window[DW_TACTICAL],XC_X_cursor);  
  
  /* clear_hooks is in lbplot and will stay in ratfor*/
  clear_hooks();      /* reset count of symbols drawn (for hooking)*/
  
WinC.dw_in_progress = YES;

  /*_______________________________________________________________*/

if (WinC.dw_changed_hw == YES) /* has the window been resized?*/  
  {
   set_background (COLOR_BACKGROUND);
     
   WinC.dw_changed_hw = NO;
     
   resize_dw_pixmaps (WinC.dw_new_h,WinC.dw_new_w);
     
   WinC.dw_current_h = WinC.dw_new_h;
   WinC.dw_current_w = WinC.dw_new_w;
       
   viewpt (0, WinC.dw_current_h, WinC.dw_current_w, 0);

   need_map = YES; 
  }
    /* Have only one Pixmap (need Map) */
else if (WinC.dw_map_pixmap == WinC.dw_live_pixmap )
  {
   need_map = YES;
  }
else
  {
   need_map = NO;
  }

  /*________________________________________________________________*/

max_radius = radius;

rad             = max_radius;            /* convert to real*/

gph.geo_radius = max_radius;
WinC.dw_radius = max_radius;
WinC.dw_center_lat = lat1;
WinC.dw_center_lon = lon1;

/* get upper, lower, left and right boundries of display area*/
lat = lat1;
lon = lon1;

bndry(lat,lon,rad/*,,gph.xl,,gph.yb,,gph.xr,,gph.yt*/);
  
/* defines the boundary of the user coordinate system that is mapped
   into the viewport. */

temp_xl= gph.xl;
temp_yb= gph.yb;
temp_xr= gph.xr;
temp_yt= gph.yt;
window(temp_xl,temp_yb,temp_xr,temp_yt); /*dwvext.c*/

top    = EXT_.vyt;  /* defined in ptvext_f.h and set in window*/
bottom = EXT_.vyb;
left   = EXT_.vxl;
right  = EXT_.vxr;

/*_______________________________________________________________*/
/* draw the MAP */
/* bitset - try replacing bitset with &? SMM 12/2013*/  

if (imap && (1 << (MAP_RIVERS-1)) != 0)
	gph.geo_rivers = 1;

if (imap && (1 << (MAP_BOUNDARIES-1)) != 0)
	gph.geo_boundaries = 1;


WinC.dw_tactical_font = DW_SMALL_TACTICAL_FONT;

/*if ($bitset(imap,0) == YES | need_map == YES)*/
 if(((imap && 1)==YES)|(need_map == YES))
  {
   last_imap = imap;

    /* Note:  Map will be drawn in DW_Map_Pixmap

    #--------------------------------------------------
    # clear old map */ 
   set_color(COLOR_BACKGROUND);   /* change current color */
   XFillRectangle (dw_display,/*WinC.dw_window[DW_TACTICAL],*/
		   WinC.dw_map_pixmap,
                   ta_win_GC.dw_GC, 0,0, /*4/14 chgd frm 100,100*/ 
		   WinC.dw_current_w, WinC.dw_current_h);

   /*XFlush(dw_display);*/   /* put this here to test SMM */
   map(&imap);         /* display map area */
     
   set_color(COLOR_TEXT); /* change current color to white */

   grid();        /* display map grid */

   mapflg = ON;   /* map being displayed*/
  }

/*____________________________________________________________________
# Move the current map to the tactical pixmap*/
if (WinC.dw_map_pixmap != WinC.dw_live_pixmap)
    {
    XCopyArea (dw_display, WinC.dw_map_pixmap, 
               WinC.dw_live_pixmap, ta_win_GC.dw_GC, 0, 0, 
               WinC.dw_current_w, WinC.dw_current_h, 0, 0);
    }

/*___________________________________________________________________
  # display the RADIUS*/
len = 0;
sprintf(radius_char,"%d",max_radius);/*convert radius to char*/ 
strcpy(buf,radius_char);
strcat (buf,"nm");
linenum = LINE_RADIUS;
colornum = COLOR_TEXT; /*white*/
 
                        /*COLOR_TEXT*/
draw_text_c(&linenum,buf,&colornum);/*Draw text (R justified)*/

/*___________________________________________________________________
  display the TIME*/

len = 0;
time_len = strlen(time); 
strncpy(buf,time,time_len);
linenum = LINE_GAMETIME;
colornum = COLOR_TEXT;
                         /* COLOR_TEXT */
draw_text_c(&linenum,buf,&colornum);/*Draw text (R justified)*/

 return (YES);
} /* end of function draw_center_c */

void bndry(float lat,float lon,float rad/*,
	   float xl, float yb,float xr,float yt*/)
{
float degdis,snoth;    /* jb snoth 9/10/90*/
float w_to_h, rad_LR, rad_TB;

 w_to_h = (float)abs(EXT_.vxl - EXT_.vxr) / (float)abs(EXT_.vyb - EXT_.vyt);

 if (w_to_h > 1.0)           /* we have a short fat window */
    {
      rad_LR = rad;
      rad_TB = rad / w_to_h;
    }
 else if (w_to_h < 1.0)       /* we have a tall skinny window */
    {
      rad_LR = rad * w_to_h;
      rad_TB = rad;
    }
else
    {
      rad_LR = rad;
      rad_TB = rad;
    }

 /*get bottom edge of display by subtracting radius of display 
(converted to degrees) from the latitude of the center point of 
the display*/
gph.yb = lat - rad_TB/60.;     /* Bottom boundary */

/* if bottom boundry exceeds -75 degrees */
if (gph.yb < -75.)
    {
    /*starting from a max of 75 degrees, offset latitude by the 
      radius and set the bottom boundry to -75 degrees */
      lat = lat - 75. - gph.yb;
      gph.yb = -75.;
    }

/* get top edge of display by adding radius of display (converted
   to degrees) to the latitude of the center point of the display*/
 gph.yt = lat + rad_TB/60.;       /* Top boundary */

 /* if top boundry exceeds 75 degrees */
if (gph.yt > 75.)
    {
    /*starting from a max of 75 degrees, offset latitude by the 
      radius and set the bottom boundry to -75 degrees */
     lat = lat - (gph.yt - 75.);
     gph.yt = 75.;
    }

/* convert degrees to radians  7/12/84*/
 snoth = abs( lat*(M_PI/180.)); 
/* Radius to degrees of longit. */
 degdis = (rad_LR/cos(snoth) )/60.; 
/*#degdis = rad_lr/cos(abs(lat/$DEGRAD))/60.  # Convert distance to degrees*/

   gph.xl = lon - degdis;  /* left boundary */

 /* if the left boundry exceeds -180, subtract (add a minus) from 360*/
if (gph.xl < -180)
    gph.xl = 360. + gph.xl;

/*right boundry is twice the radius of display away from the left boundry*/
 gph.xr = gph.xl + 2. * degdis;      /* right boundary*/
} /* end of bndry */

void grid(void)
{
int incrmt, ixr, ixl, iyt, iyb, i;
int font_w, font_h, tick, ix, iy;
int mhy, nx, ny, int_lon, int_lat, len, length;
int gototop, label_len;
int my_font;
int j;
float rad; 
char label[6];  /* String containing constructed label*/
char lon_char[4],lat_char[3];
char *ptr;
char * class;

 rad = (EXT_.wxr - EXT_.wxl) * 30.; /* Compute plot radius*/

 if      (rad > 8999.) incrmt = 30; /* Compute label increment */
 else if (rad > 5999.) incrmt = 20;
 else if (rad > 2999.) incrmt = 15;
 else if (rad > 1999.) incrmt = 10;
 else if (rad >  999.) incrmt =  5;
 else if (rad >  399.) incrmt =  4;
 else if (rad >  199.) incrmt =  2;
 else                  incrmt =  1;

 ixr = EXT_.wxr;
 ixl = EXT_.wxl;   /* Starting point multiple of increment*/
 ixl = ixl + incrmt - fmod(ixl,incrmt);

 my_font = DW_GRID_FONT;
 set_font (&my_font);
   
 font_h = WinC.dw_font_h[DW_GRID_FONT];
 font_w = WinC.dw_font_w[DW_GRID_FONT];
 
 tick = (WinC.dw_pix_per_inch * 0.1);
 
 for (i = ixl; i <= ixr; i = i + incrmt) /* Label longitude axis */
   { 
    ix = mx(i);       /* Convert to screen coordinates */
    if (((ix - font_h) >= EXT_.vxl) & 
        ((ix + font_h) <= EXT_.vxr)) {
      mhy = (EXT_.vyb)-10; /*Fudge factr above the bottom of window*/
      nx = ix;
      ny = (mhy-font_h-4);  /* bottom of tick*/

      XDrawLine(dw_display,/*WinC.dw_window[DW_TACTICAL],*/
		WinC.dw_map_pixmap,
                ta_win_GC.dw_GC,
		nx,ny,
		nx,(ny-tick));

      /*XFlush(dw_display);*/   /* put this here to test SMM */
                                    
      nx = ix-2*font_w;   /*Position for label*/
      ny = mhy;

      memset(label,0,6); 
      memset(lon_char,0,4);/*0-180E or W*/
      label_len = 0;

      int_lon = abs(i); /* Convert longitude to absolute integer*/
      if (int_lon > 180) int_lon=360-int_lon;
      /*convert int to char*/ 
      sprintf(lon_char,"%d",int_lon);
    
      strcpy(label,lon_char);/*Beg to construct label*/
      if (i < 0 | i > 180) strcat(label,"W");
      else                 strcat(label,"E");


      /* dtext = descrip(label,len-1)*/
      label_len = strlen(label);
      XDrawString (dw_display,
		   /*WinC.dw_window[DW_TACTICAL],*//*SMM */
		   WinC.dw_map_pixmap,
		   ta_win_GC.dw_GC, 
                   nx, ny, label, label_len);

      /*XFlush(dw_display);*/   /* put this here to test SMM */
     } /* end of if */
 } /* end of for */
 
 iyt = EXT_.wytt;
 iyb = EXT_.wybt;
 iyb = iyb-abs(fmod(iyb,incrmt));/*Starting lat multiple of incrmt*/
 
 for (i = iyb;i <= iyt;i = i + incrmt) /* Label latitude axis */
   {
    iy = my(i);              /* Convert to screen coordinates */

    if (((iy - font_h) >= EXT_.vyt) &    /* Dont cause wraparound */
        ((iy + font_h) <= EXT_.vyb)) 
      {    
       label_len = 0;
       memset(label,0,6); 
       memset(lat_char,0,4);/*0-90N or S*/
 
       int_lat = abs(i);    /* Integer latitude */

       nx = EXT_.vxl;       /* position for Label */
       ny = iy + font_h/2;

       sprintf(lat_char,"%d",int_lat);/*convert int to char*/ 
       strcpy(label, lat_char);/* Beg. to construct label */
       if (i < 0) ptr = strcat (label,"S");
       else       ptr = strcat (label,"N");

       /*dtext = descrip(label,len-1)*/
       label_len = strlen(label);
       XDrawString (dw_display, 
		    /*WinC.dw_window[DW_TACTICAL],*/
		    WinC.dw_map_pixmap,
                    ta_win_GC.dw_GC, 
		    nx, ny, label, label_len);

       nx = EXT_.vxl + font_w * 3.5;   /*position for Tick */
       ny = iy;

       XDrawLine (dw_display, 
		  /*WinC.dw_window[DW_TACTICAL],*/
		  WinC.dw_map_pixmap,
		  ta_win_GC.dw_GC,
                  nx, ny, nx+tick, ny);
      } /* if in the right latitude */
   } /* end of for through latitude */

/*------------------------------------------------------------
# Now label at the top according to the classification of the exercise

# Translate the logical name EXERCISE_CLASSIFICATION*/

class = getenv ("EXERCISE_CLASSIFICATION");

if (class != NULL)
   {
    /*------------------------------
    # Position at top of screen, centered*/
    label_len = strlen(class);
    nx=((EXT_.vxr-EXT_.vxl)/2 +EXT_.vxl)-(label_len/2 * font_w);
    ny=EXT_.vyt+(font_h * 1.2);
    /*------------------------------
    # Output the translated text*/

    /*dtext = descrip(class,0);*/
   
    XDrawString (dw_display, 
		 /*WinC.dw_window[DW_TACTICAL],*/
		 WinC.dw_map_pixmap,
		 ta_win_GC.dw_GC, 
		 nx, ny, class, label_len);

    }

} /*end of GRID*/

void draw_done(void)  /*Indicate end of cycle*/
{
  /*istat = draw_envelope_done()  /* finish off last of envelopes*/

if (WinC.dw_live_pixmap != WinC.dw_window[DW_TACTICAL])
  {
   XCopyArea (dw_display, 
                 WinC.dw_live_pixmap, 
		 WinC.dw_window[DW_TACTICAL],
                 ta_win_GC.dw_GC, 
                 0, 0, 
		 WinC.dw_current_w, WinC.dw_current_h,
	         0, 0);
  }
  draw_text_done();


 WinC.dw_in_progress = NO;

}
int draw_circle_c (float *lat1p,float *lon1p,
                 short int *radiusp,int *colorp,int *icode_flagp)
{
  float lat1, lon1, rad;
  float lat, lon, latf, lonf, bear, tcos; 
  float plat[36],plon[36];
  int color;
  int icode_flag;
  short int radius;
  int ip, ia, id;
  float heading, conv;
  int num_of_pts;
 lat1 = *lat1p;
 lon1 = *lon1p;
 radius = *radiusp;
 color = *colorp;
 
 set_color (color);  /*Change color attribute*/
 icode_flag = * icode_flagp;
 rad = radius;       /*Convert to real*/

 lat = lat1;         /*generate circle for display*/
 lon = lon1;
 conv = M_PI/180;
 latf = (float)lat1 * M_PI/180;
 lonf = (float)lon1 * M_PI/180;

 if (1 == 1) /*always do this version*/
   {  /*try a 1 instead of 0*/
     ip = -1; /*init. counter for 12 radiating lines 5/17/83*/

     for (ia=0; ia<360; ia=ia+10) /* line every 10 degrees*/
        {
      	 ip = ip +1;/*line counter/1st indx for PLAT, PLON  5/17/83*/
    
         bear = (float)ia/180.* M_PI;/*Bearing in radians*/

         latf = lat1 * M_PI/180;
         lonf = lon1 * M_PI/180;

         tcos = cos(latf);
	 heading = 0.0;
	 
         /*get second point for line*/
         rrb2ll(&latf,
                &lonf,
                &rad,
                &bear,
                &heading,
                &tcos);

	 plat[ip] = latf * 180/ M_PI;
	 plon[ip] = lonf * 180/ M_PI;
        }
     
     id = 0;
     num_of_pts = 36;
     draw_polygon_c (&id, plat, plon, &num_of_pts, 
		   &color, &icode_flag);
    }
 
 return(YES);
} /*end draw_circle_c*/


/*  Display Xmark at LAT/LON/COLOR */
int draw_xmark_c (float *lat1p, float *lon1p, int *colorp)
{ 
  int ix,iy,color;
  float xlt,xrt,ybt,ytt;
  float lat1, lon1;

  lat1 = *lat1p;
  lon1 = *lon1p;
  color = *colorp;

  ix = mx(lon1);
  iy = my(lat1);

  xlt = (float)ix-CHARWIDTH/2.;/*gen. values for an Xmark symbol*/
  ybt = iy;
  xrt = (float)ix + CHARWIDTH/2.;
  ytt = (float)iy + CHARHEIGHT;

  if (inbox(xlt,ybt,xrt,ytt) == NO) /*Return if symbol not visible*/
     return (NO);

  set_color(color);
  /*NTDS_XMARK defined in ptgraph.h*/
  symbol(NTDS_XMARK,ix,iy);/* Plot xmark*/

 return (YES);
} /*end of draw_xmark*/


int draw_ntds_c(/*Display NTDS symbol*/
                int *ntdsp,float *lat1p,float *lon1p,
                int *colorp, int *declutter, char name1[ARB],char name2[ARB])
{ 

 int font_h, font_w, ix, iy, lbl, id;
 int name1_len, name2_len, ntds, color, ixtl, iytl, index;
 float lat1, lon1;
 float xlt, xrt, ybt, ytt;
 char name[8];

 ntds = *ntdsp;
 lat1 = *lat1p;
 lon1 = *lon1p;
 color = *colorp;

 ix = mx(lon1);  /* Convert longitude to screen coordinates*/
 iy = my(lat1);  /* Convert latitude to screen coordinates*/

 if (strlen(name1) == 0 && strlen(name2) == 0)
      lbl = NO;/*labels on ?*/
 else lbl = YES;

 if (lbl == YES) /* Put a box around symbol*/
    {
     set_font (&WinC.dw_tactical_font);
     font_h = WinC.dw_font_h[WinC.dw_tactical_font];
     font_w = WinC.dw_font_w[WinC.dw_tactical_font];

     xlt = ix - .8 * font_w;
     xrt = xlt + 5 * font_w;
     ybt = iy + 2. * font_h;
     ytt = iy - font_h;
    }
 else /*lbl == NO*/
    {
     xlt = ix - CHARWIDTH;
     xrt = ix + CHARWIDTH;
     ybt = iy;
     ytt = iy + CHARHEIGHT;
    }

 /*symbol entirely visible?*/
 if (inbox(xlt,ybt,xrt,ytt) == NO) return(NO);

 set_color(color); /* Set color for NTDS symbol*/

 symbol(ntds,ix,iy);  /* Plot NTDS symbol*/

if (*declutter == YES)
   {
    if (lbl == YES)
       {
        id = 0;                                  /* Not hookable */
        label_declutter (name1, name2, &ix, &iy, &id);
       }
   }
else
   {
 	if (lbl == YES)
	   {  
	    ixtl = ix - 2 * font_w; /* Position to line 1 of name*/ 
	    iytl = iy + font_h + WinC.dw_pix_per_inch * 0.15;

	    name1_len =strlen(name1); 
	    strncpy(name,name1,name1_len);
	    /*kk = 0; call smovh (name1,ARB,name,kk)
	    call smovv (' ',name,kk)*/

	    if (name[1] == EXCLAIM) /*if blinking code present*/ 
	      {
	       index = 2; /*start at second character*/ 
	      }
	    else
	      {
	       index = 1; /*default to first char*/ 
	      }

	    /*dtext = descrip(name[index],kk-1);*/
	    XDrawString (dw_display, WinC.dw_live_pixmap, 
			 ta_win_GC.dw_GC, ixtl, iytl, 
			 name, name1_len);

	    iytl = iytl + font_h + 4;

	    name2_len =strlen(name2);
	    strncpy(name,name2,name2_len);
	    /*kk = 0; call smovh (name2,ARB,name,kk)
	    call smovv (' ',name,kk)*/

	    if (name[1] == EXCLAIM) /*if blinking code present*/ 
	      {
	       index = 2; /*start at second character*/ 
	      }
	    else
	      {
	       index = 1; /*default to first char*/ 
	      }

	    /*dtext = descrip(name[index],kk-1)*/
	    if (name2_len > 1)
	      {
	       XDrawString (dw_display, WinC.dw_live_pixmap, 
			       ta_win_GC.dw_GC, ixtl, iytl, 
			       name,name2_len);
	      } /*if name2_len > 1*/
	   }/*if displaying labels */
   } /* else not declutter */
  
 return(YES);
 } /*end of draw_ntds_c*/


/* Display NTDS symbol*/
int draw_unit_c(int *ntdsp, int *idp, float *latp, float *lonp, 
	      int *declutter, char name1[],char name2[], 
	      int *colorp, float *coursep, int *speedp, 
	      int *typep, int *x1p, int *x2p, int *x3p, int *infop)
{
  int ix, iy, lbl, result = NO;
  int font_h, font_w, ixtl, iytl, indx;
  int icalid[2];
  float xlt, xrt, ybt, ytt;
  int name1_len, name2_len;
  char name[8];
  /*int EXCLAIM = 33;*/ /*double check this value for LINUX*/
  int ntds, id, color, speed,type;
  int x1, x2, x3, info; 
  float lat, lon, course; 
	       
 ntds = *ntdsp;
 id = *idp;
 color = *colorp;
 speed = *speedp;
 type = *typep;
 x1 = *x1p;
 x2 = *x2p;
 x3 = *x3p;
 info = *infop;
 lat = *latp;
 lon = *lonp;
 course = *coursep;

 //if (id==0) return(NO); //Plot was overwriting ghost ground objects with ADA symbol

 ix = mx(lon); /* Convert latitude to screen coordinates*/
 iy = my(lat); /* Convert longitude to screen coordinates*/

 if (id != 0)
      add_hook(&id, &ix, &iy); /*save hook-able posit in lbplot ratfor*/

 /* are labels on or off*/
 if (strlen(name1) == 0 && strlen(name2) == 0) 
      lbl = NO; 
 else lbl = YES;

 if (lbl == YES)  /* Put a box around symbol*/
    {
     set_font (&WinC.dw_tactical_font); /*dwdraw*/
     font_h = WinC.dw_font_h[WinC.dw_tactical_font];
     font_w = WinC.dw_font_w[WinC.dw_tactical_font];

     xlt = ix - .8 * font_w;
     xrt = xlt + 5 * font_w;
     ybt = iy + 2. * font_h;
     ytt = iy - font_h;
    }
else
    {
     xlt = ix - CHARWIDTH; /*ptwand.h*/
     xrt = ix + CHARWIDTH;
     ybt = iy;
     ytt = iy + CHARHEIGHT;
    }

/* is symbol entirely visible ?*/
if (inbox(xlt,ybt,xrt,ytt) == YES)
  {
   result = YES;

   set_color(color); /* Set color for NTDS symbol*/

   symbol(ntds,ix,iy); /* Plot NTDS symbol*/

   if (*declutter == YES)
     {
       if (lbl == YES)
         {
          id = 0;                                  /* Not hookable */
          label_declutter (name1, name2, &ix, &iy, &id);
         }
      }
   else
     {
      if (lbl == YES)
        {
         ixtl = ix - 2 * font_w;  /*Position to line 1 of name*/
         iytl = iy + font_h + WinC.dw_pix_per_inch * 0.15;

         /*move name1 into name and end it with a blank*/
         name1_len = strlen(name1);
         strncpy(name,name1,name1_len);
         /*kk = 0; call smovh (NAME1,ARB,name,kk)
	   call smovv (' ',name,kk)*/

         if (name[1] == EXCLAIM)/* if blinking code present*/
           {
	    indx = 2; /* start at second character*/
           }
         else
           {
	    indx = 1; /* default to first char */
           }

         /* dtext = descrip(name[indx],kk-1);*/
         XDrawString (dw_display,WinC.dw_live_pixmap,
		      /* WinC.dw_window[DW_TACTICAL],*/ 
		      ta_win_GC.dw_GC, ixtl, iytl, 
		      name, name1_len);
         /*XFlush(dw_display);*/  

         iytl = iytl + font_h + 4;

         /*move name1 into name and end it with a blank*/
         name2_len = strlen(name2);
         strncpy(name,name2,name2_len);
         /*kk = 0; call smovh (NAME2,ARB,name,kk)
	 call smovv (' ',name,kk)*/

         if (name[1] == EXCLAIM) /* if blinking code present*/
           {
	    indx = 2;   /* start at second character*/
           }
         else
           {
	 indx = 1;  /* default to first char*/
           }

         /*dtext = descrip(name[indx],kk-1)*/

         if (name2_len > 1)
           {
	     XDrawString (dw_display,WinC.dw_live_pixmap,
		          /*WinC.dw_window[DW_TACTICAL],*/ 
		         ta_win_GC.dw_GC, ixtl, iytl, 
		         name,name2_len);
           }
         } /* if label is YES */
        /* call mwcatt ($Highlighting,0)      # turn off blinking*/
       }/* else not declutter
    }/*if declutter is YES */
  }/*if symbol is entirely in box */

 return (YES);
} /* end of draw_unit_c */

/* Display Speed Vector */
int draw_vector_c(int *idp, float *latp, float *lonp,
                 float *lengthp, float *bearingp)
{
  int ix, iy, jx, jy, kx, ky;
  int id;
  float lat,lon,length,bearing;

  id = *idp;
  lat = *latp;
  lon = *lonp;
  length = *lengthp;
  bearing = *bearingp;

  ix = mx(lon);/* Convert latitude to screen coordinates*/
  iy = my(lat);/* Convert longitude to screen coordinates*/

  jx = ix + 8.0 * sin(bearing * M_PI/180);
  jy = iy - 8.0 * cos(bearing * M_PI/180);

  if (inpnt(jx,jy) == 0) /*Return if starting point is not visible*/
    return (NO);

  /* Display speed vector with length dependent on Pix/inch*/

  kx = ix + (8.0 + length/5.0) * sin(bearing * M_PI/180);
  ky = iy - (8.0 + length/5.0) * cos(bearing * M_PI/180);
  if (inpnt(kx,ky) == 0)/* Return if ending point is not visible*/
    return (NO);

  if (jx == kx & jy == ky)/* Return if no leader to draw*/
    return (NO);

  XDrawLine(dw_display,/*WinC.dw_window[DW_TACTICAL],*/
	    WinC.dw_live_pixmap, 
	    ta_win_GC.dw_GC,
            jx, jy, kx, ky);/* inside draw_vector */

      /*call PS_Put_Line (DW_PS, jx, jy, kx, ky)*/
  /*XFlush(dw_display);*/ /*put here during testing*/

  return (YES);
} /*end of draw_vector*/

#define MaxPixelsOffScreen 5000

int draw_line_c(float *lat1p, float *lon1p,
	      float *lat2p, float *lon2p,
	      int *colorp)
{
 
 float lat1, lon1, lat2, lon2, factor;
 short int with_id;
 int color, ix, iy, jx, jy, delta, id;
 int biggest_x, biggest_y, biggest_pix;

 lat1 = *lat1p;
 lon1 = *lon1p;
 lat2 = *lat2p;
 lon2 = *lon2p;
 color = *colorp;

 with_id = 0;
 /*Not currently used SMM 3/5/14
repeat 
    {
    break
    entry draw_line_id (lat1,lon1,
                        lat2,lon2,
                        color,
                        id)

    with_id = 1;
    }
*/
 set_color (color);  /* Change current color to input color */

 ix = mx(lon1);    /* Convert latitude to screen coordinates */
 iy = my(lat1);    /* Convert longitude to screen coordinates */

 if (lon1 > 0 &  /*12/4/90 long's are different side of Date Line */
     lon2 < 0 &
     lon1 > 90)
    {
     lon2 = 360. + lon2;  /* 12/4/90 */
    }
 else if (lon1 < 0 &  /*12/4/90 long's are diff side of Date Line */
          lon2 > 0 &
          lon1 < -90)
    {
     lon2 = lon2 - 360.;  /* 12/4/90 */
    }

 jx = mx(lon2); /* Convert latitude to screen coordinates */
 jy = my(lat2); /* Convert longitude to screen coordinates */

 if (inpnt(ix,iy) == 0 &  /* Return if neither starting point or */
     inpnt(jx,jy) == 0)   /* ending point is visible */
   return (YES);

/*code added to scale down lrg offscreen pixel values 12/4/90, reh
 DECwindows has a problem handling very lrg offscreen pixel values*/
 /*biggest_pix = MAX(abs(ix),abs(jx),abs(iy),abs(jy));*/
 if (abs(ix)>abs(jx)) biggest_x = abs(ix);
 else                 biggest_x = abs(jx);
 if (abs(iy)>abs(jy)) biggest_y = abs(iy);
 else                 biggest_y = abs(jy);
 if (biggest_x > biggest_y) biggest_pix = biggest_x;
 else biggest_pix = biggest_y;

 if (biggest_pix > MaxPixelsOffScreen)
    {
     factor = (MaxPixelsOffScreen/biggest_pix);
     delta = jx - ix;
     jx = ix + (delta * factor); /* new x pixel */
     delta = jy - iy;
     jy = iy + (delta * factor); /* new y pixel */
    }

 XDrawLine (dw_display, WinC.dw_live_pixmap, 
	    ta_win_GC.dw_GC,
	    ix, iy, jx, jy);

 if (with_id)
   {
    /* call add_hook_line here....*/
    add_hook_line (&id, &ix, &iy, &jx, &jy);
   }

return (YES);
}

/* Draw a line to edge */
int draw_bearing_c(int *idp, float *latp, float *lonp, 
		 int *id2p,float *bearingp, char name1[ARB], 
		 int *colorp, 
		 int *icode_flagp)/*dashed bearing line-JFR 9/28/94*/
{ 
  int id, id2, color, icode_flag;
  int font_h, font_w, index;
  int x1, y1, x2, y2, xe, ye;
  int kk, xs, ys, ymod, xmod;
  int flag_non_solid, name1_len=0;  /* JFR 9/28/94 */
  int dash_type;
  float lat, lon, bearing, range;
  float bear_rad, orig_lat, orig_lon;
  float tcos, lat2, lon2;
  float slope, angle, heading;
  float new_radius, delta_x, delta_y;
  char name[ARB];

  id = *idp;
  lat = *latp;
  lon = *lonp;
  id2 = *id2p;
  bearing = *bearingp;
  color = *colorp;
  icode_flag = *icode_flagp;
  
  font_h = WinC.dw_font_h[WinC.dw_tactical_font];
  font_w = WinC.dw_font_w[WinC.dw_tactical_font];

  new_radius = gph.geo_radius * .98; 
  if (new_radius > 50.0) range = 50.0;
  else range = new_radius;
 
  if (new_radius > 50.0) range = 50.;
  else range = new_radius; 

/*if (draw_full(x) == YES) return (NO)*/
 
  bear_rad = bearing * M_PI/180.; /*get bearing in radians*/

  orig_lat = lat * M_PI/180;  /*convert deg to radians*/ 
  orig_lon = lon * M_PI/180;

  tcos = cos(orig_lat);
  heading = 0.0;
  
  /*get second point for line*/
  rrb2ll(&orig_lat,          /*pass origin latitude*/
          &orig_lon,          /*pass origin longitude*/
  	  &range,             /*range of 98% of radius*/
 	  &bear_rad,          /*bearing to detection*/
          &heading,
  	  &tcos);             /*cosine of latitude*/

  lat2 = orig_lat * 180/M_PI; /* convert end lat/lon to*/
  lon2 = orig_lon * 180/M_PI; /* degrees */

  x1 = mx(lon); /* Convert to screen coordinates*/
  y1 = my(lat);
  x2 = mx(lon2);
  y2 = my(lat2);

  if (inpnt(x1,y1) == 0) /*Return if starting point is not visible*/
     return (NO);

  linedg(x1,y1,x2,y2,&xe,&ye);

  set_color(color); /* Change current color 3/18/83*/

  set_dash_c(&icode_flag); 
  if (icode_flag == POLY_SOLID_CODE)
    {
     flag_non_solid = FALSE;
    }
  else
    {
     flag_non_solid = TRUE;
    }

  XDrawLine (dw_display, 
	     WinC.dw_live_pixmap, 
	     ta_win_GC.dw_GC,
             x1,y1, xe, ye);

  if (id2 != 0)
    {/* save hook-able line */
     add_hook_line (&id2, &x1, &y1, &xe, &ye);
    }

  /*Place track label along line*/

  /*kk = 0; call smovhn (NAME1,ARB,name,kk)*/
  name1_len = strlen(name1);
  strncpy(name,name1,name1_len);

  if (name1_len != 0)
    {
      /*call smovv (' ',name,kk)*//*why is this done? SMM 4/14*/

     delta_x = x2-x1;
     delta_y = y2-y1;
     if (x1 != x2)
        slope = delta_y/delta_x; /*Compute angle of rotation*/
     else
        slope = 25.;

     angle = atan(slope);
     if (slope < 0.) angle = angle + TWOPI;
  
     /*Compute starting point of text*/
 
     xs = xe;
     ys = ye;

     ymod = 0;
     xmod = 0;

     if (ys == EXT_.vyt) /* At top edge */
       {
        ymod = font_h * 1.5;
        if (x1 > x2) xmod = - font_w * name1_len;
       }
     else if (ys == EXT_.vyb ) /* At bottom edge*/
       {
        ymod = 0;
        if (x1 > x2) xmod = - font_w * name1_len;
       }

     if (xs == EXT_.vxl)/* At left edge*/
       {
        if (y1 < y2) ymod = font_h;
       }
     else if (xs == EXT_.vxr ) /* At right edge*/
       {
        if (y1 < y2) ymod = font_h;
       }

     xs = xs + xmod;
     ys = ys + ymod;

     if (xs <= EXT_.vxl + font_w)  /* beyond left edge...*/
       {
        xs = EXT_.vxl + font_w * 0.5;
       }
     else if (xs>=(EXT_.vxr-font_w*name1_len))/*beyond right edge..*/
       {
        xs = xs - font_w * (1 + name1_len);
       }

     if (ys <= EXT_.vyt + font_h)  /* above top edge...*/
       { 
        ys = EXT_.vyt + font_h * 1.5;
       }
     else if (ys >= EXT_.vyb)  /* below bottom edge...*/
       {
        ys = EXT_.vyb - font_h * 1.5;
       }
 
/*    if (((y1 >= y2) & (x2 >= x1)) | ((y1 < y2) & (x2 > x1)))
#      {
#       xs = xe - cos(angle) * font_w * kk;
#       ys = ye + sin(angle) * font_w * kk;
#      }
#    else
#      {
#       xs = xe;
#       ys = ye;
#      }*/

/*     if (name[1] == EXCLAIM)*/ /*if blinking code present*/
       /*{
	 index = 2;*/ /*start at second character*/
     /*}
     else
       {
       index = 1;*/ /* default to first char*/
     /* }*/

    /*need to find the name and its length to send to XDrawString*/
    /*dtext = descrip(name[index],kk-1)*/
     name1_len = strlen(name);
     
     XDrawString (dw_display, 
	   	  WinC.dw_live_pixmap, 
	   	  ta_win_GC.dw_GC,
		  xs, ys, name, name1_len);
    
/*#    if (index != 1)
#       call mwcatt ($Highlighting,0)   # turn off blinking
*/
    } /*end if name1_len!=0*/

 if (flag_non_solid)/* JFR 9/28/94*/
   {
    dash_type = NO;
    set_dash_c(&dash_type);/* JFR 9/28/94*/
   }
} /*end of draw_bearing_c*/


/*  Draw text (right justified)*/
void draw_text_c(int *linep, char textline[], int *colorp)
{/* send in a textline[] so it can be copied into dw_text (dwdraw.h)
    for display or manipulated first and then displayed*/ 
  int kk, x, my_line, text_line_len; 
  /*WinCommon WinC;*/
/*(short int line, char textline[], int color)*/
/* DW_TEXT_LINES 16 */
/* DW_TEXT_LENGTH 20 */
/* char dw_text[DW_TEXT_LINES][DW_TEXT_LENGTH] */
  int line, color;
  
  line = *linep;
  color = *colorp;

  my_line = line; //+ 1;

/*if (my_line < 1 | my_line > DW_TEXT_LINES) return (NO)*/
if (my_line >= 0 | my_line <= DW_TEXT_LINES)
  {
   WinC.dw_text_color[my_line] = color;/*save color of text */
   
   /*strcpy(WinC.dw_text[my_line],"\0");*//*zero out text*/
   text_line_len = strlen(textline); /*get the length of line*/
   strcpy(WinC.dw_text[my_line],textline);/*copy it*/

/* skip following code until I know we need to compress the text
   lines for display 
#------------------------------------------------------------
# output Postscript text of center point/radius/time/etc...

   if (line == LINE_VIEW)
     {
       call Trim_Compress (DW_Text[my_line], temp, kk);
     }

   if (line == LINE_RADIUS)
     {
     call Trim_Compress (DW_Text[my_line], temp, kk)
     call vmovv ("  (at ", Temp, KK)
     call vmovlls (DW_Center_Lat * $deg2rad, "NS",  temp, kk)
     call vmovv (" ", Temp, KK)
     call vmovlls (DW_Center_Lon * $deg2rad, "EW",  temp, kk)
     call vmovv (")", Temp, KK)
    }

   if (line == LINE_GAMETIME)
    {
    call Trim_Compress (DW_Text[my_line], temp, kk)
    
    call PS_Put_Info (DW_PS, 3, "Game time: "//temp[1:kk])
    }

   if (line == LINE_DAYNITE)
    {
    call Trim_Compress (DW_Text[my_line], temp, kk)
    }*/

  } /* end if (my_line > 1 | my_line <= DW_TEXT_LINES)*/
} /* end of draw_text_c */



void draw_text_done(void)
{
 int kk, font_w, font_h, x, y;
 int length_of_text = 0;
 int my_font;

 /* clear the window */

 /* check for undefined window */
 if (WinC.dw_window[DW_TEXTINFO] == 0) return; 

 set_color(COLOR_BACKGROUND); /* change current color*/

 XFillRectangle (dw_display,WinC.dw_window[DW_TEXTINFO],
                 ta_win_GC.dw_GC, 0, 0, DW_INFO_W, DW_INFO_H);

 /*select the font */
 my_font = DW_INFO_FONT;
 set_font (&my_font);

 font_h = WinC.dw_font_h[DW_INFO_FONT];
 font_w = WinC.dw_font_w[DW_INFO_FONT];

 /* draw each line of text from the bottom up...*/
 x = 4;
 y = DW_INFO_H - 4;

 for (kk = 0; kk < DW_TEXT_LINES; kk = kk + 1)
    {
     set_color (WinC.dw_text_color[kk]);

     length_of_text = strlen(WinC.dw_text[kk]);
     XDrawString (dw_display,
		  WinC.dw_window[DW_TEXTINFO], 
		  ta_win_GC.dw_GC, x, y, 
		  WinC.dw_text[kk],
		  length_of_text);/*
				    DW_TEXT_LENGTH);*/
     XFlush(dw_display);   /* put in for testing */
     y = y - font_h - 6;
    }

 /* call x$sync (DW_Display, .false.)*/
 if (WinC.dw_widget_based != YES)  XFlush(dw_display);

 return;
}

  /*Draw a polygon*/
int draw_polygon_c(int *idp,float lats[],float lons[],
                 int *npointsp,int *colorp,int *icode_flagp)
/*NOTE: A NEGATIVE VALUE for NPOINTS means 
  DON'T CLOSE THIS POLYGON..*/
{
  int flag_non_solid, last_visible, curr_visible, next_visible;
  int jx, jy, x;
  int id, npoints, color, icode_flag;
  int i, np, np2;
  float xf, yf;

  id = *idp;
  npoints = *npointsp;
  color = *colorp;
  icode_flag = *icode_flagp;

/*if (draw_full(x) == YES) return (NO)*/
  
 set_color (color); /* Change current color*/

set_dash_c (&icode_flag);
if (icode_flag == POLY_SOLID_CODE)
{
	flag_non_solid = FALSE;
}
else
{
    flag_non_solid = TRUE;
}

 np = abs(npoints);

 if (npoints > 0) np2 = np + 1;
 else             np2 = np;

 last_visible = FALSE;
 WinComPts.dw_pnt_count = -1;

 for (i=0 ; i < np2; i=i+1) /* Process set of points*/
    {
    if (i >= np)
      {
       xf = lons[0]; /* Get point 1 again*/
       yf = lats[0];
      }
    else
      {
       xf = lons[i]; /* Get point*/
       yf = lats[i];
      }

    curr_visible = (0 == clpmsk(xf,yf));/* Is it visible*/

    if ( i+1 < np) /* look ahead one point*/ 
      {
       next_visible = (0 == clpmsk(lons[i+1],lats[i+1]));
      }
    else if (i == np) /* first point (again)*/
      {
       next_visible = (0 == clpmsk(lons[0],lats[0]));
      }
    else /* no point to look at*/ 
      {  /*  (end of line series)*/
       next_visible = FALSE;
      }

    if (last_visible | curr_visible | next_visible)
      {  /* should we add this point?*/
       jx = mx (xf);
       jy = my (yf);

       WinComPts.dw_pnt_count = WinComPts.dw_pnt_count + 1;
       WinComPts.dw_pnt[WinComPts.dw_pnt_count].x = jx;
       WinComPts.dw_pnt[WinComPts.dw_pnt_count].y = jy;

       if (WinComPts.dw_pnt_count > 0)/*check for duplicate pnts*/
         {
          if (WinComPts.dw_pnt[WinComPts.dw_pnt_count-1].x == jx &
              WinComPts.dw_pnt[WinComPts.dw_pnt_count-1].y == jy)
	        {/*YES, dup'ed*/
	     WinComPts.dw_pnt_count = WinComPts.dw_pnt_count - 1; 
            }
         }

       if (WinComPts.dw_pnt_count >= DW_MAX_POINTS)
         {
	  		XDrawLines(dw_display, 
	             /*WinC.dw_window[DW_TACTICAL],*/
		    WinC.dw_live_pixmap,
                     ta_win_GC.dw_GC,
                     WinComPts.dw_pnt, 
		    WinComPts.dw_pnt_count+1, 
		    CoordModeOrigin);

	        WinComPts.dw_pnt_count = 0;/*leave this one pnt in- 0*/
	        WinComPts.dw_pnt[WinComPts.dw_pnt_count].x = mx (xf);
            WinComPts.dw_pnt[WinComPts.dw_pnt_count].y = my (yf);
         }
      }
    else if (WinComPts.dw_pnt_count > 1)/*NO, draw what we have*/
        {
         XDrawLines (dw_display,
		     /*WinC.dw_window[DW_TACTICAL],*/
		     WinC.dw_live_pixmap,
		     ta_win_GC.dw_GC,
             WinComPts.dw_pnt, 
		     WinComPts.dw_pnt_count+1, 
		     CoordModeOrigin);
      	 jx = mx (xf);
     	 jy = my (yf);

     	 WinComPts.dw_pnt_count = 0;
     	 WinComPts.dw_pnt[WinComPts.dw_pnt_count].x = jx;
     	 WinComPts.dw_pnt[WinComPts.dw_pnt_count].y = jy;

        }
    else  /* Keep last non-visible point in first entry */
        {
      	 jx = mx (xf);
     	 jy = my (yf);

     	 WinComPts.dw_pnt_count = 0;
     	 WinComPts.dw_pnt[WinComPts.dw_pnt_count].x = jx;
     	 WinComPts.dw_pnt[WinComPts.dw_pnt_count].y = jy;
	}
     last_visible = curr_visible;

    } /* end for loop of all points*/

if (WinComPts.dw_pnt_count > 0)
    {
     XDrawLines (dw_display,
		 /*WinC.dw_window[DW_TACTICAL],*/
		 WinC.dw_live_pixmap, 
		 ta_win_GC.dw_GC,
                 WinComPts.dw_pnt, 
		 WinComPts.dw_pnt_count+1, 
		 CoordModeOrigin);
    }

if (flag_non_solid)
    {
     icode_flag = POLY_SOLID_CODE;
     set_dash_c(&icode_flag);
    }

 return (YES);
  }  /*end of draw_polygon_c*/


#define TB_MAX_LINES 40
#define TB_MAX_LINE_LEN 80
#define TB_LINE_HEIGHT 2. //was 1.25 as a function of the Font Height
#define TB_CHAR_ADJUST 1.05 // as a function of line length

typedef struct {
  int tbc_font;
  int tbc_box_color;
  int tbc_pixmap;
} textblock_common;

textblock_common tb_c = {
  /*1=small,2=medium,3=big*/  2,
                              COLOR_TEXT,
                              COLOR_BOX_YELLOW
};  /*initialize struct */


void draw_textblock_init_c(char *argument,int value)
{
  if      (strcmp(argument,"font")==0) tb_c.tbc_font = value;
  else if (strcmp(argument,"boxcolor")==0) tb_c.tbc_box_color = value;
  else if (strcmp(argument,"pixmap")==0) tb_c.tbc_pixmap = value;
} /*end of draw_textblock_init_c*/


// Display Block Of Text At X/Y
int draw_textblock_c (int x,int y, /* as returned from arrow (pixels)*/
                    int relative_angle,/* as passed to arrow (deg)*/
                    char text[],   /* With $LineFeed between lines*/
                    int width,     /* text width (to wrap) or zero*/
                    int text_style,/* left/center/right (0..2)*/
                    int box_style,/*transparent/opaque/rect/shadow (0..3)*/
                    int fg_color,/*color of text (normally COLOR_TEXT)*/
                    int bg_color,/*color of fill (normally $color$background)*/
                    int id)     /* Used In Hooking (0 or unique value)*/
{
/*--------------------------------------------------------------------
# The plan:
# . Use the text & width to determine the number of lines, and...
# . compute number of characters on the longest line.
# . compute box height as lines * font_height + margin*2
# . compute box width as Longest_line * font_width + margin*2
# . determine anchor point from relative angle
#   * possible anchor points are:
#       top-left, top-center, top-right, right-center,
#       left-center, bottom-left, bottom-center, bottom-right
# . Compute x,y for opposite corners for the Box
# . Add Hook info for Box (if ID given)
# . Compute x,y for first line of text
# . draw the box (optional)
# . for each line of text:
#   * extract the text
#   * adjust the text to be left/right/centered as requested
#   * draw each line of adjusted text
#   * compute x,y location for next line
#-----------------------------------------------------------------
*/
 int   my_font, font_h, font_w;
 int   n_lines, longest_line, i, k, s, e, sp;
 int   pos_s[TB_MAX_LINES], pos_e[TB_MAX_LINES], amount;
 char  a_line[TB_MAX_LINE_LEN], entire_posit[TB_MAX_LINE_LEN];
 int   box_width, box_height, margin;
 int   UL_X, UL_Y, LR_X, LR_Y, T_X, T_Y;
 int   pattern, my_pixmap;
 int   adj_font_height;
 int   text_len=0, how_much;
 char  *token,*string;

 memset(a_line,0,TB_MAX_LINE_LEN);
 for (i=0;i<=TB_MAX_LINES;i++) {
   pos_s[i] = 0;
   pos_e[i] = 0;
 }
/*----------------------------------------
  get font metrics*/

if      (tb_c.tbc_font == 1) my_font = DW_SMALL_TACTICAL_FONT;
else if (tb_c.tbc_font == 2) my_font = DW_MEDIUM_TACTICAL_FONT;
else if (tb_c.tbc_font == 3) my_font = DW_BIG_TACTICAL_FONT;
else    /* default */        my_font = DW_MEDIUM_TACTICAL_FONT; 

set_font (&my_font);

font_h = WinC.dw_font_h[my_font];// these values are in pixels
font_w = WinC.dw_font_w[my_font];
margin = font_h / 2;

//if (tb_c.tbc_pixmap == 0) 8/20/14
 my_pixmap = WinC.dw_window[DW_TACTICAL];//WinC.dw_live_pixmap
//else                      my_pixmap = tb_c.tbc_pixmap;

/*----------------------------------------
 Use the text & width to determine the number of lines, and...
 compute number of characters on the longest line.
*/
 text_len = strlen(text);
 tb_find_lines (text, &width, &n_lines, &longest_line, &pos_s, &pos_e,&text_len);

//----------------------------------------
// . compute box height as lines * font_height + margin*2
// . compute box width as Longest_line * font_width + margin*2

 adj_font_height = font_h * TB_LINE_HEIGHT;
 box_height = (n_lines * adj_font_height)+(margin * 2);
 box_width = (longest_line * font_w * TB_CHAR_ADJUST )+(margin * 2);

/*----------------------------------------
# . determine anchor point from relative angle
#   * possible anchor points are:
#       top-left, top-center, top-right, right-center,
#       left-center, bottom-left, bottom-center, bottom-right
# . Compute x,y for opposite corners for the Box
*/
 tb_find_corners (&x,&y, &relative_angle,         // input
		 &box_height, &box_width,       // input
		 &UL_X, &UL_Y, &LR_X, &LR_Y);     // output

/*#----------------------------------------
# . Add Hook info for Box (if ID given)
*/
 if (id != 0)
    {
     add_hook_box (&id, &UL_X, &UL_Y, &LR_X, &LR_Y);
    }

/*#----------------------------------------
# . Compute x,y for first line of text
*/
 T_X = UL_X + margin;
 T_Y = UL_Y + margin + font_h;

/*#----------------------------------------
# draw the box (optional)
# Draw up to 3 things: Shadow ... Background ... Outline
*/
 if (box_style >= 3)        // shadow box...
    {
    // compute points for rect offset to Below & Right

     WinComPts.dw_pnt[0].x = UL_X + 4;
     WinComPts.dw_pnt[0].y = UL_Y + 4;
     WinComPts.dw_pnt[1].x = LR_X + 4;
     WinComPts.dw_pnt[1].y = UL_Y + 4;
     WinComPts.dw_pnt[2].x = LR_X + 4;
     WinComPts.dw_pnt[2].y = LR_Y + 4;
     WinComPts.dw_pnt[3].x = UL_X + 4;
     WinComPts.dw_pnt[3].y = LR_Y + 4;

     WinComPts.dw_pnt_count = 4;

     // draw filled rect of box_color

     set_color (tb_c.tbc_box_color);
    
     XFillPolygon (dw_display, my_pixmap, ta_win_GC.dw_GC,
                   WinComPts.dw_pnt, WinComPts.dw_pnt_count,
		   Nonconvex, CoordModeOrigin);
     pattern = 1;
    }
 
if (box_style >= 1) // Opaque box...
    {
     // compute points for rect

     WinComPts.dw_pnt[0].x = UL_X; 
     WinComPts.dw_pnt[0].y = UL_Y; 
     WinComPts.dw_pnt[1].x = LR_X; 
     WinComPts.dw_pnt[1].y = UL_Y; 
     WinComPts.dw_pnt[2].x = LR_X; 
     WinComPts.dw_pnt[2].y = LR_Y; 
     WinComPts.dw_pnt[3].x = UL_X; 
     WinComPts.dw_pnt[3].y = LR_Y; 
     WinComPts.dw_pnt[4].x = WinComPts.dw_pnt[0].x;
     WinComPts.dw_pnt[4].y = WinComPts.dw_pnt[0].y;
    
     WinComPts.dw_pnt_count = 4;

     // draw filled rect of bg_color
     set_color (bg_color);

     XFillPolygon (dw_display, my_pixmap, ta_win_GC.dw_GC,
                   WinComPts.dw_pnt, WinComPts.dw_pnt_count,
		   Nonconvex, CoordModeOrigin);
     pattern = 0;

     if (box_style >= 2)  // Regular Box...
        {
	 WinComPts.dw_pnt_count = 5;

         // draw rect of box_color

         set_color (tb_c.tbc_box_color);

         XDrawLines (dw_display, my_pixmap, ta_win_GC.dw_GC,
                     WinComPts.dw_pnt, WinComPts.dw_pnt_count, 
		     CoordModeOrigin);
        }//end if box_style >= 2
    }//end if box_style >= 1

//----------------------------------------
//. for each line of text:

 set_color (fg_color);

 //for (k = 0; k < n_lines; k++)
 // {
 //   if (pos_s[k] <= pos_e[k])             // is the line non-blank
	//     {
	  //a_line = text [pos_s[k] : pos_e[k]] //extract the text

	 // adjust the text to be left/right/centered as requested
	 // ???? not done yet

	 //if (text_style <= 0)             // Left
	  // {
	    //s = 1;
            //e = pos_e[k] - pos_s[k] + 1;
          //}
         //else 
          //{
           //s = 1;
           //e = pos_e[k] - pos_s[k] + 1;

 //if (text_style == 1)              // Centered
 //	     sp = (longest_line - e)/2;      // spaces needed
 //	 else                              // Right
 //	     sp = longest_line - e;          // spaces needed
 	     
 //	 strcat(a_line," ");
	 //a_line[sp+1:] = text [pos_s[k] : pos_e[k]];
 //	 e = e + sp;
 //	}
 string = strdup(text);
 
 if (string!=NULL) 
    {
     while ((token=strsep(&string," "))!=NULL) 
       {
        e = strlen(token);
        if (e!=0) {
	  // draw each line of adjusted text
          XDrawString (dw_display, my_pixmap, ta_win_GC.dw_GC,
		     T_X, T_Y, token,e);
          // compute next x,y 
          T_Y = T_Y + font_h *TB_LINE_HEIGHT;
	  }//if e!=0
       } // end while looking for blanks

    }//end if string!=NULL

return (YES);
}/*end of draw_textblock_c*/

void tb_find_lines_c (char text[], 
		    int width, 
		    int *n_lines, 
		    int *l_line, 
		    int pos_s[], 
		    int pos_e[])
{
// Use the text & width to determine the number of lines, and...
// compute number of characters on the longest line.
  int space, p, mx;
  int str_len;
  int num_lines=0;
  int longest_line=0;

space = 0;
mx = TB_MAX_LINE_LEN;
if (width > 0) 
  { /*use the min between mx and width*/    
   if (mx >= width) mx = width;
  }
longest_line = 1;
//num_lines = 1;
 num_lines = 0;
pos_s[num_lines] = 1;

 str_len = strlen(text);
 //for (p = 1; p <= strlen(text); p = p + 1 )
for (p = 0; p < str_len; p++)
    {
      // if (text[p:p] == "") space = p;
      //   [p:p] changed to [p] to avoid errors for now
      //           change char($linefeed) to "\n"
     if (text[p] == '\n')
        {
	 pos_e[num_lines] = p - 1;

	 /*find the max between longest_line and other*/
	 if (longest_line <= (pos_e[num_lines] - pos_s[num_lines] + 1))
	    longest_line = (pos_e[num_lines] - pos_s[num_lines] + 1);
	 num_lines = num_lines + 1;
	 pos_s[num_lines] = p + 1;
	 space = 0;
        }

    else if (p-pos_s[num_lines]+1 > mx)
        {
	 if (space != 0) pos_e[num_lines] = space - 1;
	 else            pos_e[num_lines] = p - 1;

	 p = pos_e[num_lines] + 1;
         /*find the max between longest_line and other*/
	 if (longest_line <= (pos_e[num_lines] - pos_s[num_lines] + 1)) 
	    longest_line = (pos_e[num_lines] - pos_s[num_lines] + 1);
	 num_lines = num_lines + 1;
	 pos_s[num_lines] = p + 1;
	 space = 0;
        }
    } //loop thru each character of text

pos_e[num_lines] = strlen(text);
/*find the max between longest_line and other*/
 if (longest_line <= (pos_e[num_lines] - pos_s[num_lines]+1))
    longest_line = (pos_e[num_lines] - pos_s[num_lines]+1);

*n_lines = num_lines;
 *l_line = longest_line;

}//end of tb_find_lines_c


void tb_find_corners_c (int x,int y,            // input
		      int angle,              // input
		      int height, int width,  // input
                      int *p_UL_X, int *p_UL_Y,     // output 
		      int *p_LR_X, int *p_LR_Y)     // output
{
/* . determine anchor point from relative angle
#   * possible anchor points are:
#       top-left, top-center, top-right, right-center,
#       left-center, bottom-left, bottom-center, bottom-right
# . Compute x,y for opposite corners for the Box
#
# Given an Angle,
#
#                      |    |       |     |
#                      |    |       |     |
#                      |    |       |     |
#                ______|    |_______|     |__________
#                       3       4        5
#                      135     180     225
#                         \     |     /
#                          \    |    /
#                           \   |   /
#         ______             \  v  /              ______
#              |                                  |
#              | 2 90 ------>   *   <------ 270 6 |
#         _____|                                  |_____
#                            /  ^  \
#                           /   |   \
#                          /    |    \
#                         /     |     \
#                       45      0      315
#                       1       0       7
#               ________    _________    _______
#                      |    |       |    |
#                      |    |       |    |
#                      |    |       |    |
#
*/

  int UL_X, UL_Y, LR_X, LR_Y; 
  int k;

k = (abs(angle) + 22.5) / 45.0;
 k = fmod(k,8);

switch (k)
    {
    case 0: case 4: UL_X = x - (width/2);
    case 1: case 2: case 3: UL_X = x - width;
    case 5: case 6: case 7: UL_X = x; 
    }
switch (k)
    {
    case 0: case 1: case 7: UL_Y = y;
    case 3: case 4: case 5: UL_Y = y - height;
    case 2: case 6: UL_Y = y - (height/2);
    }

LR_X = UL_X + width;
LR_Y = UL_Y + height;

*p_UL_X = UL_X; // output
*p_UL_Y = UL_Y;      
*p_LR_X = LR_X; 
*p_LR_Y = LR_Y;

} //end of tb_find_corners_c


void resize_dw_pixmaps (int new_h, int new_w)
{
/* At this point there may be 1, 2, or 3 pixmaps allocated and 
assigned. If we get here, then the size of them all needs to be 
changed....
This is done by freeing each pixmap and re-creating them.
However, one or more of the entries in the pixmap list may be 
duplicated and the duplication must be maintained....
*/
int n,k;
bool duped;
Pixmap new_pixmap[DW_PIXMAPS];
/*WinCommon WinC; */ 
/*------------------------------------------------------------
DW_pixmap[*] contains the pixmap IDs on input. Free each unique ID.
and allocate a new one...
*/
for (n = 0; n < DW_PIXMAPS; n++)
    {
      /* check out previous entries for duplication */
    duped = false;

    for (k = 0; k < n; k++)
        {
         if (WinC.dw_pixmap[n] == WinC.dw_pixmap[k])
            {
	    duped = true;      /* This one is a dup... */
            break;             /* K points to original */
            }
        } /* end of loop checking for dupes */

    if (duped)
        {
	 new_pixmap[n] = new_pixmap[k]; /* re-use the same new one */
        }
    else
        {
	 XFreePixmap (dw_display, WinC.dw_pixmap[n]);

         new_pixmap[n] = XCreatePixmap (dw_display, 
				       WinC.dw_window[DW_TACTICAL],
                                       new_w, new_h, WinC.dw_depth);
        }

    /*  print *,n," Pixmap ",dw_pixmap[n],"=old id ",new_pixmap[n],"=new id"*/
    }/* end of for loop through DW_PIXMAPS */

/*------------------------------------------------------------
# DW_pixmap[*] contains freed pixmap IDs.
# New_pixmap[*] contains the new pixmap IDs.
# Now, assign the correct pixmap IDs to the local variables used...
*/
for (n = 0; n <= DW_PIXMAPS; n++)
    {
    /* If the variable uses the old ID, use the new one...*/

     if (WinC.dw_map_pixmap  == WinC.dw_pixmap[n]) 
	      WinC.dw_map_pixmap = new_pixmap[n];
     if (WinC.dw_live_pixmap == WinC.dw_pixmap[n]) 
              WinC.dw_live_pixmap = new_pixmap[n];
     if (WinC.dw_next_pixmap == WinC.dw_pixmap[n]) 
              WinC.dw_next_pixmap = new_pixmap[n];

     WinC.dw_pixmap[n] = new_pixmap[n];      /* save the new ID */
    }

} /* end of resize_dw_pixmaps */

int alloc_color_name (Display *a_display,Screen *a_screen,
		      Visual *a_visual, 
		      int n, char color_name[],
		      int rgb[])  
{
  int status=false;
  int return_value= 0;
  int temp_class;
  Colormap color_map;
  XColor screen_color, closest_color;

  status = 0;
  temp_class = a_visual->class;
  /* class of type int */ 
  /* DirectColor colormaps can be changed dynamically*/
  /* PseudoColor colormaps can be modified anytime*/
  if (a_visual->class == PseudoColor | 
      a_visual->class == DirectColor |
      a_visual->class == TrueColor)
    {
     color_map = XDefaultColormapOfScreen(a_screen);     
     status = XAllocNamedColor(a_display,color_map, 
			       color_name, &closest_color,
			       &screen_color);
     if (status == true)        
       {       
        return_value = screen_color.pixel;
	/*	red   = 65535 & screen_color.red;
  	green = 65535 & screen_color.green;
	blue  = 65535 & screen_color.blue;*/
	rgb[0] = screen_color.red;
  	rgb[1] = screen_color.green;
	rgb[2] = screen_color.blue;
       } 
     else
        {
	  /*type *,"Color not allocated!"*/
        return_value = 0;
        rgb[0] = 0;
        rgb[1] = 0;
        rgb[2] = 0;
        }
     }
  else /* TrueColor colormaps are fixed, same with StaticColor*/
    /*GrayScale and StaticGray*/
    {/*0 for black screen, not sure what 2 is for, 
       1 is for white screen to print */
     if (n == 0 | n == 2) 
        return_value = BlackPixelOfScreen(a_screen);
     if (n == 1 )         
        return_value = WhitePixelOfScreen(a_screen);
    } 
 return(return_value);   
} /* end of alloc_color_name */


void arc_c(short int ixc,short int iyc,short int ir,
	   float s,float e)
{
/*
#####################################################################
#
# Arguments
#
#       ixc     An integer variable containing the x screen
#               coordinate of the center of the arc
#       iyc     An integer variable containing the y screen
#               coordinate of the center of the arc.
#       ir      An integer variable containing the radius of the arc
#               in screen units.
#       s       A single precision real variable containing the
#               starting angle of the arc in radians.
#       e      A single precision real variable containing the ending
#               angle of the arc in radians.
#
#####################################################################
*/
  double delta, arcsize, sina, cosa;
  double x,y, x1, y1, xf, yf;
  short int inside, n, i;
  float min_x_view, max_x_view;
  float min_y_view, max_y_view;

 WinComPts.dw_pnt_count = -1;

 delta = 2 * M_PI/NSIDES;  /*angle increment*/
 arcsize = e - s;        /*size of arc*/

 /*if angle arcsize is neg., add to 360 degrees to get positive arc*/
 if (arcsize <= 0.) arcsize = arcsize + TWOPI;

 /* get sine and cos of increment angle*/
 sina = sin(delta);
 cosa = cos(delta);

 /* get x,y components of angle s*/
 x1 = ir * cos((double)s);
 y1 = ir * sin((double)s);

 /* xf, yf defines the location of the tip of the arc segment*/
 /* from the center of origin*/
 xf = x1 + ixc;
 yf = y1 + iyc;

 /* if arc segment is outside viewport parameters*/
 if (EXT_.vxl > EXT_.vxr)
   {
    min_x_view = EXT_.vxr;
    max_x_view = EXT_.vxl;
   }
 else /*EXT_.vxr > EXT_.vxl*/ 
   {
    min_x_view = EXT_.vxl;
    max_x_view = EXT_.vxr;
   }
 if (EXT_.vyb > EXT_.vyt)
   {
     min_y_view = EXT_.vyt;
     max_y_view = EXT_.vyb;
   }
 else/*EXT_.vyt > EXT_.vyb*/
   {
     min_y_view = EXT_.vyb;
     max_y_view = EXT_.vyt;
   }

 if (xf < min_x_view | 
     xf > max_x_view |
     yf < min_y_view | 
     yf > max_y_view )  
   /* set inside flag to NO (arc segment is not in view)*/
   inside = NO;        
else
   {
     /* else arc segment is in view*/
    inside = YES;
    /* move tip of arc segment to desired position*/
    /*   call move (xf,yf)*/            

    WinComPts.dw_pnt_count = WinComPts.dw_pnt_count + 1;
    WinComPts.dw_pnt[WinComPts.dw_pnt_count].x = (int)xf;
    WinComPts.dw_pnt[WinComPts.dw_pnt_count].y = (int)yf;
   }

 n = arcsize/delta;  /*number of line segments in arc*/

 /* for each line segment in the arc*/
 for (i=1; i<=n; i=i+1)
   {
    /* move x,y by amount of angle increment*/
    x = x1*cosa - y1*sina;
    y = y1*cosa + x1*sina;
    /* save computed x,y coordinates*/
    x1 = x;
    y1 = y;
   
    /* x,y defines the location of the tip of the arc segment*/ 
    /* from the center of origin*/
    x = x + ixc;
    y = y + iyc;
   
    /* if arc segment is outside viewport parameters*/
    if (xf < min_x_view | 
        xf > max_x_view |
        yf < min_y_view | 
        yf > max_y_view )  
        /* set inside flag to NO (arc segment is not in view)*/
        inside = NO;
    /*else if current arc segment is inside viewport parameters*/
    /*and previous arc segment was outside */
    else if (inside == NO)
       {
	/* set inside flag to NO (arc segment is not in view)*/
	inside = YES;
	/*move tip of arc segment to desired position*/
	/*      call move (x,y)*/
        WinComPts.dw_pnt_count = WinComPts.dw_pnt_count + 1;
	WinComPts.dw_pnt[WinComPts.dw_pnt_count].x = (int)x;
	WinComPts.dw_pnt[WinComPts.dw_pnt_count].y = (int)y;
      }
   /*else both previous & current arc seg inside viewport parameters*/
   else                 
      {
      /* move tip of arc segment to desired position*/
      /*  call line (x,y)*/
      WinComPts.dw_pnt_count = WinComPts.dw_pnt_count + 1;
      WinComPts.dw_pnt[WinComPts.dw_pnt_count].x = (int)x;
      WinComPts.dw_pnt[WinComPts.dw_pnt_count].y = (int)y;
      }
   }/*end of for loop thru each line segment in the arc*/

  /*terminate arc at end point*/

  /*if last arc seg from loop above is inside viewport parameters*/
 if (inside == YES)
   {
    /*set x,y to coord of tip of end angle from center of origin*/
    x = ixc + ir * cos(e);
    y = iyc + ir * sin(e);

    /* if last angle is inside viewport parameters*/
    /*   if (x >= vxl & x <= vxr & y >= vyb & y <= vyt)*/
    if (xf >= min_x_view & 
        xf <= max_x_view &
        yf >= min_y_view & 
        yf <= max_y_view )  
       {
       /* move tip of last angle segment to desired position*/
  /*      call line (x,y)*/
        WinComPts.dw_pnt_count = WinComPts.dw_pnt_count + 1;
        WinComPts.dw_pnt[WinComPts.dw_pnt_count].x = (int)x;
        WinComPts.dw_pnt[WinComPts.dw_pnt_count].y = (int)y;
       }
   }/*if last arc seg from loop above is inside viewport parameters*/

if (WinComPts.dw_pnt_count > 1)
    {
     XDrawLines (dw_display, 
                 WinC.dw_live_pixmap, 
                 ta_win_GC.dw_GC,
                 WinComPts.dw_pnt, 
                 WinComPts.dw_pnt_count, 
                 CoordModeOrigin);
    }


}/*end of arc_c*/

extern CommonPointStruct decw_pt;

void x_draw_lines()
{
	int i;

	WinComPts.dw_pnt_count = decw_pt.dw_pnt_count;
 	for (i=0; i < decw_pt.dw_pnt_count; i++)
	{
		WinComPts.dw_pnt[i].x = decw_pt.dw_pnt_x[i];
		WinComPts.dw_pnt[i].y = decw_pt.dw_pnt_y[i];
	}

    XDrawLines(dw_display, 
	       WinC.dw_map_pixmap, 
               ta_win_GC.dw_GC,
               WinComPts.dw_pnt, 
	       WinComPts.dw_pnt_count, 
	       CoordModeOrigin);

}

void ang_to_pi(float line_angle)
{
  while (line_angle >= M_PI) {
    line_angle = line_angle - M_PI;
  }

  while (line_angle <= 0) {
     line_angle = line_angle + M_PI;
  } 
} /*end ang_to_pi*/



void linedg(int x1,int y1,int x2,int y2,int *xe,int *ye)
{

/*
#####################################################################
#
# Arguments
#
#  x1 Integer variable containing the first x coordinate of the line.
#  y1 Integer variable containing the first y coordinate of the line.
#  x2 Integer variable containing the second x coordinate of the line
#  y2 Integer variable containing the second y coordinate of the line
#  xe Integer variable that will contain the x coordinate of the
#     line extended to the edge of the viewport.
#  ye Integer variable that will contain the y coordinate of the
#     line extended to the edge of the viewport.
#       
##################################################################### */

float    xdiff,ydiff;
float    slope;
float    xer,yer;

// get length of x,y lines
xdiff = x2 - x1;
ydiff = y2 - y1;

if (xdiff != 0)                 // if line is not vertical
    {
    slope = ydiff/xdiff;        // Compute slope of line

    if (x2 > x1) xer = EXT_.vxr;     // Try right edge
    else         xer = EXT_.vxl;

    yer = y1 + slope * (xer - x1); // Given x on edge compute y on edge

    if (yer < EXT_.vyt | yer > EXT_.vyb)  // does it cross Top or bottom edge first ?
        {
        if (yer < EXT_.vyt) yer = EXT_.vyt;
        else           yer = EXT_.vyb;
    
        if (slope != 0.0)
            {
            xer = x1 + (yer - y1) / slope;  // Given y on edge 
                                           // compute x on edge
        
            if      (xer < EXT_.vxl) xer = EXT_.vxl;  // Check for wraparound
            else if (xer > EXT_.vxr) xer = EXT_.vxr;
            }
        else
            {
            if (x2 > x1) xer = EXT_.vxr;
            else         xer = EXT_.vxl;
            }
       }
   }
else                            // Vertical line
   {
   xer = x1;                     // x value of edge

   if (y2 < y1) yer = EXT_.vyt; // Line intersects top or bottom of viewport
   else         yer = EXT_.vyb;
   }

*xe = xer;
*ye = yer;

} 
// end of linedg

int get_font_height(int *font)
{
	int font_h;
    font_h = WinC.dw_font_h[*font];
	return (font_h);
}

int get_font_width(int *font)
{
	int font_w;
    font_w = WinC.dw_font_w[*font];
	return (font_w);
}

int get_screen_height()
{
	int screen_h;
    screen_h = WinC.dw_new_h;
	return (screen_h);
}

int get_screen_width()
{
	int screen_w;
    screen_w = WinC.dw_new_w;
	return (screen_w);
}

void draw_string(int *ix, int *iy, char name[ARB])
{
    int name_len;
	name_len =strlen(name); 
	XDrawString (dw_display, WinC.dw_live_pixmap,
		 ta_win_GC.dw_GC, *ix, *iy, 
		 name, name_len);
}

void draw_line(int *jx, int *jy, int *kx, int *ky)
{
	XDrawLine(dw_display,/*WinC.dw_window[DW_TACTICAL],*/
	    	WinC.dw_live_pixmap, 
	   		ta_win_GC.dw_GC,
            *jx, *jy, *kx, *ky);
}
 
