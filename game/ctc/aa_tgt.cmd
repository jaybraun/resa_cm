! file aa_tgt.cmd - Air to Air engagement of awsim air and air target

! fixed is mapped to fa18c
create ship CVN68 CVN68 4.5.6.5 38N 156E 270 0
relocate CVN68 38N 156E
enter air cv 11 20 fa18c cvn68 0 0 sqdrn1 strike
#
#
! create blue target flight
CREATE AIRCRAFT CVN68 TGT FA18C 10 38N 156E  30000 90 0
#
! relocate air target to engagment area
RELOCATE TGT 38N 156E
RELOCATE CVN68 38N 156E
#
! Supersede any stationing
FOR TGT COURSE 90
#
! Verify flight status. Aircraft in flight should be decremented
SHOW AIR DESTORYED TIME 1 BLUE 4
#
! ^g when engagement is over

FOR TGT BINGO
