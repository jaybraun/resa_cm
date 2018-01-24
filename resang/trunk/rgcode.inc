##########################################################################
#
#        RAINFORM(D) CODES AND DEFINITIONS - RGCODE.INC
#
##########################################################################

#==========================================================================
# Section 1 Variables   
#=========================================================================

## Quad-Word Variables

define(RGinit$File         ,CRG[041-20]) ## Name of Initialization File 
define(RGRad$File          ,CRG[045-22]) ## Name of Radar Initialization File
define(RGOut$Device        ,CRG[047-23]) ## Name of Output Device (Port to TFCC)
define(RGOut$File          ,CRG[049-24]) ## Name of Output File for messages
define(RGPort$File         ,CRG[051-25]) ## Name of Port File 
define(RGNtds$Vue          ,IRG[053-26]) ## Which is the NTDS view 

## Long-Word Variables

define(RGGrbl$Msgs         ,IRG[100]) ## Percent Garbled Messages
define(RGGrbl$Char         ,IRG[101]) ## % Garbled Characters per Grbld Msg

define(RGIntv$ESM          ,IRG[102]) ## Interval of ESM Messages
define(RGIntv$Red          ,IRG[103]) ## Interval of Red Messages
define(RGIntv$SOS          ,IRG[104]) ## Interval of SOSUS Messages
define(RGIntv$SEL          ,IRG[105]) ## Interval of SELOR Messages
define(RGIntv$HFD          ,IRG[106]) ## Interval of HFDF Messages
define(RGIntv$ROS          ,IRG[107]) ## Interval of Red OSIS Messages
define(RGIntv$WOS          ,IRG[108]) ## Interval of White OSIS Messages
define(RGIntAir$BFL        ,IRG[109]) ## Interval of BFL Air Messages
define(RGIntShp$BFL        ,IRG[110]) ## Interval of BFL Ship/Sub Messages
define(RGIntv$Golds        ,IRG[111]) ## Interv of Gld Non-ESM Msg fm Surf units
define(RGIntv$Golda        ,IRG[112]) ## Interv of Gld Non-ESM Msg fm Air units
define(RGIntv$GESMs        ,IRG[113]) ## Interval of Gold ESM Msg fm Surf units 
define(RGIntv$GESMa        ,IRG[114]) ## Interval of Gold ESM Msg fm Air units
define(RGIntv$GSELa        ,IRG[115]) ## Interval of Gold SELOR Air tgt Msgs
define(RGIntv$GSELs        ,IRG[116]) ## Interval of Gold SELOR Surf tgt Msgs
define(RGIntv$GSOS         ,IRG[117]) ## Interval of Gold SOS Messages
define(RGIntv$GHFD         ,IRG[118]) ## Interval of Gold HFDF Messages
define(RGIntv$STS          ,IRG[119]) ## Interval of SURTASS Evaluated Messages

define(RGWatch$Out         ,IRG[120]) ## Watch output Yes/No
define(last$RG$page        ,IRG[121]) ## last page used in RG (rel zero)
define(Max$RG$Words        ,IRG[122]) ## Total number of words
define(Used$RG$Slots       ,IRG[123]) ## Total DMD slots in use7

define(RGTFCC$OUT          ,IRG[124]) ## To TFCC [Y] or [N]
define(RGENET$OUT          ,IRG[125]) ## To E-NET [Y] or [N]
define(RGPOST$OUT          ,IRG[126]) ## For POST [Y] or [N]
define(RGV6$OUT            ,IRG[127]) ## For V6 [Y] or [N]
define(RGPSHOT$OUT         ,IRG[128]) ## For Potshot [Y] or [N]
define(RGFile$OUT          ,IRG[129]) ## To a designated file [Y] or [N]
define(RGRmks$OUT          ,IRG[130]) ## Include a Remarks line [Y] or [N]
define(RGAudit$Msg         ,IRG[131]) ## Include the Audit Trail [Y] or [N]
define(RGTruPos$Out        ,IRG[132]) ## Include a Ground Truth File [Y] or [N]
define(RGTruePos$Rmks      ,IRG[133]) ## Add Grnd Truth to a RMKS line (Y/N)
define(RGTruTrk$File       ,IRG[134]) ## Ground truth file number for tracks
define(RGTruBlu$File       ,IRG[135]) ## Ground truth file nbr for Blue units
define(RGTPQRpt$Intv       ,IRG[136]) ## TPQ reporting interval
define(RGIntv$GSWABS       ,IRG[137]) ## SWABS track GOLD reporting interval
define(RGCPX$OUT           ,IRG[138]) ## For CPX [Y] or [N]
define(RGIntv$GROTHR       ,IRG[139]) ## ROTHR track GOLD reporting interval
define(RGradb              ,IRG[140]) ## Use RADB line or RAD line
define(RGcase_no           ,IRG[141]) ## lima case number for sub tracks
define(RGlive              ,IRG[142]) ## Output to live ship [Y] or [N] -
                                      ##    clf 8/5/92
