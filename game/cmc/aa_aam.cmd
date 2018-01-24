! file aa_aam.cmd - Air to Air engagement of resa air and awsim air target
! fa18f is mapped to fa18f
! phenx is mapped to aim54a

create ship cv1 cvn68 4.5.9.1 38N 156E 270 0
relocate cv1 38N 156E

enter air cv 350 25 fa18f cv1 0 0 cv350 cap
#
! create blue target flight
CREATE AIRCRAFT CV1 SHOOT FA18F 1 38N 156E 30000 90 0
FOR SHOOT ACTIVATE RADAR
FOR SHOOT MISSION CAP
#
! add aam to aircraft
FOR SHOOT ADD ITEM 4 AIM54
FOR SHOOT REPLENISH TO 4 AIM54

! relocate air target to engagment area
RELOCATE SHOOT 38N 156E
RELOCATE CV1 38N 156E
#
! Supersede any stationing
FOR SHOOT STATION 90 CV1 50
#
! Weapons free air to shoot at flight on any side
FOR SHOOT WEAPONS FREE AIR
#
! Monitor engagement of geo and bogey astab
CENTER POSITION 38N 156E RADIUS 200
SHOW BOGEY 1 BLUE 4    

