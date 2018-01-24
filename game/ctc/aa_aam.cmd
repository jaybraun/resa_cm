! file aa_aam.cmd - Air to Air engagement of resa air and awsim air target
! EA6B is mapped to EA6B
! AIM120 is mapped to AIM120A
create ship CVN68 CVN68 4.5.6.5 38N 156E 270 0
relocate CVN68 38N 156E
enter air cv 1 10 ea6b cvn68 0 0 sqdrn1 strike
#
! create blue target flight
CREATE AIRCRAFT CVN68 SHOOT ea6b 1 38N 156E 30000 90 0
#
! add aam to aircraft
FOR SHOOT ADD ITEM 10 AIM120
FOR SHOOT REPLENISH TO 10 AIM120

! relocate air target to engagment area
RELOCATE SHOOT 38N 156E
RELOCATE CVN68 38N 156E
#
! Supersede any stationing
FOR SHOOT COURSE 90
#
! Weapons free air to shoot at flight on any side
FOR SHOOT WEAPONS FREE AIR
#
! Verify flight status. AAM should be decremented
SITREP SHOOT    
#
! ^g when engagement is over

FOR SHOOT WEAPONS TIGHT
FOR SHOOT BINGO

