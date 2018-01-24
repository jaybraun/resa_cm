#include "macros.inc"

define(FF,12)                        # define formfeed character
define(LF,10)                        # define linefeed character

define  $$init        0                # State Indicators... for DDL_LOAD
define  $$sys         1
define  $$tbl1        2
define  $$tblX        3
define  $$fmt1        4
define  $$fmtx        5
define  $$fld1        6
define  $$fldx        7
        
##############################

define  $$LEX_ADDRESS       01
define  $$LEX_ALTERNATE     02
define  $$LEX_CHARACTER     03
define  $$LEX_CLASS         04
define  $$LEX_COMMENTS      05
define  $$LEX_DEFINE        06
define  $$LEX_DESCRIPTION   07
define  $$LEX_DUPLICATE     08
define  $$LEX_ENTRY_SIZE    09
define  $$LEX_FIELD         10
define  $$LEX_FLAG          11
define  $$LEX_FLOATING      12
define  $$LEX_FORMAT        13
define  $$LEX_INDEX         14
define  $$LEX_INDICATOR     15
define  $$LEX_INDIRECT      16
define  $$LEX_INTEGER       17
#define  $$LEX_NAME         18
define  $$LEX_OFFSET        19
define  $$LEX_POINTER       20
define  $$LEX_POSITION      21
define  $$LEX_PRECISION     22
define  $$LEX_RANGE         23
define  $$LEX_RECORD        24
define  $$LEX_SAVE          25
define  $$LEX_SCALING       26
define  $$LEX_SIZE          27
define  $$LEX_SLOTS         28
define  $$LEX_SPECIFIC      29
define  $$LEX_SUBTABLE      30
define  $$LEX_SYSTEM        31
define  $$LEX_TABLE         32
define  $$LEX_TITLE         33
define  $$LEX_TYPE          34
define  $$LEX_VARIABLE      35
define  $$LEX_WORD          36
define  $$LEX_ZERO          37
define  $$LEX_VARIABLES     38

define  $$LEX_BITS          40
define  $$LEX_TABLE_OFFSET  41
define  $$LEX_WORD_OFFSET   42
define  $$LEX_DATE          43
define  $$LEX_FLAG_UPDATES  44
define  $$LEX_ALL           45
define  $$LEX_SIGNED        46
define  $$LEX_BYTE          47
define  $$LEX_HALF          48
define  $$LEX_VAXCHAR       49
define  $$LEX_FLAG_CHANGES  50


define  $$LEX_seperator     81
define  $$LEX_terminator    82
define  $$LEX_string        83
define  $$LEX_comma         84
define  $$LEX_maybe         85

define  $$LEX_other         90

##############################

define MAXLEXTEXT         2000
define MAXLEX             100

##############################

define(d_common,
    integer      d_w, d_w2, d_w3, d_w4, current_fd, fmt_type
    integer*2    lextextpnt, lexhead[MAXLEX], fmt_cnt, fmt_list[ARB]
    logical      opt_Inc, opt_DBD, opt_TST, opt_Ver, opt_XXX, opt_Dmp, opt_pic,
                 opt_mask, eofflag, opt_LN3, opt_ps, opt_prefix, opt_head
    character    d_file[FILENAMESIZE], lextext[MAXLEXTEXT], d_where[ARB]
   
    common /ddl_common/ d_w, d_w2, d_w3, d_w4, current_fd, fmt_type,
                 lextextpnt, lexhead, fmt_cnt, fmt_list,
                 opt_Inc, opt_DBD, opt_TST, opt_Ver, opt_XXX, opt_Dmp, opt_pic,
                 opt_mask, eofflag, opt_LN3, opt_ps, opt_prefix, opt_head,
                 d_file, lextext, d_where

    ddl_data_base
    )


###########################################################################
#
#        Definitions for the DDL Data Base
#

define (MAXTBL,3)
define (MAXSAVE,3)
define (MAXDEFINE,400)
define (MAXDefinition,2000)
define (MAXSUBTBL,10)
define (MAXFIELD,360)

define (MAXTEXT,32768)
define (MAXARRAY,arith(tbl$base,+,arith(MAXTBL,*,tbl$END)))
###########################################################################

