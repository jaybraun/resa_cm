define (chaff$SinkRate,400) # jb Sinks this many feet per min. 2/20/90

define(trace$radar$rad, istrip(trace$radar,0,M1bits)) # trace 
define(trace$radar$esm, istrip(trace$radar,1,M1bits)) # trace 
define(trace$radar$jam, istrip(trace$radar,2,M1bits)) # trace 
define(trace$radar$emi, istrip(trace$radar,3,M1bits)) # trace 

define ($OutsideChaff$code,0)
define ($ProjectionInChaff$code,1)
define ($ActuallyInChaff$code,2)

define($MaxREJPerUnit,10)
define($MaxJammersPerUnit,1)
define($MaxCommsJammersPerUnit,4)

define(RMcommon,
    Integer*4       Unit_Pointer[ unt$slots ]
    Integer*4       Unit_InChaffBarIndex[ unt$slots ]
    Integer*4       Unit_First_Sensor_Index[ unt$slots ]
    Integer*4       Unit_Last_Sensor_Index[ unt$slots ]
    Integer*4       Unit_First_Jammer_Index[ unt$slots ]
    Integer*4       Unit_Last_Jammer_Index[ unt$slots ]
    Byte            Unit_InChaff[ unt$slots ]
    Byte            Unit_Type[ unt$slots ]

    Integer*4       Device_DEQ_Pointer[ UNT$Slots * $MaxREJPerUnit]
    Integer*2       Device_Unit_Index[ UNT$Slots * $MaxREJPerUnit]              # index back to owner
    Byte            Device_Type[ UNT$Slots * $MaxREJPerUnit]
    Real*4          Device_SQRT_AntHeight[ UNT$Slots * $MaxREJPerUnit]          # SQRT altitude of device
    Integer*4       Device_Equip_Pointer[ UNT$Slots * $MaxREJPerUnit]           # eg. RAD$Pointer
    Integer*4       Device_ANT_Pointer[ UNT$Slots * $MaxREJPerUnit]             # 0 if no blockage

    Integer*4       Device_EQ_Pointer[ UNT$Slots * $MaxREJPerUnit]
    equivalence (Device_DEQ_Pointer, Device_EQ_Pointer)

    integer*4   Current_Max_Units, # counter for radar/esm/jammer list
                kjammr,            # counter for effective jammer list
                kchbar,            # counter for chaff barrier list
                kchaff,            # counter for nearby-chaff-barrier list
                isit               # 1=airsch;2=surfsch;3=jam(for trace)4/22/83
                                   # 4=approach;5=CommJammer

    real*4      brgjam[unt$slots * $MaxJammersPerUnit], # bearing to jammer [kjammr]
                pwrjam[unt$slots * $MaxJammersPerUnit], # noise power received due to jammer[kjammr]
                topchf[boy$slots], # rel. height of chaff top [kchaff]
                botchf[boy$slots], # rel. hgt of chaff bottom [kchaff]
                rngchf[boy$slots], # average range to chaff [kchaff]
                brgchf[boy$slots], # brg to 1 end of barrier [kchaff]
                subtnd[boy$slots]  # angle subtended by barrier [kchaff]

    Integer*4   Unit_With_Jammer[unt$slots * _
                   ($MaxCommsJammersPerUnit + $MaxJammersPerUnit)],  # reference to unit with jammer[kjammr]
                Jammer_DEQ[unt$slots * _
                    ($MaxCommsJammersPerUnit + $MaxJammersPerUnit)]

    Integer*4  ichfx[boy$slots]    # index to chaff barrier [kchbar]

    common /REJ_Common/ Current_Max_Units, kjammr, kchbar, kchaff, isit,
                         brgjam, pwrjam, botchf, rngchf, brgchf, subtnd, 
                         topchf, ichfx, Unit_With_Jammer, Jammer_DEQ,
                         Unit_Pointer,
                         Unit_InChaffBarIndex,
                         Unit_First_Sensor_Index,
                         Unit_Last_Sensor_Index,
                         Unit_First_Jammer_Index,
                         Unit_Last_Jammer_Index,
                         Unit_InChaff,
                         Unit_Type,
                         Device_DEQ_Pointer,
                         Device_Unit_Index,
                         Device_Type,
                         Device_SQRT_AntHeight,
                         Device_Equip_Pointer,
                         Device_ANT_Pointer


    $par$shared / REJ_Common /
    )


define($$$AirRadar,1)
define($$$SurfaceRadar,2)
define($$$ApproachRadar,3)
define($$$FireControlRadar,4)
define($$$ESM,5)
define($$$RadarJammer,6)
define($$$CommJammer,7)
