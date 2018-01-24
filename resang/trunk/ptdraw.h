/*ptdraw.h*/
#include <X11/Xlib.h>
#define ARB 100
#define DEG2RAD 0.0174533 /*$deg2rad*/
#define DW_WINDOWS 2  /* $DW$Windows */
#define DW_COLORS 16  /* $DW$Colors,16 */
#define DW_PIXMAPS 2  /* $DW$Pixmaps*/
#define DW_CONTEXTS 2 /* DW_Contexts array dimension - joa 7/96(E1602)*/
#define DW_VIEWS 10 /* DW_Views array dimension - joa 7/96(E1602)*/
#define DW_REGIONS 4

#define DW_TEXT_LINES 16 /*$DW$Text_Lines*/
#define DW_TEXT_LENGTH 20/*$DW$Text_Length*/
#define DW_DEFAULT_H 500 /* $DW$default_H */
#define DW_DEFAULT_W 600 /* $DW$default_W */

#define DW_MINIMUM_H 200  /* smallest size of primary window*/
#define DW_MINIMUM_W 300  /* was $DW$Minimum$W*/

#define DW_MAX_H 1024      /* Largest size of primary window*/
#define DW_MAX_W 1280

#define DW_INFO_H 250 /*default size of Text Info Window (12pt font)*/
#define DW_INFO_W 130        /*(E2141, 8-byte names) dew */
			    
#define DW_TACTICAL 0        /* Index to Tactical Window */
#define DW_TEXTINFO 1        /* Index to Text Information Window */

#define DW_STIPS 8 /* $DW$Stips */
#define DW_FONTS 6  /* $DW$Fonts */
/*#define W_FONT1_NAME "-adobe-utopia-medium-i-normal--*-0-p-0-*"*/
#define DW_FONT0_NAME "fixed\0"
/*#define W_FONT1_NAME "-ADOBE-COURIER-Medium-R-Normal--*-140-*-*-M-*"*/
#define DW_FONT1_NAME "fixed\0"
/*#define DW_FONT1_NAME "-adobe-utopia-medium-r-normal--*-0-p-0-*\0"*/
/*#define W_FONT2_NAME "-ADOBE-COURIER-Medium-R-Normal--*-100-*-*-M-*"*/
#define DW_FONT2_NAME "fixed\0"
/*#define DW_FONT2_NAME "-misc-fixed-medium-r-semicondensed--*-100-c-60-*\0"*/

/* this font has issues "-misc-fixed-medium-r-semicondensed--*-75-75-c-0-*\0"*/
#define DW_FONT3_NAME "fixed\0"

/*#define DW_FONT3_NAME "-adobe-utopia-medium-r-normal--*-0-p-0-*\0"*/
/*#define DW_FONT3_NAME "-misc-fixed-medium-r-semicondensed--*-75-c-60-*\0"*/
#define DW_FONT4_NAME "fixed\0"

/*#define W_FONT4_NAME "-ADOBE-HELVETICA-Medium-R-Normal--*-100-75-75-P-*"*/
/*#define DW_FONT4_NAME "-urw-century schoolbook l-bold-i-normal--*-0-p-0-*\0"*/ 
/*#define W_FONT5_NAME "-ADOBE-HELVETICA-Medium-R-Normal--*-140-75-75-P-*"*/
#define DW_FONT5_NAME "fixed\0"
/*#define DW_FONT5_NAME "-urw-century schoolbook l-medium-i-normal--*-0-p-0-*\0" */
/*#define W_FONT6_NAME "-ADOBE-HELVETICA-Medium-R-Normal--*-180-75-75-P-*"*/

/*define ($dw$Small_Tactical_Font,3)*/
#define DW_SMALL_TACTICAL_FONT 3 /*$dw$Small_Tactical_Font*/
#define DW_MEDIUM_TACTICAL_FONT 4 /*$dw$Medium_Tactical_Font*/
#define DW_BIG_TACTICAL_FONT 5 /*$dw$Big_Tactical_Font*/
#define DW_GRID_FONT 4 /*$dw$Grid_Font*/
#define DW_INFO_FONT 2 
#define DW_PICK_FONT 5 /*$dw$pick_font -used for Lat/Long Position*/ 

#define CURSORS_STORED 3
#define DW_CURSOR0_NAME XC_cross_reverse
#define DW_CURSOR1_NAME XC_watch
#define DW_CURSOR2_NAME XC_top_left_arrow

#define DW_CURSOR_CROSSHAIR WinC.dw_cursor[0]/*DW$Cursor_CrossHair*/
#define DW_CURSOR_WATCH WinC.dw_cursor[1]
#define DW_CURSOR_ARROW WinC.dw_cursor[2]

/*draw program wide global structures */
XColor xcolor;
Colormap thecolormap;
int thescreen;
Visual *dw_visual; 
Display *dw_display; /* display structure - dw_display */
Screen *dw_screen;    /* screen structure - dw_screen */
Font dw_font[DW_FONTS]; /*Font indices-dw_font[$DW$fonts]*/ 


