! file sg_cruz.pre  surface to ground cruise missile engagement
! DDG mapped to burke_ddg
! CRUZ mapped to agm-84k 
! create blue shooter ship
CREATE SHIP DDG1 DDG 1.1.2.3 38N 156E 90 0
#
! relocate shooter ship to engagment area
RELOCATE DDG1 38N 156E
#
! Supersede any stationing
FOR DDG1 COURSE 90
#
! add sam to ship
FOR DDG1 ADD ITEM 5 CRUZ
FOR DDG1 REPLENISH TO 5 CRUZ 
#
FOR DDG1 FIRE 1 CRUZ CRUISE AT POSITION 38N 155-45E
! Cruise missile should be deleted one cycle after impact
