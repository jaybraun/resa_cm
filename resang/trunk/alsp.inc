


################################################################################
##                                                                            ##
##  FILE:  ALSP.INC                                                           ##
##                                                                            ##
##  DATE CREATED:  Jan 92                                                     ##
##                                                                            ##
##  DATE MODIFIED:  16-FEB-1992 09:36                                         ##
##                                                                            ##
##  AUTHORS:  Los Alamos National Laboratory, David F. Perme, Wayne Anderson  ##
##                        Warrier Preparation Center, Robert Tracy            ##
##                                                                            ##
##  Modified: Dane Wills, 30-Sep-99                                           ##
##            Enumeration macros revised for consistancy and for E2151        ##
##                                                                            ##
##  LINKS WITH: An include file, is included in WGALSP.RAT                    ##
##                                                                            ##
##  PURPOSE:  Declares all required "define" statements for the attributes    ##
##            required in the alsp translator.  Declares required common      ##
##            blocks for mapping arrays.  Includes the required AWSIMS files  ##
##            for all alsp functionality.                                     ##
##                                                                            ##
################################################################################

################################################################################
#$%                                     ALSP required awsims include files
################################################################################

################################################################################
#$% required for ratfor so that the standard fortran statements can be used.
################################################################################

undefine read
undefine write
undefine err
undefine print

################################################################################
#$%                                                     SOME TOOLS
################################################################################

define(inc, @1 = @1 + 1)
define(dec, @1 = @1 - 1)
define($knots_to_km, 1.853)

# 2.20462 lb/kg - reference Little Pocket Reference Guide
define (kg_to_lbs,2.20462)
define (lbs_to_kg,0.45359)

################################################################################
#$%                             FILTER/LOCK/REGISTER PARAMETER TYPES
################################################################################

#######################################
#$% FILTER ATTRIBUTE PARAM TYPES

define ($filter_class               ,1)
define ($filter_attr_id             ,2)
define ($filter_attr_lat            ,3)
define ($filter_attr_long           ,4)
define ($filter_alt                 ,5)
define ($filter_heading             ,6)
define ($filter_name                ,7)
define ($filter_percent             ,8)
define ($filter_side                ,9)
define ($filter_size                ,10)
define ($filter_speed               ,11)
define ($filter_stat                ,12)
define ($filter_unit_type           ,13)

#######################################
#$% FILTER CLASS PARAM TYPES

define ($filter_class_class         ,1)
define ($filter_class_condition     ,2)

######################################
#$% FILTER CLASS CONDITION PARAMETER VALUES

define ($cond_allow                 ,1)
define ($cond_disallow              ,2)

######################################
#$% FILTER CLASS SEND DR PARAMETER VALUES

define ($send_dr_true               ,1)
define ($send_dr_false              ,2)

#######################################
#$% CANCEL LOCK PARAM TYPES

define ($cancel_lock_id             ,1)
define ($cancel_lock_attrs          ,2)

######################################
#$% JOIN PARAM TYPES

define ($join_lookahead                 ,1)
define ($join_node                      ,2)
define ($join_confederation             ,3)
define ($join_sim_start_time            ,4)
define ($join_actor_name                ,5)
define ($join_time_reg                  ,6)
define ($join_time_cons                 ,7)

######################################
#$% JOIN TIME MODE PARAMETER VALUES

define ($jtm_true                       ,1)
define ($jtm_false                      ,2)

######################################
#$% LOCK MESAAGE
define ($lock_msg_req           ,12)

######################################
#$% LOCK REQUEST PARAM TYPES

define ($lock_req_if_avail      ,1)
define ($lock_req_when_avail    ,2)

######################################
#$% LOCK STATUS ENUMERATIONS

define ($lock_secured			,1)
define ($lock_rejected			,2)
define ($lock_acquiring			,3)

######################################
#$% REFRESH PARAM TYPES

define ($refresh_id                     ,1)
define ($refresh_attr                   ,2)

#####################################
#$% REGISTER ATTRIBUTES PARAM TYPES

define ($reg_attrs_class                ,1)
define ($reg_attrs_create               ,2)
define ($reg_attrs_intr                 ,3)

#####################################
#$% REGISTER OBJECT PARAM TYPES

define ($reg_object_id                  ,1)
define ($reg_object_class               ,2)
define ($reg_object_unlock              ,3)

######################################
#$% RESIGN PARAMETER TYPES

define ($alsp_resign_no_param           ,0)
define ($alsp_resign_delete_owned       ,1)
define ($alsp_resign_disperse_owned     ,2)

######################################
#$% SIGNAL TYPE PARAMETER TYPES          # clk - 9/95 (E1457)

define ($alsp_signal_no_param          ,0)
define ($alsp_signal_register_objects  ,1)
define ($alsp_signal_register_enumerations ,2)  # SMM - 10/96 (E1694)
define ($alsp_signal_enumerations_complete ,3)  # SMM - 10/96 (E1694)

######################################
#$% UNLOCK PARAM TYPES

define ($unlock_id                      ,1)
define ($unlock_attrs                   ,2)

######################################
#$% AVAIL PARAM TYPES

define ($avail_id                       ,1)
define ($avail_attrs                    ,2)

###############################################################################
#$%                                     INTERACTION/UPDATE/CREATE PARAMETERS
###############################################################################
define ($alsp_send_no_attribute_value, -9999)   # code for attrbiute() to be sent
                                                # see mess_struct.h SEND_NO_ATTRIBUTE_VALUE
#######################################
define ($id                                             ,1)         #$% integer (xx12345)

#######################################
define ($class                      ,2)

#$% class enumerations:

    define ($aircraft_begin                    ,1)
    define ($air_fixedwing                     ,1)     
    define ($air_helicopter                    ,2)
    define ($air_helicopter_airlift            ,3)
    define ($aircraft_end                      ,3)
    define ($air_cruise_missile                ,4)     
    define ($air_ballistic_missile             ,5)    # 12/93 clk
    #6 is available for an air or ground class
    define ($ground_base                       ,7)
    define ($shore_base                        ,7)    # 10/14/93 clk
    define ($alsp_class_ground_maneuver        ,8)         
    define ($alsp_class_ground_maneuver_himad  ,9)         
    define ($alsp_class_ground_maneuver_shorad ,10)
    define ($alsp_class_ground_maneuver_allrad ,11)        
    define ($alsp_class_ground_maneuver_combat ,12)
    define ($alsp_class_ground_maneuver_support ,13)
    define ($alsp_class_ground_maneuver_support_convoy ,14)
    define ($alsp_class_ground_maneuver_support_unit ,15)
    define ($alsp_class_ground_maneuver_support_unit_tcu,16)
    define ($alsp_class_ground_maneuver_radar  ,17)        
    define ($alsp_class_ground_maneuver_tel    ,18)        
    define ($alsp_class_ground_facility        ,19)
    define ($sea_subsurface                    ,20)
    define ($sea_torpedo                       ,21)
    define ($sea_countermeasure                ,22)
    define ($sea_sonobuoy                      ,23)
    define ($sea_mine                          ,24)
    define ($sea_sweeparea                     ,25)
    define ($sea_surface_boat                  ,26)    # 8/20/93 clk
    define ($sea_surface_ship                  ,27)    # 8/20/93 clk
    define ($alsp_class_sea_surface_aav        ,28)    # clk - 10/95 (E1483)
    define ($alsp_class_sea_surface_landing_craft,29)  # clk - 10/95 (E1483)
