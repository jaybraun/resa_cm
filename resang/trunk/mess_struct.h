
/*  This file contains the following:
 *
 *  1) Typedefs that describe ALSP message structures.   
 *  2) Declarations that comprise an interface for manipulating such
 *     structures.
 */

typedef enum message_data_type_d {
        no_mess_type = 0,
        acm_actor_mess = 1,
        actor_acm_mess = 2,
        actor_actor_mess = 3
        } MESSAGE_DATA_TYPE;

/* Actor-ACM messages 
 * ------------------
 */
typedef enum ta_message_type_d {
        type_ta_dummy = 0,
        type_ta_advance_request = 1,
        type_ta_cancel_lock = 2,
        type_ta_create_request = 3,
        type_ta_delete = 4,
        type_ta_dispatch = 5,
        type_ta_event = 6,
        type_ta_event_request = 7,
        type_ta_filter_attr = 8,
        type_ta_filter_class = 9,
        type_ta_id_req = 10,
        type_ta_join = 11,
        type_ta_lock_req = 12,
        type_ta_reg_attrs = 13,
        type_ta_reg_object = 14,
        type_ta_resign = 15,
        type_ta_unlock = 16,
        type_ta_save_request = 17,
        type_ta_save_ack = 18,
        type_ta_filter_interaction = 19,       /* clk 12/93 */
        type_ta_ping = 20,                     /* clk  1/94 */
        type_ta_ping_ack = 21,                 /* clk  1/94 */
        type_ta_save_complete = 22,            /* clk 9/95 (E1458) */
        type_ta_filter_parameters = 23,        /* SMM 9/96 (E1691) */
        type_ta_error = 24,                    /* SMM 9/96 (E1692) */
        type_ta_reg_enum = 25,                 /* SMM 10/96 (E1694) */
        type_ta_signal = 26,                   /* SMM 12/96 (E1694) */
        type_ta_reg_rate = 27                  /* clk  9/97 (E1892) */
        } TA_MESSAGE_TYPE;

#define N_TA_MESS 27

/* ACM-Actor messages 
 * ------------------
 */
typedef enum at_message_type_d {
        type_at_dummy = 0,
        type_at_avail = 1,
        type_at_create = 2,
        type_at_delete = 3,
        type_at_grant_advance = 4,
        type_at_interaction = 5,
        type_at_join_ack = 6,
        type_at_lock_notify = 7,
        type_at_new_id = 8,
        type_at_refresh = 9,
        type_at_unavail = 10,
        type_at_unlock_request = 11,
        type_at_update = 12,
        type_at_start_save = 13,
        type_at_set_rate = 14,
        type_at_ping = 15,              /* clk  1/94 */
        type_at_ping_ack = 16,          /* clk  1/94 */
        type_at_signal = 17             /* clk - 9/95 (E1457) */
        } AT_MESSAGE_TYPE;

#define N_AT_MESS 17

/* Trans-Trans messages
 * --------------------
 */
typedef enum tt_message_type_d {
        type_tt_dummy = 0,
        type_tt_interaction = 1,
        type_tt_refresh = 2,
        type_tt_update = 3 
        } TT_MESSAGE_TYPE;

#define N_TT_MESS 3

/* type_ta_advance_request param types
 * -----------------------------------
 */
typedef enum ta_adv_req_params {
        par_ta_adv_req_dummy = 0,
        par_ta_adv_req_time = 1
        } TA_ADV_REQ_PARAMS;

#define N_ADV_REQ_PAR 1

/* type_ta_cancel_lock param types
 * -------------------------------
 */
typedef enum ta_cancel_lock_params_d {
        par_ta_cancel_lock_dummy = 0,
        par_ta_cancel_lock_id = 1,
        par_ta_cancel_lock_attrs = 2
        } TA_CANCEL_LOCK_PARAMS;

#define N_CANCEL_LOCK_PAR 2

/* type_ta_create_request param types
 * ----------------------------------
 */
typedef enum ta_create_request_params_d {
        par_ta_create_request_dummy = 0,
        par_ta_create_request_id = 1
        } TA_CREATE_REQUEST_PARAMS;

#define N_CREATE_REQ_PAR 1

/* type_ta_delete param types
 * --------------------------
 */
typedef enum ta_delete_params_d {
        par_ta_delete_dummy = 0,
        par_ta_delete_id = 1
        } TA_DELETE_PARAMS;

#define N_TA_DELETE_PAR 1

/* type_ta_dispatch param types
 * ----------------------------
 */
typedef enum ta_dispatch_params_d {
        par_ta_disp_dummy = 0,
        par_ta_disp_disp = 1
        } TA_DISPATCH_PARAMS;

#define N_DISPATCH_PAR 1

/* type_ta_event param types
 * -------------------------
 */
typedef enum ta_event_params_d {
        par_ta_event_dummy = 0,
        par_ta_event_event = 1
        } TA_EVENT_PARAMS;

#define N_EVENT_PAR 1
#define N_EVENT_REQ_PAR 0

/* type_ta_filter_attr param types
 * -------------------------------
 */
 typedef enum ta_filter_attr_param_types_d {
        par_ta_filter_attr_dummy = 0,
        par_ta_filter_class = 1,
        par_ta_filter_attr_id = 2,
        par_ta_filter_attr_lat = 3,
        par_ta_filter_attr_long = 4,
        par_ta_filter_alt = 5,
        par_ta_filter_heading = 6,
        par_ta_filter_name = 7,
        par_ta_filter_percent = 8,
        par_ta_filter_side = 9,
        par_ta_filter_size = 10,
        par_ta_filter_speed = 11,
        par_ta_filter_stat = 12,
        par_ta_filter_unit_type = 13
        } TA_FILTER_ATTR_PARAMS;

#define N_FILTER_ATTR_PAR 13

/* par_ta_filter_class parameter values
 * ------------------------------------
 */
typedef enum tt_class_pvals_d { 
        pval_tt_class_dummy = 0,
        pval_tt_class_air = 1,
        pval_tt_class_air_fixed = 2,
        pval_tt_class_air_heli = 3,
        pval_tt_class_air_cruise_missile = 4,
        pval_tt_class_air_tbm = 5,              /* 12/93 clk */
        pval_tt_class_grnd = 6,
        pval_tt_class_grnd_base = 7,
        pval_tt_class_grnd_mnvr = 8,
        pval_tt_class_grnd_mnvr_himad = 9,
        pval_tt_class_grnd_mnvr_shorad = 10,
        pval_tt_class_grnd_mnvr_allrad = 11,
        pval_tt_class_grnd_mnvr_combat = 12,
        pval_tt_class_grnd_mnvr_support = 13,
        pval_tt_class_grnd_mnvr_support_convoy = 14,
        pval_tt_class_grnd_mnvr_support_unit = 15,
        pval_tt_class_grnd_mnvr_support_unit_tcu = 16,
        pval_tt_class_grnd_mnvr_radar = 17,
        pval_tt__class_grnd_mnvr_tel  = 18,        
        pval_tt_class_grnd_facility = 19,
        pval_tt_class_sea_subsurface = 20,            /* 8/20/93 clk */
        pval_tt_class_sea_torpedo = 21,
        pval_tt_class_sea_countermeasure = 22,
        pval_tt_class_sea_sonobuoy = 23,
        pval_tt_class_sea_mine = 24,
        pval_tt_class_sea_sweeparea = 25,
        pval_tt_class_sea_surface_boat = 26,          /* 8/20/93 clk */
        pval_tt_class_sea_surface_ship = 27,          /* 8/20/93 clk */
        pval_tt_class_sea_surface_aav = 28,           /* clk 10/95 (E1483) */
        pval_tt_class_sea_surface_landing_craft = 29  /* clk 10/95 (E1483) */
        } TT_CLASS_PVALS;

