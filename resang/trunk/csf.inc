#______________________________________________________________________________
#
#       BGTCSF System Codes and Definitions
#
#______________________________________________________________________________

define (CSF$Master_Node_Name,"CSF$MASTER$NODE")
define (CSF$Slave_Node_Name,"CSF$SLAVE$NODE")

define (CSF$Config    ,"CONFIG.CSF")
define (TCP$Names     ,"TCP_Names")
define (TCP$Addresses ,"TCP_Addresses")

define (MBX$Master      ,"CSF_MASTER")  # CSF Master Listens here
define (MBX$Slave       ,"CSF_SLAVE")   # Slave listens here

define (Master_Log_File ,"MASTERLOG")   # Name of the CSF_cont's Log File
define (Wargame_Log_File,"GAME#LOG")    # Name of the Wargame's Log File

define ($STA$EF$Cluster_Name ,"STA_#_FLAGS")
define ($RSM$EF$Cluster_Name ,"RSM_FLAGS")
define ($CSF$EF$Cluster_Name ,"CSF_FLAGS")

define (CSF$Known_Name  ,"CSF_KNOWN_NAME")
define (CSF$Known_Port  ,"CSF_KNOWN_PORT")

define (CSF$Status_Interval,60)
define (RSM$Status_Interval,60)
define (MBX$MsgSize,512)
define (MBX$BufSize,2048)

define (GameLoad_Process_Name,"LOAD_GAME_#")

define (RemInit_Process_Name,"REMINIT_#")
define (Tcp_RemInit_Process_Name,"TCP_REMINIT_#")

define (WarGame_Process_Name,"WARGAME_#")

define (Station_Process_Name,"STA_#_CMDS")

define (ASTAB_Process_Name,"STA_#_ASB")

define (PLOT_Process_Name,"STA_#_GEO")

define (SCRIPT_Process_Name,"STA_#_SCR")

define (Menu_Process_Name,"STA_#_MENU")

define (RTU_Process_Name,"RTU_#") 



#______________________________________________________________________________
# Table 1.0                             # Transaction Codes 
#______________________________________________________________________________
# Table 1.1                             # Data Codes (TX dat$TxCode$i)
                                        #______________________________________

define ($DT$Table   ,1)
define ($DT$Variable,2)
define ($DT$Pointer ,3)
define ($DT$Message ,4)
define ($DT$Command ,5)
define ($DT$First   ,7)
define ($DT$Last    ,8)
define ($DT$ShutDown,9)


                                        #______________________________________
# Table 1.2                             # Control Codes (TX con$TxCode$i)
                                        #______________________________________

define ($CT$QueryWargamInfo  ,1)
define ($CT$ProvideWargamInfo,2)
define ($CT$ProvideRsmInfo   ,3)
define ($CT$EndWargamInfo    ,4)
define ($CT$RequestGameLoad  ,5)
define ($CT$AckNakLoad       ,6)
define ($CT$RequestGameUnLoad,7)
define ($CT$AckNakUnLoad     ,8)
define ($CT$RequestAttach    ,9)
define ($CT$AckNakAttach     ,10)
define ($CT$RequestInitData  ,11)
define ($CT$AckNakInitData   ,12)
define ($CT$EndInitData      ,13)
define ($CT$RequestStart     ,14)
define ($CT$AckNakStart      ,15)
define ($CT$RsmStatus        ,16)
define ($CT$RequestSignOff   ,17)
define ($CT$AckNakSignOff    ,18)
define ($CT$GameStatus       ,19)
define ($CT$RequestTalk      ,20)
define ($CT$Welcome          ,21)
define ($CT$AckNakUpdate     ,22)


                                        #______________________________________
# Table 1.3                             # AckNak Codes (TX con$AckNak$i)
                                        #______________________________________

define ($ACK,1)
define ($NAK,0)


#______________________________________________________________________________
# Table 2.0                             # Global Event Flag Assignments
#______________________________________________________________________________
# Sub-Table 2.1                         # RSM Event Flags (64-95)
                                        #______________________________________

define $RSM$EF$Cluster           64

define $RSM$EF$PacketReceived    64
define $RSM$EF$PlayerCmdEntered  65
define $RSM$EF$HaltRequest       66
define $RSM$EF$QueuesEmpty       67
define $RSM$EF$StatusTimer       68
define $RSM$EF$OperCmdEntered    69
define $RSM$EF$HelpComplete      70
define $RSM$EF$InitDataReady     71
define $RSM$EF$DataToGo          72     # Extracted Data Ready to go to CSF
#define $RSM$EF$tkprinter         73    # wait for prin_input sec (prin_common) 

