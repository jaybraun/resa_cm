#______________________________________________________________________
# Color Definitions (hue,whitness,blackness)   used by MEGATEK

define ($Color1$def,0,0,100)            # Black (background)
define ($Color2$def,30,20,0)            # Orange
define ($Color3$def,37,0,35)            # Brown
define ($Color4$def,62,0,10)            # Yellow
define ($Color5$def,15,100,40)          # Grey
define ($Color6$def,140,0,45)           # Green
define ($color7$def,240,0,75)           # Navy-Blue
define ($color8$def,55,0,55)            # Mud-Green
define ($color9$def,240,0,15)           # Dark Blue
define ($color10$def,195,20,0)          # Blue
define ($color11$def,275,0,0)           # Light-Purple
define ($color12$def,295,0,0)           # Magenta
define ($color13$def,0,100,0)           # white (text)
define ($color14$def,0,0,0)             # Red
define ($color15$def,0,0,30)            # Dark Red
define ($color16$def,0,85,0)            # off white

#______________________________________________________________________
# Color Definitions (names ==> index)   used by ??

define ($Color$Black        ,1)     # (background)
define ($Color$Orange       ,2)
define ($Color$Brown        ,3)
define ($Color$Yellow       ,4)
define ($Color$Grey         ,5)
define ($Color$Green        ,6)
define ($color$Navy_Blue    ,7)
define ($color$Mud_Green    ,8)
define ($color$Dark_Blue    ,9)
define ($color$Blue         ,10)
define ($color$Light_Purple ,11)
define ($color$Magenta      ,12)
define ($color$White        ,13)
define ($color$Red          ,14)
define ($color$Dark_Red     ,15)
define ($color$Off_white    ,16)


#___________________________________________________________________________
# Color Definitions (hue,lightness,saturation)   used by RAMTEK & Tectronics...
#                                                and Postscript...

define ($Color1$hls ,  0,  0,  0)       # Black (background)
define ($Color2$hls , 30, 50,100)       # Orange
define ($Color3$hls , 37, 50, 65)       # Brown
define ($Color4$hls , 60, 50,100)       # Yellow
define ($Color5$hls , 15, 70,  0)       # Grey
define ($Color6$hls ,140, 50, 55)       # Green
define ($color7$hls ,240, 50, 25)       # Navy-Blue
define ($color8$hls , 55, 50, 45)       # Mud-Green
define ($color9$hls ,240, 50, 85)       # Dark Blue
define ($color10$hls,195, 60,100)       # Blue
define ($color11$hls,275, 50,100)       # Light-Purple
define ($color12$hls,295, 50,100)       # Magenta
define ($color13$hls,  0,100,100)       # white (text)
define ($color14$hls,  0, 50,100)       # Red
define ($color15$hls,  0, 50, 70)       # Dark Red
define ($color16$hls,  0, 94,100)       # off white


#___________________________________________________________________________
# Color Definitions (Red,Green,Blue) used by Postscript
# These values were derived from the X names via an eXodus Color Editor
# (values range from 0...65535)

define ($Color1$rgb , 65535, 65535, 65280)       # White (background)
define ($Color2$rgb , 65535, 32512,     0)       # Coral (bad guys)
define ($Color3$rgb ,  3840,  3840,  3559)       # grey6 (coastlines)
define ($Color4$rgb , 65535,     0, 65280)       # magenta (fading tracks)
define ($Color5$rgb , 43008, 43008, 43176)       # LightGrey (chaff)
define ($Color6$rgb , 12800, 52240, 12850)       # LimeGreen (lobs)
define ($color7$rgb ,  8960,  8960, 36494)       # NavyBlue (rivers)
define ($color8$rgb , 12032, 20224, 12079)       # DarkGreen (boundaries,grids)
define ($color9$rgb , 27392,  8960, 36352)       # DarkSlateBlue 
define ($color10$rgb,     0,     0, 65280)       # blue (good guys)
define ($color11$rgb, 39168, 12800, 52428)       # DarkOrchid (ROTHR,elipises)
define ($color12$rgb, 56064, 28672, 56064)       # orchid (X-Marks, ESM Lobs)
define ($color13$rgb,     0,     0,   112)       # Black (text)
define ($color14$rgb, 65535,     0, 32639)       # OrangeRed (SOSUS, sinking)
define ($color15$rgb, 52224, 12800, 39321)       # VioletRed 
define ($color16$rgb, 31232, 31232, 31379)       # grey48 (neutral)

