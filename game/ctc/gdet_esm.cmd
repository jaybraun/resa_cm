! file gdet_esm.cmd  - orange ship activated radar to test ESM detection and display
! CHOJU is mapped to chong_ju_pc

! create orange ship
CREATE SHIP ORADAR1 CHOJU 6.1.0.1 33-07n 130E 180 10
CREATE SHIP ORADAR2 CHOJU 6.1.0.2 33-07n 130E 180 10
CREATE SHIP ORADAR3 CHOJU 6.1.0.3 33-07n 130E 180 10
CREATE SHIP ORADAR4 CHOJU 6.1.0.4 33-07n 130E 180 10
#
! relocate to detection area north, east, soutc and west fo center point
RELOCATE ORADAR1 33-05N 128-00E
RELOCATE ORADAR2 33-00N 128-05E
RELOCATE ORADAR3 32-55N 128-00E
RELOCATE ORADAR4 33-00N 127-55E
#
! Supersede any stationing
FOR ORADAR1 COURSE 180 SPEED 0
FOR ORADAR2 COURSE 180 SPEED 0
FOR ORADAR3 COURSE 180 SPEED 0
FOR ORADAR4 COURSE 180 SPEED 0

! Activate RADAR
for oradar1 activate radar
for oradar2 activate radar
for oradar3 activate radar
for oradar4 activate radar

  
