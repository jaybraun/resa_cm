
/* This file defines an interface to a series of general-purpose ALSP
 * utilities.  Included here is an interface to the network transport
 * software required to provide communication between each ACM and its 
 * translator and among the various ACMs in the ALSP confederation.
 *
 * To maintain an ALSP implementation that allows plug-in use of different
 * network transport mechanisms, it is important to keep all calls to
 * networking software localized within the routines that comprise this interface.
 */


/* Perform initialization activities for underlying network transport
 * mechanisms... currently ISIS. 
 */
extern void init_network_utils ();  
 /* Params : pointer to main task */

/* Execute translator or ACM main task. */
extern void main_start_up ();


/* Open network connections. */
extern void open_trans_acm_connection ();

/* Send & receive messages across network connections.
 *
 * Send routines have one parameter, a MESSAGE_STRUCT as defined in
 * mess_struct.h.  The send routines convert the MESSAGE_STRUCT to the
 * appropriate ALSP data format.  (Send routines queue messages to ensure 
 * that ISIS does not do an immediate task switch.  Message queues are
 * flushed by the routine "output_messages".
 *
 * Receive routines have one parameter, a pointer to a message.
 * (In our case, a receive routine is invoked by the ISIS task subsystem
 * whenever a message is cbcast to that entry point.  Thus, receive routine
 * parameters are pointers to ISIS messages.)
 */
extern void send_trans_acm ();
extern void receive_acm_trans ();

/* Flush queued messages. */
extern void output_messages ();


/* Accept incoming events.  This is needed because current test drivers perform
 * I/O operations that can prevent ISIS from getting control in order to process
 * incoming events.
 */
extern void accept_messages ();

/* Close network connections. */
extern void close_trans_acm_connection ();

/* Print messages on the console window.  Param: string error message */
extern void alsp_console_message ();

/* Dump debugging info */
extern void debug_print ();


