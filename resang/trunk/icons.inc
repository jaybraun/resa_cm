# file icons.inc

include "bbicn.inc"         

define($MaxDefault$Icons,8)

define ($MaxIcons,arith(ICN$Slots,+,$MaxDefault$Icons))
define ($AvgIconSize, 80)
define ($Icon_Data_Pts,arith($MaxIcons,*,$AvgIconSize))

define($Ship$DefaultIndex,1)
define($Jet$DefaultIndex,2)
define($Propellor$DefaultIndex,3)
define($Sub$DefaultIndex,4)
define($Cruise$DefaultIndex,5)
define($Torpedo$DefaultIndex,6)
define($CtrMeasure$DefaultIndex,7)
define($Helo$DefaultIndex,8)

define(ICON$Common,
        Integer*4   icon_Index_First_Data [ $MaxIcons ]
        Integer*4   icon_Index_Into_First_Data [ $MaxIcons ]
        Real*4      icon_Drawing_Scale [ $MaxIcons ]
        Integer*4   icon_Tot_Data_Pts
        Integer*2   icon_Num_Icons
        Integer*2   icon_Default_Indx[ $MaxDefault$Icons ]
        Integer*2   icon_Num_Default_Icons
        Byte        icon_x   [$Icon_Data_Pts]
        Byte        icon_y   [$Icon_Data_Pts]
        VaxCharacter icon_pen [$Icon_Data_Pts]

    COMMON / ICONCOMMN /_
        icon_Index_First_Data,
        icon_Index_Into_First_Data,
        icon_Drawing_Scale,
        icon_Tot_Data_Pts,
        icon_Num_Icons,
        icon_Default_Indx,
        icon_Num_Default_Icons,
        icon_x,
        icon_y,
        icon_pen
    )
