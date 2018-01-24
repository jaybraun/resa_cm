# SYSTEM          RG      "RAINFORM Delay Data Base"
# DATE            "09-Nov-99" # Expand Track number - DEW (E2172)
#                 # "May 6, 1999" # Moved TrackNo from word 17 to word 12, bit 11
#                               # Shifted MsgTOR to bit 0 in word 17 from bit 20
#                               # Put new variable IFFMode2 in word 17, bit 20, 12 bits long.
#                 #Aug 19, 1991" #"July 25,1991" #"12-Aug-85"
# TABLE           MSG     "Delayed Message Data"
# DESCRIPTION             "Contains Time Sensetive Data that is used for generation of time delayed messages"
# 
# ENTRY_SIZE      19
# SLOTS           3000
# ZERO
# ;
# 
# 
# FORMAT
# Field                : Title                          :Type: Word :  Pos : Size :  Range      :                                 ;
# #-------------------- -------------------------------- ---- ------ ------ ------ -----------------------------------------------
# 
# #
# MsgType              :"Message Type - RED, GOLD,etc"  : I  :   1  :   0  :   5  : 0-31        :                                 ;
# MsgDTG               :"Message Date Time Group"       : I  :   1  :   5  :  20  :             :                                 ;
# Elpsexists           :"Is Contact by mult HFDF Stas"  : I  :   1  :  25  :   1  : 0-1         :Class=Indicator                  ;
# SelRad               :"Is Selor Contact Radar"        : I  :   1  :  26  :   1  : 0-1         :Class=Indicator                  ;
# Classified           :"Was target sighted visually"   : I  :   1  :  27  :   1  : 0-1         :Class=Indicator                  ;
# FingerPrinted        :"Was target fingerprinted"      : I  :   1  :  28  :   1  : 0-1         :Class=Indicator                  ;
# SWABStrk             :"Is detection by SWABS"         : I  :   1  :  29  :   1  : 0-1         :Class=Indicator                  ;
# ROTHRtrk             :"Is detection by ROTHR"         : I  :   1  :  30  :   1  : 0-1         :Class=Indicator                  ;
# Bistatic             :"Is detection by bistatic dev"  : I  :   1  :  31  :   1  : 0-1         :Class=Indicator                  ;
# 
# DctorName            :"Name of Detector"              : C  :   2  :   1  :   8  :             :                                 ;
# SosusReg             :"SOSUS Region Number"           : I  :   2  :   0  :   4  :             :                                 ;
# SWABSReg             :"SWABS Region Number"           : I  :   2  :   0  :   4  :             :                                 ;
# DcteeName            :"Name of Detectee"              : C  :   4  :   1  :   8  :             :                                 ;
# 
# Latitude             :"Latitude for message field"    : F  :   6  :   0  : Word : (-0.5*PI-0.5*PI)   :                          ;
# Longitude            :"Longitude for message field"   : F  :   7  :   0  : Word : (PI)               :                          ;
# 
# TrueLat              :"Gnd Truth Latitude"            : F  :   8  :   0  : Word : (-0.5*PI-0.5*PI)   :                          ;
# TrueLong             :"Gnd Truth Longitude"           : F  :   9  :   0  : Word : (PI)               :                          ;
# 
# Course               :"Course for message field"      : I  :  10  :   0  :   9  : 0-359       :                                 ;
# Speed                :"Speed for message field"       : I  :  10  :   9  :  12  : 0-3000      :                                 ;
# Bearing              :"Brng/Brng of Maj Axis"         : I  :  10  :  21  :   9  : 0-359       :                                 ;
# PropMode             :"Sub Propulsion Mode"           : I  :  10  :  30  :   2  : 0-2         :                                 ;
# AirType              :"Air Type - Jet, Prop, Helo"    : I  :  10  :  30  :   2  : 0-3         :                                 ;
# HFDFreq              :"Frequency for HFDF"            : I  :  10  :   0  :  15  : 0-32767     :                                 ;
# RefPt                :"HFDF Station ID"               : I  :  10  :  15  :   6  : 1-35        :                                 ;
# 
# MajorAxis            :"Length of major axis"          : I  :  11  :   0  :  12  : 1-4000      :                                 ;
# MinorAxis            :"Length of minor axis"          : I  :  11  :  12  :  12  : 1-4000      :                                 ;
# Sensor               :"Sensor source"                 : I  :  11  :  24  :   4  : 1-15        :                                 ;
# Composition          :"Number of Aircraft"            : I  :  11  :  28  :   4  : 1-15        :                                 ;
# CntcMins             :"Duration Contact - Minutes"    : I  :  11  :   0  :   6  : 0-60        :                                 ;
# CntcHrs              :"Duration Contact - Hours"      : I  :  11  :   6  :   7  : 1-99        :                                 ;
# 
# RngRadius            :"Radius or range"               : I  :  12  :   0  :  11  : 0-2047      :                                 ;
# UnitType             :"Unit Type eg. air, surf, etc"  : I  :  12  :  21  :   3  : 0-6         :                                 ;
# TMarkSeq             :"Trademark Sequence Number"     : I  :  12  :  24  :   7  : 1-99        :                                 ;
# 
# EmitPntrIndx         :"Pointer/Index to Equipment"    : I  :  13  :   0  : Word :             :                                 ;
# 
# SeqCtcNbr            :"Sequential Contact Number"     : I  :  14  :   0  :   9  : 0-500       :                                 ;
# Lines                :"Which lines in SOS msg"        : I  :  14  :   9  :   1  : 0-1         :Class=Indicator                  ;
# Senstime             :"Message Sensor Time"           : I  :  14  :  10  :  20  :             :                                 ;
# TargetId             :"Identification target detected": I  :  14  :  30  :   2  : 0-3         :                                ; 
# 
# UpTime               :"Game minute of active detection: I  :  15  :   0  : 16   : 0-65000     :                                 ;
# DownTime             :"Game minute stop reported"     : I  :  15  :  16  : 16   : 0-65000     :                                 ;
# 
# TrackCode            :"Track designation character"   : C  :  16   :  1  :  1   :             :                                 ;
# AltDepth             :"Track altitude/depth"          : I  :  16  :   8  :  20  : 0-999999    :                                 ;
# PlatType             :"Platform Type air, surf, etc"  : I  :  16  :  28  :  3   : 0-6         :                                 ;
# 
# MsgTOR               :"Message Time of Receipt"       : I  :  17  :   0  :  20  :             :                                 ;
# IFFMode2             :"IFF Mode 2"                    : I  :  17  :  20  :  12  :             :                                 ;
# 
# Tbldctor             :"Posit of dctor in Init Table"  : I  :  18  :   0  :  16  : 0-2000      :                                 ;
# Tbldctee             :"Posit of dctee in Init Table"  : I  :  18  :  16  :  16  : 0-2000      :                                 ;
# 
# TrackNo             :"*Track number"                  : I  :  19   :  0  :  18  : 1-99999     :                                 ;