#define N_CLASS_VALS 29

/* type_ta_filter_class param types
 * --------------------------------
 */
 typedef enum ta_filter_class_param_types_d {
        par_ta_filter_class_dummy = 0,
        par_ta_filter_class_class = 1,
        par_ta_filter_class_condition = 2,
        par_ta_filter_class_send_dr = 3             /* C. Kropp (NRaD) 4/26/93 */
        } TA_FILTER_CLASS_PARAMS;

#define N_FILTER_CLASS_PAR 3

/* par_ta_filter_class_condition parameter values  
 * ----------------------------------------------
 */
typedef enum ta_fltr_class_cond_pvals_d {
        pval_ta_fc_cond_dummy = 0,
        pval_ta_fc_cond_allow = 1,
        pval_ta_fc_cond_disallow = 2
        } TA_FLTR_CLASS_COND_PVALS;

#define N_FILTER_COND_VALS 2

/* par_ta_filter_class_send_dr parameter values
 * --------------------------------------------
 */     /* C. Kropp (NRaD) 4/26/93 */
typedef enum ta_fltr_class_send_dr_pvals_d {
        pval_ta_fc_send_dr_dummy = 0,
        pval_ta_fc_send_dr_true = 1,
        pval_ta_fc_send_dr_false = 2
        } TA_FLTR_CLASS_SEND_DR_PVALS;

#define N_FILTER_SEND_DR_VALS 2

/* type_ta_filter_interaction param types
 * --------------------------------
 */
 typedef enum ta_filter_interaction_param_types_d {
        par_ta_filter_interaction_dummy = 0,
        par_ta_filter_interaction_kind = 1,
        par_ta_filter_interaction_condition = 2
        } TA_FILTER_INTERACTION_PARAMS;

#define N_FILTER_INTERACTION_PAR 2

/* type_ta_filter_parameters param types
 * --------------------------------
 */     /* Susan Miller 9/9/96 (E1691) */
 typedef enum ta_filter_parameters_param_types_d {
        par_ta_filter_parameters_dummy = 0,
        par_ta_filter_parameters_kind = 1,
        par_ta_filter_parameters_to = 2,
        par_ta_filter_parameters_from = 3
        } TA_FILTER_PARAMETERS_PARAMS;

#define N_FILTER_PARAMETERS_PAR 3

/* par_ta_filter_restrictions parameter values  
 * ----------------------------------------------
 */     /* Susan Miller 9/9/96 (E1691) */
typedef enum ta_fp_restrictions_pvals_d {
        pval_ta_fp_restrictions_dummy = 0,
        pval_ta_fp_restrictions_known = 1,
        pval_ta_fp_restrictions_owned = 2,
        pval_ta_fp_restrictions_ghosted = 3
        } TA_FLTR_PARAM_RESTRCTIONS_PVALS;

#define N_FLTR_RESTRICTIONS_VALS 3

/* type_ta_error param types     SMM 9/96 (E1692)
 * ------------------------
 */
typedef enum ta_error_params_d {
      par_ta_error_dummy = 0,
      par_ta_error_text = 1
      } TA_ERROR_PARAMS;
#define N_ERROR_PAR 1

/* type_ta_id_req param types
 * --------------------------
 */
 typedef enum ta_id_req_param_types_d {
        par_ta_id_req_dummy = 0,
        par_ta_id_req_number = 1
        } TA_ID_REQ_PARAMS;

#define N_ID_REQ_PAR 1

/* type_ta_join param types
 * ------------------------
 */
typedef enum ta_join_params_d {
        par_ta_join_dummy = 0,
        par_ta_join_lookahead = 1,
        par_ta_join_actor_name = 2
        } TA_JOIN_PARAMS;

#define N_JOIN_PAR 2

/* par_ta_join_time_mode parameter values  
 * --------------------------------------
 */
typedef enum ta_join_time_mode_pvals_d {
        pval_ta_jtm_dummy = 0,
        pval_ta_jtm_true = 1,
        pval_ta_jtm_false = 2
        } TA_JOIN_TIME_MODE_PVALS;

#define N_TIME_MODE_VALS 2

/* type_ta_lock_req param types
 * --------------------------
 */
 typedef enum ta_lock_req_param_types_d {
        par_ta_lock_req_dummy = 0,
        par_ta_lock_req_id = 1,
        par_ta_lock_req_attrs = 2,
        par_ta_lock_req_service = 3,
        par_ta_lock_req_notation = 4,
        par_ta_lock_req_comment = 5
        } TA_LOCK_REQ_PARAMS;

#define N_LOCK_REQ_PAR 5

/* par_ta_lock_req_service parameter values  
 * ----------------------------------------
 */
typedef enum ta_lock_req_serv_params_d {
        pval_ta_lock_req_serv_dummy = 0,
        pval_ta_lock_req_if_avail = 1,
        pval_ta_lock_req_when_avail = 2
        } TA_LOCK_REQ_SERV_PVALS;

#define N_LOCK_REQ_SERV_VALS 2

/* type_ta_reg_attrs param types
 * -----------------------------
 */
typedef enum ta_reg_attrs_params_d {
        par_ta_reg_attrs_dummy = 0,
        par_ta_reg_attrs_class = 1,
        par_ta_reg_attrs_create = 2,
        par_ta_reg_attrs_intr = 3,
        par_ta_reg_attrs_update = 4
        } TA_REG_ATTRS_PARAMS;

#define N_REG_ATTR_PAR 4

/* type_ta_reg_object param types
 * ------------------------------
 */
typedef enum ta_reg_object_params_d {
        par_ta_reg_object_dummy = 0,
        par_ta_reg_object_id = 1,
        par_ta_reg_object_class = 2,
        par_ta_reg_object_unlock = 3
        } TA_REG_OBJECT_PARAMS;

#define N_REG_OBJECT_PAR 3


/* SMM 10/96 (E1694)  */
/* type_ta_reg_enum param types
 * -----------------------------
 */
typedef enum ta_reg_enum_params_d {
        par_ta_reg_enum_dummy = 0,
        par_ta_reg_enum_class = 1,
        par_ta_reg_enum_kind = 2,
        par_ta_reg_enum_attr = 3,
        par_ta_reg_enum_unit_type = 4,
        par_ta_reg_enum_emitter = 5,
        par_ta_reg_enum_radar = 6,
        par_ta_reg_enum_jammer = 7,
        par_ta_reg_enum_type = 8,
        par_ta_reg_enum_weapon = 9,
        par_ta_reg_enum_alsp_val = 10,
        par_ta_reg_enum_actor_val = 11,
        par_ta_reg_enum_default = 12
        } TA_REG_ENUM_PARAMS;

