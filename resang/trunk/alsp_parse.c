 
/*  This file contains the parser for ALSP translator messages.  Incoming messages  
 *  are parsed into format-independent MESSAGE_STRUCTs, which are passed on to the 
 *  appropriate translator message handlers.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "at_mess_handler.h"
#include "ta_mess_handler.h"
#include "tt_mess_handler.h"
#include "gen_types.h"
#include "ascii_tables.h"
#include "mess_struct.h"
#include "alsp_parse.h"
#include "mess_format.h"
#include "alsp_utils.h"


#define MAX_MSG_LEN 20000        /* increased from 2700 to 20000 - SMM 10/96 (E1693) */

double atof ();

typedef void (*MESS_HANDLER) ();
typedef MESS_HANDLER MESS_HANDLER_TABLE [];

/* The parser is driven off of tables that describe the syntax of the 0.r 
 * messages. 
 */

/* The following tables describe the parameter lists for each of the incoming
 * messages.  They are indexed by the parameter type enumerations in 
 * mess_struct.h 
 */

typedef enum param_status_d {
        par_required,
        par_optional,
        par_dependable/* khc,3/93- This value indicates the status of a param. */
                      /* within a param. attribute of a mesg. will be changed */
        } PARAM_STATUS; /* to par_required if its param. attribute appear.   */
                    /* Otherwise, a param. with this status value has no     */
                    /* meaning as either require or option. In other word,   */
                    /* par_dependable is used to differentiate parameters of */ 
                    /* different states in a message.       */

typedef struct param_descriptor_d {
        PARAM_VALUE_TYPE param_type;
        PARAM_STATUS param_stat;
        BOOL param_present;
        } PARAM_DESCRIPTOR;

typedef PARAM_DESCRIPTOR PARAM_DESC_TAB [];

typedef int PARAM_MAX_DESCRIPTOR [];

 /* parameter type, parameter required?, parameter present? */ 
static PARAM_DESC_TAB ta_adv_req_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_float, par_required, FALSEVAL}
        };

static PARAM_DESC_TAB ta_cancel_lock_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_attribute_list, par_required, FALSEVAL}
        };

static PARAM_DESC_TAB ta_create_req_partypes = {
        {value_none, par_optional, FALSEVAL},           /* dummy */
        {value_int, par_required, FALSEVAL}             /* "" */
        };
        
static PARAM_DESC_TAB ta_delete_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL}
        };
        
static PARAM_DESC_TAB ta_dispatch_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_message, par_required, FALSEVAL}
        };
        
static PARAM_DESC_TAB ta_event_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_message, par_required, FALSEVAL}
        };
        

static PARAM_DESC_TAB ta_filter_attr_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_enum, par_optional, FALSEVAL},
        {value_int_list, par_optional, FALSEVAL},
        {value_float_range, par_optional, FALSEVAL},
        {value_float_range, par_optional, FALSEVAL},
        {value_int_range, par_optional, FALSEVAL},
        {value_int_range, par_optional, FALSEVAL},
        {value_string, par_optional, FALSEVAL},
        {value_int_range, par_optional, FALSEVAL},
        {value_enum_list, par_optional, FALSEVAL},
        {value_enum_list, par_optional, FALSEVAL},
        {value_float_range, par_optional, FALSEVAL},
        {value_enum_list, par_optional, FALSEVAL},
        {value_enum_list, par_optional, FALSEVAL},
        };
        
static PARAM_DESC_TAB ta_filter_class_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_enum, par_required, FALSEVAL},
        {value_enum, par_required, FALSEVAL}
        };
        
static PARAM_DESC_TAB ta_filter_interaction_partypes = {
        {value_enum, par_required, FALSEVAL},
        {value_enum, par_required, FALSEVAL}
        };
        
static PARAM_DESC_TAB ta_id_req_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL}
        };
        
static PARAM_DESC_TAB ta_join_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_float, par_optional, FALSEVAL},
        {value_string, par_required, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_float, par_optional, FALSEVAL},
        {value_string, par_required, FALSEVAL},
        {value_enum, par_optional, FALSEVAL},
        {value_enum, par_optional, FALSEVAL}
        };

        
static PARAM_DESC_TAB ta_lock_req_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_attribute_list, par_required, FALSEVAL},
        {value_enum, par_required, FALSEVAL},
        {value_string, par_optional, FALSEVAL},
        {value_string, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB ta_reg_attrs_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_enum, par_optional, FALSEVAL},
        {value_attribute_list, par_optional, FALSEVAL},
        {value_attribute_list, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB ta_reg_obj_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_enum, par_required, FALSEVAL},
        {value_attribute_list, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB ta_resign_partypes = {      /* clk 1/94 - for new resign */
        {value_none, par_optional, FALSEVAL},         /*            msg parameters */
        {value_enum, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB ta_unlock_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_attribute_list, par_required, FALSEVAL}
        };

static PARAM_DESC_TAB ta_save_req_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_float, par_required, FALSEVAL},
        {value_string, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB ta_save_ack_partypes = {
        {value_none, par_optional, FALSEVAL},           /* dummy parameter for fortran/c allignmnt */
        {value_float, par_required, FALSEVAL},          /* TIME parameter  - simulation time in decimal hours */
        {value_string, par_required, FALSEVAL}          /* CLOCK parameter - true time in hh:mm:ss format */
        };

static PARAM_DESC_TAB ta_save_complete_partypes = {
        {value_none, par_optional, FALSEVAL},           /* dummy parameter for fortran/c allignmnt */
        {value_float, par_required, FALSEVAL},          /* TIME parameter  - simulation time in decimal hours */
        {value_string, par_required, FALSEVAL}          /* CLOCK parameter - true time in hh:mm:ss format */
        };

static PARAM_DESC_TAB ta_filter_parameters_partypes = { /* SMM 9/96 (E1691) */
        {value_none, par_optional, FALSEVAL},  /* dummy parameter for fortran/c alignment */
        {value_enum, par_required, FALSEVAL},  /* KIND parameter */
        {value_enum, par_required, FALSEVAL},  /* TO parameter */
        {value_enum, par_required, FALSEVAL}   /* FROM parameter */
        };

static PARAM_DESC_TAB ta_error_partypes = {     /* SMM 9/96 (E1692) */
        {value_none, par_optional, FALSEVAL},   /* dummy parameter for fortran/c alignment */
        {value_string, par_required, FALSEVAL}  /* TO parameter */
        };

static PARAM_DESC_TAB at_avail_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_attribute_list, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB at_ping_partypes = {        /* clk  1/94 */
    {value_none, par_optional, FALSEVAL},
    {value_string, par_optional, FALSEVAL}        /* optional text */
    };

static PARAM_DESC_TAB at_ping_ack_partypes = {    /* clk  1/94 */
    {value_none, par_optional, FALSEVAL},
    {value_string, par_required, FALSEVAL}        /* optional text */
    };

static PARAM_DESC_TAB at_update_partypes = {
        {value_none, par_optional, FALSEVAL},                                  /*  0 */
        {value_int, par_required, FALSEVAL},             /* id */              /*  1 */
        {value_enum, par_required, FALSEVAL},            /* class */           /*  2 */
        {value_float, par_optional, FALSEVAL},           /* lat */             /*  3 */
        {value_float, par_optional, FALSEVAL},           /* long */            /*  4 */
        {value_int, par_optional, FALSEVAL},             /* altitude */        /*  5 */
        {value_int, par_optional, FALSEVAL},             /* arty supp */       /*  6 */
        {value_enum, par_optional, FALSEVAL},            /* aws stat */        /*  7 */
        {value_enum, par_optional, FALSEVAL},            /* cbs stat */        /*  8 */
        {value_int, par_optional, FALSEVAL},             /* elect deg */       /*  9 */
        {value_int, par_optional, FALSEVAL},             /* fire cont */       /* 10 */
        {value_int, par_optional, FALSEVAL},             /* heading */         /* 11 */
        {value_string, par_optional, FALSEVAL},          /* name */            /* 12 */
        {value_int, par_optional, FALSEVAL},             /* old id */          /* 13 */
        {value_loc_struct_list, par_optional, FALSEVAL}, /* orbit */           /* 14 */
        {value_none, par_optional, FALSEVAL},                                  /* 15 */
        {value_none, par_optional, FALSEVAL},                                  /* 16 */
        {value_none, par_optional, FALSEVAL},                                  /* 17 */
        {value_int, par_optional, FALSEVAL},             /* percent */         /* 18 */
        {value_enum, par_optional, FALSEVAL},            /* side */            /* 19 */
        {value_enum, par_optional, FALSEVAL},            /* size */            /* 20 */
        {value_int, par_optional, FALSEVAL},             /* speed */           /* 21 */
        {value_enum, par_optional, FALSEVAL},            /* status */          /* 22 */
        {value_int, par_optional, FALSEVAL},             /* sup l */           /* 23 */
        {value_int, par_optional, FALSEVAL},             /* sup m */           /* 24 */
        {value_int, par_optional, FALSEVAL},             /* sup r */           /* 25 */
        {value_loc_struct, par_optional, FALSEVAL},      /* target loc */      /* 26 */
        {value_none, par_optional, FALSEVAL},                                  /* 27 */
        {value_enum, par_optional, FALSEVAL},            /* unit type */       /* 28 */
        {value_enum, par_optional, FALSEVAL},            /* mission */         /* 29 */
        {value_enum, par_optional, FALSEVAL},            /* posture */         /* 30 */
        {value_int, par_optional, FALSEVAL},             /* orientation */     /* 31 */
        {value_float, par_optional, FALSEVAL},           /* sim time */        /* 32 */
        {value_string, par_optional, FALSEVAL},          /* call sign */       /* 33 */
        {value_float, par_optional, FALSEVAL},           /* age */             /* 34 */
        {value_int, par_optional, FALSEVAL},             /* sub depth */       /* 35 */
        {value_enum, par_optional, FALSEVAL},            /* flight ops */      /* 36 */
        {value_emit_struct_list, par_optional, FALSEVAL},   /* emitter */      /* 37 */
        {value_int, par_dependable, FALSEVAL},           /* radar */           /* 38 */
        {value_int, par_dependable, FALSEVAL},           /* jammer */          /* 39 */
        {value_int, par_dependable, FALSEVAL},           /* index */           /* 40 */
        {value_float, par_dependable, FALSEVAL},         /* orient */          /* 41 */
        {value_enum, par_dependable, FALSEVAL},          /* mode */            /* 42 */
        {value_enum, par_dependable, FALSEVAL},          /* type */            /* 43 */
        {value_enum, par_dependable, FALSEVAL},          /* emitter_status */  /* 44 */
        {value_int, par_optional, FALSEVAL},             /* c2w_factors */     /* 45 */
        {value_int, par_dependable, FALSEVAL},           /* unit */            /* 46 */
        {value_int, par_dependable, FALSEVAL},           /* emtr */            /* 47 */
        {value_float, par_dependable, FALSEVAL},         /* ew_c3 */           /* 48 */
        {value_float_list, par_dependable, FALSEVAL},    /* eq_acq */          /* 49 */
        {value_float_list, par_dependable, FALSEVAL},    /* eq_launch */       /* 50 */
        {value_float_list, par_dependable, FALSEVAL},    /* eq_guide */        /* 51 */
        {value_float_list, par_dependable, FALSEVAL},    /* eq_power */        /* 52 */
        {value_float_list, par_dependable, FALSEVAL},    /* weapon - enum reg *//* 53 */
        {value_float_list, par_dependable, FALSEVAL},    /* placehold */       /* 54 */
        {value_iff_struct_list, par_optional, FALSEVAL}, /* iff */             /* 55 */
        {value_enum, par_dependable, FALSEVAL},          /* master */          /* 56 */
        {value_int, par_dependable, FALSEVAL},           /* mode1 */           /* 57 */
        {value_int, par_dependable, FALSEVAL},           /* mode2 */           /* 58 */
        {value_int, par_dependable, FALSEVAL},           /* mode3 */           /* 59 */
        {value_int, par_dependable, FALSEVAL},           /* modec */           /* 60 */
        {value_enum, par_dependable, FALSEVAL},          /* mode4 */           /* 61 */
        {value_enum, par_dependable, FALSEVAL},          /* status - on/off */ /* 62 */
        {value_int, par_dependable, FALSEVAL},           /* mode - code # */   /* 63 */
        {value_enum, par_optional, FALSEVAL},          /* propulsion mode 64 */
        {value_sonar_struct_list, par_optional, FALSEVAL},   /* sonar */ /*65*/
        {value_enum, par_dependable, FALSEVAL},          /* sonar type */ 
        {value_enum, par_dependable, FALSEVAL},          /* sonar status */
        {value_enum, par_optional, FALSEVAL},            /* mode */        
        {value_int, par_optional, FALSEVAL},             /* depth */ 
        {value_loc_struct_list, par_optional, FALSEVAL}, /* position */ 
        {value_loc_struct_list, par_optional, FALSEVAL}, /* sweeparea location */ 
        {value_enum, par_optional, FALSEVAL},            /* sweeeparea status */          /* 72 */
        {value_string, par_optional, FALSEVAL},          /* ATOID */   
        {value_int, par_optional, FALSEVAL}           /* Strength of ship */             
        };

static PARAM_DESC_TAB at_delete_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL}
        };
        
