# ptdraw.inc
# defines for Plot Windows go here

#define ($incDECW, I%nclude 'SYS$LIBRARY:DECW$XLIBDEF')

define ($DW$Windows,2)
define ($DW$Pixmaps,2)
define ($DW$Contexts,2)       # DW_Contexts array dimension - joa 7/96 (E1602)
define ($DW$Views,10)         # DW_Views array dimension - joa 7/96 (E1602)
define ($DW$Regions,4)
define ($DW$Colors,16)
define ($DW$Text_Lines,16)
define ($DW$Text_Length,20)

define ($DW$default_X,100)      # default location and size of primary window
define ($DW$default_Y,100)
define ($DW$default_H,500)
define ($DW$default_W,600)

define ($DW$Minimum_H,200)      # smallest size of primary window
define ($DW$Minimum_W,300)

define ($DW$Maximum_H,1024)      # Largest size of primary window
define ($DW$Maximum_W,1280)

#define($DW$Info_H,300)         # default size of Text Info Window (14pt font)
define ($DW$Info_H,250)         # default size of Text Info Window (12pt font)
#define($DW$Info_W,120)         #       (fine for 5 byte names)
define ($DW$Info_W,130)         #       (E2141, 8-byte names) dew

define ($DW$Tactical,1)         # Index to Tactical Window
define ($DW$TextInfo,2)         # Index to Text Information Window

define ($DW$Stips,8)

define ($DW$Fonts,6)
define ($dw$font1_name,"-ADOBE-COURIER-Medium-R-Normal--*-140-*-*-M-*")
define ($dw$font2_name,"-ADOBE-COURIER-Medium-R-Normal--*-100-*-*-M-*")
define ($dw$font3_name,"-ADOBE-HELVETICA-Medium-R-Normal--*-120-75-75-P-*")
define ($dw$font4_name,"-ADOBE-HELVETICA-Medium-R-Normal--*-100-75-75-P-*")
define ($dw$font5_name,"-ADOBE-HELVETICA-Medium-R-Normal--*-140-75-75-P-*")
define ($dw$font6_name,"-ADOBE-HELVETICA-Medium-R-Normal--*-180-75-75-P-*")

#define ($dw$Small_Tactical_Font,3)
define ($dw$Small_Tactical_Font,4)
define ($dw$Medium_Tactical_Font,5)
define ($dw$Big_Tactical_Font,6)

define ($dw$Grid_Font,5)
define ($dw$Info_Font,3)
define ($dw$Pick_Font,6)        # used for Lat/Long Position 

define ($DW$Cursors,3)
define ($DW$Cursor1_name, x$c_cross_reverse_cursor)
define ($DW$Cursor2_name, x$c_watch_cursor)
define ($DW$Cursor3_name, x$c_top_left_arrow_cursor)

define (DW$Cursor_CrossHair,DW_Cursor[1])
define (DW$Cursor_Watch,DW_Cursor[2])
define (DW$Cursor_Arrow,DW_Cursor[3])