#___________________________________________________________________________
# Color Definitions (by name)    used by DECwindows

define ($Color1$name ,"grey12")         # Black (background)
define ($Color2$name ,"Coral")          # Orange
define ($Color3$name ,"sandybrown")     # Brown
define ($Color4$name ,"yellow")         # Yellow
define ($Color5$name ,"LightGrey")      # Grey
define ($Color6$name ,"LimeGreen")      # Green
define ($color7$name ,"NavyBlue")       # Navy-Blue
define ($color8$name ,"khaki")          # Mud-Green
define ($color9$name ,"DECWBlue")       # Dark Blue
define ($color10$name,"cyan")           # Blue
define ($color11$name,"VioletRed")      # Light-Purple
define ($color12$name,"Magenta")        # Magenta
define ($color13$name,"White")          # white (text)
define ($color14$name,"Red")            # Red
define ($color15$name,"firebrick")      # Dark Red
define ($color16$name,"Wheat")          # off white

# Inverse set of colors

define ($XColor1$name ,"White")         # White (background)
define ($XColor2$name ,"Coral")         # Orange
define ($XColor3$name ,"grey6")         # Brown
define ($XColor4$name ,"magenta")       # Yellow
define ($XColor5$name ,"LightGrey")     # Grey
define ($XColor6$name ,"LimeGreen")     # Green
define ($Xcolor7$name ,"NavyBlue")      # Navy-Blue
define ($Xcolor8$name ,"DarkGreen")     # Mud-Green
define ($Xcolor9$name ,"DarkSlateBlue") # Dark Blue
define ($Xcolor10$name,"blue")          # Blue
define ($Xcolor11$name,"DarkOrchid")    # Light-Purple
define ($Xcolor12$name,"orchid")        # Magenta
define ($Xcolor13$name,"Black")         # Black (text)
define ($Xcolor14$name,"OrangeRed")     # Red
define ($Xcolor15$name,"VioletRed")     # Dark Red
define ($Xcolor16$name,"grey48")        # Dark Gray



#______________________________________________________________________
# Object Colors

# Color codes revised for Linux
define ($color$obj$orange ,2)           # use for objects on plot
define ($color$obj$blue   ,10)          # use for objects on plot
define ($color$obj$neutral,$Color$Green)# use for objects on plot - clk 4/94 (E1096)        Distinguish between
define ($color$obj$unknown,$Color$Off_white) # use for objects on plot - clk 4/94 (E1096)   neutral and unknown
define ($color$obj$fading ,4)           # use for objects on plot
define ($color$obj$sink$B ,14)          # use for objects on plot
define ($color$obj$sink$O ,14)          # use for objects on plot
define ($color$background ,1)           # Black (unless inversed)
define ($color$Border     ,15)          # Dark Red
define ($Color$land       ,5)           # use land mass
define ($color$river      ,7)           # use for rivers
define ($color$Boundaries ,8)           # use for boundaries
define ($color$cursor     ,4)           # use for cursor
define ($color$text       ,13)          # use for text and non-circ grids
define ($color$xmark      ,12)          # use for xmarks
define ($color$grid       ,8)           # use for spider grids
define ($color$envelope   ,10)          # use for envelopes
define ($color$circle     ,10)          # use for single circle
define ($color$Route      ,6)           # use for routes
define ($default$Route$Color,$color$route)
define ($color$Sosus      ,14)          # use for sosus regions
define ($color$Minefield  ,4)           # use for Mine Fields
define ($color$SweepArea  ,8)           # use for Sweep Areas
define ($color$LandZone   ,8)           # use for landing zones
define ($color$Landing$Zone ,$color$LandZone)
define ($color$SonarEnv   ,6)           # use for sonar env regions
define ($color$SurvSat    ,14)          # use for survsat tracks
define ($color$HFDF       ,9)           # use for HFDF tracks     # khc, 12/92
define ($Color$PIM$Bmark  ,10)          # use for blue PIM xmarks
define ($Color$PIM$Omark  ,2)           # use for orange PIM xmarks
define ($Color$PIM$Nmark  ,16)          # use for neutral PIM xmarks
define ($Color$PIM$Line   ,8)           # use for PIM lines
define ($Color$NUC$CIRCLE ,14)          # use for NUCLEAR EXPOSIONS
define ($Color$LIDAR      ,11)          # use for LIDAR regions
define ($Color$SWABS      ,13)          # use for SWABS regions
define ($Color$LFA        ,11)          # Use for Bistatic Elipsis  dew 8/91
define ($Color$CruiseMissile, $Color$Magenta)    # detected cruise missiles (E1064/E1435)

