
/* Interface to trans-trans message handlers 
 *
 * The message handlers below are
 *  a) invoked by the parser, and
 *  b) passed a (pointer to a) MESSAGE_STRUCT.  
 *
 * The MESSAGE_STRUCTs are constructed by the parser and
 * contain a format-independent representation of an incoming message.
 * See file mess_struct.h for the MESSAGE_STRUCT definition.
 *
 * There should be one message handler for each trans_trans message type
 * defined in mess_struct.h.
 */

extern void handle_tt_interaction ();
extern void handle_tt_refresh ();
extern void handle_tt_update ();