define(RGIntv$GIR          ,IRG[143]) ## MAD track GOLD reporting interval -
                                      ##    clf 10/8/92
define(RGjitter            ,IRG[144]) ## Radar jitter -- clf 10/16/92
define(RGIntv$Gold         ,IRG[145]) ## interval Gold msgs
define(RGIntv$Telt	   ,IRG[146]) ## interval TACELINT msgs
define(RGIntv$Tclnt        ,IRG[147]) ## interval TACREP ELINT msgs

define(RGDevice$NBR        ,IRG[150]) ## Output device (port) number
define(RGFile$NBR          ,IRG[151]) ## Output file number
define(RGTMark$IN          ,IRG[152]) ## Include Trademark? [Y] or [N]
define(RGDevice$TTY        ,IRG[153]) ## Device is TT...? [Y] or [N]
define(RGDetector$Rmks     ,IRG[154]) ## Add Detector to a RMKS line (Y/N)
define(RGex_trk            ,IRG[155]) ## Add Exercise track to CTC line (Y/N)
                                      ## -- clf 7/17/93
define(RGjulian_off        ,IRG[156]) ## Julian day offset -- clf 9/28/93    


define(RGTacelint$JNTCS    ,IRG[160]) ## TACELINT Msgs [Y] or [N]
define(RGTcpmar$JNTCS      ,IRG[161]) ## TACREP MAR Msgs [Y] or [N]
define(RGTcpelnt$JNTCS     ,IRG[162]) ## TACREP ELINT Msgs [Y] or [N]
define(RGTcpair$JNTCS      ,IRG[163]) ## TACREP AIR  [Y] or [N]

define(RGStaNbr$CCN        ,IRG[164]) ## Station Nbr for CCN ASTAB transfer
define(RGAstabCnt$CCN      ,IRG[165]) ## Nbr of ASTABs for CCN ASTAB transfer
define(RGDepart$Port       ,IRG[166]) ## Have a port departure file? [Y]/[N]
define(RGDevice$IN         ,IRG[167]) ## Input device for POST input
define(RGFOTC$IN           ,IRG[168]) ## Read POST & put on FOTC ASTAB & GROPLOT
define(RGMiss$Pos          ,IRG[169]) ## Miss posits in Pos line (GOLD) [Y/N]
define(RGMiss$Lat          ,IRG[170]) ## Randomly miss a lat in Pos line (GOLD)
define(RGMiss$Long         ,IRG[171]) ## Randomly miss a long in Pos line (GOLD)
define(RGName$Blu          ,IRG[172]) ## Randomly garble BFLOC unit's name
define(RGClass$Blu         ,IRG[173]) ## Randomly garble BFLOC unit's class
define(RGName$Tgt          ,IRG[174]) ## Randomly garble tgt unit's name
define(RGClass$Tgt         ,IRG[175]) ## Randomly garble tgt unit's class
define(RGClass$Wzd         ,IRG[176]) ## Percentage of WIZARD/SELOR msgs
define(RGYear              ,IRG[177]) ## Exercise year
define(RGtrkpre            ,IRG[178]) ## Track Number prefix -- clf 10/14/92
define(RGcorr              ,IRG[179]) ## Perfect correlation -- clf 10/16/92

define(RGCMean$Air         ,IRG[180]) ## Air commcen distribution mean
define(RGTMean$Air         ,IRG[181]) ## Air trans distribution mean

