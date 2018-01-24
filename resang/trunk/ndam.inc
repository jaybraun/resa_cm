#______________________________________________________________________________
#
#	Nuclear Damage System Codes and Definitions
#
#______________________________________________________________________________

define (MBX$Master	,"CSF_MASTER")	# CSF Master Listens here
define (MBX$Slave	,"CSF_SLAVE")	# Slave listens here

define (Master_Log_File ,"MASTERLOG")	# Name of the CSF_cont's Log File
define (Wargame_Log_File,"GAME#LOG")	# Name of the Wargame's Log File

define ($RSM$EF$Cluster_Name ,"RSM_FLAGS")
define ($CSF$EF$Cluster_Name ,"CSF_FLAGS")

define (CSF$Known_Name	,"CSF_KNOWN_NAME")

define (CSF$Status_Interval,60)
define (RSM$Status_Interval,60)
define (MBX$MsgSize,512)


#______________________________________________________________________________
# Table 1.0				# Transaction Codes 
#______________________________________________________________________________
# Table 1.1				# Data Codes (ND dat$NDCode$i)
    					#______________________________________

define ($ND$Init    ,1)
define ($ND$Target  ,2)
define ($ND$Unit    ,3)
define ($ND$Last    ,4)
define ($ND$ShutDown,5)


    					#______________________________________


#______________________________________________________________________________
# Table 2.0				# Global Event Flag Assignments
#______________________________________________________________________________
# Sub-Table 2.1			        # NDAM Event Flags (64-95)
    					#______________________________________

define $NDAM$EF$Cluster           64

define $NDAM$EF$PacketReceived    64
define $NDAM$EF$PlayerCmdEntered  65
define $NDAM$EF$HaltRequest       66
define $NDAM$EF$QueuesEmpty       67
define $NDAM$EF$StatusTimer       68
define $NDAM$EF$OperCmdEntered    69
define $NDAM$EF$HelpComplete      70
define $NDAM$EF$InitDataReady     71
define $NDAM$EF$Plot1WakeUp       72	# request to cycle PLOT for Station 1
define $NDAM$EF$Plot2WakeUp       73	# request to cycle PLOT for Station 2
define $NDAM$EF$Plot3WakeUp       74	# request to cycle PLOT for Station 3
define $NDAM$EF$Plot4WakeUp       75	# request to cycle PLOT for Station 4
define $NDAM$EF$Menu1Ready        76	# Dynamic Menu ready for Station 1
define $NDAM$EF$Menu2Ready        77	# Dynamic Menu ready for Station 2
define $NDAM$EF$Menu3Ready        78	# Dynamic Menu ready for Station 3
define $NDAM$EF$Menu4Ready        79	# Dynamic Menu ready for Station 4
define $NDAM$EF$Menu1Update       80	# Need to Update Dyn-Menu Station 1
define $NDAM$EF$Menu2Update       81	# Need to Update Dyn-Menu Station 2
define $NDAM$EF$Menu3Update       82	# Need to Update Dyn-Menu Station 3
define $NDAM$EF$Menu4Update       83	# Need to Update Dyn-Menu Station 4
define $NDAM$EF$Mail1Ready        84	# Mail ready for Dyn-Menu from PLOT 1
define $NDAM$EF$Mail2Ready        85	# Mail ready for Dyn-Menu from PLOT 2
define $NDAM$EF$Mail3Ready        86	# Mail ready for Dyn-Menu from PLOT 3
define $NDAM$EF$Mail4Ready        87	# Mail ready for Dyn-Menu from PLOT 4
define $NDAM$EF$DataToGo          88	# Extracted Data Ready to go to CSF
define $NDAM$EF$Astab1WakeUp      89	# request to cycle ASTAB for Station 1
define $NDAM$EF$Astab2WakeUp      90	# request to cycle ASTAB for Station 2
define $NDAM$EF$Astab3WakeUp      91	# request to cycle ASTAB for Station 3
define $NDAM$EF$Astab4WakeUp      92	# request to cycle ASTAB for Station 4

    					#______________________________________
# Sub-Table 2.2			        # CSF Event Flags (96-127)
    					#______________________________________

define $CSF$EF$Cluster           96

define $CSF$EF$HaltRequest       96
define $CSF$EF$MasterCrash       97
define $CSF$EF$SlaveCrash        98
define $CSF$EF$OperatorCmd       99
define $CSF$EF$QueuesEmpty      100
define $CSF$EF$StatusTimer      101
define $CSF$EF$MasterMbx        102
define $CSF$EF$SlaveMbx         103
define $CSF$EF$ControlTx        104
define $CSF$EF$DataTx           105
define $CSF$EF$HelpComplete     106



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
define($Nuclear$Bomber$Type  ,6)	# Large fixed-wing a/c
define($Nuclear$Fighter$Type ,7)	# High-"G" fixed-wing a/c
define($Nuclear$Copter$Type  ,8)	# Helicopters

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
define ($Burst$Surface       ,2)	# Surface Burst
define ($Burst$Subsurf       ,3)	# Subsurface burst


    					#______________________________________
# Sub Table 3.5			        # Side
    					#______________________________________

define ($Side$Neutral        ,0)	# 
define ($Side$Blue           ,1)	# 
define ($Side$Orange         ,2)	# 

#______________________________________________________________________________

