! file gaa_aam.cmd - Ghosted Air to Air target of RESA engagement 
! MIG21 is mapped to MIG21

create shore obaseair obase01 6.1.1.1 38n 127-45e 
relocate obaseair 38N 127E
enter air ob 1 10 mig21 obaseair 0 0 sqdrn1 strike
#
! create blue target flight
CREATE AIRCRAFT obaseair AIRTGT mig21 1 38N 159E 5000 270 0
#

! relocate air target to engagment area
RELOCATE AIRTGT 38N 159E
#
! Supersede any stationing
FOR AIRTGT COURSE 270 speed 300
#

