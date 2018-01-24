# Define Plot Common Area for use with Gary Hartling's Megatek Menu subroutine
# Ptmain sets the following values from the configuration assignment at 
# startup.  The menu subroutine includes this common area to find out what
# device to use.  

# Com_Name[40]  = Control, Blue#, Orange#;  ends with a null
# Com_Code      = T(ablet), J(oystick), P(anel), (blank)
# Com_Menu      = M(enus), N(o Menus), P(lot Menus)
# Com_Touch[40] = logical name for the touch panel if used
# Com_Station   = Station number
# Com_View      = View Index for the Station 
# Com_Type      = STA$GeoType$i for this iteration

define (MENUcommon,
    Integer    Com_Station, Com_View, Com_unit, Com_type
    Logical*1  Com_Name[64], Com_Touch[40], Com_Code, Com_Menu
    Common /MenuCom/ Com_Station, Com_View, Com_unit, Com_type, 
                     Com_Name, Com_Touch, Com_Code, Com_Menu 
    )