#define N_REG_ENUM_PAR 12

/* type_ta_reg_rate param types          # clk 9/97 (E1892)
 * ------------------------
 */
typedef enum ta_reg_rate_params_d {
        par_ta_reg_rate_dummy = 0,
        par_ta_reg_rate_gamerate = 1
        } TA_REG_RATE_PARAMS;

#define N_REG_RATE_PAR 1

/* par_ta_resign parameter values
 * ------------------------------------
 */
typedef enum tt_resign_pvals_d {           /* clk 1/94 - for the new parameters of */
        pval_tt_resign_dummy = 0,          /*            the resign message        */
        pval_tt_resign_delete_owned = 1,
        pval_tt_resign_disperse_owned = 2
        } TT_RESIGN_PVALS;

#define N_RESIGN_VALS 2

/* type_ta_resign param types
 * ------------------------------
 */
typedef enum ta_resign_params_d {         /* clk 1/94 - for the new parameters of */
        par_ta_resign_dummy = 0,          /*            the resign message        */
        par_ta_resign_owned_units = 1
        } TA_RESIGN_PARAMS;

#define N_RESIGN_PAR 1

/* type_ta_unlock param types
 * --------------------------
 */
typedef enum ta_unlock_params_d {
        par_ta_unlock_dummy = 0,
        par_ta_unlock_id = 1,
        par_ta_unlock_attrs = 2
        } TA_UNLOCK_PARAMS;

#define N_UNLOCK_PAR 2

/* type_ta_save_request param types
 * --------------------------------
 */
typedef enum ta_save_req_params_d {
      par_ta_save_req_dummy = 0,
      par_ta_save_req_time = 1,
      par_ta_save_req_label = 2
      } TA_SAVE_REQ_PARAMS;

#define N_SAVE_REQ_PAR 2

/* type_ta_save_ack param types
 * --------------------------------
 */
typedef enum ta_save_ack_params_d {
      par_ta_save_ack_dummy = 0,
      par_ta_save_ack_time = 1,
      par_ta_save_ack_clock = 2
      } TA_SAVE_ACK_PARAMS;

#define N_SAVE_ACK_PAR 2

/* type_ta_save_complete param types
 * --------------------------------
 */
typedef enum ta_save_complete_params_d {
      par_ta_save_complete_dummy = 0,
      par_ta_save_complete_time = 1,
      par_ta_save_complete_clock = 2
      } TA_SAVE_COMPLETE_PARAMS;

#define N_SAVE_COMPLETE_PAR 2


/* type_ta_ping param types         * clk  1/94 *
 * ------------------------
 */
typedef enum ta_ping_params_d {
      par_ta_ping_dummy = 0,
      par_ta_ping_text = 1
      } TA_PING_PARAMS;
#define N_PING_PAR 1

/* type_ta_ping_ack param types     * clk  1/94 *
 * ------------------------
 */
typedef enum ta_ping_ack_params_d {
      par_ta_ping_ack_dummy = 0,
      par_ta_ping_ack_text = 1
      } TA_PING_ACK_PARAMS;
#define N_PING_ACK_PAR 1


/* type_at_avail param types
 * ----------------------------
 */
typedef enum at_avail_params_d {
        par_at_avail_dummy = 0,
        par_at_avail_id = 1,
        par_at_avail_attrs = 2
        } AT_AVAIL_PARAMS;

#define N_AVAIL_PAR 2

/* tt attribute types  
 * ------------------
 *
 * Used for UPDATE and CREATE message parameter lists
 * AND for all ATTRIBUTE LISTS!
 */
typedef enum tt_attribute_d {
        tt_attr_dummy = 0,
        tt_attr_id = 1,
        tt_attr_class = 2,
        tt_attr_lat = 3,
        tt_attr_long = 4,
        tt_attr_alt = 5,
        tt_attr_arty_supp = 6,
        tt_attr_awsim_stat = 7,
        tt_attr_cbs_stat = 8,
        tt_attr_elect_deg = 9,
        tt_attr_fire_cont = 10,
        tt_attr_heading = 11,
        tt_attr_name = 12,
        tt_attr_old_id = 13,
        tt_attr_orb_lat1 = 14,
        tt_attr_orb_lon1 = 15,
        tt_attr_orb_lat2 = 16,
        tt_attr_orb_lon2 = 17,
        tt_attr_percent = 18,
        tt_attr_side = 19,
        tt_attr_unit_size = 20,
        tt_attr_speed = 21,
        tt_attr_status = 22,
        tt_attr_supply_l = 23,
        tt_attr_supply_m = 24,
        tt_attr_supply_r = 25,
        tt_attr_target_lat = 26,
        tt_attr_target_long = 27,
        tt_attr_unit_type = 28,
        tt_attr_mission = 29,
        tt_attr_posture = 30,
        tt_attr_orientation = 31,
        tt_attr_time = 32,
        tt_attr_call_sign = 33,
        tt_attr_age = 34,                   /* khc,3/93 */
        tt_attr_depth = 35,                 /* clk 8/93 */
        tt_attr_flight_ops = 36,            /* clk 8/95 */
        tt_attr_emitter = 37,
        tt_attr_radar = 38,       /* SMM 9/94 */
        tt_attr_jammer = 39,
        tt_attr_index = 40,
        tt_attr_orient = 41,
        tt_attr_mode = 42,
        tt_attr_type = 43,
        tt_attr_emitter_status = 44,
        tt_attr_c2w_factors = 45,
        tt_attr_unit = 46,
        tt_attr_emtr = 47,
        tt_attr_ew_c3 = 48,
        tt_attr_ew_acq = 49,
        tt_attr_ew_launch = 50,
        tt_attr_ew_guide = 51,
        tt_attr_ew_power = 52,
        tt_attr_ew_surf = 53,
        tt_attr_weapon = 54,          /* SMM 1/97 E1694 */
        tt_attr_iff = 55,             /* smp - 2/98 (E1907) */
        tt_attr_iffmode_master = 56,  /* smp - 2/98 (E1907) */
        tt_attr_iffmode1 = 57,        /* smp - 2/98 (E1907) */
        tt_attr_iffmode2 = 58,        /* smp - 2/98 (E1907) */
        tt_attr_iffmode3 = 59,        /* smp - 2/98 (E1907) */
        tt_attr_iffmodeC = 60,        /* smp - 2/98 (E1907) */
        tt_attr_iffmode4 = 61,        /* smp - 2/98 (E1907) */
        tt_attr_iffmode_status = 62,  /* smp - 2/98 (E1907) */
        tt_attr_iffmode_value = 63,   /* smp - 2/98 (E1907) */
        tt_attr_propulsion_value = 64,
        tt_attr_sonar = 65,
        tt_attr_sonar_type = 66,
        tt_attr_sonar_status = 67,
        tt_attr_sonar_mode = 68,
        tt_attr_sonar_depth = 69,
        tt_attr_position  = 70,
        tt_attr_sweeparea_location  = 71,
        tt_attr_sweeparea_status  = 72,
		tt_attr_atoid = 73,
		tt_attr_strength = 74
        } TT_ATTRIBUTES;

