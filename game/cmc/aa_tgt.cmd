! file aa_tgt.cmd - Air to Air engagement of awsim air and air target
! fa18f is mapped to fa18f
! verify awsim weapon mapping

create ship cv1 cvn68 4.5.9.1 38N 156E 270 0
relocate cv1 38N 156E

enter air cv 350 25 fa18f cv1 0 0 cv350 cap

#
! create blue target flight
CREATE AIRCRAFT CV1 TGT FA18F 10 38N 156E 30000 90 0
#
! relocate air target to engagment area
RELOCATE TGT 38N 156E
RELOCATE CV1 38N 156E
#
! Supersede any stationing
FOR TGT COURSE 90
#
! Verify flight status. Aircraft in flight should be decremented
SHOW AIR DESTROYED 1 BLUE 4
#
! ^g when engagement is over

FOR TGT BINGO
