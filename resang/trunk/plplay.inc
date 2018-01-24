
define($play$stack$max,100)                 # Max length of all plans
                                            # increase from 50 to 100 - joa 11/97 (E1836)

include "parser.inc"            
include "parser32.int"            

include "orders.inc"         

define(max$lines$of$Message,8)
define(max$lines$of$Commtext,8)
define(max$lines$of$Intell,20)

define(max$points$in$pim,8)

define(max$length$of$text,75)

define($max$args,$parse$max$args)
define($order$array$size,arith($max$args,+,7))  # Time added as first word (VTS expansion)

define($YES,1)
define($TRUE,1)

define($NO,0)
define($FALSE,0)

#
# Order code number ranges

define(NO$order$lo,0)
define(NO$order$hi,99)
define(FOR$order$lo,100)
define(FOR$order$hi,399)
define(FORCE$order$lo,400)
define(FORCE$order$hi,499)
define(STD$order$lo,500)
define(STD$order$hi,899)
define(Local$order$lo,900)
define(Local$order$hi,999)


#
# order groups

define($grp$Control,    0)      # A = Control (or Instructor) orders
define($grp$Player,     1)      # B = Blue/Orange orders
define($grp$FlightPlan, 2)      # C = Launch Time Flight Plan
define($grp$COMMtext,   3)      # D = Comm Text (actual text)
define($grp$Prestored,  4)      # E = Prestored orders and Contingency plans
define($grp$Force,      5)      # F = FORCE commands (to generate scenario)
define($grp$GameCTL,    6)      # G = Game Control commands 
define($grp$InitStrt,   7)      # H = Wargam init (to start the game)
define($grp$InitDfn,    8)      # I = Wargam init (to define values)
define($grp$InitName,   9)      # J = Wargam init (for name input)
define($grp$InitNum,    10)     # K = Wargam init (for number input)
define($grp$InitYN,     11)     # L = Wargam init (for yes/no input)
define($grp$InitTask,   12)     # M = Wargam init (for task-no, track-code)
define($grp$OPPlan,     13)     # N = OP Plans (conditional logic)
# order group "O" no longer used - Improved Cruise Missile Model dropped
# the advanced cruise missile model (was never being used in exercises) - joa 2/98 (E1864)
#define($grp$WAYPOINT,   14)     # O = way-points subcommands      # av 8/93
define($grp$PIM,        15)     # P = PIM (Plan of Intended Movement)
define($grp$TechControl,16)     # Q = Tech Control orders         # joa 11/97 (E1825)
define($grp$ROUTE,      17)     # R = ROUTE subcommands           # joa 11/97 (E1830)
define($grp$ShipPlan,   18)     # S = Sequential Ship/Boat Plan   # joa 12/97 (E1864)
define($grp$TorpedoPlan,19)     # T = Sequential Torpedo Plan     # joa 12/97 (E1864)
define($grp$SubPlan,    20)     # U = Sequential Submarine Plan   # joa 12/97 (E1864)
define($grp$MissilePlan,25)     # Z = Sequential CM Plan          # joa 12/97 (E1864)

#
# Not-For Flags

define($clear$NotFor          ,-1)      # 
define($Not$Aircraft           ,0)      # A = 
define($Not$Surface            ,1)      # B = 
define($Not$Submarine          ,2)      # C = 
define($Not$ShoreBase          ,3)      # D = 
define($Not$TaskGroup          ,4)      # E = 
define($Not$Review             ,5)      # F = 
define($Not$GameInit           ,6)      # G = 
define($Not$SmallBoat          ,7)      # H = 
define($Not$GroundUnit         ,8)      # I = 
define($Not$AircraftMnvr       ,9)      # J =     # joa 11/97 (E1370)
define($Not$CruiseMissileMnvr ,10)      # K =     # joa 11/97 (E1864)
define($Not$SmallBoatMnvr     ,11)      # L =     # joa 11/97 (E1370)
define($Not$GhostedUnit       ,15)      # P =     # joa 11/97 (E1850)
define($Not$SurfaceMnvr       ,18)      # S =     # joa 11/97 (E1832)
define($Not$Torpedo           ,19)      # T =     # joa 11/97 (E1832)
define($Not$SubmarineMnvr     ,20)      # U =     # joa 11/97 (E1832)
define($Not$TorpedoMnvr       ,21)      # V =     # joa 11/97 (E1832)
define($Not$CruiseMissile     ,25)      # Z =     # joa 12/97 (E1864)




define ($stk$length,1)                   # define format for order stack
define ($stk$nord,2)
define ($stk$start,3)
define ($stk$addressee,4)
define ($stk$seq,5)
define ($stk$cnt,6)
define ($stk$args,7)
define ($stk$lastarg,arith($max$args,+,6))


define(PLAYcommon,
    integer     stkpnt, curtim, p_nargs, p_ordn
    literal     curfor, oldfor, p_args($max$args),
                stack($stk$lastarg,$play$stack$max)
    common      /playcm/ stkpnt, curtim, p_nargs, p_ordn, 
                         curfor, oldfor, p_args, stack
)
        
