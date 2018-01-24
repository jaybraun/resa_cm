# SYSTEM      AS      "AirSpace definitions"
# TABLE       SHP     "Shape"
# DESCRIPTION "Shape parameters needed from ACO to define airspace"
# 
# ENTRY_SIZE       134
# SLOTS            2000
# ZERO
# ;
# 
# SUBTABLE        POS     "Positions"
# DESCRIPTION             "  "
# SLOTS           60              
# 
# ENTRY_SIZE      2
# TABLE_OFFSET    15
# ;
# define(SHP$POS$Address1,(1+14+(SHP$POS$Index-1)*2)+1);
# 
# # Shape codes
# define (shape$line$code      ,1)
# define (shape$point$code     ,2)
# define (shape$circle$code    ,3)
# define (shape$corridor$code  ,4)
# define (shape$orbit$code     ,5)
# define (shape$polyarc$code   ,6)
# define (shape$polygon$code   ,7)
# define (shape$radarc$code    ,8)
# define (shape$track$code     ,9)
# define (shape$unknown$code   ,10)
# 
# # Vertical Dimension Type Codes
# define (verticaltype$BRRA,1)           # Base reference point to relative altitude
# define (verticaltype$BRFL,2)           # Base reference point to flight level
# define (verticaltype$RAFL,3)           # relative altitude to flight level
# define (verticaltype$RARA,4)           # relative altitude to relative altitude
# define (verticaltype$FLFL,5)           # flight level to flight level
# 
# # Altitude base reference point
# define (AltitudeRef$GL ,1)              # at Ground level
# define (AltitudeRef$MSL,2)             # at Mean sea level
# 
# # Altitude unit of measurement indicator
# define (AltitudeRef$AGL ,1)              # above Ground level
# define (AltitudeRef$AMSL,2)             # above Mean sea level
# ;
# FORMAT
# Field                : Title                          :Type: Word :  Pos : Size : Range              :                          ;
# #-------------------  -------------------------------  ----  ----   ----   ----  -------------------  ------------------------- 
# Name                 :"Airspace name"                 : V  :   1  :   1  :  24 :                     :                          ;
# ShapeCode            :"Shape Code"                    : I  :   7  :   0  :   4 : (1-10)              :                          ;
# VerticalType         :"Vertical dimension type code"  : I  :   7  :   4  :   3 : (0-5)               :                          ;
# AltitudeRef          :"Altitude reference code"       : I  :   7  :   7  :   2 : (0-2)               :                          ;
# AltitudeIndicator    :"Altitude unit of measurement"  : I  :   7  :   9  :   2 : (0-2)               :                          ;
# Radius               :"Radius(nm)"                    : I  :   7  :  11  :   8 : (0-255)             :                          ;
# OuterRadius          :"Outer radius for radarc (nm)"  : I  :   7  :  11  :   8 : (0-255)             :                          ;
# Range                :"Range(nm)"                     : I  :   7  :  11  :   8 : (0-255)             :                          ;
# NPos                 :"Number of positions"           : I  :   7  :  19  :   5 : (0-60)              :                          ;
# BeginBearing         :"arc begin Brg (deg)"           : I  :   8  :   0  :   9 : (0-360)             :                          ;
# EndBearing           :"arc end Brg (deg)"             : I  :   8  :   9  :   9 : (0-360)             :                          ;
# InnerRadius          :"Inner radius for radarc (nm)"  : I  :   8  :  18  :   8 : (0-255)             :                          ;
# Use                  :"Acronym for use"               : C  :   9  :   1  :   8 :                     :                          ;
# BottomAltitude       :"Bottom Altitude(ft)"           : I  :  11  :   0  :Word :                     :                          ;
# TopAltitude          :"Top Altitude(ft)"              : I  :  12  :   0  :Word :                     :                          ;
# PeriodStartTime      :"Start time(DTG)"               : I  :  13  :   0  :Word :                     :                          ;
# PeriodStopTime       :"Stop time(DTG)"                : I  :  14  :   0  :Word :                     :                          ;
# POS$Latitude         :"Latitude(radians)"             : F  : POS  :   0  :Word :                     :                          ;
# POS$Longitude        :"Longitude(radians)"            : F  : POS+1:   0  :Word :                     :                          ;




define(SHP$slots,2000)
define(SHP$size,IASP[$$PtrSize,$$SHP])
define(SHP$used,IASP[$$PtrUsed,$$SHP])
define(SHP$base,IASP[$$PtrBase,$$SHP])
define(SHP$last,(SHP$Base+SHP$Used-1))
define(SHP$Last$to,SHP$Used=1-SHP$Base+)

