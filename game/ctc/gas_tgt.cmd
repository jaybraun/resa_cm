! file gas_tgt.cmd - Air to Surface engagement of resa ship by  air
! FA18F is mapped to FA18F
! 
create ship CVNB5 CVN68 4.6.6.5 38N 156E 270 0
relocate CVNB5 38N 156E
enter air cv 1 10 fa18f cvnB5 0 0 sqdrn1 strike
#
! create blue target flight
CREATE AIRCRAFT CVNB5 SHOOT fa18f 1 38N 156E 30000 90 0
#
! add asm to aircraft
FOR SHOOT ADD ITEM 10 agm65
FOR SHOOT REPLENISH TO 10 agm65


! relocate air target to engagment area
RELOCATE SHOOT 38N 156E
RELOCATE CVNB5 38N 156E
#
! Supersede any stationing
FOR SHOOT COURSE 90
#
! Weapons free air to shoot at flight on any side
FOR SHOOT ACTIVATE RADAR

FOR SHOOT WEAPONS FREE ALL
#
! ^g when engagement is over

#
! Verify flight status. AAM should be decremented
SITREP SHOOT    

FOR SHOOT WEAPONS TIGHT
FOR SHOOT BINGO

