#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_types.h"
#include "ascii_tables.h"
#include "mess_struct.h"

/* The following tables are used to convert values of enumerated types to
 * strings.  They are indexed using a variable of an enumerated type and
 * therefore, must be maintained such that they match the definition of
 * the appropriate enumerated typedef.  
 */

/* All tables and corresponding enumerated type definitions
 * contain a dummy value as the first entry.  This is because
 * the ALSP utilities will frequently be called from Fortran
 * and we wish to avoid making the caller perform adjustments
 * from C 0-based arrays to Fortran 1-based arrays when
 * using our enumerated values as indices on the Fortran
 * side.
 */


/* See typedef MESSAGE_DATA_TYPE in mess_struct.h */
KEYSTRING_TABLE mess_type_keytab = {
        "dummy",
        "ACM_ACTOR",
        "ACTOR_ACM",
        "ACTOR_ACTOR"
        };

/* See typedef TA_MESSAGE_TYPE in mess_struct.h */
KEYSTRING_TABLE ta_mess_keytab = {
        "dummy",
        "ADVANCE_REQUEST",
        "CANCEL_LOCK",
        "CREATE_REQUEST",
        "DELETE",
        "DISPATCH",
        "EVENT",
        "EVENT_REQUEST",
        "FILTER_ATTR",
        "FILTER_CLASS",
        "ID_REQUEST",
        "JOIN",
        "LOCK_REQUEST",
        "REGISTER_ATTRS",
        "REGISTER_OBJECT",
        "RESIGN",
        "UNLOCK",
        "SAVE_REQUEST",
        "SAVE_ACK",
        "FILTER_INTERACTION",    /* clk 12/93 */
        "PING",                  /* clk  1/94 */
        "PING_ACK",              /* clk  1/94 */
        "SAVE_COMPLETE",         /* clk 9/95 (E1458) */
        "FILTER_PARAMETERS",     /* SMM 9/96 (E1691) */
        "ERROR",                 /* SMM 9/96 (E1692) */
        "REGISTER_ENUMERATION",  /* SMM 10/96 (E1694) */
        "SIGNAL",                /* SMM 10/96 (E1694) */
        "REGISTER_RATE"          /* clk  9/97 (E1892) */
        };

/* See typedef AT_MESSAGE_TYPE in mess_struct.h */
KEYSTRING_TABLE at_mess_keytab = {
        "dummy",
        "AVAILABLE",
        "CREATE",
        "DELETE",
        "GRANT_ADVANCE",
        "INTERACTION",
        "JOIN_ACK",
        "LOCK_NOTIFICATION",
        "NEW_IDS",
        "REFRESH_REQUEST",
        "UNAVAILABLE",
        "UNLOCK_REQUEST",
        "UPDATE",
        "START_SAVE",
        "SET_RATE",
        "PING",                  /* clk  1/94 */
        "PING_ACK",              /* clk  1/94 */
        "SIGNAL"                 /* clk - 9/95 (E1457) */
        };

/* See typedef TT_MESSAGE_TYPE in mess_struct.h */
KEYSTRING_TABLE tt_mess_keytab = {
        "dummy",
        "INTERACTION",
        "REFRESH_REQUEST",
        "UPDATE"
        };

/* See typedef ATTRIBUTE_QUALIFIER in mess_struct.h */
KEYSTRING_TABLE attr_qual_keytab = {
        "dummy",
        "NONE",
        "@DR",
        "@ALL"       /* khc,3/93- Qualifier string of emitter. */
        };

/* See typedef TA_ADVANCE_REQUEST_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_adv_req_par_keytab = {
        "dummy",
        ""
        };
        
/* See typedef TA_CANCEL_LOCK_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_cancel_lock_par_keytab = {
        "dummy",
        "ID",
        "ATTRS"
        };

/* See typedef TA_CREATE_REQUEST_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_create_req_par_keytab = {
        "dummy",
        ""
        };

/* See typedef TA_DELETE_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_delete_par_keytab = {
        "dummy",
        ""
        };

/* See typedef TA_DISPATCH_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_dispatch_par_keytab = {
        "dummy",
        "",
        };

/* See typedef TA_EVENT_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_event_par_keytab = {
        "dummy",
        ""
        };

/* See typedef TA_FILTER_ATTR_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_fltr_attr_par_keytab = {
        "dummy",
        "CLASS",
        "ID",
        "LAT",
        "LONG",
        "ALTITUDE",
        "HEADING",
        "NAME",
        "PERCENT",
        "SIDE",
        "SIZE",
        "SPEED",
        "STATUS",
        "UNIT_TYPE"
        };

/* See typedef TT_CLASS_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_class_pval_keytab = {
        "dummy",
        "AIR.FIXEDWING",
        "AIR.HELICOPTER",
        "AIR.HELICOPTER.AIRLIFT",
        "AIR.CRUISE_MISSILE",
        "AIR.TBM",                /* clk 12/93 */
        "available",
        "GROUND.BASE",
        "GROUND.MANEUVER",
        "GROUND.MANEUVER.HIMAD",
        "GROUND.MANEUVER.SHORAD",
        "GROUND.MANEUVER.ALLRAD",
        "GROUND.MANEUVER.COMBAT",
        "GROUND.MANEUVER.SUPPORT",
        "GROUND.SUPPORT.CONVOY",
        "GROUND.SUPPORT.UNIT",
        "GROUND.SUPPORT.UNIT.TCU",
        "GROUND.MANEUVER.RADAR",
        "GROUND.MANEUVER.TEL",
        "GROUND.FACILITY",
        "SEA.SUBSURFACE",           /* clk 8/20/93 */
        "SEA.TORPEDO",
        "SEA.COUNTERMEASURE",
        "SEA.SONOBUOY",
        "SEA.MINE", 
        "SEA.SWEEPAREA", 
        "SEA.SURFACE.BOAT",         /* clk 8/20/93 */
        "SEA.SURFACE.SHIP",         /* clk 8/20/93 */
        "SEA.SURFACE.AAV",          /* clk 10/95 (E1483) */
        "SEA.SURFACE.LANDING_CRAFT" /* clk 10/95 (E1483) */
        };

/* See typedef TA_FILTER_CLASS_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_fltr_class_par_keytab = {
        "dummy",
        "CLASS",
        "CONDITION",
        "SEND_DR"           /* C. Kropp (NRaD)  4/26/93 */
        };