define(RGCMean$Sub         ,IRG[182]) ## Sub commcen distribution mean
define(RGTMean$Sub         ,IRG[183]) ## Sub trans distribution mean

define(RGCMean$Shp         ,IRG[184]) ## Ship commcen distribution mean
define(RGTMean$Shp         ,IRG[185]) ## Ship trans distribution mean

define(RGCMean$Hfd         ,IRG[186]) ## HFDF commcen distribution mean
define(RGTMean$Hfd         ,IRG[187]) ## HFDF trans distribution mean

define(RGCMean$Sos         ,IRG[188]) ## SOSUS commcen distribution mean
define(RGTMean$Sos         ,IRG[189]) ## SOSUS trans distribution mean

define(RGCMean$Sel         ,IRG[190]) ## SELOR commcen distribution mean
define(RGTMean$Sel         ,IRG[191]) ## SELOR trans distribution mean

define(RGCMean$Ros         ,IRG[192]) ## OSIS RED commcen distribution mean
define(RGTMean$Ros         ,IRG[193]) ## OSIS RED trans distribution mean

define(RGCMean$Wos         ,IRG[194]) ## OSIS WHITE commcen distribution mean
define(RGTMean$Wos         ,IRG[195]) ## OSIS WHITE trans distribution mean

#define(RGCMean$Wht        ,IRG[196]) ## WHITE commcen distribution mean
#define(RGTMean$Wht        ,IRG[197]) ## WHITE trans distribution mean

define(RGCMean$Bfl         ,IRG[198]) ## Blue Force Locator commcen distro mean
define(RGTMean$Bfl         ,IRG[199]) ## Blue Force Locator trans distro mean

define(RGCMean$Glda        ,IRG[200]) ## GOLD air commcen distribution mean
define(RGTMean$Glda        ,IRG[201]) ## GOLD air transmission distribution mean

define(RGCMean$Glds        ,IRG[202]) ## GOLD surf commcen distribution mean
define(RGTMean$Glds        ,IRG[203]) ## GOLD surf transmission disto mean

define(RGCMean$GESMa       ,IRG[204]) ## GOLD air ESM commcen distribution mean
define(RGTMean$GESMa       ,IRG[205]) ## GOLD air ESM transmission distro mean

define(RGCMean$GESMs       ,IRG[206]) ## GOLD surf ESM commcen distribution mean
define(RGTMean$GESMs       ,IRG[207]) ## GOLD surf ESM transmission distro mean

define(RGCMean$GSEL        ,IRG[208]) ## GOLD Selor commcen distribution mean
define(RGTMean$GSEL        ,IRG[209]) ## GOLD Selor transmission distrib.  mean

define(RGCMean$GWiz        ,IRG[210]) ## GOLD WIZARD commcen distribution mean
define(RGTMean$GWiz        ,IRG[211]) ## GOLD WIZARD trans distribution mean

define(RGCMean$GSOS        ,IRG[212]) ## GOLD SOSUS commcen distribution mean
define(RGTMean$GSOS        ,IRG[213]) ## GOLD SOSUS transmission distrib. mean

define(RGCMean$GHFD        ,IRG[214]) ## GOLD HFDF commcen distribution mean
define(RGTMean$GHFD        ,IRG[215]) ## GOLD HFDD transmission distrib. mean

define(RGCMean$STS         ,IRG[216]) ## SURTASS commcen distribution mean
define(RGTMean$STS         ,IRG[217]) ## SURTASS trans distribution mean

define(RGSTS$Mean          ,IRG[218]) ## SURTASS distribution mean
define(RGSTS$StDev         ,IRG[219]) ## SURTASS standard deviation

define(RGCMean$GSWB        ,IRG[220]) ## SWABS commcen distribution mean
define(RGTMean$GSWB        ,IRG[221]) ## SWABS transmission distrib.  mean

define(RGCMean$GRTH        ,IRG[222]) ## ROTHR commcen distribution mean
define(RGTMean$GRTH        ,IRG[223]) ## ROTHR transmission distrib.  mean

define(RGCMean$GIR         ,IRG[224]) ## IR commcen distribution mean
define(RGTMean$GIR         ,IRG[225]) ## IR transmission distrib.  mean

