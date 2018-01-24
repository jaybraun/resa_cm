##        File TCPIODEF.inc
#/*
# *	Internet I/O Definitions!
# */
#ifndef	IO$S_FCODE
#define	IO$S_FCODE	6
#endif	IO$S_FCODE

define	IO$_SEND	(IO$_WRITEVBLK)
define	IO$_RECEIVE	(IO$_READVBLK)
define	IO$_SOCKET	(IO$_ACCESS | (0))   # 0 << IO$S_FCODE))   50
define	IO$_BIND	(IO$_ACCESS | (64))  # 1 << IO$S_FCODE))  114
define	IO$_LISTEN	(IO$_ACCESS | (128)) # 2 << IO$S_FCODE))  178
define	IO$_ACCEPT	(IO$_ACCESS | (192)) # 3 << IO$S_FCODE))  242
define	IO$_CONNECT	(IO$_ACCESS | (256)) # 4 << IO$S_FCODE))  306
define	IO$_SETSOCKOPT	(IO$_ACCESS | (320)) # 5 << IO$S_FCODE))  370
define	IO$_GETSOCKOPT	(IO$_ACCESS | (384)) # 6 << IO$S_FCODE))  434
define	IO$_SOCKETADDR	(IO$_ACCESS | (448)) # 7 << IO$S_FCODE))  498
define	IO$_IOCTL	(IO$_ACCESS | (512)) # 8 << IO$S_FCODE))  562
define	IO$_PIPE	(IO$_ACCESS | (576)) # 9 << IO$S_FCODE))  626
define	IO$_ACCEPT_WAIT	(IO$_ACCESS | (640)) # 10<< IO$S_FCODE))  690
define	IO$_NETWORK_PTY	(IO$_ACCESS | (704)) # 11<< IO$S_FCODE))  754
define	IO$_SHUTDOWN	(IO$_ACCESS | (768)) # 12<< IO$S_FCODE))  818
define	IO$_GETSOCKNAME	(IO$_ACCESS | (832)) # 13 << IO$S_FCODE)) 882
define	IO$_SOCKETPAIR	(IO$_ACCESS | (896)) # 14 << IO$S_FCODE)) 944
define	IO$_GETPEERNAME	(IO$_ACCESS | (960)) # 15 << IO$S_FCODE))1010 
define  SO_REUSEADDR    (16%0004)
define  SO_KEEPALIVE    (16%0008)
define  SO_ERROR        (16%1007)
define  SOL_SOCKET      (16%ffff)
define  sock_stream     (1)
define  af_inet         (2)
define  Port_Echo       (1056)