/* See typedef TA_FILTER_CLASS_COND_PVALS in mess_struct.h */
KEYSTRING_TABLE ta_fltr_cond_pval_keytab = {
        "dummy",
        "ALLOW",
        "DISALLOW"
        };

/* See typedef TA_FILTER_CLASS_SEND_DR_PVALS in mess_struct.h */
/* C. Kropp (NRaD)  4/26/93 */
KEYSTRING_TABLE ta_fltr_send_dr_pval_keytab = {
        "dummy",
        "TRUE",
        "FALSE"
        };

/* See typedef TA_FILTER_INTERACTION_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_fltr_interaction_par_keytab = {    /* clk 12/93 */
        "dummy",
        "KIND",
        "CONDITION"
        };

/* See typedef TA_FILTER_PARAMETERS_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_filter_parameters_par_keytab = {   /* SMM 9/96 (E1691)*/
        "dummy",
        "KIND",
        "TO",
        "FROM"
        };

/* See typedef TA_FLTR_PARAM_RESTRCTIONS_PVALS in mess_struct.h */
KEYSTRING_TABLE ta_fltr_restrctions_pval_keytab = {   /* SMM 9/96 (E1691) */
        "dummy",
        "KNOWN",
        "OWNED",
        "GHOSTED"
        };

/* See typedef TA_ERROR_PARAMS in mess_struct.h */   /* SMM 9/96 (E1692) */
KEYSTRING_TABLE ta_error_par_keytab = {
        "dummy",
        ""
        };

/* See typedef TA_ID_REQ_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_id_req_par_keytab = {
        "dummy",
        ""
        };

/* See typedef TA_JOIN_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_join_par_keytab = {
        "dummy",
        "LOOK_AHEAD",
        "ACTOR_NAME"
        };

/* See typedef TA_JOIN_TIME_MODE_PVALS in mess_struct.h */
KEYSTRING_TABLE ta_join_tm_pval_keytab = {
        "dummy",
        "TRUE",
        "FALSE"
        };

/* See typedef TA_LOCK_REQ_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_lock_req_par_keytab = {
        "dummy",
        "ID",
        "ATTRS",
        "SERVICE",
        "NOTATION",
        "COMMENT"
        };

/* See typedef TA_LOCK_REQ_SERV_PVALS in mess_struct.h */
KEYSTRING_TABLE ta_lock_serv_pval_keytab = {
        "dummy",
        "IF_AVAILABLE",
        "WHEN_AVAILABLE"
        };

/* See typedef TA_REG_ATTR_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_reg_attr_par_keytab = {
        "dummy",
        "CLASS",
        "CREATE",
        "INTEREST",
        "UPDATE"
        };

/* See typedef TA_REG_OBJECT_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_reg_object_par_keytab = {
        "dummy",
        "ID",
        "CLASS",
        "UNLOCKED_ATTRS"
        };

/* SMM 10/96 (E1694)                               */
/* See typedef TA_REG_ENUM_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_reg_enum_par_keytab = {
        "dummy",
        "CLASS",
        "KIND",
        "ATTRIBUTE",
        "UNIT_TYPE",
        "EMITTER",
        "RADAR",
        "JAMMER",
        "TYPE",
        "WEAPON",
        "ALSP_VALUE",
        "ACTOR_VALUE",
        "DEFAULT"
        };

/* See typedef TA_REG_RATE_PARAMS in mess_struct.h  -  clk 9/97 (E1892) */
KEYSTRING_TABLE ta_reg_rate_par_keytab = {
        "dummy",
        ""
        };

/* See typedef TT_RESIGN_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_resign_pval_keytab = {
        "",                          /* clk 1/94 - for the new parameters of */
        "DELETE_OWNED",              /*            the resign message        */
        "DISPERSE_OWNED"
        };

/* See typedef TA_RESIGN_PARAMS in mess_struct.h */
 KEYSTRING_TABLE ta_resign_par_keytab = {
        "dummy",                     /* clk 1/94 - for the new parameters of */
        "" };                        /*            the resign message        */

/* See typedef TA_UNLOCK_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_unlock_par_keytab = {
        "dummy",
        "ID",
        "ATTRS"
        };

/* See typedef TA_SAVE_REQ_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_save_req_par_keytab = {
        "dummy",
        "TIME",
        "LABEL"
        };

/* See typedef TA_SAVE_ACK_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_save_ack_par_keytab = {
        "dummy",
        "TIME",
        "CLOCK"
        };

/* See typedef TA_SAVE_COMPLETE_PARAMS in mess_struct.h */
KEYSTRING_TABLE ta_save_complete_par_keytab = {
        "dummy",
        "TIME",
        "CLOCK"
        };

/* See typedef AT_AVAIL_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_avail_par_keytab = {
        "dummy",
        "ID",
        "ATTRS"
        };

/* See typedef TT_POSTURE_PVALS mess_struct.h */
KEYSTRING_TABLE tt_posture_pval_keytab = {
        "dummy",
        "ATTACK",
        "DEFEND",
        "WITHDRAW",
        "MOVING",
        "MOVE_TO_CONTACT",
        "INCAPABLE",
        "WIPED_OUT"
        };

/* See typedef TT_ORIENT_PVALS mess_struct.h */
KEYSTRING_TABLE tt_orient_pval_keytab = {
        "dummy",
        "TEMP"
        };


/* See typedef TT_UNIT_TYPE_PVALS mess_struct.h */

#ifdef JTTI
#include "enum_jtti.h"
#endif

#ifdef JLCCTC
#include "enum_jlcctc.h"
#endif