define(MSG$slots,3000)
define(MSG$size,IRGP[$$PtrSize,$$MSG])
define(MSG$used,IRGP[$$PtrUsed,$$MSG])
define(MSG$base,IRGP[$$PtrBase,$$MSG])
define(MSG$last,(MSG$Base+MSG$Used-1))
define(MSG$Last$to,MSG$Used=1-MSG$Base+)

define(MSG$Pointer,kPoint_MSG)
define(MSG$Entry,19)
define(MSG$Pointer$First,MSG$pointer=MSG$Base)
define(MSG$Pointer$Next,MSG$pointer=MSG$pointer+MSG$Entry)
define(MSG$Pointer$Ok,(MSG$Pointer>=MSG$Base & MSG$Pointer<=MSG$Last))
define(MSG$Pointer$Valid,(MSG$Pointer>=MSG$Base & MSG$Pointer<(MSG$Base+MSG$Size)))
define(MSG$Pointer$Index,((MSG$Pointer-MSG$Base)/MSG$Entry+1))
define(MSG$Pointer$To,MSG$Pointer$First-MSG$Entry+(MSG$Entry)*)
define(MSG$Zero,{
    call LIB$MOVC5(0,0,0,MSG$Entry*4,iRG[MSG$Pointer])
   })
define(MSG$Zero$Declare,{
   })

