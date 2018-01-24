! file sa_gun.cmd  surface to air engagement of awsim aircraft using gun
! DDG51 mapped to burke_ddg
! G5IN mapped to gun_5in
! G5IN must be defined as $gun$code in alsp*.rin
! create blue shooter ship
CREATE SHIP DDG1 DDG51 4.5.2.3 38N 156E 90 0
#
! relocate shooter ship to engagment area
RELOCATE DDG1 38N 156E
#
! Supersede any stationing
FOR DDG1 COURSE 90 SPEED 0
#
! add sam to ship
FOR DDG1 ADD ITEM 100 g5in
FOR DDG1 REPLENISH TO 100 g5in
#
! Weapons free air
FOR DDG1 WEAPONS FREE AIR
FOR DDG1 ACTIVATE RADAR
#
!Verify ship status
SITREP DDG1
#
! ship automatically fires gun at aircraft on any side
! type ^g when engagement over

! Set weapons tight
FOR DDG1 WEAPONS TIGHT
#
! Verify ship status.  SAM count should be reduced.
SITREP DDG1
! Verify aircraft destroyed on air destroyed astab for blue 5