/* See typedef TT_UNIT_SIZE_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_unit_size_keytab = {
        "dummy",
        "ARMY",
        "BASE",
        "BRIGADE",
        "BATTALION",
        "BATTERY",
        "COMPANY",
        "CORPS",
        "DETACHMENT",
        "DIVISION",
        "HEADQUARTERS",
        "PLATOON",
        "REGIMENT",
        "SQUAD",
        "SQUADRON",
        "TEAM",
        "TASKFORCE",
        "TROOP",
        "WING",
        "SECTION",
        "ARMY_GROUP"
        };

/* See typedef TT_UNIT_FLIGHT_OPS_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_unit_flight_ops_keytab = {
        "dummy",
        "OPEN",
        "CLOSED"
        };

/* See typedef TT_SIDE_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_side_keytab = {
        "dummy",
        "RED",
        "BLUE",
        "NEUTRAL",
        "UNKNOWN"
        };

/* See typedef TT_AWSIMS_STAT_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_aws_stat_pval_keytab = {
        "dummy",
        "OPERATIONAL",
        "SUPPRESSED"
        };

/* See typedef TT_CBS_STAT_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_cbs_stat_pval_keytab = {
        "dummy",
        "OPERATIONAL",
        "MOVING",
        "SUPPRESSED",
        "DESTROYED"
        };

/* See typedef TT_UPDATE_STATUS_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_up_stat_keytab = {
        "dummy",
        "AVAILABLE",
        "COMBAT",
        "MOVE",
        "DROP",
        "FLY",
        "LIFT",
        "ON_GROUND",    /* was previously one word - 2/3/93, 12/14 - underscore */
        "ORBIT",
        "PROCEED",
        "READY",
        "DELAYED",
        "UNDERWAY",    /* clk 12/96 (E1724) */
        "INPORT",      /* joa 11/05 PR Number:606 */
        "SINKING",      /* clk 12/96 (E1724) */
        "COMPLETE",
        "INPROGRESS"
        };

/* See typedef AT_DELETE_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_delete_par_keytab = {
        "dummy",
        ""
        };

/* See typedef AT_GRANT_ADV_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_grant_adv_par_keytab = {
        "dummy",
        ""
        };

/* See typedef AT_JOIN_ACK_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_join_ack_par_keytab = {
        "dummy",
        ""
        };

/* See typedef AT_PING_PARAMS in mess_struct.h */    /* clk  1/94 */
KEYSTRING_TABLE ta_ping_par_keytab = {
        "dummy",
        ""
        };

/* See typedef AT_PING_ACK_PARAMS  mess_struct.h */  /* clk  1/94 */
KEYSTRING_TABLE ta_ping_ack_par_keytab = {
        "dummy",
        ""
        };

/* See typedef AT_LOCK_NOTIFY_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_lock_not_par_keytab = {
        "dummy",
        "ID",
        "ATTRS",
        "STATUS",
        "NOTATION",
        "COMMENT"
        };

/* See typedef AT_LOCK_NOT_STAT_PVALS in mess_struct.h */
KEYSTRING_TABLE at_lock_stat_pval_keytab = {
        "dummy",
        "SECURED",
        "REJECTED",
        "ACQUIRING"
        };

/* See typedef AT_NEW_ID_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_new_id_par_keytab = {
        "dummy",
        ""
        };

/* See typedef AT_UNAVAIL_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_unavail_par_keytab = {
        "dummy",
        "ID",
        "ATTRS"
        };

/* See typedef AT_UNLOCK_REQ_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_unlock_par_keytab = {
        "dummy",
        "ID",
        "ATTRS",
        "NOTATION"
        };

/* See typedef AT_START_SAVE_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_start_save_par_keytab = {
        "dummy",
        "TIME",
        "LABEL"
        };

/* See typedef AT_SET_RATE_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_set_rate_par_keytab = {
        "dummy",
        "",
        };

/* See typedef AT_SIGNAL_PARAMS in mess_struct.h */
KEYSTRING_TABLE at_signal_par_keytab = {
        "dummy",
        ""          /* SIGNAL TYPE has no parameter/attribute wrapper */
        };

/* See typedef AT_SIGNAL_PVALS in mess_struct.h */
KEYSTRING_TABLE at_signal_keytab = {
        "dummy",
        "REGISTER_OBJECTS",
        "REGISTER_ENUMERATIONS"    /* SMM 10/96 (E1694) */
        };

/* See typedef TA_SIGNAL_PARAMS in mess_struct.h - SMM 12/96 (E1694)*/
KEYSTRING_TABLE ta_signal_par_keytab = {
        "dummy",
        ""         
        };

/* See typedef TA_SIGNAL_PVALS in mess_struct.h - SMM 12/96 (E1694)*/
KEYSTRING_TABLE ta_signal_keytab = {
        "dummy",
        "ENUMERATIONS_COMPLETE"
        };

/* See typedef TT_INTERACTION_PARAMS in mess_struct.h */
KEYSTRING_TABLE tt_interaction_par_keytab = {
        "dummy",
        "FROM",
        "TO",
        "KIND",
        "AMOUNT",
        "DURATION",
        "PERCENT",
        "KILLS",
        "LAT",
        "LONG",
        "FIRER_LAT",
        "FIRER_LONG",
        "MISSION",
        "RADIUS",
        "NUMBER",
        "WEAPON",
        "SIZE",
        "TARGETS",
        "REPORT",
        "NUMBER",
        "TARGETS_ENGAGED",
        "HITS",
        "TYPE",     /* (SUPPLY_POL(TYPE()NUMBER())) - 9/95 SMM */
        "NUMBER",
        "REASON",
        "TARGET_REF",  /* for expanded target reference - clk 12/97 (E1911) */
        "BEN",         /* for modified target_ref as wrapper with sub-attributes */
        "BEN_SUFFIX",  /* (target_ref is used for parsing the incoming messages) */
        "TIN",         /* joa 3/09 change ttn to tin - jhm 2/98 (E1911) */
        "PIN",
        "LOC",
        "CORRELATION_ID"
        };