#######################################
define ($latitude                               ,3)         #$% real -90 to +90

#######################################
define ($longitude                              ,4)         #$% real -180 to +180

#######################################
define ($altitude                   ,5)     #$% int 0 to 200,000

#######################################
define ($arty_supp                  ,6)     #$% percent of suppression by
                                            #$% artillery, int 0 to 100

#######################################
define ($awsim_status               ,7)

#$% awsim status enumerations:

    define ($awsim_stat_operational ,1)
    define ($awsim_stat_suppressed  ,2)

#######################################
define ($cbs_status                             ,8)

#$% cbs status enumerations:

    define ($cbs_stat_operational       ,1)
    define ($cbs_stat_moving            ,2)
    define ($cbs_stat_suppressed        ,3)
    define ($cbs_stat_destroyed         ,4)

#######################################
define ($elect_deg                  ,9)     #$% percent of electronic degra-
                                            #$% dation, int 0 to 100
#######################################   
define ($fire_cont                  ,10)    #$% ability to control the firing
                                            #$% of a unit, int 0 or 1
#######################################
define ($heading                    ,11)    #$% int 0 to 360

#######################################
define ($name                       ,12)    #$% text string, name of unit

#######################################
define ($old_id                     ,13)

#######################################
# Orbit points - take up four attribute slots, but
# only one attribute is registered to the confederation
define ($orb_lat1                   ,14)
define ($orb_lon1                   ,15)
define ($orb_lat2                   ,16)
define ($orb_lon2                   ,17)
define ($alsp_orbit_points  , $orb_lat1)


#######################################
define ($percent_attr               ,18)    #$% int 0 to 1000

#######################################
define ($side                                   ,19)

#$% side (view) enumerations:

    define ($blue                               ,2)
    define ($red                                ,1)
    define ($neutral                        ,3)

#######################################
define ($size_attr                  ,20)   # not to be confused with
                                           # previous constant $size

#$% for aircraft, the number of aircraft in the flight is the SIZE

#$% size enumerations for ground units:

    define ($size_army                  ,1)
    define ($size_base                  ,2)
    define ($size_brigade               ,3)
    define ($size_battalion             ,4)
    define ($size_battery               ,5)
    define ($size_company               ,6)
    define ($size_corps                 ,7)
    define ($size_detachment            ,8)
    define ($size_division              ,9)
    define ($size_headquarters          ,10)
    define ($size_platoon               ,11)
    define ($size_regiment              ,12)
    define ($size_squad                 ,13)
    define ($size_squadron              ,14)
    define ($size_team                  ,15)
    define ($size_taskforce             ,16)
    define ($size_troop                 ,17)
    define ($size_wing                  ,18)
    define ($size_section               ,19)
    define ($size_army_group            ,20)

########################################
#$% actually this falls under the same category as $size, but is broken out
#$% separately since the parser assigns the attribute a different value

define ($salvo_size                                     ,16)

#$% Interaction salvo size enumeration

    define ($salvo_105mm            ,1)
    define ($salvo_122mm            ,2)
    define ($salvo_152mm            ,3)
    define ($salvo_175mm            ,4)
    define ($salvo_203mm            ,5)
    define ($salvo_2s1              ,6)
    define ($salvo_2s3              ,7)
    define ($salvo_2s4              ,8)
    define ($salvo_2s5              ,9)
    define ($salvo_2s7              ,10)
    define ($salvo_2s9              ,11)
    define ($salvo_8in              ,12)
    define ($salvo_bm21             ,13)
    define ($salvo_bm22             ,14)
    define ($salvo_bm27             ,15)
    define ($salvo_d20              ,16)
    define ($salvo_d30              ,17)
    define ($salvo_frog             ,18)
    define ($salvo_lance            ,19)
    define ($salvo_missile          ,20)
    define ($salvo_mlrs             ,21)
    define ($salvo_scud             ,22) 
    define ($salvo_ss1              ,23)
    define ($salvo_ss12             ,24)
    define ($salvo_ss21             ,25)

#######################################
define ($speed                      ,21)    #$% unit speed in km/hour
                                            #$% int 0 to 10,000
#######################################
define ($status                     ,22)

#$% status enumerations for ground units:

    define ($available              ,1)
    define ($combat                 ,2)
    define ($moving                 ,3)

#$% status enumerations for air units:

    define ($drop                   ,4)
    define ($fly                    ,5)#$% ICD 
    define ($lift                   ,6)
    define ($on_ground              ,7)#$% ICD
    define ($orbit                  ,8)#$% ICD
    define ($proceed                ,9)
    define ($ready                  ,10)
    define ($delayed                ,11)#$% ICD

#$% status enumerations for ship units (clk 12/96 (E1724)):
    define ($alsp_status_underway   ,12)
    define ($alsp_status_inport     ,13)
    define ($alsp_status_sinking    ,14)

#$% status enumerations for sweep areas
    define ($alsp_sweeparea_status_inprogress ,1)
    define ($alsp_sweeparea_status_complete   ,2)

#######################################
define ($supply_launcher            ,23)

#######################################
define ($supply_missile                 ,24)

#######################################
define ($supply_radar                   ,25)

#######################################
# Target location points - take up two attribute slots, but
# only one attribute is registered to the confederation
define ($target_lat                 ,26)
define ($target_long                ,27)
define ($alsp_target_loc,   $target_lat)

#######################################
define ($unit_type                              ,28)

include "pvals.inc"         


###################################################################
define ($posture                   ,30)
 
#$% posture parameter values


    define ($posture_attack                ,1)
    define ($posture_defend                ,2)
    define ($posture_withdraw              ,3)
    define ($posture_moving                ,4)
    define ($posture_move_contact          ,5)
    define ($posture_incapable             ,6)
    define ($posture_wiped                 ,7)

######################################
define ($orientation               ,31)


######################################
define ($time                      ,32)

######################################
define ($call_sign                 ,33)     # Carol Kropp (NRaD) 7/15/92