define(MSG$MsgType$I,I_MSG_MsgType)
define(xMSG$MsgType$I,($$extract(IRG[MSG$Pointer],0,M5bits)))
define(putMSG$MsgType$I,$$insert((ifelse(@1,,MSG$MsgType$I,@1)),IRG[MSG$Pointer],0,M5bits))

define(MSG$MsgDTG$I,I_MSG_MsgDTG)
define(xMSG$MsgDTG$I,($$extract(IRG[MSG$Pointer],5,M20bits)))
define(putMSG$MsgDTG$I,$$insert((ifelse(@1,,MSG$MsgDTG$I,@1)),IRG[MSG$Pointer],5,M20bits))

define(MSG$Elpsexists$I,I_MSG_Elpsexists)
define(xMSG$Elpsexists$I,($$extract(IRG[MSG$Pointer],25,M1bits)))
define(putMSG$Elpsexists$I,$$insert((ifelse(@1,,MSG$Elpsexists$I,@1)),IRG[MSG$Pointer],25,M1bits))

define(MSG$SelRad$I,I_MSG_SelRad)
define(xMSG$SelRad$I,($$extract(IRG[MSG$Pointer],26,M1bits)))
define(putMSG$SelRad$I,$$insert((ifelse(@1,,MSG$SelRad$I,@1)),IRG[MSG$Pointer],26,M1bits))

define(MSG$Classified$I,I_MSG_Classified)
define(xMSG$Classified$I,($$extract(IRG[MSG$Pointer],27,M1bits)))
define(putMSG$Classified$I,$$insert((ifelse(@1,,MSG$Classified$I,@1)),IRG[MSG$Pointer],27,M1bits))

define(MSG$FingerPrinted$I,I_MSG_FingerPrinted)
define(xMSG$FingerPrinted$I,($$extract(IRG[MSG$Pointer],28,M1bits)))
define(putMSG$FingerPrinted$I,$$insert((ifelse(@1,,MSG$FingerPrinted$I,@1)),IRG[MSG$Pointer],28,M1bits))

define(MSG$SWABStrk$I,I_MSG_SWABStrk)
define(xMSG$SWABStrk$I,($$extract(IRG[MSG$Pointer],29,M1bits)))
define(putMSG$SWABStrk$I,$$insert((ifelse(@1,,MSG$SWABStrk$I,@1)),IRG[MSG$Pointer],29,M1bits))

define(MSG$ROTHRtrk$I,I_MSG_ROTHRtrk)
define(xMSG$ROTHRtrk$I,($$extract(IRG[MSG$Pointer],30,M1bits)))
define(putMSG$ROTHRtrk$I,$$insert((ifelse(@1,,MSG$ROTHRtrk$I,@1)),IRG[MSG$Pointer],30,M1bits))

define(MSG$Bistatic$I,I_MSG_Bistatic)
define(xMSG$Bistatic$I,($$extract(IRG[MSG$Pointer],31,M1bits)))
define(putMSG$Bistatic$I,$$insert((ifelse(@1,,MSG$Bistatic$I,@1)),IRG[MSG$Pointer],31,M1bits))

define(MSG$DctorName$C,C_MSG_DctorName)
define(xMSG$DctorName$C,$$ExtLit(IRG[MSG$Pointer+1],1,8))
define(putMSG$DctorName$C,$$InsLit(ifelse(@1,,MSG$DctorName$C,@1),IRG[MSG$Pointer+1],1,8))

define(MSG$SosusReg$I,I_MSG_SosusReg)
define(xMSG$SosusReg$I,($$extract(IRG[MSG$Pointer+1],0,M4bits)))
define(putMSG$SosusReg$I,$$insert((ifelse(@1,,MSG$SosusReg$I,@1)),IRG[MSG$Pointer+1],0,M4bits))

define(MSG$SWABSReg$I,I_MSG_SWABSReg)
define(xMSG$SWABSReg$I,($$extract(IRG[MSG$Pointer+1],0,M4bits)))
define(putMSG$SWABSReg$I,$$insert((ifelse(@1,,MSG$SWABSReg$I,@1)),IRG[MSG$Pointer+1],0,M4bits))