define(sys$Id,        ddl_array[1])
define(sys$Title,     ddl_array[2])
define(sys$Text,      ddl_array[3])
define(sys$tbl_count, ddl_array[4])
define(sys$Date,      ddl_array[5])


######################################################################

define(tbl$pointer,      Ipoint_tbl)
define(tbl$pointer$first,tbl$pointer=tbl$Base)
define(tbl$pointer$next, tbl$pointer=tbl$pointer+tbl$end)
define(tbl$pointer$to,   tbl$pointer=tbl$base-tbl$end+tbl$end*)
define(tbl$base,         21)
define(tbl$pointer$ok,   (tbl$pointer>=tbl$base & tbl$pointer<=tbl$last))
define(tbl$last,         (tbl$base+tbl$end*MAXTBL))
define(tbl$END,          tbl$Fld$END)

#########

define(tbl$Id,           ddl_array[tbl$pointer+0])
define(tbl$Title,        ddl_array[tbl$pointer+1])
define(tbl$Description,  ddl_array[tbl$pointer+2])
define(tbl$Comments,     ddl_array[tbl$pointer+3])
define(tbl$Comment,      tbl$comments)
define(tbl$Entry,        ddl_array[tbl$pointer+4])
define(tbl$Variable,     ddl_array[tbl$pointer+5])        # True/False
define(tbl$Var_name,     ddl_array[tbl$pointer+6])
define(tbl$Var_size,     ddl_array[tbl$pointer+7])
define(tbl$Var_max,      ddl_array[tbl$pointer+8])
define(tbl$Slots,        ddl_array[tbl$pointer+9])
define(tbl$Alternate,    ddl_array[tbl$pointer+10])
define(tbl$Duplicate,    ddl_array[tbl$pointer+11])      # True/False
define(tbl$Record,       ddl_array[tbl$pointer+12])      # 0=Table, 1=record,
                                                         # 2=variables, 3=Const
define(tbl$Zero,         ddl_array[tbl$pointer+13])      # True/False
define(tbl$Sav_count,    ddl_array[tbl$pointer+14])
define(tbl$Def_count,    ddl_array[tbl$pointer+15])
define(tbl$Sub_count,    ddl_array[tbl$pointer+16])
define(tbl$Fld_count,    ddl_array[tbl$pointer+17])

define(tbl$Flag_Updates, ddl_array[tbl$pointer+18])        # 0=No, else ==>Name
define(tbl$Flag_All,     ddl_array[tbl$pointer+19])        # True/False
define(tbl$Gen_Ranges,   ddl_array[tbl$pointer+20])        # True/False
define(tbl$Flag_Changes, ddl_array[tbl$pointer+21])        # True/False


######################################################################
define(tbl$Sav$index,    Index_tbl_sav)
define(tbl$sav$index$ok, (tbl$sav$index>0 & tbl$sav$index<=MAXSAVE))
define(tbl$sav$base,     24)
define(tbl$sav$entry,    MAXSAVE)
define(tbl$sav$address,  (tbl$pointer+tbl$sav$base+(tbl$sav$index-1)))
define(tbl$sav$END,      arith(tbl$sav$base,+,tbl$sav$entry))

#########

define(tbl$Sav$name,     ddl_array[tbl$sav$address])


######################################################################
define(tbl$Def$index,    Index_tbl_def)
define(tbl$Def$index$ok, (tbl$def$index>0 & tbl$def$index<=MAXDEFINE))
define(tbl$Def$base,     tbl$sav$END)
define(tbl$Def$entry,    MAXDEFINE)
define(tbl$Def$address,  (tbl$pointer+tbl$def$base+(tbl$def$index-1)))
define(tbl$def$END,      arith(tbl$def$base,+,tbl$def$entry))

#########

define(tbl$Def$Indx,     ddl_array[tbl$def$address])


