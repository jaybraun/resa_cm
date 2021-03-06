! sa_expsam_cm.cmd - AEGIS ship fires explicit sam at CM and BM
CREATE SHORE TBMFIRE OBASE01 6.1.0.20 38-19N 128-27E 
! create blue shooter ship
CREATE SHIP AEGIS DDG51 4.5.2.1 3600N 131E 0 0    
RELOCATE AEGIS  36N 131E
FOR AEGIS ADD ITEM 20 SM3 
FOR AEGIS REPLENISH TO 20 SM3     
! Add CM and Ballistic missiles
FOR TBMFIRE ADD ITEM 20 M9 
FOR TBMFIRE REPLENISH TO 20 M9     
FOR TBMFIRE ADD ITEM 20 AGM84  
FOR TBMFIRE REPLENISH TO 20 AMG84 
! set roe for explicit sam          
FOR AEGIS WEAPONS FREE AIR      
FOR AEGIS ACTIVATE RADAR 
CENTER POSITION 36N 131E RADIUS 600
PLOT RADAR AEGIS SPS48 NO JAMMING CRUISE AGM84 90 20000 COLOR RED
! Fire BM and CM       
FOR TBMFIRE FIRE 1 M9 BALLISTIC AT POSITION 35-21N 129-52E 
#FOR TBMFIRE FIRE 1 AGM84 CRUISE AT POSITION 35-21N 129-52E TIME 2
SHOW AIR DEFENSE 1 BLUE 4     