static PARAM_DESC_TAB at_grant_adv_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_float, par_required, FALSEVAL}
        };

/* All pars are marked as optional here.  This is not correct,
 * but due to the special-case syntax of the interaction 
 * message, identifying required parameters in this table
 * causes problems for the parser.  Fix is to add special
 * case checks for required parameters in the code.
 */
static PARAM_DESC_TAB at_interaction_partypes = {
        {value_none, par_optional, FALSEVAL},         /* dummy           */
        {value_int, par_optional, FALSEVAL},          /* FROM            */
        {value_int, par_optional, FALSEVAL},          /* TO              */
        {value_enum, par_optional, FALSEVAL},         /* KIND            */
        {value_int, par_optional, FALSEVAL},          /* AMOUNT          */
        {value_float, par_optional, FALSEVAL},        /* DURATION        */
        {value_int, par_optional, FALSEVAL},          /* PERCENT         */
        {value_int, par_optional, FALSEVAL},          /* KILLS           */
        {value_float, par_optional, FALSEVAL},        /* LAT             */
        {value_float, par_optional, FALSEVAL},        /* LONG            */
        {value_float, par_optional, FALSEVAL},        /* FIRER_LAT       */
        {value_float, par_optional, FALSEVAL},        /* FIRER_LONG      */
        {value_enum, par_optional, FALSEVAL},         /* MISSION         */
        {value_float, par_optional, FALSEVAL},        /* RADIUS          */
        {value_int, par_optional, FALSEVAL},          /* NUMBER          */
        {value_enum, par_optional, FALSEVAL},         /* WEAPON          */
        {value_enum, par_optional, FALSEVAL},         /* SIZE            */
        {value_enum_list, par_optional, FALSEVAL},    /* TARGETS         */
        {value_string, par_optional, FALSEVAL},       /* REPORT          */
        {value_int, par_optional, FALSEVAL},          /* NUMBER          */
        {value_int, par_optional, FALSEVAL},          /* TARGETS_ENGAGED */
        {value_int, par_optional, FALSEVAL},          /* HITS            */
        {value_enum, par_optional, FALSEVAL},         /* TYPE            */
        {value_int, par_optional,  FALSEVAL},         /* NUMBER          */
        {value_enum, par_optional, FALSEVAL},         /* REASON          */
        {value_string, par_optional, FALSEVAL},       /* TARGET_REF      */
        {value_string, par_optional, FALSEVAL},       /* BEN             */
        {value_string, par_optional, FALSEVAL},       /* BEN_SUFFIX      */
        {value_string, par_optional, FALSEVAL},       /* TTN             */
        {value_string, par_optional, FALSEVAL},       /* PIN             */
        {value_string, par_optional, FALSEVAL},       /* LOCATION        */
        {value_string, par_optional, FALSEVAL}        /* CORRELATION_ID  */
        };

static PARAM_DESC_TAB at_join_ack_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_float, par_required, FALSEVAL}
        };

static PARAM_DESC_TAB at_lock_not_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_attribute_list, par_required, FALSEVAL},
        {value_enum, par_required, FALSEVAL},
        {value_string, par_optional, FALSEVAL},
        {value_string, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB at_new_id_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int_list, par_required, FALSEVAL}
        };

static PARAM_DESC_TAB at_refresh_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},             /* id */
        {value_enum, par_optional, FALSEVAL},            /* class */
        {value_attribute_list, par_optional, FALSEVAL}   /* attribute list */
        };

static PARAM_DESC_TAB at_unavail_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_attribute_list, par_required, FALSEVAL}
        };

static PARAM_DESC_TAB at_unlock_req_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_int, par_required, FALSEVAL},
        {value_attribute_list, par_required, FALSEVAL},
        {value_string, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB at_start_save_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_float, par_required, FALSEVAL},
        {value_string, par_optional, FALSEVAL}
        };

static PARAM_DESC_TAB at_set_rate_partypes = {
        {value_none, par_optional, FALSEVAL},
        {value_float, par_required, FALSEVAL},
        };

static PARAM_DESC_TAB ta_ping_partypes = {        /* clk  1/94 */
    {value_none, par_optional, FALSEVAL},
    {value_string, par_optional, FALSEVAL}        /* optional text */
    };

static PARAM_DESC_TAB ta_ping_ack_partypes = {    /* clk  1/94 */
    {value_none, par_optional, FALSEVAL},
    {value_string, par_required, FALSEVAL}        /* optional text */
    };

static PARAM_DESC_TAB at_signal_partypes = {     /* clk - 9/95 (E1457) */
        {value_none, par_optional, FALSEVAL},    /* SIGNAL(REGISTER_OBJECTS) */
        {value_enum, par_required, FALSEVAL}
        };

/* The following tables point to other tables for the parameter lists for each
 of the 
 * incoming messages.
  They are indexed by the message type enumerations in mess_struct.h 
 */
PARAM_DESCRIPTOR *ta_params [] = 
        {NULL,
        ta_adv_req_partypes,
        ta_cancel_lock_partypes,
        ta_create_req_partypes,
        ta_delete_partypes,
        ta_dispatch_partypes,
        ta_event_partypes,
        NULL,                       /* EVENT_REQUEST */
        ta_filter_attr_partypes,
        ta_filter_class_partypes,
        ta_id_req_partypes,
        ta_join_partypes,
        ta_lock_req_partypes,
        ta_reg_attrs_partypes,
        ta_reg_obj_partypes,
        ta_resign_partypes,
        ta_unlock_partypes,
        ta_save_req_partypes,
        ta_save_ack_partypes,
        ta_filter_interaction_partypes,     /* clk 12/93 */
        ta_ping_partypes,                   /* clk  1/94 */
        ta_ping_ack_partypes,               /* clk  1/94 */
        ta_save_complete_partypes,
        ta_filter_parameters_partypes,      /* SMM 9/96 (E1691) */
        ta_error_partypes                   /* SMM 9/96 (E1692) */
        };


PARAM_DESCRIPTOR *at_params [] = 
        {NULL,
        at_avail_partypes,
        at_update_partypes,         /* CREATE */
        at_delete_partypes,
        at_grant_adv_partypes,
        at_interaction_partypes,
        at_join_ack_partypes,
        at_lock_not_partypes,
        at_new_id_partypes,
        at_refresh_partypes,
        at_unavail_partypes,
        at_unlock_req_partypes,
        at_update_partypes,         /* update */
        at_start_save_partypes,
        at_set_rate_partypes,
        at_ping_partypes,                   /* clk  1/94 */
        at_ping_ack_partypes,               /* clk  1/94 */
        at_signal_partypes                  /* clk - 9/95 (E1457) */
        };