######################################################################
define(tbl$Sub$index,    Index_tbl_sub)
define(tbl$Sub$index$ok, (tbl$sub$index>0 & tbl$sub$index<=MAXSUBTBL))
define(tbl$Sub$base,     tbl$def$END)
define(tbl$sub$entry,    SIZESUBTBL)
define(tbl$Sub$address,  (tbl$pointer+tbl$sub$base+(tbl$sub$index-1)*tbl$sub$entry))
define(tbl$sub$END,      arith(tbl$sub$base,+,arith(MAXSUBTBL,*,tbl$sub$entry)))
define(SIZESUBTBL,       22)

#########

define(tbl$Sub$Id,       ddl_array[tbl$sub$address])
define(tbl$Sub$Title,    ddl_array[tbl$sub$address+1])
define(tbl$Sub$Desc,     ddl_array[tbl$sub$address+2])
define(tbl$Sub$Com,      ddl_array[tbl$sub$address+3])
define(tbl$Sub$Slots,    ddl_array[tbl$sub$address+4])
define(tbl$Sub$Add,      ddl_array[tbl$sub$address+5])
define(tbl$Sub$Pos,      ddl_array[tbl$sub$address+6])

define(tbl$Sub$EntSize,  ddl_array[tbl$sub$address+7])    # Size in Words/Bits
define(tbl$Sub$EntBit,   ddl_array[tbl$sub$address+8])    # True/False
define(tbl$Sub$TblOff,   ddl_array[tbl$sub$address+12])   # Table Offset
define(tbl$Sub$SubTbl,   ddl_array[tbl$sub$address+13])   # Name If sub-sub-tbl
define(tbl$Sub$WrdOff,   ddl_array[tbl$sub$address+14])   # Least Bit offset


######################################################################
define(tbl$Fld$index,    Index_tbl_fld)
define(tbl$Fld$index$ok, (tbl$fld$index>0 & tbl$fld$index<=MAXFIELD))
define(tbl$Fld$base,     tbl$Sub$END)
define(tbl$Fld$address,  (tbl$pointer+tbl$fld$base+(tbl$fld$index-1)*SIZEFIELD))
define(tbl$Fld$END,      arith(tbl$fld$base,+,arith(MAXFIELD,*,SIZEFIELD)))
define(SIZEFIELD,        20)

#########

define(tbl$Fld$name,     ddl_array[tbl$fld$address+0])
define(tbl$Fld$Title,    ddl_array[tbl$fld$address+1])
define(tbl$Fld$Desc,     ddl_array[tbl$fld$address+2])
define(tbl$Fld$Com,      ddl_array[tbl$fld$address+3])
define(tbl$Fld$Type,     ddl_array[tbl$fld$address+4])   # I/F/C
define(tbl$Fld$Class,    ddl_array[tbl$fld$address+5])
define(tbl$Fld$CDesc,    ddl_array[tbl$fld$address+6])
define(tbl$Fld$Indir,    ddl_array[tbl$fld$address+7])   # ==0:False, <>0:->name
define(tbl$Fld$Word,     ddl_array[tbl$fld$address+8])
define(tbl$Fld$WrdSub,   ddl_array[tbl$fld$address+9])
define(tbl$Fld$Subtbl,   tbl$Fld$WrdSub)              # Dup'd for compatibility
define(tbl$Fld$Alloc,    ddl_array[tbl$fld$address+10])  # Word / Char / Bit
define(tbl$Fld$Size,     ddl_array[tbl$fld$address+11])
define(tbl$Fld$Pos,      ddl_array[tbl$fld$address+12])
define(tbl$Fld$Off,      ddl_array[tbl$fld$address+13])
define(tbl$Fld$Scale,    ddl_array[tbl$fld$address+14])
define(tbl$Fld$Range,    ddl_array[tbl$fld$address+15])
define(tbl$Fld$Prec,     ddl_array[tbl$fld$address+16])
define(tbl$Fld$Flag,     ddl_array[tbl$fld$address+17])  # 0=No, else = n+1
define(tbl$Fld$Signed,   ddl_array[tbl$fld$address+18])  # 0=No, 1=YES
define(tbl$Fld$PosSub,   ddl_array[tbl$fld$address+19]) 


######################################################################



define(ddl_data_base,
integer*2 ddl_array[MAXARRAY]
character ddl_text[MAXTEXT]
common  /ddl_base_common/ddl_array,ddl_text                    
)