/* See typedef TT_INT_FIRE_KIND_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_int_kind_pval_keytab = {
        "dummy",
        "REPORT",
        "MINES",
        "ENGAGEMENT.AIR_TO_AIR",
        "ENGAGEMENT.AIR_TO_GROUND",
        "ENGAGEMENT.AIR_TO_SHIP",
        "ENGAGEMENT.SHIP_TO_AIR",
        "ENGAGEMENT.SHIP_TO_GROUND",
        "ENGAGEMENT.SHIP_TO_SHIP",
        "ENGAGEMENT.GROUND_TO_AIR",
        "ENGAGEMENT.GROUND_TO_GROUND",
        "ENGAGEMENT.GROUND_TO_SHIP",
        "REPORT.ATTRITION.AIR_TO_AIR",
        "REPORT.ATTRITION.AIR_TO_GROUND",
        "REPORT.ATTRITION.AIR_TO_SHIP",
        "REPORT.ATTRITION.SHIP_TO_AIR",
        "REPORT.ATTRITION.SHIP_TO_GROUND",
        "REPORT.ATTRITION.SHIP_TO_SHIP",
        "REPORT.ATTRITION.GROUND_TO_AIR",
        "REPORT.ATTRITION.GROUND_TO_GROUND",
        "REPORT.ATTRITION.GROUND_TO_SHIP",
        "AIR.SUPPLY.SUPPORTING_UNITS.REQUEST",
        "AIR.SUPPLY.POL.REQUEST",
        "AIR.SUPPLY.SUPPORTING_UNITS.ISSUE",
        "AIR.SUPPLY.POL.ISSUE"
        };

/* See typedef TT_INT_MISSION_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_int_miss_pval_keytab = {
        "dummy",
        "AD",
        "AEW",
        "AF_STRIKE",
        "AI",
        "AIRLIFT",
        "AIRTANKER",
        "AMPHIB",
        "ASW",
        "BAI",
        "CAP",              /* 10 */
        "CARGO",
        "CAS",
        "COMMERCE",
        "CSAR",                         /* clk 1/97 (E1730) */
        "DCA",
        "ESCORT",
        "FACA",                         /* clk 1/97 (E1730) */
        "GENERIC_MISSION",
        "JAMMER",
        "MAROPS",           /* 20 */
        "NAVY_STRIKE",   
        "NONE",
        "OCA",
        "RECON",                        /* 6-17-93 RECCON => RECON */
        "RELAY",
        "SAR",
        "SEAD",
        "SEA_CONTROL",
        "SEALIFT",                      /* clk 12/96 (E1724) */
        "SPEC_MISS",        /* 30 */
        "STRIKECAP",
        "STTANKER",
        "SURCAP",          
        "SURVEILLANCE",
        "TRANSFER"          /* 35 */    /* clk 1/97 (E1730) */
        };

/* See typedef TT_SALVO_WTYPE_PVALS in mess_struct.h */

/* See typedef TT_SALVO_WSIZE_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_salvo_wsize_pval_keytab = {
        "dummy",
        "105MM",
        "122MM",
        "152MM",
        "175MM",
        "203MM",
        "2S1",
        "2S3",
        "2S4",
        "2S5",
        "2S7",
        "2S9",
        "8IN",
        "BM21",
        "BM22",
        "BM27",
        "D20",
        "D30",
        "FROG",
        "LANCE",
        "MISSILE",
        "MLRS",
        "SCUD",
        "SS1",
        "SS12",
        "SS21"
        };


/* See typedef TT_SUPPLY_TYPE_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_supply_type_pval_keytab = {
        "dummy",
        "JP4"
        };


/* See typedef TT_SUPPLY_REASON_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_supply_reason_pval_keytab = {
        "dummy",
        "NONE_AVAILABLE",
        "SUPPLIER_NOT_KNOWN",
        "RECEIVER_NOT_KNOWN"
        };


/* See typedef TT_INT_TARGET_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_int_targ_pval_keytab = {
        "dummy",
        "ACQ_RADAR",
        "AIR_DEFENSE",
        "AIRCRAFT",
        "ANTI_TANK",
        "ARMOR",
        "ARTILLERY",
        "CIWS",
        "COMM",
        "DISMOUNTED",
        "ENG_BRIDGE",
        "ENGINEER",
        "FIXED_BRIDGE",
        "FLIGHTLINE",
        "FUEL",
        "GENERIC",
        "LIGHT_ARMOR",
        "MAINTENANCE",
        "MISCELLANEOUS",
        "PARKED_AIRCRAFT",
        "PERSONNEL",
        "RADAR",
        "RADARS",
        "RIP",
        "SHELTERS",
        "STORES",
        "SUPPLY_CENTER",
        "TRUCKS_VANS",
        "WEAPONS"
        };

/* changed NAMES to TYPE per JECEWSI ICD, SMM 9/94 */
/* See typedef TT_EMITTER_TYPE_PVALS in mess_struct.h */   /* KHC,2/93- Emitter name keystring */
/* See typedef TT_EMITTER_STATUS_PVALS in mess_struct.h */  /* KHC, 2/93- Emitter status keystring */
KEYSTRING_TABLE tt_emitstatus_pval_keytab = {
        "dummy",
        "OFF",
        "ON"
        };

/* See typedef TT_SONAR_TYPE_PVALS in mess_struct.h */

/* See typedef TT_SONAR_STATUS_PVALS in mess_struct.h */ 
KEYSTRING_TABLE tt_sonarstatus_pval_keytab = {
        "dummy",
        "OFF",
        "ON"
        };

/* See typedef TT_SONAR_MODE_PVALS in mess_struct.h */ 
KEYSTRING_TABLE tt_sonarmode_pval_keytab = {
        "dummy",
        "DP",
        "CZ",
        "BB"
        };

/* See typedef TT_MODE_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_emitmode_pval_keytab = {
        "dummy",
        "NO_MODE",
        "NET1",
        "NET2",
        "NET3",
        "NET4",
        "NET5",
        "NET6",
        "NET7",
        "NET8",
        "NET9",
        "ALL"
        };

/* See typedef TT_REFRESH_PARAMS in mess_struct.h */
KEYSTRING_TABLE tt_refresh_par_keytab = {
        "dummy",
        "ID",
        "CLASS",
        "ATTRS"
        };

