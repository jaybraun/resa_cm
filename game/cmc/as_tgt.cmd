! file as_tgt.cmd - Air to Surface engagement of awsim air and surface target
! DDG51 mapped to burke_ddg
#
! create blue target ship
CREATE SHIP DDG1 DDG51 4.5.2.3 38N 156E 90 0
#
! relocate shooter ship to engagment area
RELOCATE DDG1 38N 156E
#
! Supersede any stationing
FOR DDG1 COURSE 90
#
! Verify ship status. Ship should be damaged eventually
SHOW DAMAGE TIME 1 BLUE 4

