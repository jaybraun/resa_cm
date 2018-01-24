! det_masking.cmd - test terrain masking and performance

! Create blue 4 carrier
create ship cvb4 cvn68 4.5.9.1 33-37N 126-25E 270 0
relocate cvb4 33-37N 126-25E

! Create aircraft on carrier
enter air cv 100 100 fa18f cvb4 0 0 cv100 cap
enter air cv 200 100 fa18f cvb4 0 0 cv200 cap
enter air cv 300 100 fa18f cvb4 0 0 cv300 cap
enter air cv 400 100 fa18f cvb4 0 0 cv400 cap
enter air cv 500 100 fa18f cvb4 0 0 cv500 cap
enter air cv 600 100 fa18f cvb4 0 0 cv600 cap
enter air cv 700 100 fa18f cvb4 0 0 cv700 cap
enter air cv 800 100 fa18f cvb4 0 0 cv800 cap
enter air cv 900 100 fa18f cvb4 0 0 cv900 cap


! create 100 blue 4 air in single flight
CREATE AIRCRAFT CVB4 DETB41 FA18F 100 33-37N 126-25E 500 90 0
CREATE AIRCRAFT CVB4 DETB42 FA18F 100 33-37N 126-25E 500 90 0
CREATE AIRCRAFT CVB4 DETB43 FA18F 100 33-37N 126-25E 500 90 0
CREATE AIRCRAFT CVB4 DETB44 FA18F 100 33-37N 126-25E 500 90 0
CREATE AIRCRAFT CVB4 DETB45 FA18F 100 33-37N 126-25E 500 90 0
CREATE AIRCRAFT CVB4 DETB46 FA18F 100 33-37N 126-25E 500 90 0
CREATE AIRCRAFT CVB4 DETB47 FA18F 100 33-37N 126-25E 500 90 0
CREATE AIRCRAFT CVB4 DETB48 FA18F 100 33-37N 126-25E 500 90 0
CREATE AIRCRAFT CVB4 DETB49 FA18F 100 33-37N 126-25E 500 90 0
FOR 4.5.9.1 ACTIVATE RADAR

! create 99 flights from single flight of 100 aircraft in single order
FOR 4.5.9.1 SPLITOFF 99 1


! Create Blue 3 carrier
create ship cvb3 cvn68 4.4.9.1 33-12N 126-33E 270 0
relocate cvb3 33-12N 126-33E

! create aircraft on carrier
enter air cb 100 100 fa18f cvb3 0 0 cb100 cap
enter air cb 200 100 fa18f cvb3 0 0 cb200 cap
enter air cb 300 100 fa18f cvb3 0 0 cb300 cap
enter air cb 400 100 fa18f cvb3 0 0 cb400 cap
enter air cb 500 100 fa18f cvb3 0 0 cb500 cap
enter air cb 600 100 fa18f cvb3 0 0 cb600 cap
enter air cb 700 100 fa18f cvb3 0 0 cb700 cap
enter air cb 800 100 fa18f cvb3 0 0 cb800 cap
enter air cb 900 100 fa18f cvb3 0 0 cb900 cap

! create 100 blue 3 air in single flight
CREATE AIRCRAFT CVB3 DETB31 FA18F 100 33-12N 126-33E 500 90 0
CREATE AIRCRAFT CVB3 DETB32 FA18F 100 33-12N 126-33E 500 90 0
CREATE AIRCRAFT CVB3 DETB33 FA18F 100 33-12N 126-33E 500 90 0
CREATE AIRCRAFT CVB3 DETB34 FA18F 100 33-12N 126-33E 500 90 0
CREATE AIRCRAFT CVB3 DETB35 FA18F 100 33-12N 126-33E 500 90 0
CREATE AIRCRAFT CVB3 DETB36 FA18F 100 33-12N 126-33E 500 90 0
CREATE AIRCRAFT CVB3 DETB37 FA18F 100 33-12N 126-33E 500 90 0
CREATE AIRCRAFT CVB3 DETB38 FA18F 100 33-12N 126-33E 500 90 0
CREATE AIRCRAFT CVB3 DETB39 FA18F 100 33-12N 126-33E 500 90 0
FOR 4.4.9.1 ACTIVATE RADAR

! create 99 flights from single flight of 100 aircraft in single order
FOR 4.4.9.1 SPLITOFF 99 1