define(RGCMean$SPC         ,IRG[226]) ##  commcen distribution mean
define(RGTMean$SPC         ,IRG[227]) ##  transmission distrib.  mean

define(RGCMean$GLD	   ,IRG[228])
define(RGTMean$GLD	   ,IRG[229])

define(RGCMean$TEL	   ,IRG[230])
define(RGTMean$TEL         ,IRG[231])

define(RGCMean$TCL         ,IRG[232])
define(RGTMean$TCL	   ,IRG[233])

define(RGLastTime$ESM      ,FRG[234]) ## Last time of ESM Messages
define(RGLastTime$Red      ,FRG[235]) ## Last time of Red Messages
define(RGLastTime$SOS      ,FRG[236]) ## Last time of SOSUS Messages
define(RGLastTime$SEL      ,FRG[237]) ## Last time of SELOR Messages
define(RGLastTime$HFD      ,FRG[238]) ## Last time of HFDF Messages
define(RGLastTime$ROS      ,FRG[239]) ## Last time of Red OSIS Messages
define(RGLastTime$WOS      ,FRG[240]) ## Last time of White OSIS Messages
define(RGLastTimeAir$BFL   ,FRG[241]) ## Last time of BFL Air Messages
define(RGLastTimeShp$BFL   ,FRG[242]) ## Last time of BFL Ship/Sub Messages
define(RGLastTime$Golds    ,FRG[243]) ## Last time of Gld Non-ESM Msg fm Surf units
define(RGLastTime$Golda    ,FRG[244]) ## Last time of Gld Non-ESM Msg fm Air units
define(RGLastTime$GESMs    ,FRG[245]) ## Last time of Gold ESM Msg fm Surf units 
define(RGLastTime$GESMa    ,FRG[246]) ## Last time of Gold ESM Msg fm Air units
define(RGLastTime$GSELa    ,FRG[247]) ## Last time of Gold SELOR Air tgt Msgs
define(RGLastTime$GSELs    ,FRG[248]) ## Last time of Gold SELOR Surf tgt Msgs
define(RGLastTime$GSOS     ,FRG[249]) ## Last time of Gold SOS Messages
define(RGLastTime$GHFD     ,FRG[250]) ## Last time of Gold HFDF Messages
define(RGLastTime$STS      ,FRG[251]) ## Last time of SURTASS Evaluated Messages
define(RGLastTime$GSwabs   ,FRG[252]) ## Last time of Gold SWABS Messages
define(RGLastTime$GIR      ,FRG[253]) ## Last time of Gold IR Messages
define(RGLastTime$GRothr   ,FRG[254]) ## Last time of Gold ROTHR Messages
define(RGFile$Out$duration ,FRG[254]) ## Minutes to record in file before opening new file

#==========================================================================
# Section 2 CONSTANTS
#=========================================================================

# Table 1.0       Msg$MsgType$i for RAINFORM message types    Range: 0 - 31
#                 Number assignments coincide with Table 1.1

define(Max$MsgTypes   ,29)       # clf 10/8/92
define($Red$RMsg      ,1)
define($ESM$RMsg      ,2)
define($SOSUS$RMsg    ,3)
define($HFDF$RMsg     ,4)
define($SELOR$RMsg    ,5)
define($GOLD$RMsg     ,6)
define($GOLDP$RMsg    ,7)        # Port arrivals/departures
define($GESM$RMsg     ,8)
define($GSOSUS$RMsg   ,9)
define($GHFDF$RMsg   ,10)
define($GSELOR$RMsg  ,11)
define($GWIZARD$RMsg ,12)
define($OSRed$RMsg   ,13)
define($OSRedP$RMsg  ,14)        # Port arrivals/departures
define($OSWht$RMsg   ,15)
define($OSWhtP$RMsg  ,16)        # Port arrivals/departures
define($White$RMsg   ,17)
define($Special$RMsg ,18)
define($TELNT$RMsg   ,19)
define($STELNT$RMsg  ,20)
define($TCPElnt$RMsg ,21)
define($STCPELNT$RMsg,22)
define($STS$RMsg     ,23)
define($GSONBA$RMsg  ,24)        # Active sonobuoy contact
define($GSONBP$RMsg  ,25)        # Passive sonobuoy contact
define($GSONTP$RMsg  ,26)        # Passive TASS sonar contact
define($GSONP$RMsg   ,27)        # Passive sonar contact
define($GSWABS$RMsg  ,28)        # Swabs contact
define($GIR$RMsg     ,29)        # IR contact    -- clf 10/8/92

