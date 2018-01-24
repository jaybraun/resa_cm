! file gsa_sam.cmd - Air to Surface engagement of aircraft by RESA ship using SAM
! FA18F is mapped to FA18F
! ship guns against ghost air must be $gun$code
! Guns will only engage tracks designated unknown or enemy
! lcl radar detections are designated by routine lclras by side.  Unknow is not possible.
 
create shore obaseair obase01 6.1.1.1 38n 127-45e 
relocate obaseair 38N 127E
enter air ob 1 10 mig21 obaseair 0 0 sqdrn1 strike
#
! create blue target flight
CREATE AIRCRAFT obaseair GUNTGT mig21 1 38N 156E 5000 90 0
#

! relocate air target to engagment area
RELOCATE GUNTGT 38N 156E
#
! Supersede any stationing
FOR GUNTGT COURSE 90 speed 0
#

