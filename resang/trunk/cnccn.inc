# SYSTEM          CN 	"CCN data"
# DATE            "21 Oct 87"
# RECORD          CCN	"CCN Header record, revised"
# DESCRIPTION             "Fields and message format CCN Header Message, Revised"
# 
# ENTRY_SIZE       2
# ZERO
# ;
# 
# FORMAT
# Field            : Title                                    :Type: Word :  Pos : Size : Range      :                            ;
# #---------------- ------------------------------------------  ----  ----   ----   ----  ----------  --------------------------- 
# Code             :"Type of CCN Packet"                      : I  :  1   :   0  :   7  :            :                            ;
# EOF              :"Used to designate end-of-file"           : I  :  1   :   7  :   1  :            : Class= Indicator           ;
# Subcode          :"Contains Error or Filter Code"           : I  :  1   :   8  :   8  :            :                            ;
# CESOvrly         :"Overlay of bits 0 - 16, word 1"          : I  :  1   :   0  :  16  :            :                            ;
# 
# LengthLow        :"Low order byte of length"                : I  :  1   :  24  :   8  :            :                            ;
# LengthHigh       :"High order byte of length"               : I  :  1   :  16  :   8  :            :                            ;
# Length           :"Number of bytes of data in packet"       : I  :  1   :  16  :  16  :            :                            ;
# ACKWord          :"Received ACK from server"                : I  :  1   :   0  :  32  :            :                            ;
# 
# DataType         :"Data Type Code"                          : I  :  2   :   0  :   8  :            :                            ;
# DataFormat       :"Data Format code"                        : I  :  2   :   8  :   8  :            :                            ;
# DataOvrly        :"Data Format code"                        : I  :  2   :   0  :  16  :            :                            ;
# 
# ChecksumLo       :"Low order byte of checksum"              : I  :  2   :  24  :   8  :            :                            ;
# ChecksumHi       :"High order byte of checksum"             : I  :  2   :  16  :   8  :            :                            ;
# Checksum         :"Checksum value for header"               : I  :  2   :  16  :  16  :            :                            ;




define(CCN$MaxLength,2)

define(CCN$Length,2)
define(CCN$Record,ICN_CCN)
define(CCN$Record$Definition,
    Integer*4   ICN_CCN[CCN$MaxLength]
    )
define(CCN$Zero,{
    call LIB$MOVC5(0,0,0,CCN$MaxLength*4,iCN_CCN[1])
   })
define(CCN$Zero$Declare,{
   })

define(CCN$Code$I,I_CCN_Code)
define(xCCN$Code$I,($$extract(ICN_CCN[1],0,M7bits)))
define(putCCN$Code$I,$$insert((ifelse(@1,,CCN$Code$I,@1)),ICN_CCN[1],0,M7bits))

define(CCN$EOF$I,I_CCN_EOF)
define(xCCN$EOF$I,($$extract(ICN_CCN[1],7,M1bits)))
define(putCCN$EOF$I,$$insert((ifelse(@1,,CCN$EOF$I,@1)),ICN_CCN[1],7,M1bits))

define(CCN$Subcode$I,I_CCN_Subcode)
define(xCCN$Subcode$I,($$extract(ICN_CCN[1],8,M8bits)))
define(putCCN$Subcode$I,$$insert((ifelse(@1,,CCN$Subcode$I,@1)),ICN_CCN[1],8,M8bits))

define(CCN$CESOvrly$I,I_CCN_CESOvrly)
define(xCCN$CESOvrly$I,($$extract(ICN_CCN[1],0,M16bits)))
define(putCCN$CESOvrly$I,$$insert((ifelse(@1,,CCN$CESOvrly$I,@1)),ICN_CCN[1],0,M16bits))

define(CCN$LengthLow$I,I_CCN_LengthLow)
define(xCCN$LengthLow$I,($$extract(ICN_CCN[1],24,M8bits)))
define(putCCN$LengthLow$I,$$insert((ifelse(@1,,CCN$LengthLow$I,@1)),ICN_CCN[1],24,M8bits))

define(CCN$LengthHigh$I,I_CCN_LengthHigh)
define(xCCN$LengthHigh$I,($$extract(ICN_CCN[1],16,M8bits)))
define(putCCN$LengthHigh$I,$$insert((ifelse(@1,,CCN$LengthHigh$I,@1)),ICN_CCN[1],16,M8bits))

define(CCN$Length$I,I_CCN_Length)
define(xCCN$Length$I,($$extract(ICN_CCN[1],16,M16bits)))
define(putCCN$Length$I,$$insert((ifelse(@1,,CCN$Length$I,@1)),ICN_CCN[1],16,M16bits))

define(CCN$ACKWord$I,I_CCN_ACKWord)
define(xCCN$ACKWord$I,($$extract(ICN_CCN[1],0,M32bits)))
define(putCCN$ACKWord$I,$$insert((ifelse(@1,,CCN$ACKWord$I,@1)),ICN_CCN[1],0,M32bits))

define(CCN$DataType$I,I_CCN_DataType)
define(xCCN$DataType$I,($$extract(ICN_CCN[2],0,M8bits)))
define(putCCN$DataType$I,$$insert((ifelse(@1,,CCN$DataType$I,@1)),ICN_CCN[2],0,M8bits))

define(CCN$DataFormat$I,I_CCN_DataFormat)
define(xCCN$DataFormat$I,($$extract(ICN_CCN[2],8,M8bits)))
define(putCCN$DataFormat$I,$$insert((ifelse(@1,,CCN$DataFormat$I,@1)),ICN_CCN[2],8,M8bits))

define(CCN$DataOvrly$I,I_CCN_DataOvrly)
define(xCCN$DataOvrly$I,($$extract(ICN_CCN[2],0,M16bits)))
define(putCCN$DataOvrly$I,$$insert((ifelse(@1,,CCN$DataOvrly$I,@1)),ICN_CCN[2],0,M16bits))

define(CCN$ChecksumLo$I,I_CCN_ChecksumLo)
define(xCCN$ChecksumLo$I,($$extract(ICN_CCN[2],24,M8bits)))
define(putCCN$ChecksumLo$I,$$insert((ifelse(@1,,CCN$ChecksumLo$I,@1)),ICN_CCN[2],24,M8bits))

define(CCN$ChecksumHi$I,I_CCN_ChecksumHi)
define(xCCN$ChecksumHi$I,($$extract(ICN_CCN[2],16,M8bits)))
define(putCCN$ChecksumHi$I,$$insert((ifelse(@1,,CCN$ChecksumHi$I,@1)),ICN_CCN[2],16,M8bits))

define(CCN$Checksum$I,I_CCN_Checksum)
define(xCCN$Checksum$I,($$extract(ICN_CCN[2],16,M16bits)))
define(putCCN$Checksum$I,$$insert((ifelse(@1,,CCN$Checksum$I,@1)),ICN_CCN[2],16,M16bits))