define ($color$Orange$SPA ,2)           # use for orange SPAs   clk 10/1/91
define ($default$Orange$SPA$Color, $color$Orange$SPA)
define ($color$Blue$SPA   ,10)          # use for blue SPAs
define ($default$Blue$SPA$Color, $color$Blue$SPA)

define ($color$lob$ESM    ,12)          # use for ESM lines
define ($color$lob$sonar  ,6)           # use for sonar lines
define ($color$lob$jam    ,4)           # ESM jammer lines - JFR 7/27/94
define ($color$bar$chaff  ,5)           # use for chaff barriers
define ($color$bar$sonar  ,6)           # use for sonar barriers

define ($color$box$yellow ,4)           # use for yellow menu boxes
define ($color$box$blue   ,9)           # use for blue menu boxes
define ($color$box$green  ,6)           # use for green menu boxes
define ($color$box$red    ,15)          # use for red menu boxes

define ($Color$Purple     ,11)          # "Light Purple"

define ($Color$Rothr      ,11)          
define ($Color$Rothr$Blink,13)          

# New colors for EMI impacts...

define ($Color$Ant_Area    ,$color$White) # use for EMI Affected By Areas
define ($Color$EMI_Area    ,$color$Red)   # use for EMI Affected By Areas
define ($Color$EMI_Area_In ,$color$Green) # use for EMI Affected By Areas
define ($Color$Victim      ,$color$Green)
define ($Color$Target$NoEmi,$color$Green)
define ($Color$Target$WEmi ,$color$Yellow)
define ($Color$Target$Lost ,$color$Red)
define ($Color$Culprit     ,$color$Magenta)


#______________________________________________________________________
# Text Lines  (numbered from the bottom up...)

define ($Line$Hook_1    ,15)        # these added for X-Window version
define ($Line$Hook_2    ,14)
define ($Line$Hook_3    ,13)
define ($Line$Hook_4    ,12)
define ($Line$Hook_5    ,11)
define ($Line$Hook_6    ,10)

define ($Line$Pos_1    ,9)
define ($Line$Pos_2    ,8)
define ($Line$Pos_3    ,7)
define ($Line$Pos_4    ,6)     # joa 7/96 (E1602)

define ($Line$View      ,5)
define ($Line$Radius    ,4)
define ($Line$GameTime  ,3)
define ($Line$DayNite   ,2)
define ($Line$Bearing   ,1)
define ($Line$Range     ,0)


#______________________________________________________________________
# Plot Symbols   (Code number matches order in ptsymbols.dat)

define($NTDS$Enemy$AIR,1)
define($NTDS$Enemy$SUB,2)
define($NTDS$Enemy$SURF,3)

define($NTDS$Friendly$AIR,4)
define($NTDS$Friendly$SUB,5)
define($NTDS$Friendly$SURF,6)

define($NTDS$Unknown$AIR,7)
define($NTDS$Unknown$SUB,8)
define($NTDS$Unknown$SURF,9)

define($NTDS$Style$B,10)
define($NTDS$Style$O,11)
define($NTDS$Style$N,12)

define($NTDS$XMark,13)

define($NTDS$Base,14)
define($NTDS$Site,15)     # Hide Sites, cpa 7/92

define($NTDS$NRT$Mark,16)

define($NTDS$Missile,17) # advanced SAM,  SMM 1/94

define($NTDS$Local$L, 18)   # clk 12/95 (E1469)

define($NTDS$Ground_ADA, 19)         # clk 9/95 (E1456) - Generic Air Defense (within the box)
define($NTDS$Ground_Box, 20)         # clk 9/95 (E1456) - Generic Ground Box (basic box)
define($NTDS$Ground_Battalion, 21)   # clk 9/95 (E1456) - Ground Battalion Box
define($NTDS$Ground_Brigade, 22)     # clk 9/95 (E1456) - Ground Brigade Box
define($NTDS$Ground_Company, 23)     # clk 9/95 (E1456) - Ground Company Box
define($NTDS$Ground_Corps, 24)       # clk 9/95 (E1456) - Ground Corps Box
define($NTDS$Ground_Division, 25)    # clk 9/95 (E1456) - Ground Division Box
define($NTDS$Ground_Platoon, 26)     # clk 9/95 (E1456) - Ground Platoon Box
define($NTDS$Ground_Regiment, 27)    # clk 9/95 (E1456) - Ground Regiment Box
define($NTDS$Ground_Section, 28)     # clk 9/95 (E1456) - Ground Section Box
define($NTDS$Ground_Squad, 29)       # clk 9/95 (E1456) - Ground Squad Box
#______________________________________________________________________
# Polygon types