define(SHP$Pointer,kPoint_SHP)
define(SHP$Entry,134)
define(SHP$Pointer$First,SHP$pointer=SHP$Base)
define(SHP$Pointer$Next,SHP$pointer=SHP$pointer+SHP$Entry)
define(SHP$Pointer$Ok,(SHP$Pointer>=SHP$Base & SHP$Pointer<=SHP$Last))
define(SHP$Pointer$Valid,(SHP$Pointer>=SHP$Base & SHP$Pointer<(SHP$Base+SHP$Size)))
define(SHP$Pointer$Index,((SHP$Pointer-SHP$Base)/SHP$Entry+1))
define(SHP$Pointer$To,SHP$Pointer$First-SHP$Entry+ZEXT(SHP$Entry)*)
define(SHP$Zero,{
    call LIB$MOVC5(0,0,0,SHP$Entry*4,iAS[SHP$Pointer])
   })
define(SHP$Zero$Declare,{
   })

define(SHP$POS$slots,60)
define(SHP$POS$index,Index_SHP_POS)
define(SHP$POS$index$ok,(SHP$POS$Index>0&SHP$POS$Index<=SHP$POS$Slots))
define(SHP$POS$index$Valid,(SHP$POS$Index>0&SHP$POS$Index<=100))
define(SHP$POS$Address,(SHP$Pointer+14+(SHP$POS$Index-1)*2))

define(SHP$POS$Address1,(1+14+(SHP$POS$Index-1)*2)+1)
define(shape$line$code,1)
define(shape$point$code,2)
define(shape$circle$code,3)
define(shape$corridor$code,4)
define(shape$orbit$code,5)
define(shape$polyarc$code,6)
define(shape$polygon$code,7)
define(shape$radarc$code,8)
define(shape$track$code,9)
define(shape$unknown$code,10)
define(verticaltype$BRRA,1)
define(verticaltype$BRFL,2)
define(verticaltype$RAFL,3)
define(verticaltype$RARA,4)
define(verticaltype$FLFL,5)
define(AltitudeRef$GL,1)
define(AltitudeRef$MSL,2)
define(AltitudeRef$AGL,1)
define(AltitudeRef$AMSL,2)
define($Size_SHP$Name$V,24)
define($size_v_SHP_Name,$Size_SHP$Name$V)

define(SHP$Name$V,V_SHP_Name)
define(xSHP$Name$V,$$ExtVaxC(IAS[SHP$Pointer],1,24))
define(putSHP$Name$V,$$InsVaxC(ifelse(@1,,SHP$Name$V,@1),IAS[SHP$Pointer],1,24))

define(SHP$ShapeCode$I,I_SHP_ShapeCode)
define(xSHP$ShapeCode$I,($$extract(IAS[SHP$Pointer+6],0,M4bits)))
define(putSHP$ShapeCode$I,$$insert((ifelse(@1,,SHP$ShapeCode$I,@1)),IAS[SHP$Pointer+6],0,M4bits))

define(SHP$VerticalType$I,I_SHP_VerticalType)
define(xSHP$VerticalType$I,($$extract(IAS[SHP$Pointer+6],4,M3bits)))
define(putSHP$VerticalType$I,$$insert((ifelse(@1,,SHP$VerticalType$I,@1)),IAS[SHP$Pointer+6],4,M3bits))

define(SHP$AltitudeRef$I,I_SHP_AltitudeRef)
define(xSHP$AltitudeRef$I,($$extract(IAS[SHP$Pointer+6],7,M2bits)))
define(putSHP$AltitudeRef$I,$$insert((ifelse(@1,,SHP$AltitudeRef$I,@1)),IAS[SHP$Pointer+6],7,M2bits))

define(SHP$AltitudeIndicator$I,I_SHP_AltitudeIndicator)
define(xSHP$AltitudeIndicator$I,($$extract(IAS[SHP$Pointer+6],9,M2bits)))
define(putSHP$AltitudeIndicator$I,$$insert((ifelse(@1,,SHP$AltitudeIndicator$I,@1)),IAS[SHP$Pointer+6],9,M2bits))

define(SHP$Radius$I,I_SHP_Radius)
define(xSHP$Radius$I,($$extract(IAS[SHP$Pointer+6],11,M8bits)))
define(putSHP$Radius$I,$$insert((ifelse(@1,,SHP$Radius$I,@1)),IAS[SHP$Pointer+6],11,M8bits))

