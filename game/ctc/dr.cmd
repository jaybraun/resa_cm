! dr.cmd - Dead reckon ship
#dg47 mapped to ticonderoga_cg
! Create ship for dead reckoning
create ship drship cg47 4.5.1.17 39-01-38N 128-40E 270 0
relocate bcmfire 39-01-38N 128-40E
for drship course 17 speed 17
! RESA sends update message every other cycle
! WARSIM needs course and speed in update message in order to DR
! Check BSWS for speed leader on drship
