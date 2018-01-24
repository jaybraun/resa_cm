!tc20_cruise.cmd - resa sea.surface fires cruise missiles at specific ground model targets
create ship shipcm ddg51 4.5.1.8 38-26n 130e 0 0
relocate shipcm 38n 130e

for shipcm add item 20 agm84
for shipcm replenish to 20 agm84
for shipcm add item 20 tlamc
for shipcm replenish to 20 tlamc

for shipcm fire 1 tlamc cruise at position 39n 128e target road
for shipcm fire 1 tlamc cruise at position 39n 128e target radar
for shipcm fire 1 tlamc cruise at position 39n 128e target bridge engineered
for shipcm fire 1 tlamc cruise at position 39n 128e target bridge fixed
for shipcm fire 1 tlamc cruise at position 39n 128e target helicopter
for shipcm fire 1 tlamc cruise at position 39n 128e target artillery
for shipcm fire 1 tlamc cruise at position 39n 128e target airdefense
for shipcm fire 1 tlamc cruise at position 39n 128e target infantry
for shipcm fire 1 tlamc cruise at position 39n 128e target trucks
for shipcm fire 1 tlamc cruise at position 39n 128e target armor
for shipcm fire 1 tlamc cruise at position 39n 128e
! Check damage astab; check that resa send attrition report; check damage notice on swar
! Check for outgoing alsp engagement interaction; incoming attrition report; ground model
!   damage for all types of targets including non-specific target

#Results
