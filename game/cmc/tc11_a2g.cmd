!tc9_s2g.cmd - sea.surface engages ground object using 5 inch gun
create ship nrgn cvn68 4.5.1.3 37-45-21n 128.59e 0 0
relocate nrgn 37-45-21n 128.59e
enter air rg 250 25 fa18c nrgn 0 0 rg250 cap
for nrgn add item 10 gbu32
for nrgn replenish to 10 gbu32
for nrgn launch 1 fa18c bomb1 90 450 12000
load 5 gbu32
mission strike
strike position 37-29-31n 126-52-01e speed 500
stop
! Check bomb1 aircraft launches
! Check flight to target
! Check for alsp engagement interaction msg
! Check for attrition report

#Results