/* See typedef TT_ATTRIBUTE in mess_struct.h */
KEYSTRING_TABLE tt_attribute_keytab = {
        "dummy",
        "ID",
        "CLASS",
        "LAT",
        "LONG",
        "ALTITUDE",
        "ARTY_SUPP",
        "AWSIM_STATUS",
        "CBS_STATUS",
        "ELECT_DEG",
        "FIRE_CONTROL",
        "HEADING",
        "NAME",
        "OLD_ID",
        "ORBIT_POINTS",
        "ORBIT_POINTS",
        "ORBIT_POINTS",
        "ORBIT_POINTS",
        "PERCENT",
        "SIDE",
        "SIZE",
        "SPEED",
        "STATUS",
        "SUPPLY_LAUNCHER",
        "SUPPLY_MISSILE",
        "SUPPLY_RADAR",
        "TARGET_LOC",
        "TARGET_LOC",
        "UNIT_TYPE",
        "MISSION",
        "POSTURE",
        "ORIENTATION",
        "SIM_TIME",
        "CALL_SIGN",
        "AGE",              /* KHC,3/93- New parameter keystring of an UPDATE message. */  
        "DEPTH",            /* clk 8/93 - submarine depth */
        "FLIGHT_OPS",       /* clk 8/95 (E1455) */
        "EMITTER",
        "RADAR",  /* These keystrings should only appear inside */
        "JAMMER", /* EMITTER() parameter of an UPDATE or CREATE message. */
        "INDEX",   /* Added 9/94 SMM */
        "ORIENT",  /* Added 9/94 SMM */
        "MODE",    /* Added 9/94 SMM */
        "TYPE",    /* Added 9/94 SMM */
        "EMITTER_STATUS",  /* not a true keyword, just a slot holder */
        "C2W_FACTORS",     /* This keyword is a wrapper for JECEWSI ICD */
        "UNIT",            /* This keyword is within C2W_FACTORS, but is also a wrapper */
        "EMTR",            /* This keyword is within C2W_FACTORS, but is also a wrapper */
        "EW_C3",           /* within UNIT or EMTR wrappers */
        "EW_ACQ",          /* within UNIT or EMTR wrappers */
        "EW_LAUNCH",       /* within UNIT or EMTR wrappers */
        "EW_GUIDE",        /* within UNIT or EMTR wrappers */
        "EW_POWER",        /* within UNIT or EMTR wrappers */
        "EW_SURF",         /* within UNIT or EMTR wrappers - SMM 10/95 E1486 */
        "WEAPON",          /* register_enumeration message - SMM 1/97 E1694 */
        "IFF",             /* IFF wrapper attribute - smp 2/98 (E1907) */
        "MASTER",          /* IFF MASTER on/off - smp 2/98 (E1907) */
        "MODE1",           /* IFF MODE1 wrapper - smp 2/98 (E1907) */
        "MODE2",           /* IFF MODE2 wrapper - smp 2/98 (E1907) */
        "MODE3",           /* IFF MODE3 wrapper - smp 2/98 (E1907) */
        "MODEC",           /* IFF MODEC wrapper - smp 2/98 (E1907) */
        "MODE4",           /* MODE4 attribute   - smp 2/98 (E1907) */
        "STATUS",          /* the mode STATUS   - smp 2/98 (E1907) */
        "MODE",             /* the mode code #   - smp 2/98 (E1907) */
        "PROPULSION_MODE",  /* the sub propulsion */
        "SONAR",            /* the sonar wrapper attribute */
        "TYPE",             /* sonar attribute */
        "STATUS",           /* sonar attribute */
        "MODE",             /* sonar mode attribute */
        "DEPTH",            /* sonar attribute */
        "MINE_LOCATION",    /* MINE_LOCATION attribute */
        "SWEEP_LOCATION",   /* SWEEPAREA_LOCATION attribute */
        "SWEEP_STATUS",     /* SWEEPAREA attribute*/
        "ATO_ID",           /* ATOID attribute*/
        "STRENGTH"          /* STRENGTH attribute*/
        };

/* See typedef TT_IFFMODE_STATUS_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_iffmode_status_pval_keytab = {   /* smp 2/98 (E1907) */
        "DUMMY",
        "OFF",
        "ON"
        };

/* See typedef TT_PROPULSION_MODE_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_propulsion_mode_pval_keytab = { 
        "DUMMY",
        "DIESEL",
        "ELECTRIC",
        "NUCLEAR"
        };

/* See typedef TT_SWEEPAREA_STATUS_PVALS in mess_struct.h */
KEYSTRING_TABLE tt_sweeparea_status_pval_keytab = {  
        "DUMMY",
        "PROGRESS",
        "COMPLETE"
        };

typedef struct enum_descriptor_d {
        int max;
        char **tab;
        } ENUM_DESCRIPTOR;

typedef ENUM_DESCRIPTOR ENUM_DESC_TAB [];

/* Num entries, ptr to enum string value table */
static ENUM_DESC_TAB ta_fltr_attr_pval_enums = {
                {0, NULL},
                {N_CLASS_VALS, tt_class_pval_keytab},
                {0, NULL},                      /* id */
                {0, NULL},                      /* lat */
                {0, NULL},                      /* long */
                {0, NULL},                      /* alt */
                {0, NULL},                      /* heading */
                {0, NULL},                      /* name */
                {0, NULL},                      /* percent */
                {N_SIDE_VALS, tt_side_keytab},  
                {N_UNIT_SIZE_VALS, tt_unit_size_keytab},  
                {0, NULL},           
                {N_UPDATE_STAT_VALS, tt_up_stat_keytab},           
                {N_UTYPE_VALS, tt_utype_pval_keytab},        /* unit type */
                {0, NULL}           
                };

static ENUM_DESC_TAB ta_fltr_class_pval_enums = {
                {0, NULL},
                {N_CLASS_VALS, tt_class_pval_keytab},
                {N_FILTER_COND_VALS, ta_fltr_cond_pval_keytab},
                {N_FILTER_SEND_DR_VALS, ta_fltr_send_dr_pval_keytab}            /* C. Kropp (NRaD) 4/26/93 */
                };

static ENUM_DESC_TAB ta_fltr_interaction_pval_enums = {
                {0, NULL},                                     /* clk 12/93 */
                {N_INT_KIND_VALS, tt_int_kind_pval_keytab},    /* kind */
                {N_FILTER_COND_VALS, ta_fltr_cond_pval_keytab} /* condition */
                };

static ENUM_DESC_TAB ta_join_pval_enums = {
                {0, NULL},
                {0, NULL},
                {0, NULL},
                {0, NULL},
                {0, NULL},
                {0, NULL},
                {N_TIME_MODE_VALS, ta_join_tm_pval_keytab},
                {N_TIME_MODE_VALS, ta_join_tm_pval_keytab}
                };

static ENUM_DESC_TAB ta_fltr_parameters_pval_enums = {   /* SMM 9/96 (E1691) */
        {0, NULL},                                              
        {N_INT_KIND_VALS, tt_int_kind_pval_keytab},                  /* kind */
        {N_FLTR_RESTRICTIONS_VALS, ta_fltr_restrctions_pval_keytab}, /* to */
        {N_FLTR_RESTRICTIONS_VALS, ta_fltr_restrctions_pval_keytab}  /* from */
        };

