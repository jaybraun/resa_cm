# 
# SYSTEM          GC      "Game Control"
# DATE            "27-OCT-93" # VTS expansion - JFR 10/27/93  #"21-DEC-82"
# TABLE           GAM     "Game Information(VTS)"
# DESCRIPTION             "Contains all Game Dependent Information for each of the 8 possible games."
# 
# #COMMENTS                "Each table entry contains two Que Headers that must be on Quad-Word boundarys.
# #                         This means that the Entry_Size must be a multiple of 2."
# 
# ENTRY_SIZE      20
# SLOTS           8
# ZERO
# ;
# 
# 
# FORMAT
# Field                : Title                          :Type: Word :  Pos : Size :  Range      :                                 ;
# #-------------------- -------------------------------- ---- ------ ------ ------ -----------------------------------------------
# #
# Name                 :"Name of the Game "             : C  :   1  :   1  :   8  :             :                                 ;
# TimeLastLog          :"Last Statistics Log Entry"     : C  :   3  :   1  :   8  :             : Comment= "Vax QuadWord Time Val";
# LastMinute           :"Last Processed Minute of Game" : I  :   5  :   0  :  16  :  0-65000    :                                 ;
# Status               :"Current Game Status"           : I  :   5  :  16  :   4  :  0-15       : Comment= "Codes in CSF tbl 3.2" ;
# NumberBlue           :"Number of Blue Views in Game"  : I  :   5  :  20  :   4  :  0-9        :                                 ;
# NumberOrange         :"Number of Orange Views in Game": I  :   5  :  24  :   4  :  0-9        :                                 ;
# Slave                :"Slave Controller Index,0=Master: I  :   5  :  29  :   2  :  0-3        :                                 ;
# Crash                :"System Crash Indicator"        : I  :   5  :  31  :   1  :  0-1        : Class= Indicator                ;
# LogBytesIn           :"Count of Bytes Inserted"       : I  :   6  :   0  : Word :             :                                 ;
# LogBytesOut          :"Count of Bytes Extracted"      : I  :   7  :   0  : Word :             :                                 ;
# LogIn                :"Count of Records Inserted"     : I  :   8  :   0  : Word :             :                                 ;
# LogOut               :"Count of Records Extracted"    : I  :   9  :   0  : Word :             :                                 ;
# StartTime            :"*Game Minute to Start the Game": I  :  10  :   0  : Word : 0-4160000   : Comment="Now in secs"           ;
# ProcessId            :"Id of the Current Process"     : I  :  11  :   0  : Word :             :                                 ;
# ProcessCode          :"Current Process Indicator"     : I  :  12  :   0  :   4  :             : Comment= "Codes in CSF tbl 3.5" ;
# ModeOfGame           :"Current Mode of Game"          : I  :  12  :   4  :   4  :             : Comment= "Codes in CSF tbl 4.0" ;
# ReviewUntil          :"*Game Min to Drop out of Review: I  :  12  :   8  :  22  : 0-4160000   : Comment="Now in secs"           ;




define(GAM$slots,8)
define(GAM$size,IGCP[$$PtrSize,$$GAM])
define(GAM$used,IGCP[$$PtrUsed,$$GAM])
define(GAM$base,IGCP[$$PtrBase,$$GAM])
define(GAM$last,(GAM$Base+GAM$Used-1))
define(GAM$Last$to,GAM$Used=1-GAM$Base+)

define(GAM$Pointer,kPoint_GAM)
define(GAM$Entry,20)
define(GAM$Pointer$First,GAM$pointer=GAM$Base)
define(GAM$Pointer$Next,GAM$pointer=GAM$pointer+GAM$Entry)
define(GAM$Pointer$Ok,(GAM$Pointer>=GAM$Base & GAM$Pointer<=GAM$Last))
define(GAM$Pointer$Valid,(GAM$Pointer>=GAM$Base & GAM$Pointer<(GAM$Base+GAM$Size)))
define(GAM$Pointer$Index,((GAM$Pointer-GAM$Base)/GAM$Entry+1))
define(GAM$Pointer$To,GAM$Pointer$First-GAM$Entry+(GAM$Entry)*)
define(GAM$Zero,{
    call LIB$MOVC5(0,0,0,GAM$Entry*4,iGC[GAM$Pointer])
   })
define(GAM$Zero$Declare,{
   })

define(GAM$Name$C,C_GAM_Name)
define(xGAM$Name$C,$$ExtLit(IGC[GAM$Pointer],1,8))
define(putGAM$Name$C,$$InsLit(ifelse(@1,,GAM$Name$C,@1),IGC[GAM$Pointer],1,8))

define(GAM$TimeLastLog$C,C_GAM_TimeLastLog)
define(xGAM$TimeLastLog$C,$$ExtLit(IGC[GAM$Pointer+2],1,8))
define(putGAM$TimeLastLog$C,$$InsLit(ifelse(@1,,GAM$TimeLastLog$C,@1),IGC[GAM$Pointer+2],1,8))

