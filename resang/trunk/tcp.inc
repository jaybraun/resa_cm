# 
# SYSTEM          IP      "IP/TCP Data structures"
# DATE            "2 April 1986"
# RECORD          TCP  "E-Net data structures"
# DESCRIPTION             "Contains Data Structures for IP/TCP"
# 
# ENTRY_SIZE      4
# ZERO
# ;
# 
# SUBTABLE        ADR     "Inter-net Address"
# DESCRIPTION             ""
# SLOTS           4
# 
# ENTRY_SIZE      8 BITS
# TABLE_OFFSET    2
# WORD_OFFSET     0
# ;
# SUBTABLE        DUM     "Dummy Array"
# DESCRIPTION             ""
# SLOTS           8
# 
# ENTRY_SIZE      8 BITS
# TABLE_OFFSET    3
# WORD_OFFSET     0
# ;
# 
# 
# FORMAT
# Field                : Title                          :Type: Word :  Pos : Size :  Range      :                                 ;
# #-------------------- -------------------------------- ---- ------ ------ ------ -----------------------------------------------
# #
# Family              :"Sin.Sin_Family"                 : I  :   1  :   0  :  16  :             :                                 ;
# Port                :"Sin.Sin_Port"                   : I  :   1  :  16  :  16  :             :                                 ;
# ADR$Addr            :"Sin.Sin_Addr_s_Addr"            : I  :   2  : ADR  :   8  :             :                                 ;
# DUM$Zero            :"Sin.Sin_Zero"                   : I  :   3  : DUM  :   8  :             :                                 ;




define(TCP$MaxLength,4)

define(TCP$Length,4)
define(TCP$Record,IIP_TCP)
define(TCP$Record$Definition,
    Integer*4   IIP_TCP[TCP$MaxLength]
    )
define(TCP$Zero,{
    call LIB$MOVC5(0,0,0,TCP$MaxLength*4,iIP_TCP[1])
   })
define(TCP$Zero$Declare,{
   })

define(TCP$ADR$slots,4)
define(TCP$ADR$index,Index_TCP_ADR)
define(TCP$ADR$index$ok,(TCP$ADR$Index>0&TCP$ADR$Index<=TCP$ADR$Slots))
define(TCP$ADR$index$Valid,(TCP$ADR$Index>0&TCP$ADR$Index<=100))
define(TCP$ADR$Address,(1+1))
define(TCP$ADR$Position,(TCP$ADR$Index*8-8))

define(TCP$DUM$slots,8)
define(TCP$DUM$index,Index_TCP_DUM)
define(TCP$DUM$index$ok,(TCP$DUM$Index>0&TCP$DUM$Index<=TCP$DUM$Slots))
define(TCP$DUM$index$Valid,(TCP$DUM$Index>0&TCP$DUM$Index<=100))
define(TCP$DUM$Address,(1+2+(TCP$DUM$Index-1)/4))
define(TCP$DUM$Position,(MOD(TCP$DUM$Index*8-8,32)))

define(TCP$Family$I,I_TCP_Family)
define(xTCP$Family$I,($$extract(IIP_TCP[1],0,M16bits)))
define(putTCP$Family$I,$$insert((ifelse(@1,,TCP$Family$I,@1)),IIP_TCP[1],0,M16bits))

define(TCP$Port$I,I_TCP_Port)
define(xTCP$Port$I,($$extract(IIP_TCP[1],16,M16bits)))
define(putTCP$Port$I,$$insert((ifelse(@1,,TCP$Port$I,@1)),IIP_TCP[1],16,M16bits))

define(TCP$ADR$Addr$I,I_TCP_ADR_Addr)
define(xTCP$ADR$Addr$I,($$extract(IIP_TCP[2],TCP$ADR$Position,M8bits)))
define(putTCP$ADR$Addr$I,$$insert((ifelse(@1,,TCP$ADR$Addr$I,@1)),IIP_TCP[2],TCP$ADR$Position,M8bits))

define(TCP$DUM$Zero$I,I_TCP_DUM_Zero)
define(xTCP$DUM$Zero$I,($$extract(IIP_TCP[3],TCP$DUM$Position,M8bits)))
define(putTCP$DUM$Zero$I,$$insert((ifelse(@1,,TCP$DUM$Zero$I,@1)),IIP_TCP[3],TCP$DUM$Position,M8bits))
