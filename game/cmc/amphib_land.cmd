# amphib_land.cmd   Amphibious Landing Operations 

# Build Ground objects must exist for this test procedure to work
#     SRLS

# Create Amphib ship
CREATE SHIP LSD LSD41 4.5.11.30  37-51N 124-56E 0 0      

# relocate amphib ship
relocate lsd 37-51n 124-56e

# Add landing craft to ship
ENTER BOAT LC1 LCAC LSD      
ENTER BOAT LC1 LCAC LSD      
ENTER BOAT LC2 LCAC LSD      
ENTER BOAT LC3 LCAC LSD      

# Add ground equipment to amphib ship
FOR LSD ADD ITEM 100 SRLS      
FOR LSD REPLENISH TO 100 SRLS      

# Create landing zone
DEFINE LANDING LZ1  37-54N 125-03E 80  37-52N 125-08E  37-57N 125-04E  37-54N 125-02E BLUE      

# Plot landing zone
PLOT LANDING
PLOT LABEL LANDING ALL

# Create Landing lane
DEFINE ROUTE LZ1IN1 BLUE 4      
PROCEED POSITION 37-51N 125-03E      
STOP

DEFINE ROUTE LZ1OUT1 BLUE 4      
PROCEED POSITION 37-49N 124-59E      
STOP

# Boat landing plan
FOR LSD LAUNCH BOAT 1 LCAC LC1LZ1      
LOAD 5 SRLS      
EXECUTE ROUTE LZ1IN1      
LAND LZ1 SPEED 40
EXECUTE ROUTE LZ1OUT1      
MOOR