/* The following tables contain the maximum number of parameters for each of the 
 * incoming messages.  They are indexed by the message type enumerations in mess_struct.h 
 */
PARAM_MAX_DESCRIPTOR ta_max_params =
        {0,
        N_ADV_REQ_PAR,
        N_CANCEL_LOCK_PAR,
        N_CREATE_REQ_PAR,
        N_TA_DELETE_PAR,
        N_DISPATCH_PAR,
        N_EVENT_PAR,
        N_EVENT_REQ_PAR,
        N_FILTER_ATTR_PAR,
        N_FILTER_CLASS_PAR,
        N_ID_REQ_PAR,
        N_JOIN_PAR,
        N_LOCK_REQ_PAR,
        N_REG_ATTR_PAR,
        N_REG_OBJECT_PAR,
        N_RESIGN_PAR,
        N_UNLOCK_PAR,
        N_SAVE_REQ_PAR,
        N_SAVE_ACK_PAR,
        N_FILTER_INTERACTION_PAR,       /* clk 12/93 */
        N_PING_PAR,                     /* clk  1/94 */
        N_PING_ACK_PAR,                 /* clk  1/94 */
        N_SAVE_COMPLETE_PAR,            /* clk 9/95 (E1458) */
        N_FILTER_PARAMETERS_PAR,        /* SMM 9/96 (E1691) */
        N_ERROR_PAR                     /* SMM 9/96 (E1692) */
        };

PARAM_MAX_DESCRIPTOR at_max_params =
        {0,
        N_AVAIL_PAR,
        N_ATTRIBUTE_VALS,   /* create */
        N_AT_DELETE_PAR,
        N_GRANT_ADV_PAR,
        N_INTERACTION_PAR,
        N_JOIN_ACK_PAR,
        N_LOCK_NOTIFY_PAR,
        N_NEW_ID_PAR,
        N_REFRESH_PAR,
        N_UNAVAIL_PAR,
        N_UNLOCK_REQ_PAR,
        N_ATTRIBUTE_VALS,   /* update */
        N_START_SAVE_PAR,
        N_SET_RATE_PAR,
        N_PING_PAR,                    /* clk  1/94 */
        N_PING_ACK_PAR,                /* clk  1/94 */
        N_AT_SIGNAL_PAR                /* clk - 9/95 (E1457) */
        };
        
/* The following tables point to the message handling routines for each of the 
 * incoming messages.  They are indexed by the message type enumerations in mess_struct.h 
 */
static MESS_HANDLER_TABLE ta_handlers = {
        NULL,
        handle_ta_advance_request,
        handle_ta_cancel_lock,
        handle_ta_create_request,
        handle_ta_delete,
        handle_ta_dispatch,
        handle_ta_event,
        handle_ta_event_request,
        handle_ta_filter_attr,
        handle_ta_filter_class,
        handle_ta_id_req,
        handle_ta_join,
        handle_ta_lock_req,
        handle_ta_reg_attrs,
        handle_ta_reg_object,
        handle_ta_resign,
        handle_ta_unlock,
        handle_ta_save_request,
        handle_ta_save_ack,
        handle_ta_filter_interaction,        /* clk 12/93 */
        handle_ta_ping,                      /* clk  1/94 */
        handle_ta_ping_ack,                  /* clk  1/94 */
        handle_ta_save_complete,             /* clk 9/95 (E1458) */
        handle_ta_filter_parameters,         /* SMM 9/96 (E1691) */
        handle_ta_error                      /* SMM 9/96 (E1692) */
        };

static MESS_HANDLER_TABLE at_handlers = {
        NULL,
        handle_at_available,
        handle_at_create,
        handle_at_delete,
        handle_at_grant_advance,
        handle_at_interaction,
        handle_at_join_ack,
        handle_at_lock_notify,
        handle_at_new_id,
        handle_at_refresh,
        handle_at_unavail,
        handle_at_unlock_request,
        handle_at_update,
        handle_at_start_save,
        handle_at_set_rate,
        handle_at_ping,                 /* clk  1/94 */
        handle_at_ping_ack,             /* clk  1/94 */
        handle_at_signal                /* clk - 9/95 (E1457) */
        };

static MESS_HANDLER_TABLE tt_handlers = {
        NULL,
        handle_tt_interaction,
        handle_tt_refresh,
        handle_tt_update
        };

static int mess_nest_level  = 0;
static BOOL salvo_context   = FALSEVAL;
static BOOL emitter_context = FALSEVAL;             /* KHC,3/93 */
static BOOL c2w_factors_context = FALSEVAL;         /* JECEWSI ICD - clk 10/94 */
static BOOL supply_pol_context = FALSEVAL;          /* E1453 - SMM 10/95 */
static BOOL iffmode_context = FALSEVAL;             /* smp - 2/98 (E1907) */


/* Reset all param_present fields in the designated table to FALSE. 
 * The param_present fields are used to keep track of required parameters for
 * a given message type in order to detect semantic errors during parsing.
 */
static void init_ptable (table, max_params)
        
PARAM_DESC_TAB table;
int max_params;
{
    int i;

    for (i = 1; i <= max_params; i++) {
         table [i].param_present = FALSEVAL;
        }

    /* khc,3/93, reset the param. status to par_dependable. */
    /* SMM, 10/94 Changed parameter names and added new ones */
    /* Status of EMITTER is an optional update parameter */
    if (table == at_update_partypes) { 
        table [tt_attr_emitter].param_stat  = par_optional;
        table [tt_attr_jammer].param_stat = par_dependable;
        table [tt_attr_radar].param_stat  = par_dependable;
        table [tt_attr_type].param_stat  = par_dependable;
        table [tt_attr_emitter_status].param_stat = par_dependable;
        table [tt_attr_index].param_stat  = par_dependable; 
        table [tt_attr_orient].param_stat = par_dependable;
        table [tt_attr_mode].param_stat   = par_dependable;


        /* Added attributes which the EW model is to update. */
        /* Modeled after emitter above, not sure why this is */
        /* being done though.   clk 10/94                    */
        table [tt_attr_c2w_factors].param_stat = par_optional;
        table [tt_attr_unit].param_stat = par_dependable;
        table [tt_attr_emtr].param_stat = par_dependable;
        table [tt_attr_ew_c3].param_stat = par_dependable;
        table [tt_attr_ew_acq].param_stat = par_dependable;
        table [tt_attr_ew_launch].param_stat = par_dependable;
        table [tt_attr_ew_guide].param_stat = par_dependable;
        table [tt_attr_ew_power].param_stat = par_dependable;
        table [tt_attr_ew_surf].param_stat = par_dependable;

        /* Added attributes for handling the IFF wrapper attribute. */
        /* smp - 2/98 (E1907)                                       */
        table [tt_attr_iff].param_stat  = par_optional;
        table [tt_attr_iffmode_master].param_stat = par_dependable;
        table [tt_attr_iffmode1].param_stat  = par_dependable;
        table [tt_attr_iffmode2].param_stat  = par_dependable;
        table [tt_attr_iffmode3].param_stat  = par_dependable;
        table [tt_attr_iffmodeC].param_stat  = par_dependable;
        table [tt_attr_iffmode4].param_stat  = par_dependable;
        table [tt_attr_iffmode_status].param_stat = par_dependable;
        table [tt_attr_iffmode_value].param_stat  = par_dependable;
       }
}


/* Check the param_stat fields for the given table
 * against the corresponding param_present fields in order to detect
 * missing parameters that are required for correctness.
 */
static BOOL required_params_ok (table, max_params)
        
        PARAM_DESC_TAB table;
        int max_params;
{
        int i;
        BOOL ok = TRUEVAL;

        for (i = 1; i <= max_params; i++) {
                if ((table [i].param_stat == par_required)
                        && (table [i].param_present == FALSEVAL)) {
                        ok = FALSEVAL;
                        alsp_console_message(get_keyword(tt_attribute_keytab,i));
                        break;
                }
        }
        return (ok);
}

/* Having already parsed off the parameter keyword, pull off the parameter
 * value and place it in val.
 */
void get_par_value (str, val, qual, val_type)

    char                **str;
    char                *val;
    ATTRIBUTE_QUALIFIER *qual;
    PARAM_VALUE_TYPE    val_type;
{
    char    *tstr   = *str;
    char    *tstr2;
    char    *tval   = val;
    char    *tvalsave;
    char    prevchar;
    char    qualstring [10];
    char    *qualptr;
    BOOL    cont;
    int     quotecnt    = 0;

    tstr    = *str;
    cont    = (BOOL) (tstr [0] != END_PARAM [0]);
    *qual   = qual_none;

    while (cont) {
        tvalsave = tval;

        if (tstr [0] != '"') {
            tval [0] = tstr [0];
            tval++;
        } else {
            quotecnt++;
        }

        if ((val_type != value_string) && (tvalsave [0] == ' ')) {
            tval--;
        } 

        if (tstr [0] == QUAL_SYMBOL [0]) {  /* qualifier symbol is '@' */
            tval--;
            qualptr = qualstring;
            while (tstr [0] != END_PARAM [0]) {
                qualptr [0] = tstr [0];
                qualptr ++;
                tstr++;
            }
            qualptr [0] = '\0';
            *qual = (ATTRIBUTE_QUALIFIER) get_enum_value
                            (attr_qual_keytab, qualstring, N_QUALIFIER_VALS);
            prevchar = tstr [0];
        } else {
            prevchar = tstr [0];
            tstr++;
        }

        if ((quotecnt == 2) && (prevchar == '"') &&
            (tstr [0] == END_PARAM [0])) {
            cont = FALSEVAL;
            quotecnt = 0;
        } else if (quotecnt == 2) {
            quotecnt = 0;
        } else {
            cont = (BOOL) ((tstr [0] != END_PARAM [0]) ||
                           ((prevchar == '"') &&
                            (tstr [0] == END_PARAM [0])));
            if (cont == FALSEVAL) {
                if (tstr [0] == END_PARAM [0]) {
                    if (quotecnt == 1) {
                        cont = TRUEVAL;
                    }
                }
            }
        }
    }   /* End of while loop */

    tval [0] = '\0'; 
    *str = tstr;
}