static ENUM_DESC_TAB ta_reg_enum_pval_enums = {  /* SMM 10/96 (E1694) */
        {0, NULL},                                              
        {N_CLASS_VALS, tt_class_pval_keytab},            /* CLASS */
        {N_INT_KIND_VALS, tt_int_kind_pval_keytab},      /* KIND */
        {N_ATTRIBUTE_VALS, tt_attribute_keytab},         /* ATTRIBUTE */
        {N_UTYPE_VALS, tt_utype_pval_keytab},            /* UNIT_TYPE */
        {0, NULL},                                       /* EMITTER */
        {N_ATTRIBUTE_VALS, tt_attribute_keytab},         /* RADAR */
        {N_ATTRIBUTE_VALS, tt_attribute_keytab},         /* JAMMER */
        {N_EMITTER_TYPE_VALS, tt_emittype_pval_keytab},  /* TYPE */
        {N_SALVO_WTYPE_VALS, tt_salvo_wtype_pval_keytab},/* WEAPON */
        {0, NULL},                                       /* ALSP_VALUE */
        {0, NULL},                                       /* ACTOR_VALUE - text value */
        {0, NULL}                                        /* DEFAULT - text value */
        };

static ENUM_DESC_TAB ta_reg_alsp_val_pval_enums = {  /* SMM 10/96 (E1694) */
        {0, NULL},                                              
        {N_EMITTER_TYPE_VALS, tt_emittype_pval_keytab}
        };

static ENUM_DESC_TAB ta_lock_req_pval_enums = {
                {0, NULL},
                {0, NULL},
                {0, NULL},
                {N_LOCK_REQ_SERV_VALS, ta_lock_serv_pval_keytab},
                {0, NULL},
                {0, NULL}
                };

static ENUM_DESC_TAB ta_reg_attr_pval_enums = {
                {0, NULL},
                {N_CLASS_VALS, tt_class_pval_keytab},
                {0, NULL},
                {0, NULL}
                };

static ENUM_DESC_TAB ta_reg_obj_pval_enums = {
                {0, NULL},
                {0, NULL},
                {N_CLASS_VALS, tt_class_pval_keytab},
                {0, NULL}
                };

static ENUM_DESC_TAB ta_resign_pval_enums = {                /* clk 1/94 - for new resign */
                {0, NULL},                                   /*            msg parameters */
                {N_RESIGN_VALS, tt_resign_pval_keytab}
                };

static ENUM_DESC_TAB tt_update_pval_enums = {
                {0, NULL},
                {0, NULL},                                   /* id */
                {N_CLASS_VALS, tt_class_pval_keytab},        /* class */
                {0, NULL},                                   /* lat */
                {0, NULL},                                   /* long */
                {0, NULL},                                   /* alt */
                {0, NULL},                                   /* arty_supp */
                {N_AWSIM_STAT_VALS, tt_aws_stat_pval_keytab},/* awsims stat */
                {N_CBS_STAT_VALS, tt_cbs_stat_pval_keytab},  /* cbs stat */
                {0, NULL},                                   /* elect deg */
                {0, NULL},                                   /* fire cont */
                {0, NULL},                                   /* heading */
                {0, NULL},                                   /* name */
                {0, NULL},                                   /* old id */
                {0, NULL},                                   /* orbit */
                {0, NULL},                                   /* orbit */
                {0, NULL},                                   /* orbit */
                {0, NULL},                                   /* orbit */
                {0, NULL},                                   /* percent */
                {N_UNIT_SIDE_VALS, tt_side_keytab},          /* side */
                {N_UNIT_SIZE_VALS, tt_unit_size_keytab},     /* size */
                {0, NULL},                                   /* speed */
                {N_UPDATE_STAT_VALS, tt_up_stat_keytab},     /* status */
                {0, NULL},                                   /* sup l */
                {0, NULL},                                   /* sup m */
                {0, NULL},                                   /* sup r */
                {0, NULL},                                   /* target loc */
                {0, NULL},                                   /* target loc */
                {N_UTYPE_VALS, tt_utype_pval_keytab},        /* unit type */
                {N_INT_MISSION_VALS, tt_int_miss_pval_keytab}, /* mission */
                {N_POSTURE_VALS, tt_posture_pval_keytab},      /* posture */
                {0, NULL},       /* orientation */
                {0, NULL},       /* sim time */
                {0, NULL},       /* call sign */
                {0, NULL},       /* age */
                {0, NULL},       /* sub depth */
                {N_UNIT_FLIGHT_OPS_VALS, tt_unit_flight_ops_keytab},  /* flight_ops */
                {0, NULL},       /* emitter */
                {0, NULL},       /* radar */
                {0, NULL},       /* jammer */
                {0, NULL},       /* index */
                {0, NULL},       /* orient */
                {N_EMITTER_MODE_VALS, tt_emitmode_pval_keytab},     /* mode */
                {N_EMITTER_TYPE_VALS, tt_emittype_pval_keytab},     /* type  */
                {N_EMITTER_STATUS_VALS, tt_emitstatus_pval_keytab}, /* status */
                {0, NULL},       /* c2w_factors */
                {0, NULL},       /* unit */
                {0, NULL},       /* emtr */
                {0, NULL},       /* ew_c3 */
                {0, NULL},       /* ew_acq */
                {0, NULL},       /* ew_launch */
                {0, NULL},       /* ew_guide */
                {0, NULL},       /* ew_power */
                {0, NULL},       /* ew_surf */
                {0, NULL},       /* weapon */
                {0, NULL},       /* iff   */
                {N_IFFMODE_STATUS_VALS, tt_iffmode_status_pval_keytab},    /* master */
                {0, NULL},       /* mode1 */
                {0, NULL},       /* mode2 */
                {0, NULL},       /* mode3 */
                {0, NULL},       /* modec */
                {N_IFFMODE_STATUS_VALS, tt_iffmode_status_pval_keytab},/* mode4 */
                {N_IFFMODE_STATUS_VALS, tt_iffmode_status_pval_keytab},/* status */
                {0, NULL},        /* mode */
                {N_PROPULSION_MODE_VALS, tt_propulsion_mode_pval_keytab},
                {0, NULL},       /* sonar */
                {N_SONAR_TYPE_VALS, tt_sonartype_pval_keytab},     /* type  */
                {N_SONAR_STATUS_VALS, tt_sonarstatus_pval_keytab}, /* status */
                {N_SONAR_MODE_VALS, tt_sonarmode_pval_keytab},     /* mode */
                {0, NULL},                                       /* depth(ft) */
                {0, NULL},                                       /* position */
                {0, NULL},                             /* sweeparea_location */
                {N_SWEEPAREA_STATUS_VALS, tt_sweeparea_status_pval_keytab}
                };

