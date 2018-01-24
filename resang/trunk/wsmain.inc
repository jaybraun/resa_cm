# Common areas for WSMAIN (STATION)                     DEW 1/28/92

define (SetupCommon,
    integer     id_plot, id_astab, stationindx, id_script
    logical     PLOT_on, ASTAB_on, SCRIPT_on
    
    common /pl_setup_common/    PLOT_on, ASTAB_on, id_plot, id_ASTAB, 
                                stationindx, id_script, SCRIPT_on
    )

define (Qual$Common,
    Integer*4       Qual_Readonly
    VaxCharacter    Qual_Script*60
    common /Qual_common/ Qual_ReadOnly, Qual_Script
    )

define (IOCommon,
    logical  mid_command
    integer  IO_ReadOnly
    common /ws_io_common/ mid_command, IO_ReadOnly
    )

