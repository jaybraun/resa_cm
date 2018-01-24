#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_types.h"
#include "mess_struct.h"

void handle_tt_interaction (mess)

	MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_tt_refresh (mess)

	MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}

void handle_tt_update (mess)

	MESSAGE_STRUCT *mess;
{
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
}