#define N_ATTRIBUTE_VALS 74  /* make sure MAX_PARAM in alsp.inc matches */
#define SEND_NO_ATTRIBUTE_VALUE -9999

/* tt_attr_unit_type parameter values
 * ----------------------------------
 */

#include "pvals.h"

/* tt_attr_posturparameter values
 * --------------------------------
 */
typedef enum tt_posture_pvals_d {
        pval_tt_posture_dummy = 0,
        pval_tt_posture_attack = 1,
        pval_tt_posture_defend = 2,
        pval_tt_posture_withdraw = 3,
        pval_tt_posture_moving = 4,
        pval_tt_posture_move_contact = 5,
        pval_tt_posture_incapable = 6,
        pval_tt_posture_wiped = 7
        } TT_POSTURE_PVALS;

#define N_POSTURE_VALS 7

/* tt_attr_orient parameter values
 * -------------------------------
 */
typedef enum tt_orient_pvals_d {
        pval_tt_orient_dummy = 0
        } TT_ORIENT_PVALS;

#define N_ORIENT_VALS 1

/* tt_attr_awsim_stat parameter values
 * -----------------------------------
 */
typedef enum tt_awsim_stat_pvals_d {
        pval_tt_awsim_stat_dummy = 0,
        pval_tt_awsim_stat_operational = 1,
        pval_tt_awsim_stat_suppressed = 2
        } TT_AWSIM_STAT_PVALS;

#define N_AWSIM_STAT_VALS 2

/* tt_attr_cbs_stat parameter values
 * ---------------------------------
 */
typedef enum tt_cbs_stat_pvals_d {
        pval_tt_cbs_stat_dummy = 0,
        pval_tt_cbs_stat_operational = 1,
        pval_tt_cbs_stat_moving = 2,
        pval_tt_cbs_stat_suppressed = 3,
        pval_tt_cbs_stat_destroyed = 4
        } TT_CBS_STAT_PVALS;

#define N_CBS_STAT_VALS 4



/* tt_attr_emitter_status parameter values */
typedef enum tt_emitter_status_pvals_d {     /* KH,2/93 - emitter status values */
        pval_tt_emitter_status_dummy   = 0,
        pval_tt_emitter_status_off     = 1,
        pval_tt_emitter_status_on      = 2
        } TT_EMITTER_STATUS_PVALS;

#define N_EMITTER_STATUS_VALS 2

/* tt_attr_mode parameter values */
typedef enum tt_emitter_mode_pvals_d {
        pval_tt_emitter_mode_dummy   = 0,
        pval_tt_emitter_mode_no_mode = 1,
        pval_tt_emitter_mode_net1    = 2,
        pval_tt_emitter_mode_net2    = 3,
        pval_tt_emitter_mode_net3    = 4,
        pval_tt_emitter_mode_net4    = 5,
        pval_tt_emitter_mode_net5    = 6,
        pval_tt_emitter_mode_net6    = 7,
        pval_tt_emitter_mode_net7    = 8,
        pval_tt_emitter_mode_net8    = 9,
        pval_tt_emitter_mode_net9    = 10,
        pval_tt_emitter_mode_all     = 11
        } TT_EMITTER_MODE_PVALS;        

#define N_EMITTER_MODE_VALS 11

/* tt_attr_sonar_status parameter values */
typedef enum tt_sonar_status_pvals_d { 
        pval_tt_sonar_status_dummy   = 0,
        pval_tt_sonar_status_off     = 1,
        pval_tt_sonar_status_on      = 2
        } TT_SONAR_STATUS_PVALS;

#define N_SONAR_STATUS_VALS 2

/* tt_attr_sonar_mode parameter values */
typedef enum tt_sonar_mode_pvals_d { 
        pval_tt_sonar_mode_dummy   = 0,
        pval_tt_sonar_mode_dp      = 1,
        pval_tt_sonar_mode_cz      = 2,
        pval_tt_sonar_mode_bb      = 3
        } TT_SONAR_MODE_PVALS;

#define N_SONAR_MODE_VALS 3

/*########################################################################*/

typedef enum tt_side_pvals_d {
        pval_tt_side_dummy = 0,
        pval_tt_side_red = 1,
        pval_tt_side_blue = 2,
        pval_tt_side_neutral = 3,
        pval_tt_side_unknown = 4
        } TT_SIDE_PVALS;

#define N_SIDE_VALS 4

/* tt_attr_iffmode_status parameter values */
typedef enum tt_iffmode_status_pvals_d {           /* iffmodes status values */
        pval_tt_iffmode_status_dummy   = 0,        /* smp - 2/98 (E1907)     */
        pval_tt_iffmode_status_off     = 1,
        pval_tt_iffmode_status_on      = 2
        } TT_IFFMODE_STATUS_PVALS;

#define N_IFFMODE_STATUS_VALS 2

/* tt_attr_propulsion_mode parameter values */
typedef enum tt_propulsion_mode_pvals_d { 
        pval_tt_propulsion_mode_dummy     = 0,
        pval_tt_propulsion_mode_diesel    = 1,
        pval_tt_propulsion_mode_electric  = 2,
        pval_tt_propulsion_mode_nuclear   = 3,
        } TT_PROPULSION_MODE_PVALS;

#define N_PROPULSION_MODE_VALS 3

/* tt_attr_sweeparea_status parameter values */
typedef enum tt_sweeparea_status_pvals_d {  
        pval_tt_sweeparea_status_dummy        = 0,
        pval_tt_sweeparea_status_progress     = 1,
        pval_tt_sweeparea_status_complete     = 2
        } TT_SWEEPAREA_STATUS_PVALS;

#define N_SWEEPAREA_STATUS_VALS 2

/* type_at_delete param types
 * --------------------------
 */
typedef enum at_delete_params_d {
        par_at_delete_dummy = 0,
        par_at_delete_id = 1
        } AT_DELETE_PARAMS;

#define N_AT_DELETE_PAR 1

/* type_at_grant_advance param types
 * -----------------------------------
 */
typedef enum at_grant_adv_params_d {
        par_at_grant_adv_dummy = 0,
        par_at_grant_adv_time = 1
        } AT_GRANT_ADV_PARAMS;

#define N_GRANT_ADV_PAR 1

/* type_at_join_ack param types
 * ----------------------------
 */
typedef enum at_join_ack_params_d {
        par_at_join_ack_dummy = 0,
        par_at_join_ack_time = 1
        } AT_JOIN_ACK_PARAMS;

#define N_JOIN_ACK_PAR 1

/* type_at_lock_notify param types
 * -------------------------------
 */
typedef enum at_lock_notify_params_d {
        par_at_lock_notify_dummy = 0,
        par_at_lock_notify_id = 1,
        par_at_lock_notify_attrs = 2,
        par_at_lock_notify_status = 3,
        par_at_lock_notify_notation = 4,
        par_at_lock_notify_comment = 5
        } AT_LOCK_NOTIFY_PARAMS;

