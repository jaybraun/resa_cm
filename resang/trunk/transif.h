#ifndef TRANSIF_H_INC
#define TRANSIF_H_INC

#define NUM_ATTR 30 /* xyzzy -- replace with cons. from Karla's code */
#define MAX_STR_LEN 100
#define MAX_NAME_LEN 50
#define ARRAY_STR_LENS 80 /* FORTRAN string lengths in string arrays must */
		          /* match (i.e. declared as character*32 array() */

#define ALSP_SPD_MAX 0

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif


extern void alsp_write_mbx();

extern void alsp_create_request();

extern void alsp_cancel_lock();

extern void alsp_join();

extern void alsp_resign();

extern void alsp_lock_request();

extern void alsp_unlock();

extern void alsp_register_attributes();

extern void alsp_register_object();

extern void alsp_filter_attr();

extern void alsp_filter_class();

extern void alsp_filter_interaction();

extern void alsp_gen_unique_id();

extern void alsp_advance_time();

extern void alsp_request_next_event();

extern void alsp_save_request();

extern void alsp_send_update();

extern void alsp_send_refresh();

extern void alsp_send_interact();

extern void alsp_receive_available();

extern void alsp_receive_delete();

extern void alsp_receive_advance();

extern void alsp_receive_join_ack();

extern void alsp_receive_lock_not();

extern void alsp_receive_new_ids();

extern void alsp_receive_ping();

extern void alsp_receive_ping_ack();

extern void alsp_receive_unavailable();

extern void alsp_receive_unlock_req();

extern void alsp_receive_create();

extern void alsp_receive_interact();

extern void alsp_receive_update();

extern void alsp_receive_refresh();

extern void alsp_receive_start_save();

extern void alsp_receive_set_rate();

extern void alsp_send_save_request();

extern void alsp_send_save_ack();

extern void alsp_send_save_complete();

extern void alsp_receive_signal();         /* clk - 9/95 (E1457) */

extern void alsp_filter_parameters();      /* SMM 9/96 (E1691) */

extern void alsp_send_error();             /* SMM 9/96 (E1692) */

#endif
