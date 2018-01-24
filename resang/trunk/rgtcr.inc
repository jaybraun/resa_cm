# 
# SYSTEM          RG      "RAINFORM Delay Data Base"
# DATE            "Oct 16, 1992"
# TABLE           TCR     "Track numbers"
# DESCRIPTION             "Contains cross reference between FOTC and RESA track numbers"
# 
# ENTRY_SIZE      3
# SLOTS           1000
# ZERO
# ;
# 
# 
# FORMAT
# Field                : Title                          :Type: Word :  Pos : Size :  Range      :                                 ;
# #-------------------- -------------------------------- ---- ------ ------ ------ -----------------------------------------------
# #
# RESAtn               :"RESA Track Number"             : C  :   1  :   1  :   8  :             :                                 ;
# Init                 :"Initialization index"          : I  :   1  :   0  :  16  : 0-2000      :                                 ;
# BlueForce            :"Blue Force Indicator"          : I  :   3  :   0  :   1  : 0-1         :Class=Indicator                  ;




define(TCR$slots,1000)
define(TCR$size,IRGP[$$PtrSize,$$TCR])
define(TCR$used,IRGP[$$PtrUsed,$$TCR])
define(TCR$base,IRGP[$$PtrBase,$$TCR])
define(TCR$last,(TCR$Base+TCR$Used-1))
define(TCR$Last$to,TCR$Used=1-TCR$Base+)

define(TCR$Pointer,kPoint_TCR)
define(TCR$Entry,3)
define(TCR$Pointer$First,TCR$pointer=TCR$Base)
define(TCR$Pointer$Next,TCR$pointer=TCR$pointer+TCR$Entry)
define(TCR$Pointer$Ok,(TCR$Pointer>=TCR$Base & TCR$Pointer<=TCR$Last))
define(TCR$Pointer$Valid,(TCR$Pointer>=TCR$Base & TCR$Pointer<(TCR$Base+TCR$Size)))
define(TCR$Pointer$Index,((TCR$Pointer-TCR$Base)/TCR$Entry+1))
define(TCR$Pointer$To,TCR$Pointer$First-TCR$Entry+(TCR$Entry)*)
define(TCR$Zero,{
    call LIB$MOVC5(0,0,0,TCR$Entry*4,iRG[TCR$Pointer])
   })
define(TCR$Zero$Declare,{
   })

define(TCR$RESAtn$C,C_TCR_RESAtn)
define(xTCR$RESAtn$C,$$ExtLit(IRG[TCR$Pointer],1,8))
define(putTCR$RESAtn$C,$$InsLit(ifelse(@1,,TCR$RESAtn$C,@1),IRG[TCR$Pointer],1,8))

define(TCR$Init$I,I_TCR_Init)
define(xTCR$Init$I,($$extract(IRG[TCR$Pointer],0,M16bits)))
define(putTCR$Init$I,$$insert((ifelse(@1,,TCR$Init$I,@1)),IRG[TCR$Pointer],0,M16bits))

define(TCR$BlueForce$I,I_TCR_BlueForce)
define(xTCR$BlueForce$I,($$extract(IRG[TCR$Pointer+2],0,M1bits)))
define(putTCR$BlueForce$I,$$insert((ifelse(@1,,TCR$BlueForce$I,@1)),IRG[TCR$Pointer+2],0,M1bits))
