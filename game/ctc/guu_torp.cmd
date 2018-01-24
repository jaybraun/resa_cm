! file guu_torp.cmd  sub to sub torpedo engagement.  Setup target
! ROMEO is mapped to romeo_sscre

! create blue shooter sub
CREATE SHIP OSUBTGT ROMEO 6.1.0.7 38N 135-0-3E 275 0
#
! relocate TGT SUB to engagment area
RELOCATE OSUBTGT 38N 135-02E
#
! Supersede any stationing
FOR OSUBTGT COURSE 180 SPEED 0