typedef struct { 
  float dw_center_lat;            
  float dw_center_lon;             
  int dw_screen_h;     /* screen Height - dw_screen_h */
  int dw_screen_w;     /* screen Width - dw_screen_w */
  int dw_pix_per_inch; /* screen resolution*/
  int dw_depth;        /* number of planes - was dw_depth */ 
  int dw_color_map;    /* Default Color Map */
  int dw_attr_mask;    /* attributes mask*/
  int dw_window[DW_WINDOWS]; /*window ids-dw_window[$dw$windows]*/
  int dw_pixmap[DW_PIXMAPS]; /*Pix Map ids-dw_pixmap[$dw$pixmaps]*/
  int dw_contexts[DW_CONTEXTS];/*MDM Context ids-joa 7/96(E1602)*/
  int dw_views[DW_VIEWS];    /* MDM View ids - joa 7/96 (E1602)*/
  int dw_region[DW_REGIONS]; /* Region ids*/
  int dw_color[2][DW_COLORS];/*Color indx-DW_color[0:1,$DW$colors]*/
  int dw_rgb[2][DW_COLORS][3];/*RGB Values-DW_RGB[0:1,$DW$colors,3]*/
  int dw_cursor[CURSORS_STORED];/* Cursor indices */ 
  int dw_font_h[DW_FONTS];/*Font Height-dw_font_h[$DW$fonts]*/
  int dw_font_w[DW_FONTS];/*Font Width-dw_font_w[$DW$fonts]*/
  int dw_stipple_count; /*Nbr of Stipple indices-DW_Stipple_Count */
  int dw_stipple[DW_STIPS];/*Stipple indices-DW_Stipple[$DW$Stips]*/
  int dw_stipple_h[DW_STIPS];/*Stipple hgt-DW_Stipple_H[$DW$Stips]*/
  int dw_stipple_w[DW_STIPS];/*Stipple width-DW_Stipple_W[$DW$Stips]*/
  int dw_current_x; /* DW_Current_X */
  int dw_current_y; /* DW_Current_Y */
  int dw_current_h; /* DW_Current_H */
  int dw_current_w; /* DW_Current_W */
  int dw_maximum_h; /* DW_Maximum_H */
  int dw_maximum_w; /* DW_Maximum_W */
  int dw_new_h; /* DW_New_H */
  int dw_new_w; /* DW_New_W */
  int dw_changed_hw;
  int dw_changed_view;/*to keep track of when view changed-joa 7/96 (E1602)*/
  int dw_inverse; /* DW_Inverse */
  int dw_dashed_line;
  int dw_widget_based;/*=0;*/ /* set to $YES if using UIL/Widgets*/

  int dw_map_pixmap;         /* 0 - DW_Map_Pixmap*/
  int dw_live_pixmap;        /* should be 1 - DW_Live_Pixmap */
  int dw_next_pixmap;        /* should be 1 - DW_Next_Pixmap */
  int dw_clip_bitmap;        /* joa 7/96 (E1602)*/

  int dw_map_context;        /* joa 7/96 (E1602) */
  int dw_live_context;       /* joa 7/96 (E1602) */
  int dw_next_context;       /* joa 7/96 (E1602) */

  int dw_wakeup_ef; /* DW_Wakeup_EF */

  int dw_window_name_len; /* DW_Window_Name_Len */
  int dw_display_name_len; /* DW_Display_Name_Len */
  int dw_unique_name_len; /* DW_Unique_Name_Len */

  int dw_text_color[DW_TEXT_LINES];/*DW_Text_Color*/
  int dw_tactical_font; /*Indx to Font for tactical use-DW_Tactical_Font */
  int dw_info_font; /*Indx to Font for Info Text use-DW_Info_Font*/ 
  int dw_in_progress; /* YES if between DRAW_Center & Draw_Done */
  int dw_radius;      /* Radius of tactical display*/
  int dw_ps;          /* Postscript Output Handle*/

  char dw_window_name[ARB];/* DW_Window_Name*64 */
  char dw_display_name[32]; /* DW_Display_Name*32 */
  char dw_unique_name[32]; /* DW_Unique_Name*32 */
  char dw_text[DW_TEXT_LINES][DW_TEXT_LENGTH];/*DW_Text*/
  } WinCommon; 
  
typedef struct {
  int w_swda_mask;
  XSetWindowAttributes w_swda;
} WinCommonSWDA;

typedef struct {
  int dw_GC_mask;
  int dw_clip_ones;
  int dw_clip_zeros;
  GC  dw_GC, dw_GC_2;
  XGCValues GC_values;
} WinCommonGC;

#define DW_MAX_POINTS 3000
typedef struct {
  int dw_pnt_count;
  int dw_pnt_x[DW_MAX_POINTS]; 
  int dw_pnt_y[DW_MAX_POINTS]; 
} CommonPointStruct;

/*should the value of DW_MAX_POINTS be the same as NTDS_MAX_Points?*/

typedef struct {
  int dw_pnt_count;
  XPoint dw_pnt[DW_MAX_POINTS]; /* defined in Xlib.h, short x, y*/
} WinCommonPoints;

#define NTDS_MAX_POINTS 300
typedef struct {
  int ntds_used, ntds_count, ntds_syms[30];
  int ntds_x[NTDS_MAX_POINTS], ntds_y[NTDS_MAX_POINTS],
    ntds_pen[NTDS_MAX_POINTS];
} NTDSCommon;	  

WinCommon WinC;
WinCommonSWDA ta_win_swda; 
WinCommonGC ta_win_GC;
WinCommonPoints WinComPts;
NTDSCommon NTDSsymDef;