static ENUM_DESC_TAB at_lock_not_pval_enums = {
                {0, NULL},
                {0, NULL},
                {0, NULL},
                {N_LOCK_STATUS_VALS, at_lock_stat_pval_keytab},
                {0, NULL},
                {0, NULL}
                };

static ENUM_DESC_TAB tt_interaction_pval_enums = {
                {0, NULL},
                {0, NULL},                                     /* from */
                {0, NULL},                                     /* to */ 
                {N_INT_KIND_VALS, tt_int_kind_pval_keytab},    /* kind */
                {0, NULL},                                     /* amount */
                {0, NULL},                                     /* duration */
                {0, NULL},                                     /* percent */
                {0, NULL},                                     /* kills */
                {0, NULL},                                     /* lat */
                {0, NULL},                                     /* long */
                {0, NULL},                                     /* firer_lat */
                {0, NULL},                                     /* firer_long */
                {N_INT_MISSION_VALS, tt_int_miss_pval_keytab}, /* mission */
                {0, NULL},                                     /* radius */
                {0, NULL},                                     /* salvo num */
                {N_SALVO_WTYPE_VALS, tt_salvo_wtype_pval_keytab},   /* weapon */
                {N_SALVO_WSIZE_VALS, tt_salvo_wsize_pval_keytab},   /* size */
                {N_INT_TARGET_VALS, tt_int_targ_pval_keytab},  /* target */
                {0, NULL},                                     /* report */
                {0, NULL},                                     /* number */
                {0, NULL},                                     /* targets_engaged */
                {0, NULL},                                     /* hits */
                {N_SUPPLY_TYPE_VALS, tt_supply_type_pval_keytab}, /* type */
                {0, NULL},                                     /* number */
                {N_SUPPLY_REASON_VALS, tt_supply_reason_pval_keytab}, /* reason */
                {0, NULL},                                     /* target_ref */
                {0, NULL},                                     /* ben */
                {0, NULL},                                     /* ben_suffix */
                {0, NULL},                                     /* ttn */
                {0, NULL},                                     /* pin */
                {0, NULL}                                      /* location */
                };

static ENUM_DESC_TAB at_signal_type_pval_enums = {    /* clk - 9/95 (E1457) */
                {0, NULL},             /* dummy */
                {N_SIGNAL_VALS, at_signal_keytab}
                };

static ENUM_DESC_TAB ta_signal_type_pval_enums = {    /* SMM - 12/96 (E1694) */
                {0, NULL},             /* dummy */
                {N_TA_SIGNAL_VALS, ta_signal_keytab}
                };

/* Given a message type, a message subtype, and a parameter type,
 * look up the specified parameter's ascii keyword value.
 */
char *get_param_keyword (mtype, msubtype, partype) 
        
        MESSAGE_DATA_TYPE mtype;
        int msubtype; 
        int partype;
{
        char *result;

        result = get_keyword (
                        ptable_lookup_by_mess_type (mtype,
                        msubtype), partype);

        return (result);
}

/* Given a message type and a message subtype, find the ascii
 * table which contains the list of keywords for the specified
 * message's parameter list.
 */
char **ptable_lookup_by_mess_type (mtype, msubtype)
        
        int mtype;
        int msubtype;
{
        char **result;

        static char **ta_par_tab [] =
            {NULL,                              /* dummy */
             ta_adv_req_par_keytab,             /* ADVANCE_REQUEST */
             ta_cancel_lock_par_keytab,         /* CANCEL_LOCK */
             ta_create_req_par_keytab,          /* CREATE_REQUEST */
             ta_delete_par_keytab,              /* DELETE */
             ta_dispatch_par_keytab,            /* DISPATCH */
             ta_event_par_keytab,               /* EVENT */
             NULL,                              /* EVENT_REQUEST */
             ta_fltr_attr_par_keytab,           /* FILTER_ATTR */
             ta_fltr_class_par_keytab,          /* FILTER_CLASS */
             ta_id_req_par_keytab,              /* ID_REQUEST */
             ta_join_par_keytab,                /* JOIN */
             ta_lock_req_par_keytab,            /* LOCK_REQUEST */
             ta_reg_attr_par_keytab,            /* REGISTER_ATTRS */
             ta_reg_object_par_keytab,          /* REGISTER_OBJECT */
             ta_resign_par_keytab,              /* RESIGN */
             ta_unlock_par_keytab,              /* UNLOCK */
             ta_save_req_par_keytab,            /* SAVE_REQUEST */
             ta_save_ack_par_keytab,            /* SAVE_ACK */
             ta_fltr_interaction_par_keytab,    /* FILTER_INTERACTION */
             ta_ping_par_keytab,                /* PING */
             ta_ping_ack_par_keytab,            /* PING_ACK */
             ta_save_complete_par_keytab,       /* SAVE_COMPLETE */
             ta_filter_parameters_par_keytab,   /* FILTER_PARAMETERS (E1691) */ 
             ta_error_par_keytab,               /* ERROR (E1692) */
             ta_reg_enum_par_keytab,            /* REGISTER_ENUMERATION (E1694)*/
             ta_signal_par_keytab,              /* SIGNAL (E1694) */
             ta_reg_rate_par_keytab             /* REGISTER_RATE (E1892) */
            };

        static char **at_par_tab [] =
            {NULL,                              /* dummy */
             at_avail_par_keytab,               /* AVAILABLE */
             tt_attribute_keytab,               /* CREATE */
             at_delete_par_keytab,              /* DELETE */
             at_grant_adv_par_keytab,           /* GRANT_ADVANCE */
             tt_interaction_par_keytab,         /* INTERACTION */
             at_join_ack_par_keytab,            /* JOIN_ACK */
             at_lock_not_par_keytab,            /* LOCK_NOTIFICATION */
             at_new_id_par_keytab,              /* NEW_IDS */
             tt_refresh_par_keytab,             /* REFRESH_REQUEST */
             at_unavail_par_keytab,             /* UNAVAILABLE */
             at_unlock_par_keytab,              /* UNLOCK_REQUEST */
             tt_attribute_keytab,               /* UPDATE */
             at_start_save_par_keytab,          /* START_SAVE */
             at_set_rate_par_keytab,            /* SET_RATE */
             at_signal_par_keytab               /* SIGNAL */
             };

        static char **tt_par_tab [] =
                {NULL,
                 tt_interaction_par_keytab,
                 tt_refresh_par_keytab,
                 tt_attribute_keytab
                 };

        switch (mtype) {
                case actor_acm_mess:
                        result = (char **) ta_par_tab [msubtype];
                        break;
                case acm_actor_mess:
                        result = (char **) at_par_tab [msubtype];
                        break;
                case actor_actor_mess:
                        result = (char **) tt_par_tab [msubtype];
                        break;
        }

        return (result);
}