define($poly$history    ,1)
define($poly$barrier    ,2)
define($poly$sosus      ,3)
define($poly$sonar      ,4)
define($poly$grid       ,5)
define($poly$pim        ,6)
define($Poly$NucBlast   ,7)
define($Poly$NucCircle  ,8)
define($Poly$SQUARE     ,9)             # jb                    6/15/87
define($Poly$MineAct    ,10)            # jb                    8/4/87
define($Poly$MineDact   ,11)            # jb                    8/4/87
define($Poly$B_MineField,10)            # jb                    8/4/87
define($Poly$O_MineField,11)            # jb                    8/4/87
define($Poly$Rothr      ,12)            # DEW 3/26/90
define($Poly$LIDAR      ,13)            # SMM/CLK 2/26/91
define($Poly$B_SWABS    ,14)            # SMM/CLK 3/7/91
define($Poly$O_SWABS    ,15)            # SMM/CLK 3/7/91
define($Poly$LFA        ,16)            # dew 8/13/91
define($Poly$ActBarrier ,17)            # cml 8/14/91
define($Poly$SolidTrack ,18)            # trs 9/30/91
define($Poly$DottedTrack,19)            # trs 9/30/91
define($Poly$DashedTrack,20)            # trs 9/30/91
define($Poly$SPA        ,21)            # clk 10/91
define($Poly$JammerESM  ,22)            # JFR 9/28/94 for jammer alertment per 1147

#----------------------------------------------------------------------
# The following allows definition of dashes for specific polygons

Define($Poly$Types$Max   ,23)       # this number must be >= largest code above

Define($Poly$Solid$Code         ,0)        # Solid
Define($Poly$Dashed$Code     ,1)        # 8-4-2-4
Define($Poly$LongDash$Code ,2)        # 16-8-4-8
Define($Poly$Dot$Code            ,-1)      # 1-1
Define($Poly$Dot2$Code          ,-2)      # 2-2
Define($Poly$Dot3$Code          ,-3)      # 3-3
Define($Poly$Dot4$Code          ,-4)      # 4-4
Define($Poly$Dot5$Code          ,-5)      # 5-5 
Define($Poly$Dot6$Code           ,-6)     # 6-6

Define($Poly$Styles, 
    Integer*4   Poly_Line_Style [ $Poly$Types$Max ]

    Data    Poly_Line_Style [ $Poly$History     ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$Barrier     ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$Sosus       ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$Sonar       ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$Grid        ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$PIM         ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$Nucblast    ] / $Poly$Dashed$Code /
    Data    Poly_Line_Style [ $Poly$Nuccircle   ] / $Poly$Dashed$Code /
    Data    Poly_Line_Style [ $Poly$SQUARE      ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$MineAct     ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$MineDact    ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$B_Minefield ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$O_Minefield ] / $Poly$Dashed$Code /
    Data    Poly_Line_Style [ $Poly$Rothr       ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$PIM         ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$LIDAR       ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$B_Swabs     ] / $Poly$Dashed$Code /
    Data    Poly_Line_Style [ $Poly$O_Swabs     ] / $Poly$Dashed$Code /
    Data    Poly_Line_Style [ $Poly$LFA         ] / $Poly$Dot$Code    /
    Data    Poly_Line_Style [ $Poly$ActBarrier  ] / $Poly$Dashed$Code /
    Data    Poly_Line_Style [ $Poly$SolidTrack  ] / $Poly$Solid$Code  / 
    Data    Poly_Line_Style [ $Poly$DottedTrack ] / $Poly$Dot2$Code   / 
    Data    Poly_Line_Style [ $Poly$DashedTrack ] / $Poly$Dot6$Code   / 
    Data    Poly_Line_Style [ $Poly$SPA         ] / $Poly$Solid$Code  /
    Data    Poly_Line_Style [ $Poly$JammerESM   ] / $Poly$Dashed$Code / 
)

define($Poly$Line$Style, Poly_Line_Style [ @1 ] )