#define N_LOCK_NOTIFY_PAR 5

/* par_at_lock_notify_status parameter values
 * ------------------------------------------
 */
typedef enum at_lock_not_stat_pvals_d {
        pval_at_lock_not_stat_dummy = 0,
        pval_at_lock_not_stat_sec = 1,
        pval_at_lock_not_stat_rej = 2,
        pval_at_lock_not_stat_acq = 3
        } AT_LOCK_NOT_STAT_PVALS;
        
#define N_LOCK_STATUS_VALS 3

/* type_at_new_id param types
 * --------------------------
 */
typedef enum at_new_id_params_d {
        par_at_new_id_dummy = 0,
        par_at_new_id_id_list = 1
        } AT_NEW_ID_PARAMS;
        
#define N_NEW_ID_PAR 1

/* type_at_unavail param types
 * ---------------------------
 */
typedef enum at_unavail_params_d {
        par_at_unavail_dummy = 0,
        par_at_unavail_id = 1,
        par_at_unavail_attrs = 2
        } AT_UNAVAIL_PARAMS;
        
#define N_UNAVAIL_PAR 2

/* type_at_unlock_request param types
 * ----------------------------------
 */
typedef enum at_unlock_req_params_d {
        par_at_unlock_req_dummy = 0,
        par_at_unlock_req_id = 1,
        par_at_unlock_req_attrs = 2,
        par_at_unlock_req_notation = 3
        } AT_UNLOCK_REQ_PARAMS;

#define N_UNLOCK_REQ_PAR 3

/* type_at_start_save param types
 * --------------------------------
 */
typedef enum at_start_save_params_d {
      par_at_start_save_dummy = 0,
      par_at_start_save_time = 1,
      par_at_start_save_label = 2
      } AT_START_SAVE_PARAMS;

#define N_START_SAVE_PAR 2

/* type_at_set_rate param types
 * --------------------------------
 */
typedef enum at_set_rate_params_d {
      par_at_set_rate_dummy = 0,
      par_at_set_rate_rate = 1,
      } AT_SET_RATE_PARAMS;

#define N_SET_RATE_PAR 1

/* type_at_signal param types       clk - 9/95 (E1457)
 * --------------------------
 */
typedef enum at_signal_params_d {
        par_at_signal_dummy = 0,
        par_at_signal_type = 1
        } AT_SIGNAL_PARAMS;

#define N_AT_SIGNAL_PAR 1

/* signal parameter values          clk - 9/95 (E1457)
 * --------------------------
 */
typedef enum at_signal_pvals_d {
        pval_at_signal_dummy = 0,
        pval_at_signal_register_objects = 1,
        pval_at_signal_register_enumeration = 2    /* SMM 10/96 (E1694) */
        } AT_SIGNAL_PVALS;

#define N_SIGNAL_VALS 2

/* type_ta_signal param types    SMM - 12/96 (E1694)
 * --------------------------
 */
typedef enum ta_signal_params_d {
        par_ta_signal_dummy = 0,
        par_ta_signal_type = 1
        } TA_SIGNAL_PARAMS;

#define N_TA_SIGNAL_PAR 1

/* actor to acm signal parameter values   SMM - 12/96 (E1694)
 * --------------------------
 */
typedef enum ta_signal_pvals_d {
        pval_ta_signal_dummy = 0,
        pval_ta_signal_enumerations_complete = 1    /* SMM 12/96 (E1694) */
        } TA_SIGNAL_PVALS;

#define N_TA_SIGNAL_VALS 1

/* unit side parameter values
 * --------------------------
 */
typedef enum tt_unit_side_pvals_d {
        pval_tt_unit_side_dummy = 0,
        pval_tt_unit_side_red = 1,
        pval_tt_unit_side_blue = 2,
        pval_tt_unit_side_neutral = 3,
        pval_tt_unit_side_unknown = 4
        } TT_UNIT_SIDE_PVALS;

#define N_UNIT_SIDE_VALS 4

/* unit flight_ops parameter values
 * --------------------------------
 */
typedef enum tt_unit_flight_ops_pvals_d {
        pval_tt_unit_flight_ops_dummy = 0,
        pval_tt_unit_flight_ops_open = 1,
        pval_tt_unit_flight_ops_closed = 2
        } TT_UNIT_FLIGHT_OPS_PVALS;

#define N_UNIT_FLIGHT_OPS_VALS 2

/* unit size parameter values
 * --------------------------
 */
typedef enum tt_unit_size_pvals_d {
        pval_tt_unit_sz_dummy = 0,
        pval_tt_unit_sz_army = 1,
        pval_tt_unit_sz_base = 2,
        pval_tt_unit_sz_brigade = 3,
        pval_tt_unit_sz_battalion = 4,
        pval_tt_unit_sz_battery = 5,
        pval_tt_unit_sz_company = 6,
        pval_tt_unit_sz_corps = 7,
        pval_tt_unit_sz_detachment = 8,
        pval_tt_unit_sz_division = 9,
        pval_tt_unit_sz_hq = 10,
        pval_tt_unit_sz_platoon = 11,
        pval_tt_unit_sz_regiment = 12,
        pval_tt_unit_sz_squad = 13,
        pval_tt_unit_sz_squadron = 14,
        pval_tt_unit_sz_team = 15,
        pval_tt_unit_sz_taskforce = 16,
        pval_tt_unit_sz_troop = 17,
        pval_tt_unit_sz_wing = 18,
        pval_tt_unit_sz_section = 19,
        pval_tt_unit_sz_army_group = 20
        } TT_UNIT_SIZE_PVALS;

#define N_UNIT_SIZE_VALS 20

/* par_tt_update_status values
 * ----------------------------
 */
typedef enum tt_update_status_pvals_d {
        pval_tt_update_stat_dummy = 0,
        pval_tt_update_stat_avail = 1,
        pval_tt_update_stat_combat = 2,
        pval_tt_update_stat_move = 3,
        pval_tt_update_stat_drop = 4,
        pval_tt_update_stat_fly = 5,
        pval_tt_update_stat_lift = 6,
        pval_tt_update_stat_grnd = 7,
        pval_tt_update_stat_orbit = 8,
        pval_tt_update_stat_proceed = 9,
        pval_tt_update_stat_ready = 10,
        pval_tt_update_stat_delayed = 11,
        pval_tt_update_stat_underway = 12,   /* clk 12/96 (E1724) */
        pval_tt_update_stat_inport = 13,     /* clk 12/96 (E1724) */
        pval_tt_update_stat_sinking = 14,     /* clk 12/96 (E1724) */
        pval_tt_update_stat_complete = 15,
        pval_tt_update_stat_inprogress = 16
        } TT_UPDATE_STATUS_PVALS;

#define N_UPDATE_STAT_VALS 14