/* Pull off the parameter keyword and place it in keyword.
 */
void get_par_keyword (str, keyword, len, syntax_ok, done)
        
        char **str;
        char *keyword;
        int *len;
        BOOL *syntax_ok;
        BOOL *done;
{
        BOOL ok = TRUEVAL;
        BOOL complete = FALSEVAL;
        char *strend = *str;
        char *tstr, *keystart;
        int tlen = *len;
        int i;

        while (strend [0] == ' ') strend++;  /* pull off leading blanks */
        keystart = strend;

        /* if qualifier @ALL is used, stop here since there won't be a */
        /* value associated with it. */
        if (strend [0] == QUAL_SYMBOL [0] && emitter_context == TRUEVAL) {
            return; 
        }

        while (strend [0] != START_PARAM [0]) { /* read up to ( */
                if (strend [0] == '\0') {
                       /* E2222 joa CT01
                          alsp_console_message
                          ("Syntax error in alsp_parse (get_par_keyword).");
                       */
                        ok = FALSEVAL; 
						break; 
                }
                strend++;
                if ((strend [0] == END_PARAM [0])  /* found end ) */ 
                        && (strend [1] != '\0')) { /* and not a null */
                        complete = TRUEVAL;
                        break;
                } else {
                        tlen++;
                }
        }
        strend++;  /* incr string end */
        *len = tlen; 

        tstr = strend - 2;
        while (tstr [0] == ' ') {
                tlen--;
                if (tstr == *str) break;
                tstr--;
        }
        /* put keyword into variable keyword ending with null character*/
        strncpy (keyword, keystart, tlen); *(keyword+tlen)  = '\0';

        *str = strend;
        *done = complete;
        *syntax_ok = ok;
}

/* Parse for part of an ALSP location attribute of the form (LAT(0.0)LONG(0.0)). */
static BOOL parse_loc_attribute (str, keyword, arg_value)

        char **str;
        char *keyword;
        float *arg_value;
{
        char buff [80];
        char *tstr = *str;
        int len = 0;
        BOOL ok = TRUEVAL;
        BOOL complete = FALSEVAL;
        ATTRIBUTE_QUALIFIER dummy;

        get_par_keyword (&tstr, buff, &len, &ok, &complete);
        if ((!ok) || (complete == TRUEVAL) || (len == 0)) {
                alsp_console_message
                        ("Syntax error in alsp_parse (parse_loc_attribute).");
                return (FALSEVAL);
        };
        if (strncmp (buff, keyword, strlen (buff)) != 0) {
                alsp_console_message
                        ("Syntax error in alsp_parse (parse_loc_attribute).");
                return (FALSEVAL);
        }
        get_par_value (&tstr, buff, &dummy, value_float);
        *arg_value = atof (buff);
        *str = tstr;
        return (TRUEVAL);
}

/* Parse for an ALSP location attribute of the form (LAT(0.0)LONG(0.0)). */
static BOOL parse_loc (par, str)

        MESS_PARAM_LIST *par;
        char **str;
{
        BOOL ok;
        float lat, lon;
        char *tstr = *str;
        
        ok = parse_loc_attribute (&tstr, "LAT", &lat);
        tstr++;
        if (ok == TRUEVAL) {
                ok = parse_loc_attribute (&tstr, "LONG", &lon);
                set_par_loc_value (par, lat, lon);
                tstr++;
                *str = tstr;
        }
        return (ok);
}

/* Parse for an ALSP location list of the form LAT(0.0)LONG(0.0),LAT(0.0)LONG(0.0).... */
static BOOL parse_loc_list (par, str)

        MESS_PARAM_LIST *par;
        char **str;
{
        char *tstr = *str;
        char *tstr2;
        LOCLIST list;
        int nvals = 0;
        int i;
        BOOL ok = TRUEVAL;
        float lat, lon;

        for (i = 0; i < MAX_LIST; i++) {
                list [i].lat = 0.0;
                list [i].lon = 0.0;
        }
        while (tstr [0] != END_PARAM [0]) {
                tstr2 = tstr;
                while (tstr [0] != END_PARAM [0]) tstr++;
                tstr++;
                while (tstr [0] != END_PARAM [0]) tstr++;
                tstr++;
                nvals++;
                ok = parse_loc_attribute (&tstr2, "LAT", &lat);
                tstr2++;
                if (ok == TRUEVAL) {
                        ok = parse_loc_attribute (&tstr2, "LONG", &lon);
                        list [nvals - 1].lat = lat;
                        list [nvals - 1].lon = lon;
                }
        }
        set_par_loc_list (par, list, nvals);
        *str = tstr;
        return (ok);
}

/* Parse for a list of emitters of the following form */
/* EMITTER(JAMMER(TYPE(RADAR_JAM)INDEX(1)STATUS(ON)ORIENT(145.5)MODE(COMM_JAM)) */
/*          RADAR(TYPE(APG_76)INDEX(2)STATUS(OFF))) */
/* At this point only EMITTER has been pulled off the message */
static BOOL parse_emitter_list (par, str, m, table, mtype, msubtype, 
                                max_params)

    MESS_PARAM_LIST *par;
    char **str;
    MESSAGE_STRUCT *m;
    PARAM_DESC_TAB table;
    MESSAGE_DATA_TYPE mtype;
    int msubtype;
    int max_params;
{
    EMITTERLIST list;       /* defined in MESS_STRUCT.H */
    int i, tlen, ptype, j;
    BOOL ok = TRUEVAL;
    BOOL done = FALSEVAL;
    BOOL get_next_param = FALSEVAL;
    char buff[MAX_MESSAGE_LEN];
    char *strend = *str;
    PARAM_VALUE_TYPE val_type;
    ATTRIBUTE_QUALIFIER qual;
    int emitter_number = -1;
    int paren_depth = 1;

    /* initialize values to zero or null */
    for (i = 0; i < MAX_EMITTERS; i++) {
        for (j = 0; j < N_EMIT_PARAMS; j++) {
            list[i].emitter_params[j] = FALSEVAL;;
        }
        list[i].emitter_cat= 0;
        list[i].emitter_type.enum_value = 0;
        list[i].emitter_type.enum_string = NULL;
        list[i].emitter_index = 0;
        list[i].emitter_status.enum_value = 0;
        list[i].emitter_status.enum_string = NULL;
        list[i].emitter_orientation = 0.0;
        list[i].emitter_mode.enum_value = 0;
        list[i].emitter_mode.enum_string = NULL;
    }

   while (!done) { /* Start parsing parameters */
        tlen = 0;

        /* return a param keyword in buff */
        get_par_keyword (&strend, buff, &tlen, &ok, &done); 
        if (!ok) {
            init_ptable (table, max_params);
            return (FALSEVAL);
        }

        /* if this is the @ALL qualifier, handle it here */
        if (strend [0] == QUAL_SYMBOL [0] && emitter_context == TRUEVAL) {
            get_par_value (&strend, buff, &qual, val_type);
            set_par_attr_qual (par, qual);
        }

        if (tlen > 0) {
            ptype = get_enum_value (
                    ptable_lookup_by_mess_type (mtype, msubtype), 
                    buff, max_params);

            paren_depth++;
        } /* end if tlen > 0 */

        if (ptype == tt_attr_status) ptype = tt_attr_emitter_status;

        if (ptype == tt_attr_radar) {
            table[tt_attr_type].param_stat  = par_optional;
            table[tt_attr_emitter_status].param_stat = par_optional;
            table[tt_attr_index].param_stat = par_required;
            
            emitter_number++;
            list[emitter_number].emitter_cat = tt_attr_radar; 
            list[emitter_number].emitter_params[0] = TRUEVAL;
        }
        else if (ptype == tt_attr_jammer) {
            table[tt_attr_type].param_stat  = par_optional;
            table[tt_attr_emitter_status].param_stat = par_optional;
            table[tt_attr_index].param_stat = par_required;
            table[tt_attr_orient].param_stat = par_optional;
            table[tt_attr_mode].param_stat = par_optional;

            emitter_number++;
            list[emitter_number].emitter_cat = tt_attr_jammer; 
            list[emitter_number].emitter_params[0] = TRUEVAL;
        }

        if ((ptype != tt_attr_jammer) &&
            (ptype != tt_attr_radar))
            {
            /* Get the type of the param. value */
            val_type = table[ptype].param_type;  
            /* Get the parameter value */
            get_par_value (&strend, buff, &qual, val_type);
    
            switch (val_type) {  
                case value_int: /* emitter index, category handled above */
                    list[emitter_number].emitter_index = atoi(buff);
                    list[emitter_number].emitter_params[2] = TRUEVAL;
                    table[ptype].param_present = TRUEVAL;
                    break;
                case value_enum: { /* status, type, mode */
                    int emax, eval;
                    char *etab;
            
                    pval_table_lookup_by_type (mtype,msubtype,ptype,
                                                        &etab,&emax);
                    if (emax == 0) {
                        alsp_console_message ("Enum lookup failed in alsp_parse.");
                    } else { 
                        eval = get_enum_value (etab, buff, emax);
                        if (ptype == tt_attr_type) {
                            list[emitter_number].emitter_type.enum_value = 
                                eval;
                            list[emitter_number].emitter_type.enum_string =
                                get_keyword(etab,eval);
                            list[emitter_number].emitter_params[1] = TRUEVAL;
                        }
                        else if (ptype == tt_attr_emitter_status) {
                            list[emitter_number].emitter_status.enum_value = 
                                eval;
                            list[emitter_number].emitter_status.enum_string = 
                                get_keyword(etab,eval);
                            list[emitter_number].emitter_params[3] = TRUEVAL;
                        }
                        else if (ptype == tt_attr_mode) {
                            list[emitter_number].emitter_mode.enum_value = 
                                eval;
                            list[emitter_number].emitter_mode.enum_string = 
                                get_keyword(etab,eval);
                            list[emitter_number].emitter_params[5] = TRUEVAL;
                        }
                    table[ptype].param_present = TRUEVAL;
                    } /* end if emax != 0 */
                } /* end of case value_enum */
                break;
                case value_float:  /* emitter_orientation */
                    list[emitter_number].emitter_orientation = atof(buff);
                    table[ptype].param_present = TRUEVAL;
                    list[emitter_number].emitter_params[4] = TRUEVAL;
                    break;
                default:
                    break;
            } /* end of switch */         

            while ((strend [0] == END_PARAM [0]) &&
                   (paren_depth != 0)) {
                paren_depth--;
                strend++;
            }                
        }
        if (paren_depth == 0) 
        
    done = TRUEVAL;

    } /* end while (!done) */

    
    if (emitter_number >= 0) {
        /* add one to emitter_number since it started at zero */
        emitter_number++;
        set_par_emitter_list(par, list, &emitter_number);
    }
    strend--; /* move pointer back one space */
    *str = strend;
    return (ok);
}