######################################
define ($age                        ,34)    # Carol Kropp (NRaD)    4/93
define ($depth                      ,35)    # Carol Kropp (NRaD)    8/93
define ($alsp_attr_flight_ops       ,36)    # Carol Kropp (NRaD)    8/95
    define ($alsp_flight_ops_open   ,1)
    define ($alsp_flight_ops_closed ,2)

######################################
define ($emitter                   ,37)
define ($radar                     ,38)
define ($jammer                    ,39)     
define ($emitter_index             ,40)     # Susan Miller 9/94
define ($emitter_orient            ,41)
define ($emitter_mode              ,42)     # Susan Miller 9/94
# Changed names to type per JECEWSI ICD 9/94, Susan Miller
define ($emitter_type              ,43)     # Carol Kropp (NRaD)
define ($emitter_status            ,44)     # Carol Kropp (NRaD)
    define ($emitter_off           ,1)
    define ($emitter_on            ,2)



# The following are part of the JECEWSI ICD, the CW2_FACTORS is an
# attribute which has the sub-attrbutes of UNIT and EMTR (repeating).
# The UNIT and EMTR are also wrappers containing the other attributes
# of EW_C3, EW_ACQ EW_LAUNCH, EW_GUIDE, and EW_POWER.
# Sample:  CW2_FACTORS(UNIT(EW_C3(0.5)EW_POWER(65.0,-45.4,-123.5,...)))
# Note:  Only the UNIT data is to be handled in the 1995 confed.
define ($alsp_attr_cw2_factors       ,45)   # the actual attribute
define ($alsp_attr_unit              ,46)
define ($alsp_attr_emtr              ,47)
define ($alsp_attr_ew_c3             ,48)
define ($alsp_attr_ew_acq            ,49)
define ($alsp_attr_ew_launch         ,50)
define ($alsp_attr_eq_guide          ,51)
define ($alsp_attr_eq_power          ,52)
define ($alsp_attr_eq_surf           ,53)

# add weapon, which is considered an attribute for the
# register_enumeration messages not the create and updates - SMM 1/97 E1694
define ($alsp_attr_weapon            ,54) 

# Add update attribute IFF with subattributes for each of the
# modes and the master on/off switch.    smp - 2/98 (E1907)
define ($iff                       ,55)
define ($iffmaster                 ,56)
define ($iffmode1                  ,57)
define ($iffmode2                  ,58)
define ($iffmode3                  ,59)
define ($iffmodeC                  ,60)
define ($iffmode4                  ,61)
define ($iffmode_status            ,62)
define ($iffmode_value             ,63)
    define ($iffmode_off            ,1)
    define ($iffmode_on             ,2)

######################################
# Propulsion mode for sea.subsurface.submarine

define ($Propulsion_Mode         ,64)
    define ($Propulsion_diesel    ,1)
    define ($Propulsion_electric  ,2)
    define ($Propulsion_nuclear   ,3)

######################################
# Active sonars for air, surface and subsurface

define ($sonar                     ,65) # SONAR
define ($sonar_type                ,66) # RESA sonar name
define ($sonar_status              ,67) 
    define ($sonar_off             ,1)
    define ($sonar_on              ,2)
define ($sonar_mode                ,68) # optional
    define ($sonar_dp              ,1)
    define ($sonar_cz              ,2)
    define ($sonar_bb              ,3)
define ($sonar_depth               ,69) # optional; feet

######################################
# Minefields

define ($position                  ,70) # optional

######################################
# Sweepareas

define ($sweeparea_location        ,71) # optional
define ($sweeparea_status          ,72) # optional

######################################

define ($ato_id                    ,73) # optional
define ($strength                  ,74) # optional

######################################

#$% The following definitions are the enumerations for interactions
#$% All names are prefixed with "int_" to differentiate conflicts such as
#$% lat, long, mission, percent, etc.

#######################################
#$% interaction ids
define ($to_id                                          ,2)

#######################################
define ($int_firing                 ,1)

#######################################
define ($int_victim                 ,2)

#######################################
define ($int_kind                   ,3)

#$% v_kind enumerations:

    define ($report                         ,1)
    define ($mines                          ,2)

    define ($air_to_air                     ,3)   # clk 2/93
    define ($air_to_ground                  ,4)   # clk 9/93
    define ($air_to_ship                    ,5)   # clk 9/93
    define ($ship_to_air                    ,6)   # clk 9/93
    define ($ship_to_ground                 ,7)   # clk 9/93
    define ($ship_to_ship                   ,8)   # clk 9/93
    define ($ground_to_air                  ,9)   # clk 9/93
    define ($ground_to_ground               ,10)  # clk 9/93
    define ($ground_to_ship                 ,11)  # clk 9/93

    define ($report_air_to_air              ,12)   # clk 12/93
    define ($report_air_to_ground           ,13)   # clk 12/93
    define ($report_air_to_ship             ,14)   # clk 12/93
    define ($report_ship_to_air             ,15)   # clk 12/93
    define ($report_ship_to_ground          ,16)   # clk 12/93
    define ($report_ship_to_ship            ,17)   # clk 12/93
    define ($report_ground_to_air           ,18)   # clk 12/93
    define ($report_ground_to_ground        ,19)   # clk 12/93
    define ($report_ground_to_ship          ,20)   # clk 12/93

    define ($supporting_units_request       ,21)   # SMM 9/95
    define ($pol_request                    ,22)   # SMM 9/95
    define ($supporting_units_issue         ,23)   # SMM 9/95
    define ($pol_issue                      ,24)   # SMM 9/95

    define ($low_alsp_interaction_type      ,$air_to_air)
    define ($high_alsp_interaction_type     ,$pol_issue)
    define ($low_alsp_report_type           ,$report_air_to_air)
    define ($high_alsp_report_type          ,$report_ground_to_ship)

#######################################
define ($int_fire_quant             ,4)

#######################################
define ($int_duration               ,5)

#######################################
define ($int_percent                ,6)     
                                                            
#######################################
define ($int_kills                  ,7)     #$% number of kills, int 0 to 1000

#######################################
define ($int_hits                   ,21)    #$% number of hits, int 0 to 1000

#######################################
define ($int_lat                    ,8)  

#######################################
define ($int_long                   ,9)   

#######################################
define ($int_firer_lat              ,10)  

#######################################
define ($int_firer_long             ,11)   

#######################################
#$% mission is doubly defined since it is a different enumeration based
#$% upon whether or not it is an interaction and any other.

define ($int_mission                            ,12)
define ($mission                                        ,29)    

