#______________________________________________________________________________
#
#	Nuclear Damage System Codes and Definitions
#
#______________________________________________________________________________
#______________________________________________________________________________
#   Variables    ( SEE   NDCTL.inc  )
#______________________________________________________________________________

#______________________________________________________________________________
# Table 1.0				# Transaction Codes 
#______________________________________________________________________________


#______________________________________________________________________________
# Table 3.0				# Status Codes
#______________________________________ #______________________________________
# Sub-Table 3.1			        # Unit Nuclear Damage Type
    					#______________________________________

define($Nuclear$Undef$Type  ,0)		# Undefined
define($Nuclear$CV$Type     ,1)		# CV/CVN
define($Nuclear$CG$Type     ,2)		# CG/CGN/DDG/FFG
define($Nuclear$DD$Type     ,3)		# DD/FF
define($Nuclear$Aux$Type    ,4)		# Auxilary
define($Nuclear$Sub$Type    ,5)		# Submarine
define($Nuclear$Bomber$Type, 6)		# Low-G  aircraft
define($Nuclear$Fighter$Type,7)		# High-G aircraft
define($Nuclear$Copter$Type, 8)		# Rotor  aircraft


    					#______________________________________
# Sub Table 3.2			        # Medium of Travel
    					#______________________________________

define($Medium$Undefined   ,0)		# Undefined
define($Medium$Air         ,1)		# Air
define($Medium$Submerged   ,2)		# Submerged Submarine
define($Medium$Surface     ,3)		# Surface or Periscope depth


    					#______________________________________
# Sub Table 3.3			        # Depth
    					#______________________________________


define($Depth$Undefined    ,0)		# Undefined
define($Depth$400          ,1)		# 400 ft.
define($Depth$600          ,2)		# 600 ft.



    					#______________________________________
# Sub Table 3.4			        # Burst Type
    					#______________________________________


define ($Burst$Undefined     ,0)	# Undefined
define ($Burst$Air           ,1)	# Air burst
define ($Burst$Surface       ,3)	# Surface Burst		7/15/85
define ($Burst$Subsurf       ,2)	# Subsurface burst	7/15/85


    					#______________________________________
# Sub Table 3.5			        # Side
    					#______________________________________

define ($Side$Neutral        ,0)	# 
define ($Side$Blue           ,1)	# 
define ($Side$Orange         ,2)	# 


#______________________________________________________________________________
# Table 4.2
#______________________________________________________________________________

define($idle,0)
define($busy,1)

define($No,0)
define($Yes,1)

define($Off,0)
define($On,1)

define($False,0)
define($True,1)

define($Nak,0)
define($Ack,1)


define($idle$code,0)
define($busy$code,1)

define($No$Code,0)
define($Yes$Code,1)

define($Off$Code,0)
define($On$Code,1)

define($Old$Code,0)
define($New$Code,1)

define($False$Code,0)
define($True$Code,1)