/* Parse  a list of sonars of the following form */
/* SONAR((TYPE(BQQ5A)STATUS(ON)MODE(DP)DEPTH(50)) */
/*       (TYPE(PERCH)STATUS(OFF)) */
/* At this point only SONAR has been pulled off the message */

static BOOL parse_sonar_list (par, str, m, table, mtype, msubtype, 
                                max_params)

    MESS_PARAM_LIST *par;
    char **str;
    MESSAGE_STRUCT *m;
    PARAM_DESC_TAB table;
    MESSAGE_DATA_TYPE mtype;
    int msubtype;
    int max_params;
{
    SONARLIST list;       /* defined in MESS_STRUCT.H */
    int i, tlen, ptype, j;
    BOOL ok = TRUEVAL;
    BOOL done = FALSEVAL;
    BOOL get_next_param = FALSEVAL;
    char buff[MAX_MESSAGE_LEN];
    char *strend = *str;
    PARAM_VALUE_TYPE val_type;
    ATTRIBUTE_QUALIFIER qual;
    int sonar_number = -1;
    int paren_depth = 1;

    /* initialize values to zero or null */
    for (i = 0; i < MAX_SONARS; i++) {
        for (j = 0; j < N_SONAR_PARAMS; j++) {
            list[i].sonar_params[j] = FALSEVAL;;
        }
        list[i].sonar_type.enum_value = 0;
        list[i].sonar_type.enum_string = NULL;
        list[i].sonar_status.enum_value = 0;
        list[i].sonar_status.enum_string = NULL;
        list[i].sonar_mode.enum_value = 0;
        list[i].sonar_mode.enum_string = NULL;
        list[i].sonar_depth = -1;
    }

   while (!done) { /* Start parsing parameters */
        tlen = 0;

        /* return a param keyword in buff */
        get_par_keyword (&strend, buff, &tlen, &ok, &done); 
        if (!ok) {
            init_ptable (table, max_params);
            return (FALSEVAL);
        }

        if (tlen > 0) {
            ptype = get_enum_value (
                    ptable_lookup_by_mess_type (mtype, msubtype), 
                    buff, max_params);

            paren_depth++;
        } /* end if tlen > 0 */

        /* hande duplicate attributes */
        if (ptype == tt_attr_type)   ptype = tt_attr_sonar_type;
        if (ptype == tt_attr_status) ptype = tt_attr_sonar_status;
        if (ptype == tt_attr_mode)   ptype = tt_attr_sonar_mode;
        if (ptype == tt_attr_depth)  ptype = tt_attr_sonar_depth;

        /* Get the type of the param. value */
        val_type = table[ptype].param_type;  
        /* Get the parameter value */
        get_par_value (&strend, buff, &qual, val_type);
    
        switch (val_type) {  
            case value_int: 
                    list[sonar_number].sonar_depth = atoi(buff);
                    list[sonar_number].sonar_params[3] = TRUEVAL;
                    table[ptype].param_present = TRUEVAL;
                    break;
            case value_enum: { /* status, type, mode */
                    int emax, eval;
                    char *etab;
            
                    pval_table_lookup_by_type (mtype,msubtype,ptype,
                                                        &etab,&emax);
                    /* hande duplicate attributes */
                    if (ptype == tt_attr_type)   ptype = tt_attr_sonar_type;
                    if (ptype == tt_attr_status) ptype = tt_attr_sonar_status;
                    if (ptype == tt_attr_mode)   ptype = tt_attr_sonar_mode;
                    if (ptype == tt_attr_depth)  ptype = tt_attr_sonar_depth;

                    if (emax == 0) {
                        alsp_console_message ("sonar attribute enum failure.");
                    } else { 
                        eval = get_enum_value (etab, buff, emax);
                        if (ptype == tt_attr_sonar_type) {
                            table[tt_attr_sonar_type].param_stat  = par_required;
                            table[tt_attr_sonar_status].param_stat = par_required;
            
                            sonar_number++;
                            list[sonar_number].sonar_type.enum_value = 
                                eval;
                            list[sonar_number].sonar_type.enum_string =
                                get_keyword(etab,eval);
                            list[sonar_number].sonar_params[0] = TRUEVAL;
                        }
                        else if (ptype == tt_attr_sonar_status) {
                            list[sonar_number].sonar_status.enum_value = 
                                eval;
                            list[sonar_number].sonar_status.enum_string = 
                                get_keyword(etab,eval);
                            list[sonar_number].sonar_params[1] = TRUEVAL;
                        }
                        else if (ptype == tt_attr_sonar_mode) {
                            list[sonar_number].sonar_mode.enum_value = 
                                eval;
                            list[sonar_number].sonar_mode.enum_string = 
                                get_keyword(etab,eval);
                            list[sonar_number].sonar_params[2] = TRUEVAL;
                        }
                    table[ptype].param_present = TRUEVAL;
                    } /* end if emax != 0 */
            } /* end of case value_enum */
            break;
            default:
                    break;
        } /* end of switch */         

        while ((strend [0] == END_PARAM [0]) &&
                (paren_depth != 0)) {
                    paren_depth--;
                    strend++;
        }                
        if (paren_depth == 0) 
            done = TRUEVAL;

    } /* end while (!done) */

    
    if (sonar_number >= 0) {
        /* add one to sonar_number since it started at zero */
        sonar_number++;
        set_par_sonar_list(par, list, &sonar_number);
    }
    strend--; /* move pointer back one space */
    *str = strend;
    return (ok);
}

/* Parse for IFF wrapper attribute.  The attribute format:   */
/*  iff(master(on/off)mode1(status(on/off)mode(mode1 value)) */
/*                    mode2(status(on/off)mode(mode2 value)) */
/*                    mode3(status(on/off)mode(mode3 value)) */
/*                    modec(status(on/off)mode(modec value)) */
/*                    mode4(on/off))                         */
/*  The interior sub-attributes are optional for any single  */
/*  update/create message.    smp - 2/98 (E1907)             */