define (DECW$common,

    INTEGER*4 DW_Display                # display id
    INTEGER*4 DW_Screen                 # screen id
    INTEGER*4 DW_Screen_H               # screen Height
    INTEGER*4 DW_Screen_W               # screen Width
    INTEGER*4 DW_Pix_Per_Inch           # screen resolution
    INTEGER*4 DW_Depth                  # number of planes
    INTEGER*4 DW_Color_Map              # Default Color Map
    INTEGER*4 DW_ATTR_MASK              # attributes mask
    INTEGER*4 DW_Window[$DW$windows]    # window ids
    INTEGER*4 DW_Pixmap[$DW$pixmaps]    # Pix Map ids
    INTEGER*4 DW_Contexts[$DW$contexts] # MDM Context ids - joa 7/96 (E1602)
    INTEGER*4 DW_Views[$DW$views]       # MDM View ids - joa 7/96 (E1602)
    INTEGER*4 DW_Region[$DW$regions]    # Region ids
    INTEGER*4 DW_color[0:1,$DW$colors]  # Color indices
    INTEGER*4 DW_RGB[0:1,$DW$colors,3]  # RGB Values
    INTEGER*4 DW_cursor[$DW$cursors]    # Cursor indices
    INTEGER*4 DW_Font[$DW$fonts]        # Font indices
    INTEGER*4 DW_Font_H[$DW$fonts]      # Font Height
    INTEGER*4 DW_Font_W[$DW$fonts]      # Font Width
    INTEGER*4 DW_Stipple_Count          # Number of Stipple indices
    INTEGER*4 DW_Stipple[$DW$Stips]     # Stipple indices
    INTEGER*4 DW_Stipple_H[$DW$Stips]   # Stipple height
    INTEGER*4 DW_Stipple_W[$DW$Stips]   # Stipple width

    INTEGER*4 DW_Current_X
    INTEGER*4 DW_Current_Y
    INTEGER*4 DW_Current_H
    INTEGER*4 DW_Current_W
    INTEGER*4 DW_Maximum_H
    INTEGER*4 DW_Maximum_W
    INTEGER*4 DW_New_H
    INTEGER*4 DW_New_W
    INTEGER*4 DW_Changed_HW
    INTEGER*4 DW_Changed_View           # to keep track of when view changed - joa 7/96 (E1602)
    INTEGER*4 DW_Inverse
    INTEGER*4 DW_Dashed_Line
    INTEGER*4 DW_Widget_Based / 0 /     # set to $YES if using UIL/Widgets

    INTEGER*4 DW_Map_Pixmap             # 1
    INTEGER*4 DW_Live_Pixmap            # should be 2
    INTEGER*4 DW_Next_Pixmap            # should be 2
    INTEGER*4 DW_Clip_Bitmap            # joa 7/96 (E1602)

    INTEGER*4 DW_Map_Context            # joa 7/96 (E1602)
    INTEGER*4 DW_Live_Context           # joa 7/96 (E1602)
    INTEGER*4 DW_Next_Context           # joa 7/96 (E1602)

    INTEGER*4 DW_Wakeup_EF
    VaxCharacter    DW_Window_Name*64
    integer*4       DW_Window_Name_Len
    VaxCharacter    DW_Display_Name*32
    integer*4       DW_Display_Name_Len
    VaxCharacter    DW_Unique_Name*32
    integer*4       DW_Unique_Name_Len

    VAXCHARACTER*$DW$Text_Length DW_Text[$DW$Text_Lines]
    integer*4 DW_Text_Color[$DW$Text_Lines]

    INTEGER*4 DW_Tactical_Font          # Index to Font for tactical use
    INTEGER*4 DW_Info_Font              # Index to Font for Info Text use
    INTEGER*4 DW_In_Progress            # YES if between DRAW_Center & Draw_Done
    INTEGER*4 DW_Radius                 # Radius of tactical display
    REAL*4    DW_Center_Lat             # 
    REAL*4    DW_Center_Lon             # 
    Integer*4 DW_PS                     # Postscript Output Handle

    common / decw_Common / DW_Display, DW_Screen, 
            DW_Screen_H, DW_Screen_W, DW_Depth, DW_Color_Map, 
            DW_ATTR_MASK, DW_Window, DW_Pixmap, DW_Region,
            DW_Color, DW_Cursor, DW_Font, DW_Font_H, DW_Font_W,
            DW_Current_X, DW_Current_Y, DW_Current_H, DW_Current_W, 
            DW_Maximum_H, DW_Maximum_W, DW_Changed_HW,
            DW_Map_Pixmap, DW_Live_Pixmap, DW_Next_Pixmap,
            DW_Text, DW_Tactical_Font, DW_Info_Font, DW_Inverse,
            DW_Wakeup_EF, DW_Window_Name, DW_New_H, DW_New_W,
            DW_Pix_Per_Inch, DW_Text_Color, DW_Dashed_Line,
            DW_In_Progress, DW_Widget_Based,
            DW_Radius, DW_Center_Lat, DW_Center_Lon,
            DW_Window_Name_Len, DW_Display_Name, DW_Display_Name_Len,
            DW_Unique_Name, DW_Unique_Name_Len,
            DW_Stipple, DW_Stipple_H, DW_Stipple_W, DW_Stipple_Count,
            DW_PS, DW_RGB,
            DW_Contexts, DW_Views,
            DW_Map_Context, DW_Live_Context, DW_Next_Context,
            DW_Clip_Bitmap, DW_Changed_View
    )

define (dw$color,DW_Color[DW_Inverse,@1])

define (DECW$Common_Event,
    RECORD /X$EVENT/ DW_Event           # input event
    common /decw_event/ DW_Event
    )

define (DECW$Common_Visual,
    RECORD /X$VISUAL/ DW_VISUAL         # visual type
    common /decw_visual/ DW_Visual
    )

define (DECW$Common_SWDA,
    INTEGER*4   DW_SWDA_MASK
    RECORD /X$SET_WIN_ATTRIBUTES/ DW_SWDA   # used to create windows

    common /decw_visual/ DW_SWDA_Mask, DW_SWDA
    )

define (DECW$Common_GC,
    INTEGER*4   DW_GC_MASK
    INTEGER*4   DW_GC, DW_GC_2                  # added DW_GC_2 - joa 7/96 (E1602)
    INTEGER*4   DW_Clip_Ones, DW_Clip_Zeros     # joa 7/96 (E1602)
#    RECORD /X$GC_VALUES/ DW_GC_Values           # used to create GC

    common /decw_GCVL/ DW_GC_Mask, DW_GC, 
                       DW_GC_2, DW_Clip_Ones, DW_Clip_Zeros
                       #DW_GC_Values
    )

define (DW$Max_Points,3000)
define (DECW$Common_Points,
    INTEGER*4   DW_Pnt_Count
    #RECORD /x$point/ DW_Pnt[DW$Max_Points]      # used to draw lines
    integer*4   DW_Pnt_X[DW$Max_Points] 
    integer*4   DW_Pnt_Y[DW$Max_Points] 
    common /decw_pt/ DW_Pnt_count, DW_Pnt_X, DW_Pnt_y
    )

define (NTDS_Max_Points,300)
define (Ntds_common,
    integer*2   ntds_used, ntds_count, ntds_syms[30]
    logical*1   ntds_x[NTDS_Max_Points], 
                ntds_y[NTDS_Max_Points], 
                ntds_pen[NTDS_Max_Points]

    common /ntds_com/ ntds_used, ntds_count, ntds_syms, 
                      ntds_x, ntds_y, ntds_pen
    )

