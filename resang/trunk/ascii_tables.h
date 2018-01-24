
/* This file defines an interface to a set of routines and tables that
 * are used together to convert between values of enumerated types
 * and ascii strings.
 */


#define START_PARAM		"("
#define END_PARAM		")"
#define DELIMITER_LEN 	1

#define LIST_DELIM 		","
#define QUAL_SYMBOL		"@"
#define PERIOD_SYMBOL   "."  /* SMM 3/98 (E1956) */

typedef char *KEYSTRING_TABLE [];

/* Ascii tables ..... */
extern KEYSTRING_TABLE mess_type_keytab;
extern KEYSTRING_TABLE ta_mess_keytab;
extern KEYSTRING_TABLE at_mess_keytab;
extern KEYSTRING_TABLE tt_mess_keytab;

extern KEYSTRING_TABLE attr_qual_keytab;

extern KEYSTRING_TABLE ta_adv_req_par_keytab;
extern KEYSTRING_TABLE ta_cancel_lock_par_keytab;
extern KEYSTRING_TABLE ta_create_req_par_keytab;
extern KEYSTRING_TABLE ta_delete_par_keytab;
extern KEYSTRING_TABLE ta_dispatch_par_keytab;
extern KEYSTRING_TABLE ta_event_par_keytab;
extern KEYSTRING_TABLE ta_fltr_attr_par_keytab;
extern KEYSTRING_TABLE tt_pval_class_keytab;
extern KEYSTRING_TABLE ta_fltr_class_par_keytab;
extern KEYSTRING_TABLE ta_fltr_cond_pval_keytab;
extern KEYSTRING_TABLE ta_id_req_par_keytab;
extern KEYSTRING_TABLE ta_join_par_keytab;
extern KEYSTRING_TABLE ta_join_tm_pval_keytab;
extern KEYSTRING_TABLE ta_lock_req_par_keytab;
extern KEYSTRING_TABLE ta_lock_serv_pval_keytab;
extern KEYSTRING_TABLE ta_reg_attr_par_keytab;
extern KEYSTRING_TABLE ta_reg_object_par_keytab;
extern KEYSTRING_TABLE ta_resign_par_keytab;            /* clk 1/94 - for additional resign parameters */
extern KEYSTRING_TABLE ta_unlock_par_keytab;
extern KEYSTRING_TABLE ta_save_req_par_keytab;
extern KEYSTRING_TABLE ta_save_ack_par_keytab;
extern KEYSTRING_TABLE ta_save_complete_par_keytab;
extern KEYSTRING_TABLE at_avail_par_keytab;
extern KEYSTRING_TABLE at_create_par_keytab;
extern KEYSTRING_TABLE tt_gr_branch_pval_keytab;
extern KEYSTRING_TABLE tt_air_branch_pval_keytab;
extern KEYSTRING_TABLE tt_side_keytab;
extern KEYSTRING_TABLE tt_unit_size_keytab;
extern KEYSTRING_TABLE tt_unit_flight_ops_keytab;
extern KEYSTRING_TABLE at_delete_par_keytab;
extern KEYSTRING_TABLE at_grant_adv_par_keytab;
extern KEYSTRING_TABLE at_join_ack_par_keytab;
extern KEYSTRING_TABLE at_lock_not_par_keytab;
extern KEYSTRING_TABLE at_lock_stat_pval_keytab;
extern KEYSTRING_TABLE at_new_id_par_keytab;
extern KEYSTRING_TABLE at_unavail_par_keytab;
extern KEYSTRING_TABLE at_unlock_par_keytab;
extern KEYSTRING_TABLE at_start_save_par_keytab;
extern KEYSTRING_TABLE at_set_rate_par_keytab;
extern KEYSTRING_TABLE tt_interaction_par_keytab;
extern KEYSTRING_TABLE tt_refresh_par_keytab;
extern KEYSTRING_TABLE tt_attribute_keytab;
extern KEYSTRING_TABLE at_signal_par_keytab;         /* clk - 9/95 (E1457) */
extern KEYSTRING_TABLE ta_reg_rate_par_keytab;       /* clk - 9/97 (E1892) */
extern KEYSTRING_TABLE tt_iffmode_status_pval_keytab;
extern KEYSTRING_TABLE tt_propulsion_mode_pval_keytab; 

/* Return parameter keyword table for given mess type & subtype */
extern char **ptable_lookup_by_mess_type ();

/* Return enum keyword table for given mess type, subtype & par type */
extern void pval_table_lookup_by_type ();

/* Return the parameter keyword */
extern char *get_param_keyword ();

/* Return the ascii keyword given table & index (enumerated value) */
extern char *get_keyword ();

/* Return the enumerated value given table & keyword string  */
extern int get_enum_value ();