typedef enum tt_salvo_wsize_pvals_d {
        pval_tt_salvo_wsize_dummy = 0,
        pval_tt_salvo_wsize_105mm = 1,
        pval_tt_salvo_wsize_122mm = 2,
        pval_tt_salvo_wsize_152mm = 3,
        pval_tt_salvo_wsize_175mm = 4,
        pval_tt_salvo_wsize_203mm = 5,
        pval_tt_salvo_wsize_2s1 = 6,
        pval_tt_salvo_wsize_2s3 = 7,
        pval_tt_salvo_wsize_2s4 = 8,
        pval_tt_salvo_wsize_2s5 = 9,
        pval_tt_salvo_wsize_2s7 = 10,
        pval_tt_salvo_wsize_2s9 = 11,
        pval_tt_salvo_wsize_8in = 12,
        pval_tt_salvo_wsize_bm21 = 13,
        pval_tt_salvo_wsize_bm22 = 14,
        pval_tt_salvo_wsize_bm27 = 15,
        pval_tt_salvo_wsize_d20 = 16,
        pval_tt_salvo_wsize_d30 = 17,
        pval_tt_salvo_wsize_frog = 18,
        pval_tt_salvo_wsize_lance = 19,
        pval_tt_salvo_wsize_missile = 20,
        pval_tt_salvo_wsize_mlrs = 21,
        pval_tt_salvo_wsize_scud = 22,
        pval_tt_salvo_wsize_ss1 = 23,
        pval_tt_salvo_wsize_ss12 = 24,
        pval_tt_salvo_wsize_ss21 = 25
        } TT_SALVO_WSIZE_PVALS;

#define N_SALVO_WSIZE_VALS 25


/* par_tt_int_supply_type values
 * -----------------------------
 */
typedef enum tt_supply_type_pvals_d {
        pval_tt_supply_type_dummy = 0,
        pval_tt_supply_type_jp4 = 1
        } TT_SUPPLY_TYPE_PVALS;

#define N_SUPPLY_TYPE_VALS 1


/* par_tt_int_supply_reason values             SMM 10/95 per E1453
 * -----------------------------
 */
typedef enum tt_supply_reason_pvals_d {
        pval_tt_supply_reason_dummy = 0,
        pval_tt_supply_reason_none_available = 1,
        pval_tt_supply_reason_supplier_not_known = 2,
        pval_tt_supply_reason_receiver_not_known = 3
        } TT_SUPPLY_REASON_PVALS;

#define N_SUPPLY_REASON_VALS 3

/* type_tt_interaction param types
 * ---------------------------------
 */
typedef enum tt_interaction_params_d {
        par_tt_int_dummy = 0,
        par_tt_int_firing = 1,
        par_tt_int_victim = 2,
        par_tt_int_fire_kind = 3,
        par_tt_int_fire_quant = 4,
        par_tt_int_duration = 5,
        par_tt_int_percent = 6,
        par_tt_int_kills = 7,
        par_tt_int_lat = 8,
        par_tt_int_long = 9,
        par_tt_int_firer_lat = 10,
        par_tt_int_firer_long = 11,
        par_tt_int_mission = 12,
        par_tt_int_radius = 13,
        par_tt_int_salvo_num = 14,
        par_tt_int_salvo_wtype = 15,
        par_tt_int_salvo_size = 16,
        par_tt_int_target = 17,
        par_tt_int_report = 18,
        par_tt_int_rep_num = 19,
        par_tt_int_targets_eng = 20,
        par_tt_int_hits = 21,
        par_tt_int_supply_type = 22,
        par_tt_int_supply_qty = 23,
        par_tt_int_supply_reason = 24,
        par_tt_target_ref = 25,
        par_tt_target_ref_ben = 26,
        par_tt_target_ref_ben_suf = 27,
        par_tt_target_ref_ttn = 28,
        par_tt_target_ref_pin = 29,
        par_tt_target_ref_loc = 30,
        par_tt_int_correlation_id = 31
        } TT_INTERACTION_PARAMS;

#define N_INTERACTION_PAR 31

/* par_tt_int_fire_kind values
 * ---------------------------
 */
typedef enum tt_int_fire_kind_pvals_d {
        pval_tt_int_kind_dummy = 0,
        pval_tt_int_kind_report = 1,
        pval_tt_int_kind_mines = 2,
        pval_tt_int_kind_air_to_air = 3,                    /* clk 2/93 */
        pval_tt_int_kind_air_to_ground = 4,                 /* clk 9/93 */
        pval_tt_int_kind_air_to_ship = 5,                   /* clk 9/93 */
        pval_tt_int_kind_ship_to_air = 6,                   /* clk 9/93 */
        pval_tt_int_kind_ship_to_ground = 7,                /* clk 9/93 */
        pval_tt_int_kind_ship_to_ship = 8,                  /* clk 9/93 */
        pval_tt_int_kind_ground_to_air = 9,                 /* clk 9/93 */
        pval_tt_int_kind_ground_to_ground = 10,             /* clk 9/93 */
        pval_tt_int_kind_ground_to_ship = 11,               /* clk 9/93 */
        pval_tt_int_kind_report_air_to_air = 12,            /* clk 12/93 */
        pval_tt_int_kind_report_air_to_ground = 13,         /* clk 12/93 */
        pval_tt_int_kind_report_air_to_ship = 14,           /* clk 12/93 */
        pval_tt_int_kind_report_ship_to_air = 15,           /* clk 12/93 */
        pval_tt_int_kind_report_ship_to_ground = 16,        /* clk 12/93 */
        pval_tt_int_kind_report_ship_to_ship = 17,          /* clk 12/93 */
        pval_tt_int_kind_report_ground_to_air = 18,         /* clk 12/93 */
        pval_tt_int_kind_report_ground_to_ground = 19,      /* clk 12/93 */
        pval_tt_int_kind_report_ground_to_ship = 20,        /* clk 12/93 */
        pval_tt_int_kind_supporting_units_request = 21,     /* smm 9/95 */
        pval_tt_int_kind_pol_request = 22,                  /* smm 9/95 */
        pval_tt_int_kind_supporting_units_issue = 23,       /* smm 9/95 */
        pval_tt_int_kind_pol_issue = 24                     /* smm 9/95 */
        } TT_INT_FIRE_KIND_PVALS;

#define N_INT_KIND_VALS 24

/* par_tt_int_mission values
 * -------------------------
 */
typedef enum tt_int_mission_pvals_d {
        pval_tt_int_miss_dummy = 0,
        pval_tt_int_miss_ad = 1,
        pval_tt_int_miss_aew = 2,
        pval_tt_int_miss_af_strike = 3,
        pval_tt_int_miss_ai = 4,
        pval_tt_int_miss_airlift = 5,
        pval_tt_int_miss_airtanker = 6,
        pval_tt_int_miss_amphib = 7,
        pval_tt_int_miss_asw = 8,
        pval_tt_int_miss_bai = 9,
        pval_tt_int_miss_cap = 10,
        pval_tt_int_miss_cargo = 11,
        pval_tt_int_miss_cas = 12,
        pval_tt_int_miss_commerce = 13,
        pval_tt_int_miss_csar = 14,              /* clk 1/97 (E1730) */
        pval_tt_int_miss_dca = 15,
        pval_tt_int_miss_escort = 16,
        pval_tt_int_miss_faca = 17,              /* clk 1/97 (E1730) */
        pval_tt_int_miss_generic_mission = 18,
        pval_tt_int_miss_jammer = 19,
        pval_tt_int_miss_marops = 20,
        pval_tt_int_miss_navy_strike = 21,
        pval_tt_int_miss_none = 22,
        pval_tt_int_miss_oca = 23,
        pval_tt_int_miss_recon = 24,
        pval_tt_int_miss_relay = 25,
        pval_tt_int_miss_sar = 26,
        pval_tt_int_miss_sead = 27,
        pval_tt_int_miss_sea_control = 28,
        pval_tt_int_miss_sealift = 29,           /* clk 12/96 (E1724) */
        pval_tt_int_miss_spec_miss = 30,
        pval_tt_int_miss_strikecap = 31,
        pval_tt_int_miss_sttanker = 32,
        pval_tt_int_miss_surcap = 33,
        pval_tt_int_miss_surveillance = 34,
        pval_tt_int_miss_transfer = 35           /* clk 1/97 (E1730) */
        } TT_INT_MISSION_PVALS;

