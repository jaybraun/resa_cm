! file gsono_actdet.cmd  sub closes in on active sonobuoy barrier
! ROMEO is mapped to romeo_sscre

! create orange sub to be detected
CREATE SHIP OSUBDET ROMEO 6.1.0.8 33-07n 130E 180 10
#
! relocate DET SUB to detection area
RELOCATE OSUBDET 33-07N 130E
#
! Supersede any stationing
FOR OSUBDET COURSE 180 SPEED 10

! No Active sonar track until 2.0 nm
  