# Table 1.1       Msg$MsgType$i for JINTACCS message types
#                 Number assignments coincide with Table 1.0

define ($Marred$Jntcs   ,1)
define ($Maresm$Jntcs   ,2)
define ($Marblu$Jntcs   ,3)
define ($Marwht$Jntcs   ,4)
define ($Tacelint$Jntcs ,5)
define ($Tacrepelt$Jntcs,6)      # TACREP(ELINT)

# Table 1.5   Msg Catagory: to SUN workstation (RAINFORM/ASTAB) or to CCN (DTS)

define ($MsgCat$CCN      ,1)
define ($MsgCat$SUN      ,2)
define ($MsgCat$SUNRAIN  ,3)
define ($MsgCat$SUNASTAB1,4)
define ($MsgCat$SUNASTAB2,5)
define ($MsgCat$SUNASTAB3,6)
define ($MsgCat$SUNASTAB4,7)

# Table 2.0       Msg$posline$i

# Table 2.1       Msg$posline$i for Red Msgs

define ($Arealn$Rain  ,0)
define ($Brngln$Rain  ,1)
define ($Elipln$Rain  ,2)
define ($Lobsln$Rain  ,3)
define ($Timeln$Rain  ,4)
define ($Tmposln$Rain ,5)
define ($Poselln$Rain ,6)

# Table 2.2       Msg$posline$i for Gold Msgs

define ($Posln$Rain   ,0)
define ($Lobln$Rain   ,1)
define ($Radln$Rain   ,2)

# Table 3.0       Msg$UnitType$i

define ($Unk$RType        ,0)
define ($None$RType       ,0)
define ($Air$RType        ,1)
define ($Sub$RType        ,2)
define ($Surf$RType       ,3)
define ($Surfwac$RType    ,4)
define ($CruiseMiss$RType ,5)
define ($AirSub$RType     ,6)  
define ($AirSurf$RType    ,6)
define ($ROTHRsite$RType  ,7)

# Table 3.5       Ship Types or Status

define ($unk$RShip   ,0)
define ($nav$RShip   ,1)
define ($navnp$RShip ,2)
define ($oth$RShip   ,3)
define ($othnp$RShip ,4)
define ($mer$RShip   ,5)
define ($fsh$RShip   ,6)
define ($sts$RShip   ,7)
define ($stsnp$RShip ,8)
define ($tom$RShip   ,9)   # Cruise missle shooter
define ($obg$RShip  ,10)   # SAG main unit
define ($rth$RSta   ,11)   # ROTHR reporting station
define ($navnru$Rship,12)  # Navy Non Reporting Unit -  joa 8/98 (E2027)

# Table 4.0       Delay Catagory (MSG$DelayCat$I) not yet in MSG Table

define ($Unk$RDelay   ,0)
define ($Air$RDelay   ,1)
define ($Sub$RDelay   ,2)
define ($Ship$RDelay  ,3)
define ($HFDF$RDelay  ,4)
define ($SOSUS$RDelay ,5)
define ($SELOR$RDelay ,6)
define ($ROSIS$RDelay ,7)
define ($WOSIS$RDelay ,8)
define ($White$RDelay ,9)
define ($Spec$RDelay ,10)
define ($GOLDA$RDelay ,11)
define ($GOLDS$RDelay ,12)
define ($GESMA$RDelay ,13)
define ($GESMS$RDelay ,14)
define ($GSEL$RDelay  ,15)
define ($GWIZ$RDelay  ,16)
define ($GSOS$RDelay  ,17)
define ($GHFD$RDelay  ,18)
define ($TASS$RDelay  ,19)
define ($GSWB$RDelay  ,20)
define ($GROTHR$RDelay,21)
define ($GIR$RDelay   ,22)