#define N_INT_MISSION_VALS 32

/* par_tt_int_target values
 * -------------------------
 */
typedef enum tt_int_target_pvals_d {
        pval_tt_int_targ_dummy = 0,
        pval_tt_int_targ_acq_radar = 1,
        pval_tt_int_targ_airdef = 2,
        pval_tt_int_targ_aircraft = 3,
        pval_tt_int_targ_antitn = 4,
        pval_tt_int_targ_armor = 5,
        pval_tt_int_targ_arty = 6,
        pval_tt_int_targ_ciws = 7,
        pval_tt_int_targ_comm = 8,
        pval_tt_int_targ_dismnt = 9,
        pval_tt_int_targ_eng_brg = 10,
        pval_tt_int_targ_engr = 11,
        pval_tt_int_targ_fixed_brg = 12,
        pval_tt_int_targ_filghtline = 13,
        pval_tt_int_targ_fuel = 14,
        pval_tt_int_targ_generic = 15,
        pval_tt_int_targ_larmor = 16,
        pval_tt_int_targ_maintenance = 17,
        pval_tt_int_targ_misc = 18,
        pval_tt_int_targ_parked = 19,
        pval_tt_int_targ_personnel = 20,
        pval_tt_int_targ_radar = 21,
        pval_tt_int_targ_radars = 22,
        pval_tt_int_targ_rip = 23,
        pval_tt_int_targ_shelters = 24,
        pval_tt_int_targ_stores = 25,
        pval_tt_int_targ_supply = 26,
        pval_tt_int_targ_trkvan = 27,
        pval_tt_int_targ_weapons = 28
        } TT_INT_TARGET_PVALS;

#define N_INT_TARGET_VALS 28

/* type_tt_refresh param types
 * ----------------------------
 */
typedef enum type_tt_refresh_params_d {
        par_tt_refresh_dummy = 0,
        par_tt_refresh_id = 1,
        par_tt_refresh_class = 2,
        par_tt_refresh_attrs = 3
        } TT_REFRESH_PARAMS;
        
#define N_REFRESH_PAR 3

typedef enum param_value_type_d {
        value_none,
        value_int,
        value_int_range,
        value_int_list,
        value_enum,
        value_enum_list,
        value_float,
        value_float_range,
        value_loc_struct,
        value_string,
        value_message,
        value_attribute_list,
        value_loc_struct_list,
        value_emit_struct_list,
        value_float_list,
        value_iff_struct_list,    /*  smp 2/98 (E1907) */
        value_sonar_struct_list
        } PARAM_VALUE_TYPE;

typedef struct int_range_d {
        int lower;
        int upper;
        } INT_RANGE;

typedef struct float_range_d {
        float lower;
        float upper;
        } FLOAT_RANGE;

typedef struct loc_struct_d {
        float lat;
        float lon; 
        } LOC_STRUCT;

/*#define MAX_LIST 30*/
#define MAX_LIST N_ATTRIBUTE_VALS+1

typedef int INTLIST [MAX_LIST];

typedef float FLOATLIST [MAX_LIST];

typedef struct int_list_d {
        int nvals;
        INTLIST list;
        } INT_LIST;
        
typedef struct float_list_d {
        int nvals;
        FLOATLIST list;
        } FLT_LIST;
        
typedef struct enum_value_d {
        int enum_value;
        char *enum_string;  /* NOTE:  SHOULD BE PTR INTO STATIC TABLE! */
        } ENUM_VALUE;

typedef ENUM_VALUE ENUMLIST [MAX_LIST];

typedef struct enum_list_d {
        int nvals;
        ENUMLIST list;
        } ENUM_LIST;

typedef LOC_STRUCT LOCLIST [MAX_LIST];

typedef struct loc_list_d {
        int nvals;
        LOCLIST list;
        } LOC_LIST;

typedef BOOL ALSP_ATTRIBUTE_LIST [N_ATTRIBUTE_VALS + 1];

typedef enum attribute_qualifier_d {
        qual_dummy = 0,
        qual_none = 1,
        qual_dr = 2,
        qual_all = 3        /* khc,3/93- Qualifier key-enumerator of emitter (@ALL). */
        } ATTRIBUTE_QUALIFIER;

#define N_QUALIFIER_VALS 3

#define MAX_EMITTERS 80
#define N_EW_SECTOR_VALS 8
#define N_EMIT_PARAMS 6

#define MAX_SONARS 11
#define N_SONAR_PARAMS 4

#define MAX_IFFMODES 5          /* smp 2/98 (E1907) */
#define N_IFF_PARAMS 6          /* smp 2/98 (E1907) */

#define MAX_POSITIONS 20

typedef struct emit_values_d {
        BOOL emitter_params[N_EMIT_PARAMS];
        int emitter_cat;
        ENUM_VALUE emitter_type;
        int emitter_index;
        ENUM_VALUE emitter_status;
        float emitter_orientation;
        ENUM_VALUE emitter_mode;
        }EMITTER_VALUES;

typedef EMITTER_VALUES EMITTERLIST[MAX_EMITTERS];

typedef struct emitter_list_d {
        int nvals;
        EMITTERLIST list;
        } EMITTER_LIST;

/* sonar list */

typedef struct sonar_values_d {
        BOOL sonar_params[N_SONAR_PARAMS];
        ENUM_VALUE sonar_type;
        ENUM_VALUE sonar_status;
        ENUM_VALUE sonar_mode;
        int sonar_depth;
        }SONAR_VALUES;

typedef SONAR_VALUES SONARLIST[MAX_SONARS];

typedef struct sonar_list_d {
        int nvals;
        SONARLIST list;
        } SONAR_LIST;

/* position list */

typedef struct position_values_d {
        float latitude;
        float longitude;
        }POSITION_VALUES;

typedef POSITION_VALUES POSITIONLIST[MAX_POSITIONS];

typedef struct position_list_d {
        int nvals;
        POSITIONLIST list;
        } POSITION_LIST;

typedef struct iff_values_d {        /* smp 2/98 (E1907) */
        BOOL iffmode_params;
        ENUM_VALUE iffmode_status;
        int iffmode_value;
        }  IFFMODESTR;