define $RSM$EF$Station1WakeUp    74     # request to cycle Station #1
define $RSM$EF$Station2WakeUp    75     # request to cycle Station #2
define $RSM$EF$Station3WakeUp    76     # request to cycle Station #3
define $RSM$EF$Station4WakeUp    77     # request to cycle Station #4
define $RSM$EF$Station5WakeUp    78     # request to cycle Station #5
define $RSM$EF$Station6WakeUp    79     # request to cycle Station #6
define $RSM$EF$Station7WakeUp    80     # request to cycle Station #7
define $RSM$EF$Station8WakeUp    81     # request to cycle Station #8
define $RSM$EF$Station9WakeUp    82     # request to cycle Station #9
define $RSM$EF$Station10WakeUp   83     # request to cycle Station #10
define $RSM$EF$Station11WakeUp   84     # request to cycle Station #11
define $RSM$EF$Station12WakeUp   85     # request to cycle Station #12
define $RSM$EF$Station13WakeUp   86     # request to cycle Station #13
define $RSM$EF$Station14WakeUp   87     # request to cycle Station #14
define $RSM$EF$Station15WakeUp   88     # request to cycle Station #15
define $RSM$EF$Station16WakeUp   89     # request to cycle Station #16

# The remaining RSM EF's should be deleted after check out

define $RSM$EF$Plot2WakeUp       75     # request to cycle PLOT for Station 2
define $RSM$EF$Plot3WakeUp       76     # request to cycle PLOT for Station 3
define $RSM$EF$Plot4WakeUp       77     # request to cycle PLOT for Station 4
define $RSM$EF$Menu1Ready        78     # Dynamic Menu ready for Station 1
define $RSM$EF$Menu2Ready        79     # Dynamic Menu ready for Station 2
define $RSM$EF$Menu3Ready        80     # Dynamic Menu ready for Station 3
define $RSM$EF$Menu4Ready        81     # Dynamic Menu ready for Station 4
define $RSM$EF$Menu1Update       82     # Need to Update Dyn-Menu Station 1
define $RSM$EF$Menu2Update       83     # Need to Update Dyn-Menu Station 2
define $RSM$EF$Menu3Update       84     # Need to Update Dyn-Menu Station 3
define $RSM$EF$Menu4Update       85     # Need to Update Dyn-Menu Station 4
define $RSM$EF$Mail1Ready        86     # Mail ready for Dyn-Menu from PLOT 1
define $RSM$EF$Mail2Ready        87     # Mail ready for Dyn-Menu from PLOT 2
define $RSM$EF$Mail3Ready        88     # Mail ready for Dyn-Menu from PLOT 3
define $RSM$EF$Mail4Ready        89     # Mail ready for Dyn-Menu from PLOT 4
define $RSM$EF$Astab1WakeUp      90     # request to cycle ASTAB for Station 1
define $RSM$EF$Astab2WakeUp      91     # request to cycle ASTAB for Station 2
define $RSM$EF$Astab3WakeUp      92     # request to cycle ASTAB for Station 3
define $RSM$EF$Astab4WakeUp      93     # request to cycle ASTAB for Station 4

                                        #______________________________________
# Sub-Table 2.2                         # CSF Event Flags (96-127)
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

define $RLI$EF$RESA_wakeup       107    # For the RESA/LSE Interface
define $RLI$EF$RLI_wakeup        108    # clk 1/96 (E1469)

define $RTU$EF$RESA_wakeup       109    # For the RESA/RTU Interface
define $RTU$EF$RTU_wakeup        110    # clk 4/96 (E1498)

define $RCI$EF$Event_Flag        111    # clk 10/96 (E1695)
define $PRESCRIPT$EF$RESA_wakeup 112    # For the RESA/PRESCRIPT/alsp$review Interface

# Sub-Table 2.3                         # STA Event Flags (96-127)
                                        #______________________________________

define $STA$EF$Cluster           96

define $STA$EF$PlotWakeUp        101    # request to cycle PLOT 
define $STA$EF$AstabWakeUp       102    # request to cycle ASTAB 
define $STA$EF$MenuReady         103    # Dynamic Menu ready for Station 
define $STA$EF$MenuUpdate        104    # Need to Update Dyn-Menu 
define $STA$EF$MailReady         105    # Mail ready for Dyn-Menu from PLOT 
define $STA$EF$tkprinter         106    # wait for prin_input sec (prin_common) 


#______________________________________________________________________________
# Table 3.0                             # Status Codes
#______________________________________ #______________________________________
# Sub-Table 3.1                         # Remote Host Status Codes
                                        #______________________________________

define($Host$NotConnected,0)            # Initial Condition
define($Host$Crashed     ,1)            # if DECnet lines drops out

define($Host$NotAttached ,2)            # when DECnet line comes up or 
                                        # when RequestSignoff comes