#$% mission/interact_mission enumerations:

    define ($mission_ad                 ,1)
    define ($mission_aew                ,2)
    define ($mission_af_strike          ,3)
    define ($mission_ai                 ,4)
    define ($mission_airlift            ,5)
    define ($mission_airtanker          ,6)
    define ($mission_amphib             ,7)
    define ($mission_asw                ,8)
    define ($mission_bai                ,9)
    define ($mission_cap                ,10)
    define ($mission_cargo              ,11)
    define ($mission_cas                ,12)
    define ($mission_commerce           ,13)
    define ($mission_csar               ,14)       # clk 1/97 (E1730)
    define ($mission_dca                ,15)
    define ($mission_escort             ,16)
    define ($mission_faca               ,17)       # clk 1/97 (E1730)
    define ($mission_generic            ,18)
    define ($mission_jammer             ,19)
    define ($mission_marops             ,20)
    define ($mission_navy_strike        ,21)
    define ($mission_none               ,22)
    define ($mission_oca                ,23)
    define ($mission_reccon             ,24)
    define ($mission_relay              ,25)
    define ($mission_sar                ,26)
    define ($mission_sead               ,27)
    define ($mission_sea_control        ,28)
    define ($mission_sealift            ,29)       # clk 12/96 (E1724)
    define ($mission_spec_miss          ,30)
    define ($mission_strikecap          ,31)
    define ($mission_sttanker           ,32)
    define ($mission_surcap             ,33)
    define ($mission_surveillance       ,34)
    define ($mission_transfer           ,35)       # clk 1/97 (E1730)

########################################
define ($int_radius                 ,13)     #$% float, radius of fire in
                                             #$% km, 0 to 1000
########################################
define ($int_salvo_num              ,14)

########################################
define ($weapon_type                ,15)

#######################################
define ($target                         ,17)

#$% target enum:
#$% the names are short due to space needs in
#$%     alsp_target_data.inc

    define ($max_num_alsp_targets       ,3)
    define ($alsp_target_acq_radar      ,1)     #$% acquisradar (cbs)
    define ($alsp_target_ad             ,2)     #$% air defense (cbs)
    define ($alsp_target_aircraft       ,3)     #$% aircraft (awsim)
    define ($alsp_target_tank           ,4)     #$% anti tank (cbs)
    define ($alsp_target_armor          ,5)     #$% armor (cbs)
    define ($alsp_target_artll          ,6)     #$% artillery (cbs)
    define ($alsp_target_ciws           ,7)     #$% close in weapon system
    define ($alsp_target_comm           ,8)     #$% communication equip (awsim)
    define ($alsp_target_dismntd        ,9)     #$% dismounted (cbs)
    define ($alsp_target_eng_brg        ,10)    #$% engineer bridge (cbs)
    define ($alsp_target_engnr          ,11)    #$% engineer (cbs)
    define ($alsp_target_fix_brg        ,12)    #$% fixed bridge (cbs)
    define ($alsp_target_flightline     ,13)    #$% flightline (awsim)
    define ($alsp_target_fuel           ,14)    #$% fuel (awsim)
    define ($alsp_target_generic        ,15)    #$% generic target (awsim)
    define ($alsp_target_lt_armr        ,16)    #$% light armor (cbs)
    define ($alsp_target_maintenance    ,17)    #$% maintenance facilities (awsim)
    define ($alsp_target_misc           ,18)    #$% miscellaneous (cbs)
    define ($alsp_target_parked_ac      ,19)    #$% parked aircraft (cbs)
    define ($alsp_target_personnel      ,20)    #$% personnel (awsim)
    define ($alsp_target_radar          ,21)    #$% radar (cbs)
    define ($alsp_target_radars         ,22)    #$% radars (awsim)
    define ($alsp_target_rip            ,23)    #$% road intersection (cbs)
    define ($alsp_target_shelters       ,24)    #$% aircraft shelters (awsim)
    define ($alsp_target_stores         ,25)    #$% base stores (awsim)
    define ($alsp_target_sup_cnt        ,26)    #$% supply center (cbs)
    define ($alsp_target_trks_vn        ,27)    #$% trucks and vans (cbs)
    define ($alsp_target_weapons        ,28)    #$% weapons (awsim)

#######################################
define ($int_report                 ,18)

#######################################
define ($int_targets_engaged        ,20)        # clk 2/93, to match par_tt_int_targets_eng in mess_struct.h

#######################################
define ($int_supply_type            ,22)    # SMM 9/95, to match par_tt_int_supply_pol_fuel_type
                                            # in mess_struct.h
    define ($alsp_supply_jp4          ,1)     

#######################################
define ($int_supply_qty             ,23)    # SMM 9/95, to match par_tt_int_supply_pol_fuel_qty
                                            # in mess_struct.h

#######################################
define ($int_supply_reason          ,24)    # SMM 10/95, to match par_tt_int_supply_pol_fuel_qty
                                            # in mess_struct.h

    define ($alsp_supply_none_available    ,1)
    define ($alsp_supply_supplier_not_known,2)
    define ($alsp_supply_receiver_not_known,3)

#######################################
define ($int_target_ref             ,25)    # clk 12/97 (E1911) - a string value
define ($int_target_ref_ben         ,26)    # jhm 1/98 (E1931) - new msg format
define ($int_target_ref_ben_suffix  ,27)    # jhm 1/98 (E1931) - new msg format
define ($int_target_ref_ttn         ,28)    # jhm 1/98 (E1931) - new msg format
define ($int_target_ref_pin         ,29)    # jhm 1/98 (E1931) - new msg format
define ($int_target_ref_loc         ,30)    # jhm 1/98 (E1931) - new msg format
define ($int_correlation_id         ,31)    # joa 6/11

#######################################
#$% qualifier enumerations:

        define ($no_qualifier                   ,0)
        define ($non_dr                                 ,1)
        define ($dr                                             ,2)

#######################################
#$% message codes (dispatch_flag)

define ($event_message                          ,0)
define ($dispatch_message                       ,1)

#######################################
#$% create error message codes

define ($no_class_match                 ,1)
define ($no_missile_match               ,2)
define ($no_radar_match                 ,3)                        
define ($no_class_type                  ,4)
define ($no_more_slots                  ,5)
define ($dup_id                         ,6)
define ($no_more_slots                  ,7)
define ($no_unit_type_map               ,8)
define ($shared_base_registered         ,9)
define ($shared_base_is_owned          ,10)
define ($base_not_shared               ,11)
define ($no_available_agd_slots        ,12)

#######################################
#$% update and incoming interaction error message codes