define(MSG$DcteeName$C,C_MSG_DcteeName)
define(xMSG$DcteeName$C,$$ExtLit(IRG[MSG$Pointer+3],1,8))
define(putMSG$DcteeName$C,$$InsLit(ifelse(@1,,MSG$DcteeName$C,@1),IRG[MSG$Pointer+3],1,8))

define(MSG$Latitude$F,F_MSG_Latitude)
define(xMSG$Latitude$F,FRG[MSG$Pointer+5])
define(putMSG$Latitude$F,FRG[MSG$Pointer+5]=(ifelse(@1,,MSG$Latitude$F,@1)))

define(MSG$Longitude$F,F_MSG_Longitude)
define(xMSG$Longitude$F,FRG[MSG$Pointer+6])
define(putMSG$Longitude$F,FRG[MSG$Pointer+6]=(ifelse(@1,,MSG$Longitude$F,@1)))

define(MSG$TrueLat$F,F_MSG_TrueLat)
define(xMSG$TrueLat$F,FRG[MSG$Pointer+7])
define(putMSG$TrueLat$F,FRG[MSG$Pointer+7]=(ifelse(@1,,MSG$TrueLat$F,@1)))

define(MSG$TrueLong$F,F_MSG_TrueLong)
define(xMSG$TrueLong$F,FRG[MSG$Pointer+8])
define(putMSG$TrueLong$F,FRG[MSG$Pointer+8]=(ifelse(@1,,MSG$TrueLong$F,@1)))

define(MSG$Course$I,I_MSG_Course)
define(xMSG$Course$I,($$extract(IRG[MSG$Pointer+9],0,M9bits)))
define(putMSG$Course$I,$$insert((ifelse(@1,,MSG$Course$I,@1)),IRG[MSG$Pointer+9],0,M9bits))

define(MSG$Speed$I,I_MSG_Speed)
define(xMSG$Speed$I,($$extract(IRG[MSG$Pointer+9],9,M12bits)))
define(putMSG$Speed$I,$$insert((ifelse(@1,,MSG$Speed$I,@1)),IRG[MSG$Pointer+9],9,M12bits))

define(MSG$Bearing$I,I_MSG_Bearing)
define(xMSG$Bearing$I,($$extract(IRG[MSG$Pointer+9],21,M9bits)))
define(putMSG$Bearing$I,$$insert((ifelse(@1,,MSG$Bearing$I,@1)),IRG[MSG$Pointer+9],21,M9bits))

define(MSG$PropMode$I,I_MSG_PropMode)
define(xMSG$PropMode$I,($$extract(IRG[MSG$Pointer+9],30,M2bits)))
define(putMSG$PropMode$I,$$insert((ifelse(@1,,MSG$PropMode$I,@1)),IRG[MSG$Pointer+9],30,M2bits))

define(MSG$AirType$I,I_MSG_AirType)
define(xMSG$AirType$I,($$extract(IRG[MSG$Pointer+9],30,M2bits)))
define(putMSG$AirType$I,$$insert((ifelse(@1,,MSG$AirType$I,@1)),IRG[MSG$Pointer+9],30,M2bits))

define(MSG$HFDFreq$I,I_MSG_HFDFreq)
define(xMSG$HFDFreq$I,($$extract(IRG[MSG$Pointer+9],0,M15bits)))
define(putMSG$HFDFreq$I,$$insert((ifelse(@1,,MSG$HFDFreq$I,@1)),IRG[MSG$Pointer+9],0,M15bits))

define(MSG$RefPt$I,I_MSG_RefPt)
define(xMSG$RefPt$I,($$extract(IRG[MSG$Pointer+9],15,M6bits)))
define(putMSG$RefPt$I,$$insert((ifelse(@1,,MSG$RefPt$I,@1)),IRG[MSG$Pointer+9],15,M6bits))

