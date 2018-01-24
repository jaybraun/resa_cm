! file uu_torp.cmd  sub to sub torpedo engagement
! SN688 mapped to los_angeles_ssn
! MK46A mapped to mk46 
! create blue shooter sub
CREATE SHIP SSNTORP SN688 4.4.0.14 38NN 135E 275 0

#
! relocate shooter ship to engagment area
RELOCATE SSNTORP 38N 135E
#
! Supersede any stationing
FOR SSNTORP COURSE 90 SPEED 0
#
! add torpedo to sub
FOR SSNTORP ADD ITEM 5 MK46A
FOR SSNTORP REPLENISH TO 5 MK46A 

! Activate sensors
FOR SSNTORP ACTIVATE SONAR ACTIVE

#
FOR SSNTORP FIRE 1 MK46A TORPEDO BEARING 90 0.0 SHALLOW