# Table 5.0       Msg$Event$i  (Event Field for TMPOS line)
################ Currently not in use ####################
define ($Gained$REvent  ,1)
define ($Holding$REvent ,2)
define ($Broken$REvent  ,3)
define ($Lost$REvent    ,4)
define ($Fixed$REvent   ,5)
define ($NoFix$REvent   ,6)

# Table 5.5       Msg$Sensor$I  (Sensor Field for TMPOS line)  Range: 0 - 15

define ($Other$RSens  ,0)
define ($Radar$RSens  ,1)
define ($ESM$RSens    ,2)
define ($AESM$RSens   ,3)
define ($Visual$RSens ,4)
define ($SOSUS$RSens  ,5)
define ($SonAct$RSens ,6)
define ($SonPas$RSens ,7)
define ($HFDF$RSens   ,8)
define ($SELOR$rSens  ,9)
define ($OSIS$RSens   ,10)
define ($TASS$Rsens   ,11)
define ($SWABS$RSens  ,12)
define ($LIDAR$RSens  ,13)
define ($MAD$RSens    ,14)
define ($IR$RSens     ,15)

# Table 6.0       Msg$PropMode$i (A); Msg$AirType$i (B)

define ($Unknown$Prop ,0)    # A
define ($Diesel$Prop  ,1)    # A
define ($Nuclear$Prop ,2)    # A

define ($Unknown$Acc  ,0)    # B
define ($Jet$Acc      ,1)    # B
define ($Prop$Acc     ,2)    # B
define ($Helo$Acc     ,3)    # B

# Table 7.0       Message Generator Type (igentype)

define ($Rain$Gen     ,0)
define ($Post$Gen     ,1)
define ($Jntcs$Gen    ,2)

# Table 8.0          Force codes for the ctc line

define ($FC$unk$unknown,   0)
define ($FC$air$hostile,   1)
define ($FC$air$unknown,   2)
define ($FC$air$friendly,  3)
define ($FC$sub$hostile,   4)
define ($FC$sub$unknown,   5)
define ($FC$sub$friendly,  6)
define ($FC$surf$hostile,  7)
define ($FC$surf$unknown,  8)
define ($FC$surf$friendly, 9)
define ($FC$air$neutral,  12)
define ($FC$sub$neutral,  15)
define ($FC$surf$neutral, 18)
#define ($FC$unk$unknown,  19)

# Table 9.0          Miscellaneous

define($idle,0)
define($busy,1)

define($No,0)
define($Yes,1)

define($Off,0)
define($On,1)

define($False,0)
define($True,1)

define ($Blue$Designator,$letB)

define(sendmsg,0)
define(nomsg,999)

# Table 9.1       Save/Restore Audit

define($save,0)
define($restore,1)

# Table 9.2       Lat/Long Format

define($nocksumcomma,0)
define(cksum$nocomma,1)
define(comma$nocksum,2)

# Table 10.0  Function Code (ifuncode) V6 transmit/receive

define(enter$tx$msg ,1)  # From v6 to msg gen: "send your message"
define(reject$tx$msg,2)  # From v6 to msg gen: "your msg rejected"
define(accept$tx$msg,3)  # From v6 to msg gen: "your msg accepted"

# Table 10.1  msgstatus - V6 response to query to send msg from msg gen and
#                         after msg sent whether it is rejected or accepted 

#define(nomsg        ,0)  # No message activity
#define(sendmsg      ,1)  # It is of to send the message
#define(rejectmsg    ,2)  # The sent msg is rejected
#define(acceptmsg    ,3)  # The sent msg is accepted.

# Table 10.2  Message Reject Code (irejcode) V6 transmit/receive

define (queue$space ,1)   # Transmit queue space exceeded
define (illegal$add ,2)   # Illegal address specified in the Request
                          # to Enter Transmit Message
define (frame$time  ,3)   # Transmit data frame interarrival time
                          # exceeded (5 seconds)
define (net$flag    ,4)   # Invalid net flag indicator
define (no$sid      ,5)   # Attempt to enter data link msg before (V)6
                          # operator has asgnd the SID to this station

# Table 10.3  Target Track Id codes

define $TRK_No_ID 0
define $TRK_ID    1
define $TRK_Class 2

