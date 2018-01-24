! file sono_actdet.cmd - Active sonobuoy barrier detects orange sub
! P3C is mapped to P3C
! SSQ62 is mapped to SSQ62
! SSQ62 has max det range of 2 nm
create ship  LNCHP3C CVN68 4.5.6.5 38N 156E 270 0
relocate LNCHP3C 38N 156E
enter air ln 1 10 p3c lnchp3c 0 0 sqdrn1 strike
#
! create blue target flight
CREATE AIRCRAFT lnchp3c sonodrop p3c 1 33N 130E 5000 90 0
#
! add ssq62 to aircraft
FOR sonodrop ADD ITEM 10 ssq62
FOR sonodrop REPLENISH TO 10 ssq62

! relocate air target to engagment area
RELOCATE sonodrop 33N 130E

! Lay sonobuoy barrier
for sonodrop barrier chevron 33n 130-02e 0 ssq62 1 3

show active sonar 1 blue 4
center position 33n 130e radius 60
! Look for active sonar track at 2 nm