static BOOL parse_iffmode_list (par, str, m, table, mtype, msubtype, 
                                max_params)

    MESS_PARAM_LIST *par;
    char **str;
    MESSAGE_STRUCT *m;
    PARAM_DESC_TAB table;
    MESSAGE_DATA_TYPE mtype;
    int msubtype;
    int max_params;
{
/*    IFFMODELIST listIFF;        defined in MESS_STRUCT.H */
    IFFMODESTR iffmode_array[N_IFF_PARAMS];
    int i, tlen, ptype, j;
    BOOL ok = TRUEVAL;
    BOOL done = FALSEVAL;
    BOOL get_next_param = FALSEVAL;
    BOOL iffmode_master_params = FALSEVAL;
    char buff[MAX_MESSAGE_LEN];
    char *strend = *str;
    PARAM_VALUE_TYPE val_type;
    ATTRIBUTE_QUALIFIER qual;
    int iff_number = -1;
    int paren_depth = 1;
    int primary_ptype = 0;

    /* initialize values to zero or null */
    for (i = 0; i < N_IFF_PARAMS; i++) {
        iffmode_array[i].iffmode_params = FALSEVAL;
        iffmode_array[i].iffmode_value = 0;
        iffmode_array[i].iffmode_status.enum_value = 0;
        iffmode_array[i].iffmode_status.enum_string = NULL;
   }

   while (!done) { /* Start parsing parameters */
        tlen = 0;

        /* return a param keyword in buff */
        get_par_keyword (&strend, buff, &tlen, &ok, &done); 
        if (!ok) {
            init_ptable (table, max_params);
            return (FALSEVAL);
        }

        /* if this is the @ALL qualifier, handle it here */
        if (strend [0] == QUAL_SYMBOL [0] && iffmode_context == TRUEVAL) {
            get_par_value (&strend, buff, &qual, val_type);
            set_par_attr_qual (par, qual);
        }

        if (tlen > 0) {
            ptype = get_enum_value (
            ptable_lookup_by_mess_type (mtype, msubtype), 
            buff, max_params);

            paren_depth++;
        } /* end if tlen > 0 */

        if (ptype == tt_attr_status) ptype = tt_attr_iffmode_status;
        if (ptype == tt_attr_mode) ptype = tt_attr_iffmode_value;

        if (ptype == tt_attr_iffmode_master) {
            table[tt_attr_iffmode_status].param_stat = par_required;
            table[tt_attr_iffmode_value].param_stat = par_optional;
            iffmode_array[0].iffmode_params = TRUEVAL;
            primary_ptype = tt_attr_iffmode_master;
        } /* end MASTER */
        else if (ptype == tt_attr_iffmode1) {
            table[tt_attr_iffmode_status].param_stat = par_required;
            table[tt_attr_iffmode_value].param_stat = par_optional;
            iffmode_array[1].iffmode_params = TRUEVAL;
            primary_ptype = tt_attr_iffmode1;
        } /* end MODE1 */
        else if (ptype == tt_attr_iffmode2) {
            table[tt_attr_iffmode_status].param_stat = par_required;
            table[tt_attr_iffmode_value].param_stat = par_optional;
            iffmode_array[2].iffmode_params = TRUEVAL;
            primary_ptype = tt_attr_iffmode2;
        } /* end MODE2 */
        else if (ptype == tt_attr_iffmode3) {
            table[tt_attr_iffmode_status].param_stat = par_required;
            table[tt_attr_iffmode_value].param_stat = par_optional;
            iffmode_array[3].iffmode_params = TRUEVAL;
            primary_ptype = tt_attr_iffmode3;
        } /* end MODE3 */
        else if (ptype == tt_attr_iffmodeC) {
            table[tt_attr_iffmode_status].param_stat = par_required;
            table[tt_attr_iffmodeC].param_stat = par_optional;
            iffmode_array[4].iffmode_params = TRUEVAL;
            primary_ptype = tt_attr_iffmodeC;
         } /* end MODEC */ 
        else if (ptype == tt_attr_iffmode4){
            table[tt_attr_iffmode_status].param_stat = par_required;
            iffmode_array[5].iffmode_params = TRUEVAL;
            primary_ptype = tt_attr_iffmode4;
        } /* end MODE4 */

        if ((ptype == tt_attr_iffmode_master) ||
            (ptype == tt_attr_iffmode4)       ||
            (ptype == tt_attr_iffmode_status) ||
            (ptype == tt_attr_iffmode_value))  {         
            
            /* Get the type of the param. value */
            val_type = table[ptype].param_type;  
            /* Get the parameter value */
            get_par_value (&strend, buff, &qual, val_type);
    
            if (val_type == value_enum) { /* MASTER, STATUS */
                int emax, eval;
                char *etab;
                
                pval_table_lookup_by_type (mtype,msubtype,ptype,
                                                    &etab,&emax);
                if (emax == 0) {
                    alsp_console_message ("Enum lookup failed in alsp_parse.");
                } 
                else { 
                    eval = get_enum_value (etab, buff, emax);
                      
                    if (primary_ptype == tt_attr_iffmode_master) {
                        iffmode_array[0].iffmode_status.enum_value = 
                            eval;
                        iffmode_array[0].iffmode_status.enum_string = 
                            get_keyword(etab,eval);
                        iffmode_array[0].iffmode_params = TRUEVAL;
                        table[tt_attr_iffmode_status].param_stat = par_dependable;
                    } /* end MASTER */
                    else if (primary_ptype == tt_attr_iffmode1) {
                        iffmode_array[1].iffmode_status.enum_value = 
                            eval;
                        iffmode_array[1].iffmode_status.enum_string = 
                            get_keyword(etab,eval);
                        iffmode_array[1].iffmode_params = TRUEVAL;
                        table[tt_attr_iffmode_status].param_stat = par_dependable;
                    } /* end MODE1 */
                    else if (primary_ptype == tt_attr_iffmode2) {
                        iffmode_array[2].iffmode_status.enum_value = 
                            eval;
                        iffmode_array[2].iffmode_status.enum_string = 
                            get_keyword(etab,eval);
                        iffmode_array[2].iffmode_params = TRUEVAL;
                        table[tt_attr_iffmode_status].param_stat = par_dependable;
                    } /* end MODE2 */
                    else if (primary_ptype == tt_attr_iffmode3) {
                        iffmode_array[3].iffmode_status.enum_value = 
                            eval;
                        iffmode_array[3].iffmode_status.enum_string = 
                            get_keyword(etab,eval);
                        iffmode_array[3].iffmode_params = TRUEVAL;
                        table[tt_attr_iffmode_status].param_stat = par_dependable;
                    } /* end MODE3*/
                    else if (primary_ptype == tt_attr_iffmodeC) {
                        iffmode_array[4].iffmode_status.enum_value = 
                            eval;
                        iffmode_array[4].iffmode_status.enum_string = 
                            get_keyword(etab,eval);
                        iffmode_array[4].iffmode_params = TRUEVAL;
                        table[tt_attr_iffmode_status].param_stat = par_dependable;
                    } /* end MODEC */
                    else if (primary_ptype == tt_attr_iffmode4) {
                        iffmode_array[5].iffmode_status.enum_value = 
                            eval;
                        iffmode_array[5].iffmode_status.enum_string = 
                            get_keyword(etab,eval);
                        iffmode_array[5].iffmode_params = TRUEVAL;
                        table[tt_attr_iffmode_status].param_stat = par_dependable;
                        primary_ptype = 0; /* MODE4 has no MODE value */
                    } /* end MODE4 */
                } /* enum lookup worked */
            } /* end of STATUS */
            else if (val_type == value_int) { /* iff modes: mode1, mode2, mode3, modec */
                if (primary_ptype == tt_attr_iffmode1){
                    iffmode_array[1].iffmode_value = atoi(buff);
                    iffmode_array[1].iffmode_params = TRUEVAL;
                    table[ptype].param_present = TRUEVAL;
                    primary_ptype = 0;
                } /* end MODE1 */
                else if (primary_ptype == tt_attr_iffmode2){
                    iffmode_array[2].iffmode_value = atoi(buff);
                    iffmode_array[2].iffmode_params = TRUEVAL;
                    table[ptype].param_present = TRUEVAL;
                    primary_ptype = 0;
                } /* end MODE2 */
                else if (primary_ptype == tt_attr_iffmode3){
                    iffmode_array[3].iffmode_value = atoi(buff);
                    iffmode_array[3].iffmode_params = TRUEVAL;
                    table[ptype].param_present = TRUEVAL;
                    primary_ptype = 0;
                } /* end MODE3 */
                else if (primary_ptype == tt_attr_iffmodeC){
                    /* Put altitude info into array  */
                    iffmode_array[4].iffmode_value =  atoi(buff);   
                    iffmode_array[4].iffmode_params = TRUEVAL;
                    table[ptype].param_present = TRUEVAL;
                    primary_ptype = 0;
                } /* end MODEC */
            } /* case value_int */
        } /* end if MASTER, STATUS or MODE keyword */
        while ((strend [0] == END_PARAM [0]) &&
               (paren_depth != 0)) {
            paren_depth--;
            strend++;
        }   /* end while strend [0]  */
                
    if (paren_depth == 0) 
       done = TRUEVAL;

    } /* end while (!done) */
    set_par_iffmode_list(par, iffmode_array);

    strend--; /* move pointer back one space */
    *str = strend;
    return (ok);
}

/* Parse for alsp filter criteria as specified by a range of integer or floats */
static BOOL parse_range (par, str, pvtype)

        MESS_PARAM_LIST *par;
        char *str;
        PARAM_VALUE_TYPE pvtype;
{
        char *tstr = str;
        char *tstr2;
        char buff [80];
        int tlen = 0;
        int loweri, upperi;
        float lowerf, upperf;

        tstr2 = tstr;
        debug_print ("ALSP_PARSE");
        debug_print (str);
        while (tstr[0] != LIST_DELIM [0]) {
                if (tstr [0] == '\0') {
                        alsp_console_message 
                                ("Syntax error in alsp_parse (parse_range).");
                        return (FALSEVAL);
                } else {
                        tlen++;
                        tstr++;
                }
        }
        strncpy (buff, tstr2, tlen);
        *(buff+tlen)  = '\0';

        if (pvtype == value_int_range) {
                loweri = atoi (buff);
        } else {
                lowerf = atof (buff);
        }
        tstr++;
        tstr2 = tstr;
        tlen = 0;
        while (tstr[0] != '\0') {
                tlen++;
                tstr++;
        }
        strncpy (buff, tstr2, tlen);
        *(buff+tlen)  = '\0';
        if (pvtype == value_int_range) {
                upperi = atoi (buff);
                set_par_int_range (par, loweri, upperi);
        } else {
                upperf = atof (buff);
                set_par_float_range (par, lowerf, upperf);
        }
        return (TRUEVAL);
}

/* Parse the following ALSP parameter list items:
 * integer lists
 * enumeration value lists
 * attribute lists
 * float lists     # added for JECEWSI ICD - 10/94 clk
 */
