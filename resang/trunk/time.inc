define (TIMcommon,
    tim$record$definition
    common/tim/tim$record
)

define   (dtgstr_macro,
    {
     lenengdtgstr = 0
     if (@1 < 0.)                       # Illegal time; return blank string
     {
         call smovv ("          ",@2,lenengdtgstr)
     }
     else
     {
         irc = MIN_TO_DTG(_                 # Convert time-step to DTG
                      Xtim$StartDtg$F,  # dtg of sim time 0; 5 element array
    				        #  1-year,2-mon,3-day,4-hr,5-min
                      @1,               # minutes to be added to InitDtg:in:float:min
                      EngDtg)           # Resultant dtg ddhhmm:real array: out
         call SmovNoyrMoDatetime(_    	# DTG array to string
                             engdtg,	# DTG:in:array[5]:real
                             @2,	# DTG(dd-hhmm:ss):out:array:character
                             lenengdtgstr)# Current position in str:inout:integer
     }
                      })