/* Given a message type, a message subtype, and a parameter type,
 * find the ascii table which contains the enumerated ascii values
 * for the parameter specified.  Also find the number of entries
 * in that table.
 */
void pval_table_lookup_by_type (mtype, msubtype, ptype, tab, max)

        MESSAGE_DATA_TYPE mtype;
        int msubtype, ptype;
        char **tab;
        int *max;
{
        static ENUM_DESCRIPTOR *ta_pval_tab [] =
                {NULL,                          /* dummy */            
                NULL,                           /* ADVANCE_REQUEST */  
                NULL,                           /* CANCEL_LOCK */      
                NULL,                           /* CREATE_REQUEST */   
                NULL,                           /* DELETE */           
                NULL,                           /* DISPATCH */                                
                NULL,                           /* EVENT */            
                NULL,                           /* EVENT_REQUEST */    
                ta_fltr_attr_pval_enums,        /* FILTER_ATTR */       
                ta_fltr_class_pval_enums,       /* FILTER_CLASS */      
                NULL,                           /* ID_REQUEST */        
                ta_join_pval_enums,             /* JOIN */              
                ta_lock_req_pval_enums,         /* LOCK_REQUEST */      
                ta_reg_attr_pval_enums,         /* REGISTER_ATTRS */    
                ta_reg_obj_pval_enums,          /* REGISTER_OBJECT */   
                ta_resign_pval_enums,           /* RESIGN */            
                NULL,                           /* UNLOCK */            
                NULL,                           /* SAVE_REQUEST */      
                NULL,                           /* SAVE_ACK */          
                ta_fltr_interaction_pval_enums, /* FILTER_INTERACTION */      
                NULL,                           /* PING */          
                NULL,                           /* PING_ACK */          
                NULL,                           /* SAVE_COMPLETE */          
                ta_fltr_parameters_pval_enums,  /* FILTER_PARAMETERS (E1691) */
                NULL,                           /* ERROR (E1692) */
                ta_reg_enum_pval_enums,         /* REGISTER_ENUMERATION (E1694) */
                ta_signal_type_pval_enums       /* SIGNAL (E1694) */
                };

        static ENUM_DESCRIPTOR *at_pval_tab [] =
            {NULL,                           /* dummy */
            NULL,                            /* AVAILABLE */
            tt_update_pval_enums,            /* CREATE */
            NULL,                            /* DELETE */
            NULL,                            /* GRANT_ADVANCE */
            tt_interaction_pval_enums,       /* INTERACTION */
            NULL,                            /* JOIN_ACK */
            at_lock_not_pval_enums,          /* LOCK_NOTIFICATION */
            NULL,                            /* NEW_IDS */
            NULL,                            /* REFRESH_REQUEST */
            NULL,                            /* UNAVAILABLE */
            NULL,                            /* UNLOCK_REQUEST */
            tt_update_pval_enums,            /* UPDATE */
            NULL,                            /* START_SAVE */
            NULL,                            /* SET_RATE */
            NULL,                            /* PING */
            NULL,                            /* PING_ACK */
            at_signal_type_pval_enums        /* SIGNAL */
            };

        ENUM_DESCRIPTOR *ettemp;

        switch (mtype) {
                case actor_acm_mess:
                        ettemp = ta_pval_tab [msubtype];
                        break;
                case acm_actor_mess:
                        ettemp = at_pval_tab [msubtype];
                        break;
                case actor_actor_mess:
                        break;
        }
        if (ettemp == NULL) {
                *max = 0;
                tab = NULL; 
        } else {
                *max = ettemp [ptype].max;
                *tab = (char *) ettemp [ptype].tab;
        }
}

/* Given an enumerated value and the appropriate keyword table,
 * return the table's ascii keyword value that corresponds to the
 * enumerated value.  (Tables are organized such that an
 * enumerated value defined in mess_struct.h has a corresponding
 * ascii keyword defined here.  The keyword is looked up in its
 * ascii table using the appropriate enumerated value as an
 * index.)
 */
char *get_keyword (tab, enum_val)
        
    KEYSTRING_TABLE tab;
    int enum_val;
{
    return (tab[enum_val]);
}

/* Given a keyword, the appropriate table to look it up in, and
 * the number of entries in the table, find the keyword's
 * enumerated value.  (Tables are organized such that a
 * keyword's index position in the table = it's enumerated value
 * in mess_struct.h definitions.)
 */
int get_enum_value (tab, keyword, max)
        
    KEYSTRING_TABLE tab;
    char *keyword;
    int max;
{
    BOOL found = FALSEVAL;
    int enum_val = 0;

    while (!found) {
        if (strncmp (keyword, tab [enum_val], strlen (keyword)) == 0) {
            if (strlen (keyword) == strlen (tab [enum_val])) {
                found = TRUEVAL;
            } else {
                enum_val++;
                if (enum_val > max) {
                    return (0);
                }
            }
        } else if (strncmp (tab [enum_val], "", 1) == 0) {
            found = TRUEVAL;
        } else {
            enum_val++;
            if (enum_val > max) {
                return (0);
            }
        }
   }
    return (enum_val);
}
