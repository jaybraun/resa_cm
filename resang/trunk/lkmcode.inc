####################  LKMCODE.INC ##########################

#Table 1.0      Binary Code Conversion For Message Label Fields

define ($Label$M1,2%0001)     # Data Reference Position Message
define ($Label$M81,2%1000)    # Data Reference Position Amplify Message
define ($Label$M2,2%0010)     # Air Track Position Message
define ($Label$M82,2%1000)    # Air Position Amplify Message
define ($Label$M3,2%0011)     # Surface Track Position Message
define ($Label$M83,2%1000)    # Surface Position Amplify Message
define ($Label$M4A,2%0100)    # ASW Primary Messge
define ($SubLabel$M4A,2%0000) # ASW Primary Message
define ($Label$M84A,2%1000)   # ASW Amplify Message
define ($Label$M6a,2%0110)    # ECM intercept data message
define ($Sublabel$M6A,2%00)   # ECM intercept data message (Sublabel)
define ($Label$M6B,2%0110)    # ESM primary message
define ($Sublabel$M6B,2%01)   # ESM primary message (Sublabel)
define ($Label$M86B,2%1000)   # ESM amplify message
define ($Label$M6C,2%0110)    # ESM parameter message
define ($Sublabel$M6C,2%10)   # ESM parameter message (Sublabel)
define ($Label$M86C,2%1000)   # ESM parametric amplify message 
define ($Label$M6D,2%0110)    # E.W. Coordination & Control
define ($Sublabel$M6D,2%11)   # E.W. Coordination & Control (Sublabel)
define ($Label$M9A,2%1001)    # M.9A Managment Message (Information)
define ($SubLabel$M9A,2%0000) # M.9A Management Message Amp (Information)
define ($Label$M9B,2%1001)    # M.9B Management Message (Pairing/Association)
define ($SubLabel$M9B,2%0001) # M.9B Management Message Amp (Pairing/Assoc.)
define ($Label$M9D,2%1001)    # M.9D Management Message (LINK-11 Monitor)
define ($SubLabel$M9D,2%0011) # M.9D Management Message Amp (LINK-11 Monitor.)
define ($Label$M11D,2%1011)   # M.11D IFF/SIF Message
define ($SubLabel$M11D,2%0011)# M.11D IFF/SIF Message
define ($Label$M1200,2%1100)  # M.12.0.0 Training Position Report Message
define ($SubLabel$M1200,2%000)# M.12.0.0 Training Position Report Message
define ($Label$M1201,2%1100)  # M.12.0.1 Training Position Amplify Message
define ($SubLabel$M1201,2%001)# M.12.0.1 Training Position Amplify Message
define ($Label$M1202,2%1100)  # M.12.0.2 Parameter Change Message (Training)
define ($SubLabel$M1202,2%010)# M.12.0.2 Parameter Change Message (Training)
define ($Label$M1203,2%1100)  # M.12.0.3 IFF/SIF Message Modes 1 & 2 (Training)
define ($SubLabel$M1203,2%011)# M.12.0.3 IFF/SIF Message Modes 1 & 2 (Training)
define ($Label$M1231,2%1100)  # M.12.31 Timing Message
define ($SysInd$M1231,2%11111)# M.12.31 System Indicator For Timing Message

# Table 1.5   Msg Catagory: to SUN workstation (RAINFORM/ASTAB) or to CCN (DTS)

define ($MsgCat$CCN      ,1)
define ($MsgCat$SUN      ,2)
define ($MsgCat$SUNRAIN  ,3)
define ($MsgCat$SUNASTAB ,4)

#Table 2.0      Originator Environment Catagories

define ($Surf$Cat, 0)         # Catagory Surface
define ($Sub$Cat, 1)          # Catagory Submarine
define ($Shore$Cat, 2)        # Catagory Land
define ($Air$Cat, 3)          # Catagory Air
define ($Unk$Cat, 7)          # Catagory Unknown

#Table 3.0      ID Values     M4A$TrackIdentity$I, M3$TrackIdentity$I, 
#                             M2$TrackIdentity$I 
define ($ID$Unknown,2%00)     # Unknown
define ($ID$AFriend,2%01)     # Friend (non-NTDS unit)
define ($ID$Hostile,2%10)     # Hostile
define ($ID$BFriend,2%11)     # Friend (NTDS unit)
define ($ID$EVUnknown,2%11)   # Evaluated Unknown (for submarines)

#Table 4.0 Threat Evaluation (TE)   M6a$TE$i 

define ($TE$NoStmnt,2%00)     # No Statement
define ($TE$Unknown,2%01)     # Unknown
define ($TE$Friend, 2%10)     # Non-Threat/Friend
define ($TE$Threat, 2%11)     # Threat

#Table 5.0 Platform (PL)   M6a$platform$i 

define ($PL$Unknown, 2%00)     # Unknown
define ($PL$Missile, 2%01)     # Missile
define ($PL$Air,     2%10)     # Airborne
define ($PL$Surface, 2%11)     # Surface
define ($PL$Sub,    2%100)     # Submarine
define ($PL$Land,   2%101)     # Land
define ($PL$AMCU,   2%110)     # Airborne Missile Control Unit
define ($PL$SMCU,   2%111)     # Surface/Sub Missile Control Unit

#Table 6.0             message groups in lkreadin

define ($lkm1$msgs,     1)    # M.1 & M.81 msg group
define ($lkm2$msgs,     2)    # M.2 & M.82 msg group
define ($lkm3$msgs,     3)    # M.3 & M.83 msg group
define ($lkm4a$msgs,    4)    # M.4a & M.84a msg group
define ($lkm6a$msgs,    6)    # M.6a & M.86a msg group
define ($lkm6bc$msgs,   7)    # M.6b,M.86b,M.6c,M.86c msg group


#Table 7.0             Contact types for wargame/NTDS track number conversion
# Note: 1 to 100 used by radindx in M.6B and M.6C series for unique radar

define ($radair$ctc,    101)    # Radar air contact  (M.2 series msgs)
define ($radsurf$ctc,   102)    # Radar surface contact (M.3 series msgs)
define ($radsub$ctc,    103)    # Radar submarine contact (M.4A series msgs)
define ($jam$ctc,       104)    # All ESM type contacts  (M.6A series msgs)

#Table 8.0             Raid/Track size for lkr$tracksize$i

define ($unk$tgts,      0)    # Unestimated number of targets
define ($one$tgts,      1)    # A single target
define ($few$tgts,      2)    # Few targets (2 - 7)
define ($many$tgts,     3)    # Many targets (8 or more)

define ($max_unit,300)        # Max number of units that can be NTDS equipped
                              # including ships (20), S3A (20) & P3 (11).

#Table 9.0             Miscellaneous

define($datamiles,(@1*1.01269)) # Convert nautical miles to NTDS data miles.
define($nautmiles,(@1/1.01269)) # Convert NTDS data miles to nautical miles.

define($busy$stat,      0)    # Waiting for WARGAME to go to next game minute
define($sending$stat,   1)    # Generating LINK-11 messages
define($pause$stat,     2)    # Game in pause mode

define($unk$side,      0)
define($blue$side,     1)
define($orange$side,   2)