define(GAM$LastMinute$I,I_GAM_LastMinute)
define(xGAM$LastMinute$I,($$extract(IGC[GAM$Pointer+4],0,M16bits)))
define(putGAM$LastMinute$I,$$insert((ifelse(@1,,GAM$LastMinute$I,@1)),IGC[GAM$Pointer+4],0,M16bits))

define(GAM$Status$I,I_GAM_Status)
define(xGAM$Status$I,($$extract(IGC[GAM$Pointer+4],16,M4bits)))
define(putGAM$Status$I,$$insert((ifelse(@1,,GAM$Status$I,@1)),IGC[GAM$Pointer+4],16,M4bits))

define(GAM$NumberBlue$I,I_GAM_NumberBlue)
define(xGAM$NumberBlue$I,($$extract(IGC[GAM$Pointer+4],20,M4bits)))
define(putGAM$NumberBlue$I,$$insert((ifelse(@1,,GAM$NumberBlue$I,@1)),IGC[GAM$Pointer+4],20,M4bits))

define(GAM$NumberOrange$I,I_GAM_NumberOrange)
define(xGAM$NumberOrange$I,($$extract(IGC[GAM$Pointer+4],24,M4bits)))
define(putGAM$NumberOrange$I,$$insert((ifelse(@1,,GAM$NumberOrange$I,@1)),IGC[GAM$Pointer+4],24,M4bits))

define(GAM$Slave$I,I_GAM_Slave)
define(xGAM$Slave$I,($$extract(IGC[GAM$Pointer+4],29,M2bits)))
define(putGAM$Slave$I,$$insert((ifelse(@1,,GAM$Slave$I,@1)),IGC[GAM$Pointer+4],29,M2bits))

define(GAM$Crash$I,I_GAM_Crash)
define(xGAM$Crash$I,($$extract(IGC[GAM$Pointer+4],31,M1bits)))
define(putGAM$Crash$I,$$insert((ifelse(@1,,GAM$Crash$I,@1)),IGC[GAM$Pointer+4],31,M1bits))

define(GAM$LogBytesIn$I,I_GAM_LogBytesIn)
define(xGAM$LogBytesIn$I,IGC[GAM$Pointer+5])
define(putGAM$LogBytesIn$I,IGC[GAM$Pointer+5]=(ifelse(@1,,GAM$LogBytesIn$I,@1)))

define(GAM$LogBytesOut$I,I_GAM_LogBytesOut)
define(xGAM$LogBytesOut$I,IGC[GAM$Pointer+6])
define(putGAM$LogBytesOut$I,IGC[GAM$Pointer+6]=(ifelse(@1,,GAM$LogBytesOut$I,@1)))

define(GAM$LogIn$I,I_GAM_LogIn)
define(xGAM$LogIn$I,IGC[GAM$Pointer+7])
define(putGAM$LogIn$I,IGC[GAM$Pointer+7]=(ifelse(@1,,GAM$LogIn$I,@1)))

define(GAM$LogOut$I,I_GAM_LogOut)
define(xGAM$LogOut$I,IGC[GAM$Pointer+8])
define(putGAM$LogOut$I,IGC[GAM$Pointer+8]=(ifelse(@1,,GAM$LogOut$I,@1)))

define(GAM$StartTime$I,I_GAM_StartTime)
define(xGAM$StartTime$I,IGC[GAM$Pointer+9])
define(putGAM$StartTime$I,IGC[GAM$Pointer+9]=(ifelse(@1,,GAM$StartTime$I,@1)))

define(GAM$ProcessId$I,I_GAM_ProcessId)
define(xGAM$ProcessId$I,IGC[GAM$Pointer+10])
define(putGAM$ProcessId$I,IGC[GAM$Pointer+10]=(ifelse(@1,,GAM$ProcessId$I,@1)))

define(GAM$ProcessCode$I,I_GAM_ProcessCode)
define(xGAM$ProcessCode$I,($$extract(IGC[GAM$Pointer+11],0,M4bits)))
define(putGAM$ProcessCode$I,$$insert((ifelse(@1,,GAM$ProcessCode$I,@1)),IGC[GAM$Pointer+11],0,M4bits))

define(GAM$ModeOfGame$I,I_GAM_ModeOfGame)
define(xGAM$ModeOfGame$I,($$extract(IGC[GAM$Pointer+11],4,M4bits)))
define(putGAM$ModeOfGame$I,$$insert((ifelse(@1,,GAM$ModeOfGame$I,@1)),IGC[GAM$Pointer+11],4,M4bits))

define(GAM$ReviewUntil$I,I_GAM_ReviewUntil)
define(xGAM$ReviewUntil$I,($$extract(IGC[GAM$Pointer+11],8,M22bits)))
define(putGAM$ReviewUntil$I,$$insert((ifelse(@1,,GAM$ReviewUntil$I,@1)),IGC[GAM$Pointer+11],8,M22bits))
