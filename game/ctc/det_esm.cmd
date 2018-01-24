! file set_esm.cmd - ship esm detects and displays orange radars
! DDG51 mapped to burke_ddg

! create blue ESM ship
CREATE SHIP DDGESM DDG51 4.5.2.6 33N 128E 90 0
#
! relocate ESM ship to detection area
RELOCATE DDGESM 33N 128E
#
! Supersede any stationing
FOR DDGESM COURSE 90 SPEED 0
#
for ddgesm activate esm

center position 33n 128e radius 60

! Sign on blue 4 station to see lob