define(SHP$OuterRadius$I,I_SHP_OuterRadius)
define(xSHP$OuterRadius$I,($$extract(IAS[SHP$Pointer+6],11,M8bits)))
define(putSHP$OuterRadius$I,$$insert((ifelse(@1,,SHP$OuterRadius$I,@1)),IAS[SHP$Pointer+6],11,M8bits))

define(SHP$Range$I,I_SHP_Range)
define(xSHP$Range$I,($$extract(IAS[SHP$Pointer+6],11,M8bits)))
define(putSHP$Range$I,$$insert((ifelse(@1,,SHP$Range$I,@1)),IAS[SHP$Pointer+6],11,M8bits))

define(SHP$NPos$I,I_SHP_NPos)
define(xSHP$NPos$I,($$extract(IAS[SHP$Pointer+6],19,M5bits)))
define(putSHP$NPos$I,$$insert((ifelse(@1,,SHP$NPos$I,@1)),IAS[SHP$Pointer+6],19,M5bits))

define(SHP$BeginBearing$I,I_SHP_BeginBearing)
define(xSHP$BeginBearing$I,($$extract(IAS[SHP$Pointer+7],0,M9bits)))
define(putSHP$BeginBearing$I,$$insert((ifelse(@1,,SHP$BeginBearing$I,@1)),IAS[SHP$Pointer+7],0,M9bits))

define(SHP$EndBearing$I,I_SHP_EndBearing)
define(xSHP$EndBearing$I,($$extract(IAS[SHP$Pointer+7],9,M9bits)))
define(putSHP$EndBearing$I,$$insert((ifelse(@1,,SHP$EndBearing$I,@1)),IAS[SHP$Pointer+7],9,M9bits))

define(SHP$InnerRadius$I,I_SHP_InnerRadius)
define(xSHP$InnerRadius$I,($$extract(IAS[SHP$Pointer+7],18,M8bits)))
define(putSHP$InnerRadius$I,$$insert((ifelse(@1,,SHP$InnerRadius$I,@1)),IAS[SHP$Pointer+7],18,M8bits))

define(SHP$Use$C,C_SHP_Use)
define(xSHP$Use$C,$$ExtLit(IAS[SHP$Pointer+8],1,8))
define(putSHP$Use$C,$$InsLit(ifelse(@1,,SHP$Use$C,@1),IAS[SHP$Pointer+8],1,8))

define(SHP$BottomAltitude$I,I_SHP_BottomAltitude)
define(xSHP$BottomAltitude$I,IAS[SHP$Pointer+10])
define(putSHP$BottomAltitude$I,IAS[SHP$Pointer+10]=(ifelse(@1,,SHP$BottomAltitude$I,@1)))

define(SHP$TopAltitude$I,I_SHP_TopAltitude)
define(xSHP$TopAltitude$I,IAS[SHP$Pointer+11])
define(putSHP$TopAltitude$I,IAS[SHP$Pointer+11]=(ifelse(@1,,SHP$TopAltitude$I,@1)))

define(SHP$PeriodStartTime$I,I_SHP_PeriodStartTime)
define(xSHP$PeriodStartTime$I,IAS[SHP$Pointer+12])
define(putSHP$PeriodStartTime$I,IAS[SHP$Pointer+12]=(ifelse(@1,,SHP$PeriodStartTime$I,@1)))

define(SHP$PeriodStopTime$I,I_SHP_PeriodStopTime)
define(xSHP$PeriodStopTime$I,IAS[SHP$Pointer+13])
define(putSHP$PeriodStopTime$I,IAS[SHP$Pointer+13]=(ifelse(@1,,SHP$PeriodStopTime$I,@1)))

define(SHP$POS$Latitude$F,F_SHP_POS_Latitude)
define(xSHP$POS$Latitude$F,FAS[SHP$POS$Address])
define(putSHP$POS$Latitude$F,FAS[SHP$POS$Address]=(ifelse(@1,,SHP$POS$Latitude$F,@1)))

define(SHP$POS$Longitude$F,F_SHP_POS_Longitude)
define(xSHP$POS$Longitude$F,FAS[SHP$POS$Address+1])
define(putSHP$POS$Longitude$F,FAS[SHP$POS$Address+1]=(ifelse(@1,,SHP$POS$Longitude$F,@1)))