define ($no_unit_type_match                     ,1)
define ($no_shooter_match                       ,2)
define ($no_weapon_match                        ,3)
define ($no_target_match                        ,4)
define ($dead_shooter                           ,5)
define ($T_Error                                ,9999)
define ($target_not_wanted                      ,6)
define ($position_flag                          ,-9999)
define ($no_weapon_specified                    ,7)
define ($weapon_not_in_database                 ,8)
define ($incorrect_actor_id                     ,9)
define ($unit_kind_mismatch                     ,10)
define ($interaction_not_handled                ,11)
define ($ghost_to_ghost_engagement              ,12)
define ($being_deleted_status                   ,13)
define ($no_weapon_enumeration                  ,14)     # smm 3/97 (E1795)
define ($CBS_Did_it                             ,-100) 

#######################################
#$% lock/unlock/refresh/update request error message codes

define ($no_id_match                            ,1)
define ($incorrect_class                        ,2)
define ($incorrect_attribute                    ,3)
define ($zero_id                                ,4)
define ($race_condition                         ,5)
define ($unit_engaged                           ,6)
define ($unit_deleted                           ,7)
define ($unit_is_not_a_ghost                    ,8)
define ($bad_update_attribute                   ,9)
define ($boy_deleted                            ,10)
define ($swp_deleted                            ,11)

#######################################
#$% debug statements

define ($alsp_wait, {for(i=1;i<=@1;inc(i)){}})  #$% a forever loop 

#######################################
#$% update flags for the alsp_dr_flags array

define ($alsp_kill_it                           ,0)
define ($alsp_create_it                         ,1)
define ($alsp_relocate_or_emitter_change        ,2)     # used in wgopord.rat - so change there also if changed
define ($restart                                        ,-100)

#######################################
#$% v_restriction enumerations for message filter_parameters
#$% Susan Miller 9/96 (E1691)

define ($alsp_filter_none    ,0)
define ($alsp_filter_known   ,1)
define ($alsp_filter_owned   ,2)
define ($alsp_filter_ghosted ,3)

#######################################
#$% required array parameters

define ($alsp_msg_len                   ,512) # the max length of an alsp message
define (MAX_PARAM                       ,74)  # Max number of parameters in update msgs
define (MAX_INTERACT_PARAM              ,31)  # Max number of parameters in interaction msgs
define (MAX_STR_LEN                     ,80)  # Max length of string array parameters
define (MAX_SAMSITE_TYPE                ,38)  
define (MAX_RADAR_TYPE                  ,70)          
define (MAX_TARGET_TYPE                 ,44)
define (MAX_TARG                        ,3)   # Max number of target priorities in interaction msgs
define (DR$FLAGS                        ,8)   # Number of items to determine dr
# DR Flags:     1 - id          4 - speed               7 - status
#               2 - altitude    5 - relocate/emitter    8 - size
#               3 - heading     6 - mission
define (MAX$SLOTS                       ,UNT$SLOTS + 1)
define (MAX_FILTERS                     ,13)    # CLK 6/92 to match mess_struct.h - N_filter_attr_par
define (MAX_LIST_LEN                    ,30)    # CLK 6/92 to match mess_struct.h - Max_list

define (Max$Alsp$Emitters, 80)                # clk 3/10/93
define (Max$Alsp$Sonars,   10)
define (Max$Alsp$IFFModes, 6)                 # smp 2/98 (E1907)
define (Number$EW$Sectors, 8)                 # clk 10/94 - needs to match N_EW_SECTOR_VALS in mess_struct.h
define (Max$Alsp$Positions,10)

#######################################
#$% max enumeration definitions

define ($number_of_unit_enumerations,    2500)

define ($number_of_cmc_maximum,          100)   # see BBCMC.DEF (slots)
                                                # JHM E2203 CT00 FIX FROM 50

##################################################
# MAX Enumeration Data arrays
# These limit the data loaded via ALSP_ENUMERATION_DATA.RIN     # DEW 9/99
#
# These numbers are slightly larger than currently needed (by 10-20%) 
# to allow for additions, without requiring the recompile of anything
# other than ALSP_MAPPING.RAT
# Actual numbers used can be displayed with the following command
#   $ SEARCH ALSP_MAPPING.FOR "data number_of"
#   (after RATing ALSP_MAPPING w/a new RIN file)
#
# These numbers are also predefined, before the DATA blocks, because 
# DATA declaration with implied-do loops, requires it.
#

define ($max$number_of_aircraft_maps    ,1500)
define ($max$number_of_emitter_maps     ,1500)
define ($max$number_of_sonar_maps       ,100)
define ($max$number_of_missile_maps     ,200)
define ($max$number_of_sea_maps         ,$number_of_unit_enumerations)
define ($max$number_of_torpedo_maps     ,100)
define ($max$number_of_sonobuoy_maps    ,100)
define ($max$number_of_mine_maps        ,100)
define ($max$number_of_countermeasure_maps     ,100)
define ($max$number_of_weapon_maps      ,2500) #CT04 JHM from 1000

#######################################
#$% global section needed for mapping of unit types from an enumeration to an
#$% RESA name

define (ALSP_FLAGS, 
        integer name_search
        common /FLAG_IT/ name_search)
                                                        
define (sam_common,     
        literal samsite_unit_type[6, MAX_SAMSITE_TYPE]
        common /alsp_sam/ samsite_unit_type)    

define (rad_common,
        literal radar_unit_type[4, MAX_RADAR_TYPE]
        common /alsp_rad/ radar_unit_type)
                                           
define (alsp_target_common,
        integer alsp_target_matrix[6, MAX_TARGET_TYPE]
        common /alsp_target/ alsp_target_matrix)


#------------------------------------------------------------
#   The following 5 common macros, hold the ALSP enumeration data   # E2151 DEW
#
#   The Enumeration data is defined/loaded by the file:
#       ALSP_ENUMERATION_DATA.RIN   
#
#   This file might be significantly larger than before, but it will keep
#   the entire collection in one file, hopefully easier to manage.
#   It is also only necessary to include the DATA blocks once.  Therefore,
#   only ALSP_MAPPING.RAT is intended to include the RIN file.
#
#   The .RIN suffix is used to denote that including the file 
#   "generates" fortran code.  Whereas a .INC suffix denotes
#   macro definitions only.
#
#   To help organize the new structures the names of the common 
#   blocks, and the 'count' names have been revised for consistancy.
#
#   Original .inc file      Array Loaded            'Common' Macro          'Count' Macro
#   ===================     =================       ==============          ==================
#   ALSP_AIRCRAFT_DATA.INC  aircraft_unit_type      aircraft_common         max_air_type
#   ALSP_EMITTER_DATA.INC   alsp_emitter_list       alsp_emitter_common     number_of_mapped_emitters
#   ALSP_MISSILE_DATA.INC   missile_unit_mappings   missile_unit_common     $number_of_missile_maps
#   ALSP_SEA_UNIT_DATA.INC  sea_unit_mappings       sea_unit_common         $number_of_sea_maps 
#   ALSP_WEAPON_DATA.INC    weapon_type             alsp_weapon_common      max_weapon_type
#
#                           Array Loaded            New 'Common' Macro      New 'Count' Variable
#                           =================       ==================      =====================
#                                                   alsp$aircraft_common    number_of_aircraft_maps
#                           [no change]             alsp$emitter_common     number_of_emitter_maps
#                                                   alsp$missile_common     number_of_missile_maps
#                                                   alsp$sea_common         number_of_sea_maps
#                                                   alsp$weapon_common      number_of_weapon_maps
#
#   The actual number of data lines loaded into each of the arrays is 
#   recorded into the new 'count' variable indicated above.
#
#   This complete ALSP Enumeration re-organization is done under E2151
#       D. Wills, 9/30/99
#
#----------------------------------------

