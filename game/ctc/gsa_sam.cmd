! file gsa_sam.cmd - Air to Surface engagement of aircraft by RESA ship using SAM
! FA18F is mapped to FA18F
! 
create ship CVNB5 CVN68 4.6.6.5 38N 156E 270 0
relocate CVNB5 38N 156E
enter air cv 1 10 fa18f cvnB5 0 0 sqdrn1 strike
#
! create blue target flight
CREATE AIRCRAFT CVNB5 SAMTGT fa18f 1 38N 156E 30000 90 0
#

! relocate air target to engagment area
RELOCATE SAMTGT 38N 156E
RELOCATE CVNB5 38N 156E
#
! Supersede any stationing
FOR SAMTGT COURSE 90 speed 0
#

