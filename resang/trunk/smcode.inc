# station manager codes and common

# station definitions
define (status$common,
	integer losta, hista
	character staro[80,STA$slots]
	character staside[80,STA$slots]
	character staview[80,STA$slots]
	character staip[80,STA$slots]
	character staastabs[80,STA$slots]
	common/statuscommon/losta, hista, staro, staside, staview, staip, staastabs
)

# Hide multiple dimension array iteration order
define(sta_ro,     staro    [1,@1])
define(sta_side,   staside  [1,@1])
define(sta_view,   staview  [1,@1])
define(sta_ip,     staip    [1,@1])
define(sta_astabs, staastabs[1,@1])


#Station Interface Return Codes
define (STA$RUNNING, 0)
define (STA$STARTED, 1)
define (STA$STOPPED, 2)
define (STA$ABNORMAL,3)
define (STA$NOTFOUND,4)
define (STA$ERROR,   5)