define (alsp$aircraft_common,
    integer*4   number_of_aircraft_maps,
                aircraft_fill

    LitCharEquiv(aircraft_unit_type,3,$max$number_of_aircraft_maps)
                            # [1,x] - alsp enumeration index
                            # [2,x] - RESA recognized name
                            # [3,x] - ALSP class

    common /alsp_air/ _
        number_of_aircraft_maps,
        aircraft_fill,
        aircraft_unit_type
    )

define (Data$fmt_air_name,
    DATA aircraft_unit_typeC [2,$use(Data_Count)])

define (Data$fmt_air_data1,
    DATA aircraft_unit_type [1,$use(Data_Count)])

define (Data$fmt_air_data3,
    DATA aircraft_unit_type [3,$use(Data_Count)])

define (Data$Aircraft,
     $incr(Data_Count)
     Data$fmt_air_name / @2 / 
     Data$fmt_air_data1 / @1 / 
     Data$fmt_air_data3 / @3 / 
)

#----------------------------------------

define (alsp$position_common,

    integer*4   number_of_positions
    real*4      position_lat_array[Max$Alsp$Positions],
                position_lon_array[Max$Alsp$Positions]


    common /alsp_position/ _
        number_of_positions,
        position_lat_array,
        position_lon_array
    )

#----------------------------------------

define (alsp$sonar_common,
    integer*4   number_of_sonar_maps,
                number_of_sonars,
                alsp_default_sonar, 
                sonar_all_qualifier,
                sonar_type_array[Max$Alsp$Sonars],
                sonar_status_array[Max$Alsp$Sonars],
                sonar_mode_array[Max$Alsp$Sonars],
                sonar_depth_array[Max$Alsp$Sonars]

    LitCharEquiv(alsp_sonar_list,4,$max$number_of_sonar_maps)
                                # [1,x] - alsp enumeration index
                                # [2,x] - RESA recognized name
                                # [3,x] - ALSP class


    common /alsp_sonars/ _
        alsp_sonar_list,
        number_of_sonar_maps,
        number_of_sonars,
        alsp_default_sonar, 
        sonar_all_qualifier,
        sonar_type_array,
        sonar_status_array,
        sonar_mode_array,
        sonar_depth_array
    )

define (Data$fmt_sonar_name,
    DATA alsp_sonar_listC [2,$use(Data_Count)])

define (Data$fmt_sonar_data1,
    DATA alsp_sonar_list [1,$use(Data_Count)])

define (Data$fmt_sonar_data3,
    DATA alsp_sonar_list [3,$use(Data_Count)])

define (Data$Sonar,
     $incr(Data_Count)
     Data$fmt_sonar_name / @2 / 
     Data$fmt_sonar_data1 / @1 / 
     Data$fmt_sonar_data3 / @3 / 
)
#----------------------------------------

define (alsp$emitter_common,
    integer*4   number_of_emitter_maps,
                alsp_default_iff, 
                alsp_default_jammer, 
                alsp_default_radar,
                alsp_default_air_radar,
                alsp_default_nav_radar,
                alsp_default_surface_radar,
#                emitter_fill, 
                default_iff_transponder

    LitCharEquiv(alsp_emitter_list,4,$max$number_of_emitter_maps)
                                # [1,x] - alsp enumeration index
                                # [2,x] - RESA recognized name
                                # [3,x] - EQ Ident Code
                                # [4,x] - ALSP class

    common /alsp_emitters/ _
        number_of_emitter_maps,
        alsp_default_iff, 
        alsp_default_jammer, 
        alsp_default_radar,
        alsp_default_air_radar,
        alsp_default_nav_radar,
        alsp_default_surface_radar,
        default_iff_transponder,
        alsp_emitter_list
    )

define (Data$fmt_emitter_name,
    DATA alsp_emitter_listC [2,$use(Data_Count)])

define (Data$fmt_emitter_data1,
    DATA alsp_emitter_list [1,$use(Data_Count)])

define (Data$fmt_emitter_data3,
    DATA alsp_emitter_list [3,$use(Data_Count)])

define (Data$fmt_emitter_data4,
    DATA alsp_emitter_list [4,$use(Data_Count)])

define (Data$Emitter,
     $incr(Data_Count)
     Data$fmt_emitter_name / @2 / 
     Data$fmt_emitter_data1 / @1 / 
     Data$fmt_emitter_data3 / @3 / 
     Data$fmt_emitter_data4 / @4 / 
)

#----------------------------------------

define (alsp$missile_common,
    integer*4   number_of_missile_maps, missile_fill 

    LitCharEquiv(missile_unit_mappings, 3,$max$number_of_missile_maps)
                                    # [1,x] - alsp enumeration index
                                    # [2,x] - RESA recognized name
                                    # [3,x] - ALSP class

    common /alsp_missile/_
        number_of_missile_maps, 
        missile_fill,
        missile_unit_mappings
    )

define (Data$fmt_missile_name,
    DATA missile_unit_mappingsC [2,$use(Data_Count)])

define (Data$fmt_missile_data1,
    DATA missile_unit_mappings [1,$use(Data_Count)])

define (Data$fmt_missile_data3,
    DATA missile_unit_mappings [3,$use(Data_Count)])

define (Data$Missile,
     $incr(Data_Count)
     Data$fmt_missile_name / @2 / 
     Data$fmt_missile_data1 / @1 / 
     Data$fmt_missile_data3 / @3 / 
)

#----------------------------------------

define (ALSP$sea_common,     
    integer*4   number_of_sea_maps, sea_fill 

    LitCharEquiv(sea_unit_mappings, 3, $max$number_of_sea_maps)  
                                # [1,x] - ALSP enumeration index
                                # [2,x] - RESA recognized name
                                # [3,x] - ALSP Class  SMM 10/96 (E1694)

    common /alsp_sea/_
        number_of_sea_maps, 
        sea_fill, 
        sea_unit_mappings
    )

