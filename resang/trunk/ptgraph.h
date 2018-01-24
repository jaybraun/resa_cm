#define MAX_MAP_POINTS 250000
#define MAPTBL_SIZE MAX_MAP_POINTS/5

#define BOUNDARIES 7777 /* country boundaries */

#define RIVERS      8888  /* rivers */
#define LAND        9999  /* land mass */
#define USER_HI     9916  /* highest user-def. color */
#define USER_LO     9901  /* lowest  user-def. color */
#define USER_BASE   9900  /* excess over color number */
#define USER_HI_D   9816  /* highest DASHED user-def. color */
#define USER_LO_D   9801  /* lowest  DASHED user-def. color */
#define USER_BASE_D 9800 /*excess over DASHED color number */

typedef struct {
  int geo_display, geo_rivers, geo_boundaries, geo_textsize;
  int geo_radius;
  int maptbl_ptr,mapdef_ptr;
  float xl, yb, xr, yt;
} GPH_common;

typedef struct {
  int kseg,lseg,iseg;
} GPH1_common;

typedef struct {
  float mapdef[2][MAX_MAP_POINTS];
  int maptbl[MAPTBL_SIZE];
  char pagefill[2][512];
  int buffer_end;
} GPH2_common;

#define AIR_TYPE 1 /* $$$AIR */
#define SUB_TYPE 2
#define SURF_TYPE 3
#define LAND_TYPE 4 /*added 1/91, for plotting and erasing bases*/
#define BOAT_TYPE 5 /*added 8/91, for plotting and erasing boats*/
#define SITE_TYPE 6 /*added 7/92, for plotting and erasing sites*/
#define MISS_TYPE 7 /*added 1/94, for plotting SAM missiles*/
#define VEHICLE_TYPE 8
#define MAX_TYPE 9
/* Note: Ground types are handled separately - clk 9/95*/

#define ENEMY_SIDE 1
#define FRIENDLY_SIDE 2
#define NEUTRAL_SIDE 3 
#define UNKNOWN_SIDE 4
#define MAX_SIDE 5

#define ORANGE_SIDE 1
#define BLUE_SIDE 2

#define NTDS_ENEMY_AIR 1 /*$NTDS$Enemy$AIR*/
#define NTDS_ENEMY_SUB 2
#define NTDS_ENEMY_SURF 3 
#define NTDS_FRIENDLY_AIR 4 
#define NTDS_FRIENDLY_SUB 5 
#define NTDS_FRIENDLY_SURF 6 
#define NTDS_UNKNOWN_AIR 7 
#define NTDS_UNKNOWN_SUB 8 
#define NTDS_UNKNOWN_SURF 9 
#define NTDS_STYLE_B 10
#define NTDS_STYLE_O 11 
#define NTDS_STYLE_N 12
#define NTDS_XMARK 13
#define NTDS_BASE 14 
#define NTDS_SITE 15   /* Hide Sites, cpa 7/92 */
#define NTDS_NRT_MARK 16
#define NTDS_MISSILE 17  /*advanced SAM,  SMM 1/94 */
#define NTDS_LOCAL_L 18  /*clk 12/95(E1469)*/
#define NTDS_GROUND_ADA 19 /*clk 9/95(E1456)-Generic Air Defense (within the box)*/
#define NTDS_GROUND_BOX 20 /*clk 9/95(E1456)-Generic Ground Box (basic box)*/
#define NTDS_GROUND_BATTALION 21 /*clk 9/95(E1456)-Ground Battalion Box */
#define NTDS_GROUND_BRIGADE 22 /*clk 9/95(E1456)-Ground Brigade Box*/
#define NTDS_GROUND_COMPANY 23 /*clk 9/95 (E1456)-Ground Company Box*/
#define NTDS_GROUND_CORPS 24 /*clk 9/95 (E1456)-Ground Corps Box*/
#define NTDS_GROUND_DIVISION 25/*clk 9/95 (E1456)-Ground Division Box*/
#define NTDS_GROUND_PLATOON 26/*clk 9/95 (E1456)-Ground Platoon Box*/ 
#define NTDS_GROUND_REGIMENT 27/*clk 9/95 (E1456)-Ground Regiment Box*/
#define NTDS_GROUND_SECTION 28/*clk 9/95 (E1456)-Ground Section Box*/
#define NTDS_GROUND_SQUAD 29/*clk 9/95 (E1456)-Ground Squad Box*/