define(MSG$MajorAxis$I,I_MSG_MajorAxis)
define(xMSG$MajorAxis$I,($$extract(IRG[MSG$Pointer+10],0,M12bits)))
define(putMSG$MajorAxis$I,$$insert((ifelse(@1,,MSG$MajorAxis$I,@1)),IRG[MSG$Pointer+10],0,M12bits))

define(MSG$MinorAxis$I,I_MSG_MinorAxis)
define(xMSG$MinorAxis$I,($$extract(IRG[MSG$Pointer+10],12,M12bits)))
define(putMSG$MinorAxis$I,$$insert((ifelse(@1,,MSG$MinorAxis$I,@1)),IRG[MSG$Pointer+10],12,M12bits))

define(MSG$Sensor$I,I_MSG_Sensor)
define(xMSG$Sensor$I,($$extract(IRG[MSG$Pointer+10],24,M4bits)))
define(putMSG$Sensor$I,$$insert((ifelse(@1,,MSG$Sensor$I,@1)),IRG[MSG$Pointer+10],24,M4bits))

define(MSG$Composition$I,I_MSG_Composition)
define(xMSG$Composition$I,($$extract(IRG[MSG$Pointer+10],28,M4bits)))
define(putMSG$Composition$I,$$insert((ifelse(@1,,MSG$Composition$I,@1)),IRG[MSG$Pointer+10],28,M4bits))

define(MSG$CntcMins$I,I_MSG_CntcMins)
define(xMSG$CntcMins$I,($$extract(IRG[MSG$Pointer+10],0,M6bits)))
define(putMSG$CntcMins$I,$$insert((ifelse(@1,,MSG$CntcMins$I,@1)),IRG[MSG$Pointer+10],0,M6bits))

define(MSG$CntcHrs$I,I_MSG_CntcHrs)
define(xMSG$CntcHrs$I,($$extract(IRG[MSG$Pointer+10],6,M7bits)))
define(putMSG$CntcHrs$I,$$insert((ifelse(@1,,MSG$CntcHrs$I,@1)),IRG[MSG$Pointer+10],6,M7bits))

define(MSG$RngRadius$I,I_MSG_RngRadius)
define(xMSG$RngRadius$I,($$extract(IRG[MSG$Pointer+11],0,M11bits)))
define(putMSG$RngRadius$I,$$insert((ifelse(@1,,MSG$RngRadius$I,@1)),IRG[MSG$Pointer+11],0,M11bits))

define(MSG$UnitType$I,I_MSG_UnitType)
define(xMSG$UnitType$I,($$extract(IRG[MSG$Pointer+11],21,M3bits)))
define(putMSG$UnitType$I,$$insert((ifelse(@1,,MSG$UnitType$I,@1)),IRG[MSG$Pointer+11],21,M3bits))

define(MSG$TMarkSeq$I,I_MSG_TMarkSeq)
define(xMSG$TMarkSeq$I,($$extract(IRG[MSG$Pointer+11],24,M7bits)))
define(putMSG$TMarkSeq$I,$$insert((ifelse(@1,,MSG$TMarkSeq$I,@1)),IRG[MSG$Pointer+11],24,M7bits))

define(MSG$EmitPntrIndx$I,I_MSG_EmitPntrIndx)
define(xMSG$EmitPntrIndx$I,IRG[MSG$Pointer+12])
define(putMSG$EmitPntrIndx$I,IRG[MSG$Pointer+12]=(ifelse(@1,,MSG$EmitPntrIndx$I,@1)))

define(MSG$SeqCtcNbr$I,I_MSG_SeqCtcNbr)
define(xMSG$SeqCtcNbr$I,($$extract(IRG[MSG$Pointer+13],0,M9bits)))
define(putMSG$SeqCtcNbr$I,$$insert((ifelse(@1,,MSG$SeqCtcNbr$I,@1)),IRG[MSG$Pointer+13],0,M9bits))

define(MSG$Lines$I,I_MSG_Lines)
define(xMSG$Lines$I,($$extract(IRG[MSG$Pointer+13],9,M1bits)))
define(putMSG$Lines$I,$$insert((ifelse(@1,,MSG$Lines$I,@1)),IRG[MSG$Pointer+13],9,M1bits))

