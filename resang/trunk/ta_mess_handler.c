#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_types.h"
#include "mess_struct.h"


void handle_ta_advance_request (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_cancel_lock (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_create_request (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_delete (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_dispatch (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_event (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_event_request (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_filter_attr (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_filter_class (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_filter_interaction (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_id_req (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_join (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_lock_req (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_reg_attrs (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_reg_object (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_resign (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_unlock (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_save_request (mess)

    MESSAGE_STRUCT *mess;
{
}

void handle_ta_save_ack (mess)

    MESSAGE_STRUCT *mess;
{
}

void handle_ta_save_complete (mess)

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_ping (mess)      /* clk  1/94 */

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_ping_ack (mess)  /* clk  1/94 */

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_filter_parameters (mess)  /* SMM 9/96 (E1691) */

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_ta_error (mess)      /* SMM 9/96 (E1692) */

    MESSAGE_STRUCT *mess;
{
#ifdef DEBUG

    dump_mess_struct (mess); 
#endif
}
