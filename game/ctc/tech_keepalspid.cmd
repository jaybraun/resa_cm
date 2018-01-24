! file tech_keepalspid.cmd - keep RESA unit alspid during rejoin
delete unit cvn68
delete unit slot
delete unit cvn70
create ship CVN68 CVN68 4.5.6.5 38N 156E 270 0
enter air cv 1 10 ea6b cvn68 0 0 sqdrn1 strike
CREATE AIRCRAFT CVN68 SLOT ea6b 1 38N 156E 30000 90 0
create ship CVN70 CVN68 4.5.6.5 38N 156E 270 0
! ^g when aircraft created

! ^g when joined. Use hub copy <file> .:13811 to enter join_ack, signal and grant_advance messages
join

FOR SLOT BINGO
! ^g when aircraft recovers

display alsp id cvn70
resign remove delete
! after rejoin cvn70 should have same alspid

