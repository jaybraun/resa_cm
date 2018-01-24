/*
--#__________________________________________________________________
# Color Definitions (names ==> index)   used by ??
*/
#define COLOR_BLACK 1      /*(background) previously $Color$Black */
#define COLOR_ORANGE 2
#define COLOR_BROWN 3
#define COLOR_YELLOW 4
#define COLOR_GREY 5
#define COLOR_GREEN 6
#define COLOR_NAVYBLUE 7
#define COLOR_MUDGREEN 8
#define COLOR_DARKBLUE 9
#define COLOR_BLUE 10
#define COLOR_LIGHTPURPLE 11
#define COLOR_MAGENTA 12
#define COLOR_WHITE 13
#define COLOR_RED 14
#define COLOR_DARKRED 15
#define COLOR_OFFWHITE 16
/*_________________________________________________________________
  # Color Definitions (by name)    used by DECwindows  */
/* previously $Color1$name */
#define COLOR1_NAME  "Black"        /* Black (background) */
#define COLOR2_NAME  "OrangeRed"         /* Orange *//*better contrast on darkgrey land 8/26/15*/
#define COLOR3_NAME  "sandybrown"    /* Brown */
#define COLOR4_NAME  "yellow"        /* Yellow */
#define COLOR5_NAME  "DarkGrey"     /* Grey *//*better contrast with cyan 8/26/15*/
#define COLOR6_NAME  "LimeGreen"     /* Green */
#define COLOR7_NAME  "NavyBlue"      /* Navy-Blue */
#define COLOR8_NAME  "khaki"         /* Mud-Green */
#define COLOR9_NAME  "Blue"      /* Dark Blue */
#define COLOR10_NAME  "cyan"          /* Blue */
#define COLOR11_NAME "VioletRed"     /* Light-Purple */
#define COLOR12_NAME "Magenta"       /* Magenta */
#define COLOR13_NAME "White"         /* white (text) */
#define COLOR14_NAME "Red"           /* Red */
#define COLOR15_NAME "firebrick"     /* Dark Red */
#define COLOR16_NAME "Wheat"         /* off white */

/* Inverse set of colors - previous names followed convention 
   $XColor1$name */

#define INVCOLOR1_NAME  "White"        /* White (background) */
#define INVCOLOR2_NAME  "Coral"        /* Orange */
#define INVCOLOR3_NAME  "Grey6"        /* Brown */
#define INVCOLOR4_NAME  "Magenta"      /* Yellow */
#define INVCOLOR5_NAME  "LightGrey"    /* Grey */
#define INVCOLOR6_NAME  "LimeGreen"    /* Green */
#define INVCOLOR7_NAME  "NavyBlue"     /* Navy-Blue */
#define INVCOLOR8_NAME  "DarkGreen"    /* Mud-Green */
#define INVCOLOR9_NAME  "DarkSlateBlue"/* Dark Blue */
#define INVCOLOR10_NAME  "Blue"         /* Blue */
#define INVCOLOR11_NAME "DarkOrchid"   /* Light-Purple */
#define INVCOLOR12_NAME "orchid"       /* Magenta */
#define INVCOLOR13_NAME "Black"        /* Black (text) */
#define INVCOLOR14_NAME "OrangeRed"    /* Red */
#define INVCOLOR15_NAME "VioletRed"    /* Dark Red */
#define INVCOLOR16_NAME "grey48"       /* Dark Gray */


/*#_________________________________________________________________
  # Object Colors  */

#define COLOR_OBJ_ORANGE 2             /* use for objects on plot */
#define COLOR_OBJ_BLUE 10          /* use for objects on plot */
#define COLOR_OBJ_NEUTRAL COLOR_GREEN /* use for objects on plot - clk 4/94 (E1096)      Distinguish between */
#define COLOR_OBJ_UNKNOWN COLOR_OFFWHITE /* use for objects on plot - clk 4/94 (E1096)   neutral and unknown */
#define COLOR_OBJ_FADING 4             /* use for objects on plot */
#define COLOR_OBJ_SINK_B 14   /* use for objects on plot */
#define COLOR_OBJ_SINK_O 14   /* use for objects on plot */
#define COLOR_BACKGROUND COLOR_BLACK /*$Color$Background-Black(unless inversed)*/
#define COLOR_BORDER 15            /*$Color$Border- Dark Red */
#define COLOR_LAND 5               /*$Color$land- use land mass */
#define COLOR_RIVER 7              /*$color$river- use for rivers */
#define COLOR_BOUNDARIES 8 /*$color$boundaries- use for boundaries */
#define COLOR_CURSOR 4     /*$color$cursor use for cursor */
#define COLOR_TEXT 13          /*$color$text use for text and non-circ grids */
#define COLOR_XMARK 12                /* use for xmarks */

#define COLOR_BOX_YELLOW  4  /* use for yellow menu boxes */

/*__________________________________________________________________
# Text Lines  (numbered from the bottom up...)
*/
#define LINE_HOOK_1 15  /*these added for X-Window version*/
#define LINE_HOOK_2 14  /*$line$hook_2 */
#define LINE_HOOK_3 13
#define LINE_HOOK_4 12
#define LINE_HOOK_5 11
#define LINE_HOOK_6 10

#define LINE_POS_1 9
#define LINE_POS_2 8
#define LINE_POS_3 7
#define LINE_POS_4 6    // joa 7/96 (E1602)

#define LINE_VIEW 5     /*$Line$View*/
#define LINE_RADIUS 4   /*$Line$Radius*/
#define LINE_GAMETIME 3 /*$Line$GameTime*/
#define LINE_DAYNITE 2  /*$Line$DayNite*/
#define LINE_BEARING 1  /*$Line$Bearing*/
#define LINE_RANGE 0    /*$Line$Range*/


/*-------------------------------------------------------------------
# The following allows definition of dashes for specific polygons
*/

#define POLY_SOLID_CODE 0 /*Solid MUST be zero*/
#define POLY_DASHED_CODE 3 /*1 wasn't distinct enough a dash in Linux*/
#define POLY_LONGDASH_CODE 2
#define POLY_DOT_CODE (-1)
#define POLY_DOT2_CODE (-2)
#define POLY_DOT3_CODE (-3)
#define POLY_DOT4_CODE (-4)
#define POLY_DOT5_CODE (-5) /* trs 9/30/91*/
#define POLY_DOT6_CODE (-6) /* trs 9/30/91*/