define (Data$fmt_sea_name,
    DATA sea_unit_mappingsC [2,$use(Data_Count)])

define (Data$fmt_sea_data1,
    DATA sea_unit_mappings [1,$use(Data_Count)])

define (Data$fmt_sea_data3,
    DATA sea_unit_mappings [3,$use(Data_Count)])

define (Data$Sea_unit,
     $incr(Data_Count)
     Data$fmt_sea_name / @2 / 
     Data$fmt_sea_data1 / @1 / 
     Data$fmt_sea_data3 / @3 / 
)
#----------------------------------------

define (ALSP$torpedo_common,     
    integer*4   number_of_torpedo_maps, torpedo_fill 

    LitCharEquiv( torpedo_unit_mappings, 3, $max$number_of_torpedo_maps)  
                                # [1,x] - ALSP enumeration index
                                # [2,x] - RESA recognized name
                                # [3,x] - ALSP Class

    common /alsp_torpedo/_
        number_of_torpedo_maps, 
        torpedo_fill, 
        torpedo_unit_mappings
    )

define (Data$fmt_torpedo_name,
    DATA torpedo_unit_mappingsC [2,$use(Data_Count)])

define (Data$fmt_torpedo_data1,
    DATA torpedo_unit_mappings [1,$use(Data_Count)])

define (Data$fmt_torpedo_data3,
    DATA torpedo_unit_mappings [3,$use(Data_Count)])

define (Data$Torpedo_unit,
     $incr(Data_Count)
     Data$fmt_torpedo_name / @2 / 
     Data$fmt_torpedo_data1 / @1 / 
     Data$fmt_torpedo_data3 / @3 / 
)
#----------------------------------------
define (ALSP$sonobuoy_common,     
    integer*4   number_of_sonobuoy_maps, sonobuoy_fill 

    LitCharEquiv(sonobuoy_unit_mappings, 3, $max$number_of_sonobuoy_maps)  
                                # [1,x] - ALSP enumeration index
                                # [2,x] - RESA recognized name
                                # [3,x] - ALSP Class

    common /alsp_sonobuoy/_
        number_of_sonobuoy_maps, 
        sonobuoy_fill, 
        sonobuoy_unit_mappings
    )

define (Data$fmt_sonobuoy_name,
    DATA sonobuoy_unit_mappingsC [2,$use(Data_Count)])

define (Data$fmt_sonobuoy_data1,
    DATA sonobuoy_unit_mappings [1,$use(Data_Count)])

define (Data$fmt_sonobuoy_data3,
    DATA sonobuoy_unit_mappings [3,$use(Data_Count)])

define (Data$Sonobuoy_unit,
     $incr(Data_Count)
     Data$fmt_sonobuoy_name / @2 / 
     Data$fmt_sonobuoy_data1 / @1 / 
     Data$fmt_sonobuoy_data3 / @3 / 
)

#----------------------------------------
define (ALSP$mine_common,     
    integer*4   number_of_mine_maps, mine_fill 

    LitCharEquiv(mine_unit_mappings, 3, $max$number_of_mine_maps)  
                                 # [1,x] - ALSP enumeration index
                                 # [2,x] - RESA recognized name
                                 # [3,x] - ALSP Class

    common /alsp_mine/_
        number_of_mine_maps, 
        mine_fill, 
        mine_unit_mappings
    )

define (Data$fmt_mine_name,
    DATA mine_unit_mappingsC [2,$use(Data_Count)])

define (Data$fmt_mine_data1,
    DATA mine_unit_mappings [1,$use(Data_Count)])

define (Data$fmt_mine_data3,
    DATA mine_unit_mappings [3,$use(Data_Count)])

define (Data$Mine_unit,
     $incr(Data_Count)
     Data$fmt_mine_name / @2 / 
     Data$fmt_mine_data1 / @1 / 
     Data$fmt_mine_data3 / @3 / 
)

#----------------------------------------

define (ALSP$countermeasure_common,     
    integer*4   number_of_countermeasure_maps, countermeasure_fill 

 LitCharEquiv(countermeasure_unit_mappings,3,$max$number_of_countermeasure_maps)
                                # [1,x] - ALSP enumeration index
                                # [2,x] - RESA recognized name
                                # [3,x] - ALSP Class  SMM 10/96 (E1694)

    common /alsp_countermeasure/_
        number_of_countermeasure_maps, 
        countermeasure_fill, 
        countermeasure_unit_mappings
    )

define (Data$fmt_countermeasure_name,
    DATA countermeasure_unit_mappingsC [2,$use(Data_Count)])

define (Data$fmt_countermeasure_data1,
    DATA countermeasure_unit_mappings [1,$use(Data_Count)])

define (Data$fmt_countermeasure_data3,
    DATA countermeasure_unit_mappings [3,$use(Data_Count)])

define (Data$Countermeasure_unit,
     $incr(Data_Count)
     Data$fmt_countermeasure_name / @2 / 
     Data$fmt_countermeasure_data1 / @1 / 
     Data$fmt_countermeasure_data3 / @3 / 
)

#----------------------------------------

define (alsp$weapon_common,
    integer number_of_weapon_maps,
            default_aam_mc_ptr,
            default_sam_mc_ptr,
            default_asm_mc_ptr, 
            default_bomb_bom_ptr,
            default_mine_bom_ptr,
            default_gun_tor_ptr,
            default_cm_cmc_ptr, 
            default_tbm_cmc_ptr 
    LitCharEquiv(weapon_type, 4, $max$number_of_weapon_maps) # 3 became 4 SMM 10/96 (E1694)
    common /alsp_weapon/_
            weapon_type,
            number_of_weapon_maps,
            default_aam_mc_ptr,
            default_sam_mc_ptr,
            default_asm_mc_ptr, 
            default_bomb_bom_ptr,
            default_mine_bom_ptr,
            default_gun_tor_ptr,
            default_cm_cmc_ptr, 
            default_tbm_cmc_ptr
    )

define (Data$fmt_weapon_name,
    DATA weapon_typeC [2,$use(Data_Count)])

define (Data$fmt_weapon_data1,
    DATA weapon_type [1,$use(Data_Count)])

define (Data$fmt_weapon_data3,
    DATA weapon_type [3,$use(Data_Count)])

define (Data$fmt_weapon_data4,
    DATA weapon_type [4,$use(Data_Count)])

define (Data$Weapon,
     $incr(Data_Count)
     Data$fmt_weapon_name / @2 / 
     Data$fmt_weapon_data1 / @1 / 
     Data$fmt_weapon_data3 / @3 / 
     Data$fmt_weapon_data4 / @4 / 
)