typedef struct IFFMODE_listIFF_d {   /* smp 2/98 (E1907) */
        IFFMODESTR iffmode_array[N_IFF_PARAMS];
        } IFFMODELIST;

typedef union param_val_d {
        int val_integer;
        ENUM_VALUE val_enum;
        float val_float;
        char *val_string;
        LOC_STRUCT val_loc;
        struct message_struct_d *mess;  
        INT_RANGE integer_range;
        FLOAT_RANGE float_pt_range;
        INT_LIST integer_list;
        ENUM_LIST en_list;
        LOC_LIST location_list;
        ALSP_ATTRIBUTE_LIST attr_list;
        EMITTER_LIST list_of_emitters;
        FLT_LIST float_list;
        IFFMODESTR list_of_iffmodes[N_IFF_PARAMS];      /* smp 2/98 (E1907) */
        SONAR_LIST list_of_sonars;
        POSITION_LIST list_of_positions;
        } PARAM_VAL;

typedef struct param_value_d {
        PARAM_VALUE_TYPE val_type;
        ATTRIBUTE_QUALIFIER qual;
        PARAM_VAL val;
        } PARAM_VALUE;

typedef int PARAM_TYPE;

typedef struct mess_param_list_d {
        PARAM_TYPE param_type;  
        PARAM_VALUE param_data;
        struct mess_param_list_d *next;
        } MESS_PARAM_LIST;

typedef union message_subtype_d {
        AT_MESSAGE_TYPE at_type;
        TA_MESSAGE_TYPE ta_type;
        TT_MESSAGE_TYPE tt_type;
        } MESSAGE_SUBTYPE;

typedef struct message_type_d {
        MESSAGE_DATA_TYPE mess_type;
        MESSAGE_SUBTYPE mess_subtype;
        } MESSAGE_TYPE;

typedef struct mess_header_d {
        MESSAGE_TYPE alsp_mess_type;
        } MESS_HEADER;

typedef struct message_struct_d {
        MESS_HEADER header;
        MESS_PARAM_LIST *params;
        } MESSAGE_STRUCT;

/* A MESSAGE_STRUCT is a format-independent data structure that contains
 * an ALSP message.  Incoming messages are parsed into MESSAGE_STRUCTs and
 * outgoing messages are translated from MESSAGE_STRUCTs.
 */

/* The following extern declarations define an interface for manipulating
 * MESSAGE_STRUCTs.  It is intended that all access to MESSAGE_STRUCTs
 * be made via this interface.  This is especially critical for ALSP
 * translator code that is written in a language other than C.
 */

extern MESSAGE_STRUCT *new_message (); 
        /* m = new_message (mtype, msubtype);    */

extern MESSAGE_STRUCT *copy_message (); 
        /* m = copy_message (orig);    */

extern void free_message ();
        /* free_message (m);              */

extern void free_param_space ();
        /* free_param_space (par) */

extern MESS_PARAM_LIST *new_param (); 
        /* new_param (ptype, mess);          */

extern MESS_PARAM_LIST *next_param (); 
        /* list = next_param (list)       */

extern MESSAGE_DATA_TYPE get_message_type ();
        /* i = get_message_type (mess);       */

extern int get_message_subtype ();
        /* i = get_message_subtype (mess);    */

extern MESS_PARAM_LIST *get_message_parameters ();
        /* p = get_message_parameters (mess);    */

extern int get_parameter_type ();
        /* ptype = get_parameter_type (par); */

extern int get_par_int_value ();
        /* i = get_par_int_value (par);    */

extern ATTRIBUTE_QUALIFIER get_par_attr_qual ();
        /* q = get_par_attr_qual (par); */

extern int get_par_enum_value ();
        /* e = get_par_enum_value (par);    */

extern char *get_par_enum_string ();
        /* s = get_par_enum_string (par);    */

extern float get_par_float_value ();  
        /* f = get_par_float_value (par);  */

extern char *get_par_string_value (); 
        /* s = get_par_string_value (par); */

extern void get_par_loc_value ();
        /* get_par_loc_value (par, &lat, &lon); */

extern MESSAGE_STRUCT *get_par_mess_value (); 
        /* m = get_par_mess_value (par);    */

extern void get_par_int_range ();
        /* get_par_int_range (par, &lower, &upper); */

extern void get_par_float_range ();
        /* get_par_float_range (par, &lower, &upper); */

extern void get_par_int_list ();
        /* get_par_int_list (par, intlist, &nvals); */

extern void get_par_float_list ();
        /* get_par_float_list (par, float_list, nvals); */

extern void get_par_enum_list ();
        /* get_par_enum_list (par, enumlist, &nvals); */

extern void get_par_loc_list ();
        /* get_par_loc_list (par, loclist, &nvals); */

extern void get_par_attr_list (); 
        /* get_par_attr_list (par, a);    */

extern void get_par_emitter_list();
        /* get_par_emitter_list(par, list, nvals); */

extern void get_par_sonar_list();
        /* get_par_sonar_list(par, list, nvals); */

extern void get_par_position_list();
        /* get_par_position_list(par, list, nvals); */

extern void get_par_iffmode_list();   /* smp 2/98 (E1907) */
        /* get_par_iffmode_list(par, listIFF); */

extern void set_par_attr_qual ();
        /* set_par_int_value (par, q);   */

extern void set_par_int_value ();
        /* set_par_int_value (par, i);   */

extern void set_par_enum_value ();
        /* set_par_enum_value (par, e);   */

extern void set_par_enum_string ();
        /* set_par_enum_string (par, s);   */
        
extern void set_par_float_value ();
        /* set_par_float_value (par, f); */

extern void set_par_string_value ();    
        /* set_par_float_value (par, s); */

extern void set_par_mess_value ();      
        /* set_par_mess_value (par, m);  */

extern void set_par_loc_value ();      
        /* set_par_loc_value (par, lat, lon);  */

extern void set_par_int_range ();
        /* set_par_int_range (par, lower, upper); */

extern void set_par_float_range ();
        /* set_par_float_range (par, lower, upper); */

extern void set_par_int_list ();
        /* set_par_int_list (par, intlist, nvals); */

extern void set_par_float_list ();
        /* set_par_float_list (par, float_list, nvals); */

extern void set_par_enum_list ();
        /* set_par_enum_list (par, enumlist, nvals); */
        
extern void set_par_loc_list ();
        /* set_par_loc_list (par, loclist, nvals); */

extern void set_par_attr_list ();      
        /* set_par_attr_list (par, a);  */

extern MESS_PARAM_LIST *lookup ();  
        /* par = lookup (param_type, mess_ptr); */

#ifdef DEBUG
    extern void dump_mess_struct ();
#endif

extern void set_par_emitter_list();
        /* set_par_emitter_list (par, list, nvals); */

extern void set_par_sonar_list();
        /* set_par_sonar_list (par, list, nvals); */

extern void set_par_position_list();
        /* set_par_position_list (par, list, nvals); */

extern void set_par_iffmode_list();       /* smp 2/98 (E1907) */
        /* set_par_iffmode_list (par, listIFF); */
