! ew.cmd - Activate Jammer against WARSIM RADAR
!          Activate RADAR for ESM detection by WARSIM
# cvn68 mapped to nimitz_cvn
# aw148 mapped to apq153_air
# alq99 mapped to alq99
create ship CVN68 CVN68 4.5.6.5 39-01N 128-52E 270 0
relocate CVN68 39-01N 128-52E
enter air ea 1 10 ea6b cvn68 0 0 sqdrn1 strike
for cvn68 scramble 1 ea6b mission 1234a 90 100 10000
activate radar using aq148
activate jammer position 39-01n 127-27e using alq99
proceed position 39-01n 127-22E
bingo
