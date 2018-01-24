
############################################################
#
#       Misc constants

define($TRUE,1)
define($FALSE,0)

define($stat$nogood$,0)
define($stat$ok$,1)
define($stat$complete$,2)

############################################################
#
#       parser common definition

define(pr_common,
    literal     pvarg[ARB]
    integer     pr_stack,
                prstk[6,50],
                piarg[ARB],
                line$length,
                Flag$order$group,
                Flag$NotFor
    integer*4   R_Handle                
    character   line$buf[0:MAXLINE]

    common      /xparser_common/pvarg, 
                                pr_stack, prstk, piarg, line$length, 
                                Flag$order$group, Flag$NotFor, R_Handle,
                                line$buf
    )

define (line$length,line_length)                    # undo use of $'s
define (Flag$order$group,Flag_order_group)          #   "
define (Flag$NotFor,Flag_NotFor)                    #   "
define (line$buf,line_buf)                          #   "

############################################################
#
#       stack definitions (via stack)

define (stack$line_level$pos,   prstk[1,pr_stack])
define (stack$line_range$pos,   prstk[2,pr_stack])
define (stack$token_level$pos,  prstk[3,pr_stack])
define (stack$LowNode,          prstk[4,pr_stack])
define (stack$NodeCount,        prstk[5,pr_stack])
define (stack$ArgCount,         prstk[6,pr_stack])


############################################################
#
#       argument definitions (via stack$ArgCount)

define (arg$tokenIndx,  piarg[Stack$ArgCount])
define (arg$Value,      pvarg[Stack$ArgCount])
define (Last$arg$Value, pvarg[Stack$ArgCount-1])


############################################################
#
#       Line buffer macros

define(line$char,line$buf[line$length @1])
define(line$char$next,line$char(+1))
define(line$char$prior,line$char(-1))
define(line$TokenText,line$buf[stack$line_level$pos+1])

############################################################
#
#       Parser Table Definitions 
#
############################################################
#
#       Control Info Definitions 

define(table$TokenIndx, table[1])
define(table$TokenCount,table[2])
define(table$TokenSize, table[3])
define(table$NodeIndx,  table[4]) 
define(table$NodeCount, table[5])
define(table$NodeSize,  table[6])
define(table$Included,  table[7])


############################################################
#
#       TOKEN Definition (via token$index)


define(token$address,(table$TokenIndx+token$index*table$TokenSize))

define(token$Type,      table[token$address+0])
define(token$TextIndx,  table[token$address+1])

define(token$index,token_index)                 # undo use of $'s

####################
# Indirect text

define(token$Text,      table[token$TextIndx])

############################################################
#
#       NODE Definition (via node$index)

define(node$address,(table$NodeIndx+(node$index-1)*table$NodeSize))

define(node$OrderNumber,table[node$address+0])
define(node$TokenIndx,  table[node$address+1])
define(node$PromptIndx, table[node$address+2])
define(node$SiblingIndx,table[node$address+3])
define(node$ChildIndx,  table[node$address+4])
define(node$GroupFlags, table[node$address+5])
define(node$NotForFlags,table[node$address+6])

####################
# Indirect text

define(node$Prompt,     table[node$PromptIndx])

############################################################