static void parse_list (par, str, mtype, msubtype, ptype, pvtype)

        MESS_PARAM_LIST     *par;
        char                *str;
        MESSAGE_DATA_TYPE   mtype;
        int                 msubtype;
        int                 ptype;
        PARAM_VALUE_TYPE    pvtype;
{
        int                 nvals = 0;
        ALSP_ATTRIBUTE_LIST a;
        INTLIST             ilist;
        FLOATLIST           flist;    /* type defined in mess_struct.h */
        ENUMLIST            elist;
        char                *tstr, *tstr2;
        int                 i, tlen;
        char                listbuff [MAX_MESSAGE_LEN];
        int                 emax, eval;
        char                *etab;

        switch (pvtype) {
                case value_attribute_list:
                        for (i = 0; i <= N_ATTRIBUTE_VALS; i++) a [i] = FALSEVAL;
                        break;
                case value_int_list:
                        for (i = 0; i < MAX_LIST; i++) ilist [i] = 0;
                        break;
                case value_enum_list:
                        for (i = 0; i < MAX_LIST; i++) {
                                elist [i].enum_value = 0;
                                elist [i].enum_string = NULL;
                        }
                        break;
                case value_float_list:
                        for (i = 0; i < MAX_LIST; i++) flist [i] = 0.0;
                        break;
                default:
                        alsp_console_message ("Bad param value type in parse list.");
                        exit (0);
        }
        tstr = str;
        while (tstr [0] != '\0') {
                tlen = 0;  tstr2 = tstr;
                while (tstr[0] != LIST_DELIM [0]) {
                        if (tstr [0] == '\0') break;
                        tlen++;
                        tstr++;
                }
                nvals++;
                if (tstr [0] != '\0') tstr++;
                strncpy (listbuff, tstr2, tlen);
                *(listbuff+tlen)  = '\0';

                /* convert incoming message to upper case just in case  SMM 2/95 */
/*                for (i = 0; i <= tlen; i++) listbuff[i] = toupper(listbuff[i]);
*/
                switch (pvtype) {
                        case value_attribute_list:
                                a [get_enum_value (tt_attribute_keytab,
                                        listbuff, N_ATTRIBUTE_VALS)] = TRUEVAL;
                                break;
                        case value_int_list:
                                ilist [nvals - 1] = atoi (listbuff);
                                break;
                        case value_enum_list:
                                pval_table_lookup_by_type (mtype, msubtype, 
                                        ptype, &etab, &emax);
                                if (emax == 0) {
                                        alsp_console_message
                                                ("Enum lookup failed in alsp_parse.");
                                } else {
                                        /* Use ptr into static string table! */
                                        eval = get_enum_value (etab, listbuff, emax);
                                        elist [nvals - 1].enum_value = eval;
                                        elist [nvals - 1].enum_string = 
                                                                        get_keyword (etab, eval);
                                }
                                break;
                        case value_float_list:
                                flist [nvals - 1] = atof (listbuff);
                                break;
                }
        }
        switch (pvtype) {
                case value_attribute_list:
                        set_par_attr_list (par, a);
                        break;
                case value_int_list:
                        set_par_int_list (par, ilist, nvals);
                        break;
                case value_enum_list:
                        set_par_enum_list (par, elist, nvals);
                        break;
                case value_float_list:
                        set_par_float_list (par, flist, nvals);
                        break;
        }
}

/* Handle the special case of air unit size.  Tables assume size is integer,
 * which is not correct for air units.
 */
static void fix_air_unit_size_if_needed (m)

        MESSAGE_STRUCT  *m;             /* Input: stored with param. type (ptype) */
{                                   /*        and param. value. */
        MESS_PARAM_LIST *class, *size;
        int             c, s;  

        class = lookup (tt_attr_class, m);
        if (class != NULL) {
                c = get_par_enum_value (class);
                if ((c == (int) pval_tt_class_air) ||
                        (c == (int) pval_tt_class_air_fixed) ||
                        (c == (int) pval_tt_class_air_heli) ||
                        (c == (int) pval_tt_class_air_cruise_missile) ||
                        (c == (int) pval_tt_class_air_tbm)) {
                        size = lookup (tt_attr_unit_size, m);
                        if (size != NULL) {
                                s = get_par_enum_value (size);
                                set_par_int_value (size, s);
                        }
                }
        }
}

/* Parse the ALSP parameter list.
 */
static char *parse_params (m, str, mtype, msubtype)

