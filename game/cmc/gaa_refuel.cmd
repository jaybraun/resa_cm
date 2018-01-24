! gaa_refuel.cmd - Launch Aerial tanker
! Launch tanker with mission airtnk and callsign airfuel.
! Note: ghosted tanker must has mission airtnk and port.ini must have DEFINE ALSP AIRSUPPORT BLUE VIEW 4
create ship CVFUEL cvn68 4.5.6.5 39-01N 128-52E 270 0
relocate CVFUEL 39-01N 128-52E
enter air cf 11 20 c130 cvfuel 0 0 sqdrn2 tanker

! Launch tanker
for cvfuel scramble 1 c130 mission airtnk callsign airfuel 90 100 10000
mission tanker
proceed position 39-01N 130E
bingo time 60

