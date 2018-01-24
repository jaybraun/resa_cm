!tc12_g2a.cmd - ground model engages resa air.fixedwing using san6
create ship nrgn cvn73 4.5.1.3 37-45-21n 128.59e 0 0
relocate nrgn 37-45-21n 128.59e
enter air rg 350 25 fa18f nrgn 0 0 rg350 cap
for nrgn launch 1 fa18f tgt 90 450 12000
mission reconn
activate radar
proceed position 38-41-47n 126-38-43e
proceed position 38-41-48n 126-38-44e time +10
proceed position 38-41-47n 126-38-43e time +10
bingo
! Check bomb1 aircraft launches
! Check flight to target
! Check for alsp engagement interaction msg
! Check for attrition report

#Results
