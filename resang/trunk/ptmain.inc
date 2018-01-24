
# Define Plot Common Area

define ($maxOBJ,200)
define ($maxBASE,50)
define ($maxLOB,100)
define ($maxBARR,30)
define ($maxXMARK,20)

define ($Map$Rivers    ,1)
define ($Map$Boundaries,2)
define ($Map$TextSize  ,3)
define ($Map$Terrain   ,4)
define ($Symbology$BasePos, 5)      # joa 7/96 (E1602)
define ($Symbology$NumBits, 2)      # joa 7/96 (E1602)

define (PLOTcommon,
    Literal iobs[10], ishobj[250], ishobx[250]
    integer istation, kstation, ifd, isbct, iobjct, kobjct, ibin, icirc
    integer idetch, kmap
    integer idelcsa,irotas,irotdn,irotindx,iselcsa
    integer evaldir[144]
    literal ctkcd
    real    qlat, qlon, dlat, dlon
    real*4 rotlat[13,9],rotlon[13,9],radialadd[9]
    real*4 imin, kmin
    integer*4   current_geo_symbology      # joa 7/96 (E1602)
    integer using_newgui
    Common /plot_com/ iobs, ishobj, ishobx, istation, kstation,
                     ifd, isbct, iobjct, kobjct, ibin, icirc, ctkcd,
                     qlat, qlon, dlat, dlon, idetch, imin, kmap, kmin,
                     irotindx,irotas,irotdn,iselcsa,idelcsa,rotlat,
                     rotlon,radialadd,evaldir,current_geo_symbology,using_newgui
    )

# following are used for the MDM graphics application routines - joa 7/96 (E1602)
define ($max$arrowhead$points, 10)

Define (Arrow$Common,
    Integer*4   Ac_Style                #
    Integer*4   Ac_Thickness            # Pixels
    Integer*4   Ac_Head_Length          # Pixels
    Integer*4   Ac_Line_Length          # Pixels
    Integer*4   Ac_Head_Offset          # Distance From Point To Tip Of Arrow
    Integer*4   Ac_Tail_Offset          # Distance From Butt To Computed Point
    Integer*4   Ac_Head_X               # Initial X/Y Location Pointed To
    Integer*4   Ac_Head_Y
    Integer*4   Ac_Tail_X               # Computed X/Y Location Pointed From
    Integer*4   Ac_Tail_Y
    Integer*4   Ac_Pattern              # code (solid fill)
    Integer*4   Ac_Pixmap          
               
    Common /Arrow_Common/ Ac_Style, Ac_Thickness,
                Ac_Head_Length, Ac_Line_Length,
                Ac_Head_Offset, Ac_Tail_Offset,
                Ac_Head_X, Ac_Head_Y, Ac_Tail_X, Ac_Tail_Y,
                Ac_Pattern, Ac_Pixmap
    )
               
define ($tb_max$lines,40)
define ($tb_max$line_len,80)
define ($tb$line_height,1.25)       # as a function of the Font Height
define ($tb$char_adjust,1.10)       # as a function of char width

Define (TextBlock$Common,
    Integer*4   tbc_font 
    Integer*4   tbc_box_color
    Integer*4   tbc_pixmap
               
    Common /TextBlock_Common/ Tbc_Font, Tbc_Box_Color, tbc_pixmap
    )
#-----------------------------------------------------------------------

