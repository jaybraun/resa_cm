define($MaxMsgLen,400)
define($MaxMsgArgLen,400)

define (MessageCommon,
    character msg_lib_msg[$MaxMsgLen]
    character msg_lib_msgarg[$MaxMsgArgLen]
    integer msg_lib_msglen
    integer UserOrdId
    common /message/msg_lib_msg,msg_lib_msgarg,msg_lib_msglen,UserOrdId
) 
