! IFF.CMD  - ATO style Reconnaissance Air Mission
create shore aswbase rodn 4.5.9.1 37-10N 127-33E
enter air aw 1 1 ep3e aswbase 0 0 sqdrn1 rec
DELETE ROUTE N2125B ALL
DEFINE ROUTE N2125B BLUE 4
!ATO UNIT: [CTF72], AIRCRAFT TYPE: [EP3E], BASED AT: [RJSM]
!ATO SCL PRIMARY [BEST] = [1 BEST ] SECONDARY [-] = []
MISSION REC
SET IFF 1 00 2 0553 3 0553
ACTIVATE RADAR
!BLOCK 20000 60000
PROCEED POSITION N41-30-00 E131-30-00 ALTITUDE 20000
ORBIT 30 N41-30-00 E131-30-00 N37-01-10 E129-46-45 ALTITUDE 24000
!CANCEL AIRSPACE !AT 220000
BINGO TO aswbase time 60
FOR aswbase CANCEL SCRAMBLE N2125B ATOID UDLB
FOR aswbase SCRAMBLE 1 EP3E MISSION N2125B ATOID UDLB CALLSIGN WE03 0 205 20000
EXECUTE ROUTE N2125B
STOP
! Fly over friendly ground and verify that iff keeps aircraft from being engaged.

