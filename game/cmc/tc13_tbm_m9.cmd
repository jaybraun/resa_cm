!tc13_tbm_m9.cmd - resa shorebase fires cruise tbm at ground model target
create shore redbase seoul 6.1.1.1 37-45n 131-30e
relocate redbase 37-45n 131-30e

for redbase add item 20 m9
for redbase replenish to 20 m9

for redbase fire 1 m9 ballistic at position 37-45-22.7n 129-57-08e

! Check for successful fire; check for outgoing alsp engagement interaction msg; 
! Check for incoming attrition report

#Results