#------------------------------------------------------------

# Carol Kropp - 5/93
# Initialize the mapping for unit types, so that searching the mapping arrays
# does not occur each time a create or an update message is received.
# Hopefully there will be a speed increase by storing the mappings by index matching.
define (alsp_unit_enum_common,

    # Incoming_unit_type_enum_map_array is indexed by the ALSP unit enumeration
    # $alsp_unit_... constants (found above).  The data stored is the pointer
    # to the characteristic's of the unit type.

    integer  incoming_unit_enum_map_array [$number_of_unit_enumerations]

    integer  incoming_ac_enum_map_array [$number_of_unit_enumerations]
    integer  incoming_cm_enum_map_array [$number_of_unit_enumerations]
    integer  incoming_sea_enum_map_array [$number_of_unit_enumerations]
    integer  incoming_torpedo_enum_map_array [$number_of_unit_enumerations]
    integer  incoming_sonobuoy_enum_map_array [$number_of_unit_enumerations]
    integer  incoming_mine_enum_map_array [$number_of_unit_enumerations]
    integer  incoming_countermeasure_enum_map_array [$number_of_unit_enumerations]

    equivalence (incoming_unit_enum_map_array, incoming_ac_enum_map_array,
                 incoming_cm_enum_map_array,   incoming_sea_enum_map_array,
                 incoming_torpedo_enum_map_array,
                 incoming_sonobuoy_enum_map_array,
                 incoming_mine_enum_map_array,
                 incoming_countermeasure_enum_map_array)


    # Outgoing_torpedo_enum_map_array is indexed by TRP$Pointer$index.  The
    # data stored is the integer representation of the ALSP enumeration.
    integer  outgoing_torpedo_enum_map_array [TRP$SLOTS]

    # Outgoing_sonobuoy_enum_map_array is indexed by SOB$Pointer$index.  The
    # data stored is the integer representation of the ALSP enumeration.
    integer  outgoing_sonobuoy_enum_map_array [SOB$SLOTS]

    # Outgoing_mine_enum_map_array is indexed by BOM$Pointer$index.  The
    # data stored is the integer representation of the ALSP enumeration.
    integer  outgoing_mine_enum_map_array [BOM$SLOTS]

    # Outgoing_countermeasure_enum_map_array is indexed by CTM$Pointer$index.  The
    # data stored is the integer representation of the ALSP enumeration.
    integer  outgoing_countermeasure_enum_map_array [CTM$SLOTS]

    integer  default_incoming_ac_type
    integer  default_incoming_helo_type
    integer  default_incoming_ship_type
    integer  default_incoming_boat_type
    integer  default_incoming_submarine_type
    integer  default_incoming_countermeasure_type
    integer  default_incoming_torpedo_type
    integer  default_incoming_sonobuoy_type
    integer  default_incoming_mine_type
    integer  default_incoming_cm_type       # cruise missiles
    integer  default_incoming_tbm_type      # tactical ballistic missiles

    common /alsp_unit_enum/ incoming_unit_enum_map_array,
                            outgoing_torpedo_enum_map_array,
                            outgoing_sonobuoy_enum_map_array,
                            outgoing_mine_enum_map_array,
                            outgoing_countermeasure_enum_map_array,
                            default_incoming_ac_type,
                            default_incoming_helo_type,
                            default_incoming_ship_type,
                            default_incoming_boat_type,
                            default_incoming_submarine_type,
                            default_incoming_countermeasure_type,
                            default_incoming_torpedo_type,
                            default_incoming_sonobuoy_type,
                            default_incoming_mine_type,
                            default_incoming_cm_type,
                            default_incoming_tbm_type
    )

################

define (SensorCommon,
            integer    number_of_emitters
            integer    sensor_list_sensor_code[Max$Alsp$Emitters]
            integer    sensor_list_characteristic_pointer[Max$Alsp$Emitters]
            integer    sensor_list_sensor_state[Max$Alsp$Emitters]
            integer    sensor_list_sensor_index[Max$Alsp$Emitters]
            real       sensor_list_sensor_orient[Max$Alsp$Emitters]
            integer    sensor_list_sensor_mode[Max$Alsp$Emitters]
            common /sensor_common/ number_of_emitters,
                                   sensor_list_sensor_code,
                                   sensor_list_characteristic_pointer,
                                   sensor_list_sensor_state,
                                   sensor_list_sensor_index,
                                   sensor_list_sensor_orient,
                                   sensor_list_sensor_mode
)

################

################

define (SonarCommon,
            integer    number_of_sonar_eq
            integer    sonar_list_code[Max$Alsp$Emitters]
            integer    sonar_list_characteristic_pointer[Max$Alsp$Emitters]
            integer    sonar_list_state[Max$Alsp$Emitters]
            integer    sonar_list_mode[Max$Alsp$Emitters]
            integer    sonar_list_depth[Max$Alsp$Emitters]
            common /sonar_common/ number_of_sonar_eq,
                                  sonar_list_code,
                                  sonar_list_characteristic_pointer,
                                  sonar_list_state,
                                  sonar_list_mode,
                                  sonar_list_depth

)
################

define (max_alsp_units, 2000)   # max number of units
define (alsp_change_flag_common,
        integer alsp_dr_flags[DR$FLAGS, max_alsp_units]
        common /alsp_dr_chng_fl/ alsp_dr_flags)

#$% 12-MAR-1992 10:59 adding an alspid/unit table index, index
#$% so a binary search can be executed
define (alsp_index_common,
        logical resort_flag
        integer alsp_index[2, MAX$SLOTS]
        common /alsp_index_cmmn/ alsp_index, resort_flag)


######################################################
#
# clk 2/94:
# Bit mask constants used for UNT$ALSPModifiedFlags$I
# 
  define (bitmask0, 1)
  define (bitmask1, 2)
  define (bitmask2, 4)
  define (bitmask3, 8)
  define (bitmask4, 16)
  define (bitmask5, 32)
  define (bitmask6, 64)
  define (bitmask7, 128)
  define (bitmask8, 256)
  define (bitmask9, 512)
  define (bitmask10, 1024)
  define (bitmask11, 2048)
  define (bitmask12, 4096)
  define (bitmask13, 8192)
  define (bitmask14, 16384)
  define (bitmask15, 32768)
  define (bitmask16, 65536)

########################################################################

define ($Ship$GunCode, 0)   # since there are separate ALSP enumerations
                            # for ship_to_ship enagements - ARGH!!!
                            # Zero is $Null$Code for DEQ$Ident$I values,
                            # so it should be safe to use it like this.
                            # The routines that use the outgoing weapon
                            # mapping for guns will need to send it in.

