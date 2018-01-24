! aa_refuel.cmd - Aerial refueling
! Launch fighter.  Tell fighter to refuel from tanker using callsign
create ship BCVFUEL cvn68 4.5.6.5 39-01N 128-52E 270 0
relocate BCVFUEL 39-01N 128-52E
enter air cf 1 10 fa18f bcvfuel 0 0 sqdrn1 strike
center position 39n 128e radius 200
show flight 1 blue 4

! Launch fighter
for bcvfuel scramble 1 fa18f mission gogo 90 100 10000
mission strike
station 90 bcvfuel 50
stop

! ^g when flights launch so refuel will be entered

for gogo expend 10000 jp 
for gogo refuel callsign airfuel onload 10000
