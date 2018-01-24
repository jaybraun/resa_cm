! file sg_cm.cmd  surface to ground cruise missile engagement
! DDG51 mapped to burke_ddg
! SLAM mapped to agm84e_slam 
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
FOR DDG1 ADD ITEM 5 SLAM
FOR DDG1 REPLENISH TO 5 SLAM 
#
FOR DDG1 FIRE 1 SLAM CRUISE AT POSITION 38N 155-45E
! Cruise missile should be deleted one cycle after impact