MESSAGE_STRUCT      *m;      /* Output: stored with parameter type and value */ 
char                *str;    /* Input: pointer to a string for parse */
MESSAGE_DATA_TYPE   mtype;   /* Input: message type at, ta, or tt */
int                 msubtype;/* Input: index to table ??_mess_keytab */
{
    char                buff [MAX_MESSAGE_LEN];
    char                *strend, *strbeg;
    BOOL                done                = FALSEVAL;
    BOOL                ok                  = TRUEVAL;
    int                 tlen;
    int                 ptype, pmax;
    MESS_PARAM_LIST     *par;
    PARAM_DESCRIPTOR    **partab;
    MESSAGE_STRUCT      *mstruct;
    BOOL                special_case;
    BOOL                salvo_set           = FALSEVAL;
    BOOL                create_or_update    = FALSEVAL;
    BOOL                interaction         = FALSEVAL;
    BOOL                supply_pol_set      = FALSEVAL;
    ATTRIBUTE_QUALIFIER qual;
    PARAM_VALUE_TYPE    val_type;
/*    EMITTERLIST         list;    /* structure for emitter information */
/*    SONARLIST        sonar_list; /* structure for sonar information */
/*    IFFMODESTR iffmode_array[N_IFF_PARAMS];    /* structure for iff mode info - smp 2/98 (E1907) */

    if (mtype == acm_actor_mess) {      /* number of parameters of a message */
         pmax   = at_max_params [msubtype]; /*  is finite. */
         partab = at_params;                 /* pmax - number of parameter in */
         if ((msubtype == type_at_create) || /*  a message.                  */
             (msubtype == type_at_update)) { /* partab - a list of pointers pointing */
             create_or_update = TRUEVAL; /* to the specific message parameter descriptor list. */
         } 
         else if (msubtype == type_at_interaction) {
             interaction = TRUEVAL;
         }
    } 
    else { 
         pmax   = ta_max_params [msubtype];
         partab = ta_params;
    }

    strend = str + 1;
    if (pmax == 0) return (strend);

    while (!done) { /* Start parsing parameters */
         tlen         = 0;
         strbeg       = strend;
         special_case = FALSEVAL;

         if (pmax > 1) { /* Keyword present */
                           /* return a param. keyword in buff */
             get_par_keyword (&strend, buff, &tlen, &ok, &done); 
             if (!ok) {
                 init_ptable (partab [msubtype], pmax);
                 return (NULL);
             }
         }
         if ((tlen > 0) || (pmax == 1)) {
             if (pmax > 1) { /* return index of table ??_??_par_keytab */
                 ptype = get_enum_value (
                    /* return pointer to table ??_??_par_keytab */ 
                    ptable_lookup_by_mess_type (mtype, msubtype), buff, pmax);                    
                    /* Very ugly but we have overloaded keywords added at the last minute.*/
                 if ((salvo_context == FALSEVAL) &&
                     (supply_pol_context == FALSEVAL)) {
                     /* still parsing string outside of SALVO */
                     /*  but inside of PARAMETERS */
                     if ((interaction == TRUEVAL) && 
                         (ptype == par_tt_int_salvo_num)) {
                                        /* ptype is NUMBER which is inside */
                        /* INTERACTION(PARAMETERS(SALVO(NUMBER(--)))) */
                         ptype = par_tt_int_rep_num; 
                     }
                 }

                 if (supply_pol_context == TRUEVAL) {
                     /* still parsing string outside of SUPPLY_POL */
                     /*  but inside of PARAMETERS */
                     if ((interaction == TRUEVAL) && 
                         (ptype == par_tt_int_salvo_num)) {
                                        /* ptype is NUMBER which is inside */
                        /* INTERACTION(PARAMETERS(SUPPLY_POL(TYPE(--)NUMBER(--)))) */
                         ptype = par_tt_int_supply_qty; 
                         /* supply_pol_context = FALSEVAL; */
                     }
                 }
                 if (ptype == 0) {
                     /* Here is the special case where 'SALVO' */
                     /* keyword not appear in tt_interaction_par_keytab */
                     if (strncmp (buff, "SALVO", strlen (buff)) == 0) {
                         salvo_set = TRUEVAL;
                     }
                     if (strncmp (buff, "SUPPLY_POL", strlen (buff)) == 0) {
                         supply_pol_set = TRUEVAL;
                     }
                     if ((strncmp (buff,"PARAMETERS", strlen (buff)) == 0) ||
                         (salvo_set == TRUEVAL) ||
                         (supply_pol_set == TRUEVAL)) {
                         if (salvo_set == TRUEVAL) {
                            /* parse string inside SALVO here */
                            salvo_context = TRUEVAL;
                         }      
                         if (supply_pol_set == TRUEVAL) {
                            /* parse string inside SUPPLY_POL */
                            supply_pol_context = TRUEVAL;
                         }      
                                /* Recursive call */
                         strend  = parse_params (m, strend - 1, mtype,msubtype);
						 if (!strend) {
							return (NULL);
						 }

                         if (salvo_set == TRUEVAL) {
                             salvo_context = FALSEVAL;
                         }      
                         if (supply_pol_set == TRUEVAL) {
                            supply_pol_context = FALSEVAL;
                         }      
                         special_case    = TRUEVAL;
                     }
                     else { 
                            char error_buff[256];
/*                          alsp_console_message
                            ("Unrecognized param type in alsp_parse."); */
                            sprintf (error_buff,"Unrecognized param type in alsp_parse: %s\n", buff);
                            alsp_console_message(error_buff);
                          init_ptable (partab [msubtype], pmax);
                          return (NULL);
                     }
                 }
                 else if (ptype == tt_attr_emitter) {
                     partab [msubtype][tt_attr_jammer].param_stat = par_optional;
                     partab [msubtype][tt_attr_radar].param_stat  = par_optional;
                     emitter_context = TRUEVAL;
                 }
                 else if (ptype == tt_attr_c2w_factors) {     /* JECEWSI ICD - 10/94 */
                     c2w_factors_context = TRUEVAL;
                     partab [msubtype][tt_attr_unit].param_stat = par_optional;
                     partab [msubtype][tt_attr_emtr].param_stat = par_optional;
                     partab [msubtype][ptype].param_present = TRUEVAL;   
                     par = new_param (ptype, m);
                     set_par_int_value (par, ptype);
                     strend  = parse_params (m, strend - 1, mtype,msubtype);
					 if (!strend) {
						return (NULL);
					 }
                     c2w_factors_context = FALSEVAL;
                     special_case        = TRUEVAL;
                 } 
                 else if ((ptype == tt_attr_unit || ptype == tt_attr_emtr) &&
                          (c2w_factors_context == TRUEVAL)                  ) {
                     partab [msubtype][tt_attr_ew_c3].param_stat = par_optional;
                     partab [msubtype][tt_attr_ew_acq].param_stat = par_optional;
                     partab [msubtype][tt_attr_ew_launch].param_stat = par_optional;
                     partab [msubtype][tt_attr_ew_guide].param_stat = par_optional;
                     partab [msubtype][tt_attr_ew_power].param_stat = par_optional;
                     partab [msubtype][tt_attr_ew_surf].param_stat = par_optional;
                     partab [msubtype][ptype].param_present = TRUEVAL;   
                     par = new_param (ptype, m);
                     set_par_int_value (par, ptype);
                     strend  = parse_params (m, strend - 1, mtype,msubtype);
					 if (!strend) {
						return (NULL);
					 }
                     special_case = TRUEVAL;
                 } 
             } /* end if (pmax > 1) */
             else { 
                 ptype = 1;
             } /* End of if..then..else */

             if (!special_case) {
                 /* khc,3/93- A parameter that has status set to par_dependable */
                 /* (no meaning) is encountered. */
                 if (partab [msubtype][ptype].param_stat == par_dependable) {
                     init_ptable (partab [msubtype], pmax);
                     return (NULL);
                 }
                 par = new_param (ptype, m);
                 /* if not end ), so there is a value */
                 if (strend [0] != END_PARAM [0]) {  
                     /* Set flag to table ??_??_partype */
                     partab [msubtype][ptype].param_present = TRUEVAL;   
                 }
                 /* Get the type of the param. value */
                 val_type = partab [msubtype][ptype].param_type;  
                 if (val_type == value_loc_struct) {
                     ok = parse_loc (par, &strend);
                     if (ok != TRUEVAL) {
                         init_ptable (partab [msubtype], pmax);
                         return (NULL);
                     }
                 } 
                 else if (val_type == value_loc_struct_list) {
                    ok = parse_loc_list (par, &strend);
                    if (ok != TRUEVAL) {
                        init_ptable (partab [msubtype], pmax);
                        return (NULL);
                    }
                 } 
                 else if (val_type == value_emit_struct_list) {
                     ok = parse_emitter_list (par, &strend, m, partab[msubtype],  
                                         mtype, msubtype, pmax);
                     if (ok != TRUEVAL) {
                         init_ptable (partab [msubtype], pmax);
                         return (NULL);
                     }
                    emitter_context = FALSEVAL;
                 }
                 else if (val_type == value_sonar_struct_list) {
                     ok = parse_sonar_list (par, &strend, m, partab[msubtype],  
                                         mtype, msubtype, pmax);
                     if (ok != TRUEVAL) {
                         init_ptable (partab [msubtype], pmax);
                         return (NULL);
                     }
                 }
                 else if (val_type == value_iff_struct_list) {   /* smp 2/98 (E1907) */
                     ok = parse_iffmode_list (par, &strend, m, partab[msubtype],
                                              mtype, msubtype, pmax);
                     if (ok != TRUEVAL) {
                         init_ptable (partab [msubtype], pmax);
                         return (NULL);
                     }
                    iffmode_context = FALSEVAL;     /* check out */
                 }
                 else { /* pull parameter value and return in buff */
                     get_par_value (&strend, buff, &qual, val_type);
                     set_par_attr_qual (par, qual);
                 }

                 switch (partab [msubtype][ptype].param_type) {
                     case value_int:
                        set_par_int_value (par, atoi (buff));
                        break;
                     case value_enum: {
                         int emax, eval;
                         char *etab;
    
                         pval_table_lookup_by_type (mtype,msubtype,ptype,
                                                    &etab,&emax);
                         if (emax == 0) {
                             alsp_console_message
                             ("Enum lookup failed in alsp_parse.");
                         } 
                         else {
                             eval = get_enum_value (etab, buff, emax);
                             /* if the attribute is size, then it can be enumerated or an */
                             /* integer values, so make sure integer sizes are processed  */
                             /* SMM 7/97 (E1861)                                          */
                             if ((eval == 0) & (ptype == tt_attr_unit_size)) { 
                                 set_par_enum_value (par, atoi(buff));
                                 /* Use ptr to static string table! */
                                 set_par_enum_string (par,
                                     get_keyword (tt_unit_size_keytab, 0));
                             } 
                             else { 
                                 set_par_enum_value (par, eval);
                                 /* Use ptr to static string table! */
                                 set_par_enum_string (par, 
                                     get_keyword(etab,eval));
                             }
                         }
                     }
                     break;
                     case value_float:
                        set_par_float_value (par, atof (buff));
                        break;
                     case value_string:
                        set_par_string_value (par, buff);
                        break;
                     case value_message:
                        mess_nest_level ++;
                        strend = alsp_parse (strbeg, &mstruct);
                        mess_nest_level --;
                        set_par_mess_value (par, mstruct);
                        break;
                     case value_attribute_list:
                        parse_list (par, buff,mtype, msubtype, ptype, 
                                    value_attribute_list);
                        break;
                     case value_int_range:
                        ok = parse_range (par, buff, value_int_range);
                        if (ok != TRUEVAL) {
                             init_ptable (partab [msubtype], pmax);
                             return (NULL);
                        }
                        break;
                     case value_float_range:
                        ok = parse_range (par, buff, value_float_range);
                        if (ok != TRUEVAL) {
                             init_ptable (partab [msubtype], pmax);
                             return (NULL);
                        }
                        break;
                     case value_int_list:
                        parse_list (par,buff,mtype,msubtype,ptype,value_int_list);
                        break;
                     case value_float_list:
                        parse_list (par,buff,mtype,msubtype,ptype,value_float_list);
                        break;
                     case value_enum_list:
                        parse_list (par,buff,mtype,msubtype,ptype,value_enum_list); 
                        break;
                     default:
                        break;
                 }   /* End of switch */
             }   /* End of if (!special_case) */

            if (pmax > 1)
                strend++; 
    
    /* need to work on this since it currently will never happen */
            /* KHC, 3/93 - Case of qualifier, @ALL,  appear inside EMITTER() */
            if (strend [0] == QUAL_SYMBOL [0] && emitter_context == TRUEVAL) {
                 get_par_value (&strend, buff, &qual, val_type);
                 set_par_attr_qual (par, qual);
            }

            if (strend [0] == END_PARAM [0]) {
                 done = TRUEVAL;
            }

         }   /* End of if ((tlen > 0) || (pmax == 1)) */
    }   /* End of while (!done) */

    if (!required_params_ok (partab [msubtype], pmax)) {
         alsp_console_message("Required parameter missing in alsp_parse.");
         init_ptable (partab [msubtype], pmax);
         return (NULL);
        }

    if (emitter_context == FALSEVAL &&
        c2w_factors_context == FALSEVAL) {
         /* KHC,3/93 Avoid a recursive call to reset the appear param. */
         init_ptable (partab [msubtype], pmax);  /* flag to false. */

         if (create_or_update) {
             /* And avoid a recursive call to call */
             /* Fix mstruct special case */      /* fix_air_unit_size_if_needed() more than */
             fix_air_unit_size_if_needed (m);    /* once. */
            }
        }
    return (strend);

}   /* End of parse_params() */

/* Parse an alsp message. */
char *alsp_parse (mess, mstruct)

char            *mess;      /* Incoming message */
MESSAGE_STRUCT  **mstruct;  /* Output structure */
{
    char                *t, *start, *end;
    int                 tlen = 0;
    char                buff [80];
    int                 msubtype;
    MESSAGE_DATA_TYPE   mtype;
    MESSAGE_STRUCT      *m;
    char                *next_char;
    int                 i;
    
    t       = mess;

    /* convert incoming message to upper case just in case  SMM 2/95 */
/*    for (i = 0; i <= MAX_MSG_LEN; i++) mess[i] = toupper(mess[i]);
*/
    mtype   = acm_actor_mess;
    while (t [0] == ' ') t++;    /* remove any leading blanks */
    start = t;
    while (t [0] != START_PARAM [0])           /* get message header */
        {
         if (t [0] == '\0') 
            {
             alsp_console_message ("Unrecognized message in alsp_parse");
             return (NULL);
            }
         t++; 
         tlen++;
        }
    end = t - 1;
    while (end [0] == ' ') 
        {
         tlen--;
         end--;
        }

    strncpy (buff, start, tlen);
    *(buff+tlen)  = '\0';

    /* convert incoming message to upper case just in case  SMM 2/95 */
/*    for (i = 0; i <= tlen; i++) buff[i] = toupper(buff[i]);
*/
    msubtype = get_enum_value (at_mess_keytab, buff, N_AT_MESS);

    if (msubtype == 0) 
        {
         mtype = actor_acm_mess;
         msubtype = get_enum_value (ta_mess_keytab, buff, N_TA_MESS);
         if (msubtype == 0) 
            {
             alsp_console_message ("Unrecognized message in alsp_parse");
             return (NULL);
             /* Don't need to check tt types since they also parse as at.*/
            }
        }

    m = new_message (mtype, msubtype);
    *mstruct = m;

    next_char = parse_params (m, t, mtype, msubtype);
    if ((mess_nest_level == 0) && (next_char != NULL)) 
        {
         if (mtype == acm_actor_mess) 
            {
             (*(at_handlers [msubtype])) (m);
            } 
         else 
            {
             (*(ta_handlers [msubtype])) (m);
            }
        }
    free_message (m);       /* Wayne Anderson 24-JUN-93 */
    return (next_char);     /* Plug memory leak */
}

PARAM_VALUE_TYPE get_alsp_param_data_type (mtype, msubtype, ptype)

        MESSAGE_DATA_TYPE mtype;
        int msubtype;
        int ptype;
{
        PARAM_DESCRIPTOR **partab;

        if (mtype == acm_actor_mess) {
                partab = at_params;
        } else {
                partab = ta_params;
        }
        return (partab[msubtype][ptype].param_type);

}
