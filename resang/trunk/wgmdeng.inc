define (NUcommon,
    integer kside, iBURST, iYIELD, INDXtgt, iDEPTH, IntcptFlag, nPH
    real*4      Flat, Flon
common/NUcommon_z/kside,iBURST,iYIELD,INDXtgt,iDEPTH,IntcptFlag,nPH,Flat,Flon
 )

define($Blip$EMIType,1)
define($Decm$EMIType,2)
define($Rboc$EMIType,3)
define($Jamr$EMIType,4)

#$% msg codes
define($InWater$Code,1)
define($AcquiredTarget$Code,2)
define($Exploded$Code,3)
define($ShutDown$Code,4)
define($ExplosionAt$Code,5)

define (target$slots,(shc$slots+acc$slots+cmc$slots))
define (weapon$slots,(cmc$slots+mc$slots+bom$slots))

define (Eng$ProbHits,
    byte                eph[target$slots,weapon$slots], eph_loaded
    common  /ehp_com/   eph, eph_loaded
    )
define (Eng$ProbIndx,
    integer             T_indx, W_indx
    common  /epi_com/   T_indx, W_indx
    )
