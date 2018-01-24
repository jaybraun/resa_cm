

/* Interface to acm_trans message handlers 
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

extern void handle_at_available ();
extern void handle_at_create ();
extern void handle_at_delete ();
extern void handle_at_grant_advance ();
extern void handle_at_interaction ();
extern void handle_at_join_ack ();
extern void handle_at_lock_notify ();
extern void handle_at_new_id ();
extern void handle_at_refresh ();
extern void handle_at_unavail ();
extern void handle_at_unlock_request ();
extern void handle_at_update ();
extern void handle_at_start_save ();
extern void handle_at_set_rate();
extern void handle_at_ping();                 /* clk  1/94 */
extern void handle_at_ping_ack();             /* clk  1/94 */
extern void handle_at_signal ();              /* clk - 9/95 (E1457) */

