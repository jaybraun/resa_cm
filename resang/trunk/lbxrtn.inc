##############################################################################
# LBXRTN.INC
#   This file contains the common areas needed for widget references in BEES
#   and RESA.
#
# Modifications:
#   3/17/93 Tom Smith   Extracted from MOTIF.RAT and DECW.rat
#
##############################################################################

define ($no,  0)
define ($yes, 1)

define ($Widget_Array_Size,1024)
define ($Dup_Widget_Array_Size,512)

define (Widget$Common,      # Used by CB_CREATE_WIDGET & CB_WIDGET_ID & CB_WIDGET_REF
    integer*4   First_Widget_id       / 0 /
    integer*4   First_Widget_display  / 0 /
    integer*4   Widget_Id_Array [ $Widget_array_size ] /$Widget_array_size * 0 /

    integer*4   Dup_Widget_ID_Array   [ $Dup_Widget_array_size ]
    integer*2   Dup_Widget_Count      / 0 /
    integer*2   Dup_Widget_Ref_Array  [ $Dup_Widget_array_size ]

    common      / widget_common / First_Widget_Id, First_Widget_Display, 
                                  Widget_Id_Array, Dup_Widget_ID_Array,
                                  Dup_Widget_Count, Dup_Widget_Ref_Array
    )