define(MSG$Senstime$I,I_MSG_Senstime)
define(xMSG$Senstime$I,($$extract(IRG[MSG$Pointer+13],10,M20bits)))
define(putMSG$Senstime$I,$$insert((ifelse(@1,,MSG$Senstime$I,@1)),IRG[MSG$Pointer+13],10,M20bits))

define(MSG$TargetId$I,I_MSG_TargetId)
define(xMSG$TargetId$I,($$extract(IRG[MSG$Pointer+13],30,M2bits)))
define(putMSG$TargetId$I,$$insert((ifelse(@1,,MSG$TargetId$I,@1)),IRG[MSG$Pointer+13],30,M2bits))

define(MSG$UpTime$I,I_MSG_UpTime)
define(xMSG$UpTime$I,($$extract(IRG[MSG$Pointer+14],0,M16bits)))
define(putMSG$UpTime$I,$$insert((ifelse(@1,,MSG$UpTime$I,@1)),IRG[MSG$Pointer+14],0,M16bits))

define(MSG$DownTime$I,I_MSG_DownTime)
define(xMSG$DownTime$I,($$extract(IRG[MSG$Pointer+14],16,M16bits)))
define(putMSG$DownTime$I,$$insert((ifelse(@1,,MSG$DownTime$I,@1)),IRG[MSG$Pointer+14],16,M16bits))

define(MSG$TrackCode$C,C_MSG_TrackCode)
define(xMSG$TrackCode$C,$$ExtLit(IRG[MSG$Pointer+15],1,1))
define(putMSG$TrackCode$C,$$InsLit(ifelse(@1,,MSG$TrackCode$C,@1),IRG[MSG$Pointer+15],1,1))

define(MSG$AltDepth$I,I_MSG_AltDepth)
define(xMSG$AltDepth$I,($$extract(IRG[MSG$Pointer+15],8,M20bits)))
define(putMSG$AltDepth$I,$$insert((ifelse(@1,,MSG$AltDepth$I,@1)),IRG[MSG$Pointer+15],8,M20bits))

define(MSG$PlatType$I,I_MSG_PlatType)
define(xMSG$PlatType$I,($$extract(IRG[MSG$Pointer+15],28,M3bits)))
define(putMSG$PlatType$I,$$insert((ifelse(@1,,MSG$PlatType$I,@1)),IRG[MSG$Pointer+15],28,M3bits))

define(MSG$MsgTOR$I,I_MSG_MsgTOR)
define(xMSG$MsgTOR$I,($$extract(IRG[MSG$Pointer+16],0,M20bits)))
define(putMSG$MsgTOR$I,$$insert((ifelse(@1,,MSG$MsgTOR$I,@1)),IRG[MSG$Pointer+16],0,M20bits))

define(MSG$IFFMode2$I,I_MSG_IFFMode2)
define(xMSG$IFFMode2$I,($$extract(IRG[MSG$Pointer+16],20,M12bits)))
define(putMSG$IFFMode2$I,$$insert((ifelse(@1,,MSG$IFFMode2$I,@1)),IRG[MSG$Pointer+16],20,M12bits))

define(MSG$Tbldctor$I,I_MSG_Tbldctor)
define(xMSG$Tbldctor$I,($$extract(IRG[MSG$Pointer+17],0,M16bits)))
define(putMSG$Tbldctor$I,$$insert((ifelse(@1,,MSG$Tbldctor$I,@1)),IRG[MSG$Pointer+17],0,M16bits))

define(MSG$Tbldctee$I,I_MSG_Tbldctee)
define(xMSG$Tbldctee$I,($$extract(IRG[MSG$Pointer+17],16,M16bits)))
define(putMSG$Tbldctee$I,$$insert((ifelse(@1,,MSG$Tbldctee$I,@1)),IRG[MSG$Pointer+17],16,M16bits))

define(MSG$TrackNo$I,I_MSG_TrackNo)
define(xMSG$TrackNo$I,($$extract(IRG[MSG$Pointer+18],0,M18bits)))
define(putMSG$TrackNo$I,$$insert((ifelse(@1,,MSG$TrackNo$I,@1)),IRG[MSG$Pointer+18],0,M18bits))
