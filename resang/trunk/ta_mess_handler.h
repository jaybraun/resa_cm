
/* Interface to trans-acm message handlers 
 *
 * The message handlers below are
 *  a) invoked by the parser, and
 *  b) passed a (pointer to a) MESSAGE_STRUCT.  
 *
 * The MESSAGE_STRUCTs are constructed by the parser and
 * contain a format-independent representation of an incoming message.
 * See file mess_struct.h for the MESSAGE_STRUCT definition.
 *
 * There should be one message handler for each trans_acm message type
 * defined in mess_struct.h.
 */

extern void handle_ta_advance_request ();
extern void handle_ta_cancel_lock ();
extern void handle_ta_create_request ();
extern void handle_ta_delete ();
extern void handle_ta_dispatch ();
extern void handle_ta_event ();
extern void handle_ta_event_request ();
extern void handle_ta_filter_attr ();
extern void handle_ta_filter_class ();
extern void handle_ta_filter_interaction ();         /* clk 12/93 */
extern void handle_ta_id_req ();
extern void handle_ta_join ();
extern void handle_ta_lock_req ();
extern void handle_ta_join ();
extern void handle_ta_ping ();                       /* clk  1/94 */
extern void handle_ta_ping_ack ();                   /* clk  1/94 */
extern void handle_ta_reg_attrs ();
extern void handle_ta_reg_object ();
extern void handle_ta_resign ();
extern void handle_ta_unlock ();
extern void handle_ta_save_request ();
extern void handle_ta_save_ack ();
extern void handle_ta_save_complete ();
extern void handle_ta_filter_parameters ();  /* SMM 9/96 (E1691) */
extern void handle_ta_error ();              /* SMM 9/96 (E1692) */