define($Host$Attached    ,3)            # when ATTACH Transaction comes
define($Host$StartInit   ,4)            # when RequestInit Transaction comes
define($Host$InitOnLine  ,5)            # when RemInit NOTICE comes
define($Host$InitAtRSM   ,6)            # when RemInit ACK comes
define($Host$Initialized ,7)            # when AckInitData comes from RSM
define($Host$Running     ,8)            # when the Game Starts


                                        #______________________________________
# Sub Table 3.2                         # War Game Status Codes
                                        #______________________________________

define($Game$NotLoaded   ,0)            # Initial condition (UNLOAD)
define($Game$Crashed     ,1)            # set when WarCyc ERROR Comes (STOP ??)
define($Game$Shutdown    ,2)            #   when WarCyc ACK Comes

define($Game$StartLoad   ,3)            #   when GameLoad spawned (LOAD)
define($Game$Loading     ,4)            #   when GameLoad NOTICE comes
define($Game$Loaded      ,5)            #   when GameLoad ACK comes

define($Game$StartingUp  ,6)            #   when WARCYC Spawned (START)
define($Game$Paused      ,7)            #   when WarCyc NOTICE comes
define($Game$Executing   ,8)            #   when WarCyc NOTICE Comes

define($Game$NotAvailable,9)            # If no Master/Slave Assignment



                                        #______________________________________
# Sub Table 3.3                         # Queue Status Codes
                                        #______________________________________


define($QUE$Empty         ,0)
define($QUE$BeingBuilt    ,1)
define($QUE$WaitingToSend ,2)
define($QUE$Going         ,3)
define($QUE$Coming        ,4)
define($QUE$AwaitingDisb  ,5)
define($QUE$BeingDisb     ,6)



                                        #______________________________________
# Sub Table 3.4                         # Extraction Status Codes
                                        # (TX dat$UpdateLevel$i, 30-SEP-82)
                                        #______________________________________


define ($Ext$PrimaryFailure     ,0)     # Failure at basic level (in EXTRACT)
define ($Ext$HiComplete         ,1)     # Queue full after HI level, before LO
define ($Ext$LoComplete         ,2)     # Queue full after LO level, before BOT
define ($Ext$Complete           ,3)     # Success in all update attempts
define ($Ext$Inter4fail$Code    ,4)     # Intermediate value:  PROGRAM PROBLEM
define ($Ext$Inter5fail$Code    ,5)     # Intermediate value:  PROGRAM PROBLEM


                                        #______________________________________
# Sub Table 3.5                         # Current Process Codes
                                        # (GAM$ProcessCode$i,RSM$ProcessCode$i)
                                        #______________________________________

define ($Proc$LoadGame          ,1)     # 
define ($Proc$Wargame           ,2)     # 
define ($Proc$RemInit           ,3)     # 

#______________________________________________________________________________

#______________________________________________________________________________
# Table 4.0                             # Ack/Nak Reason Codes
#______________________________________ #______________________________________
# Sub-Table 4.1                         # Ack Reason Codes
                                        #______________________________________

define($Ack$Ok           ,1)            # All was OK


                                        #______________________________________
# Sub Table 4.2                         # Nak Reason Codes
                                        #______________________________________

define($Nak$Syntax       ,1)            # 
define($Nak$ParamError   ,2)            # 
define($Nak$NotAttached  ,3)            # 
define($Nak$BadSite      ,4)            # 
define($Nak$BadGame      ,5)            # 
define($Nak$StillAttached,6)            # when attaching
define($Nak$AlreadyLoaded,7)            # 
define($Nak$GameRunning  ,8)            # when attaching
define($Nak$ProgramFault ,9)            # SubProcess ERRORed out
define($Nak$RSMDisabled  ,10)           # The requesting RSM is disabled
define($Nak$NotLoaded    ,11)           # can't UNLOAD what isn't there
define($Nak$MidStatus    ,12)           # 
define($Nak$SlaveDown    ,13)           # the CSF Slave Process is not Up
define($Nak$NotLoadedYet ,14)           # can't INIT till Load complete



#______________________________________________________________________________
# Table 5.0                             # Mode of Game Codes
                                        # (CON$ModeOfGame$i)
                                        #______________________________________

define ($Mode$Normal            ,0)     # 
define ($Mode$New               ,1)     # 
define ($Mode$Continue          ,2)     # 
define ($Mode$Review            ,3)     # 
define ($Mode$Restart           ,4)     # 

#______________________________________________________________________________
# Table 6.0                             # TCP/IP defines
                                        #______________________________________
define($abort_tcp,1)
define($close_tcp,2)

define($controlTx,0)
define($data,1)

define($accepted$length,20)
define($sock_stream,1)
define($af_inet,2)               
define($extract_tcp_error,ishft(iand(16%7fff,@1),-3))
define($extract_tcp_data_len,$$extract(@1,0,M15bits))
define($extract_tcp_data_indicator,$$extract(@1,15,M1bits))
define($PUBLIC_DIR,"TCP_PUBLIC")
