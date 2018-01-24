#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ISISDEF
#include <isis.h>
#include "isis_defines.h"
#endif

#include "gen_types.h"
#include "alsp_utils.h"
#include "mess_struct.h"
#include "globals.h"
#include "mess_format.h"


/* This file contains a series of general purpose ALSP utilities.
 * Originally, the routines herein were intended to support LANL's
 * version of ACM Level 1.  Now, they are used primarily for
 * testing the 0.r translator message generation and parse utilities.
 *
 * Specifically, this file contains an interface to network transport
 * functions required by ALSP.  We have tried to construct this
 * interface such that the ALSP software is relatively independent
 * of the underlying network transport software.  To maintain this
 * separation, it is critical that any calls to Isis be kept inside
 * routines contained here.
 */

typedef struct mess_q_entry_d {
	MESSAGE_STRUCT *mess;
	struct mess_q_entry_d *next;
	} MESS_Q_ENTRY;

#ifdef ISISDEF
static address *ta_pg;
#else
static int *ta_pg;
#endif

static int loc_rank;


static MESS_Q_ENTRY *mess_out_q;
static MESS_Q_ENTRY *end_out_q;

static void (*main_routine) ();

static BOOL output_trans_acm (mess)

	MESSAGE_STRUCT *mess;
{
	int len;
	int replies [MAX_CONFEDERATION_SIZE];
	char curr_mess_outgoing [MAX_MESSAGE_LEN];  

#ifdef DEBUG
	dump_mess_struct (mess);
#endif

	gen_alsp_message (mess, &len, curr_mess_outgoing);
	alsp_console_message (curr_mess_outgoing);
#ifdef ISISDEF
	cbcast (ta_pg, TRANS_ACM_ENTRY,
		"%C", curr_mess_outgoing,
		len, ALL, "%d", replies);
#endif
	return (TRUEVAL);
}


void output_messages ()
{
	MESS_Q_ENTRY *temp;
	MESSAGE_STRUCT *m;

	if (mess_out_q != NULL) {
		m = mess_out_q->mess;

		/* Make sure q is in the correct state before outputting messages! */
		temp = mess_out_q;
		mess_out_q = mess_out_q->next;
		if (mess_out_q == NULL) {
			end_out_q = NULL;
		}
		free (temp);

		alsp_console_message ("SENDING m>>>");
#ifdef DEBUG
    	dump_mess_struct (mess);
#endif

		output_trans_acm (m);
	}
}

void accept_messages ()
{
#ifdef ISISDEF
	isis_accept_events (ISIS_ASYNC);
#endif
}

static void new_q_mess (m)

	MESSAGE_STRUCT *m;
{
	MESS_Q_ENTRY *new;

	alsp_console_message ("ADDING TO OUT Q>>>");
	new = (MESS_Q_ENTRY *) malloc (sizeof (*new));
	if (new == NULL) {
		alsp_console_message ("Malloc failure in new_q_mess");
		exit (0);
	} else {
		new->mess = m;
		new->next = NULL;
		if (mess_out_q == NULL) {
			mess_out_q = new;
		} else {
			end_out_q->next = new;
		}
		end_out_q = new;
	}
}

void init_network_utils (main_rout)
	
	void (*main_rout) ();
{
	main_routine = main_rout;
#ifdef ISISDEF
	isis_init (0);
	isis_task (main_routine, "main_task");
	isis_entry (TRANS_ACM_ENTRY, receive_acm_trans, "receive_acm_trans");
#endif
}

int group_change_loc (pg_view, arg)

#ifdef ISISDEF
	groupview *pg_view;
#else
	int *pg_view;
#endif

	int arg;
{
	int i;

#ifdef ISISDEF
	i = 0;
	while (!addr_ismine (&pg_view->gv_members[i])) 
		i++;
	loc_rank = i + 1;
    return (i + 1);
#else
    return (*pg_view);
#endif
}


void open_trans_acm_connection ()
{
	int replies [MAX_CONFEDERATION_SIZE];
	char pgname [15];
	
#ifdef ISISDEF
	sprintf (pgname, "loc%d", my_actor_id);
	ta_pg = pg_join (pgname, PG_MONITOR, group_change_loc,
				0, 0);

	if (addr_isnull (ta_pg)) {
		alsp_console_message ("Service not available - open_trans_acm");
		exit (0);
	}
	isis_start_done ();
#endif
}

void main_start_up ()

{
#ifdef ISISDEF
	isis_mainloop (main_routine);
#else
	(*main_routine) ();
#endif
}


void send_acm_trans (mess)

	MESSAGE_STRUCT *mess;
{
	new_q_mess (mess);
}


void receive_acm_trans (msg)
#ifdef ISISDEF
	message *msg;
#else 
	int *msg;
#endif
{
	int len;
	char curr_mess_incoming [MAX_MESSAGE_LEN];  
	MESSAGE_STRUCT *dummy;
#ifdef ISISDEF
	alsp_console_message ("ACM_Trans receive task starting");
	msg_get (msg, "%C", curr_mess_incoming, &len); 
	alsp_console_message ("ACM-Trans message received\n");
	alsp_parse (curr_mess_incoming, &dummy);
	output_messages ();
	alsp_console_message ("ACM_Trans receive task ending");
	reply (msg, "%d", my_actor_id);
#endif
}


void send_trans_acm (mess)

	MESSAGE_STRUCT *mess;
{
	new_q_mess (mess);        
}

void close_trans_acm_connection ()
{
}

void alsp_console_message (mess)
	
	char *mess;
{
	printf ("%s\n", mess);
}

void debug_print (mess) 

	char *mess;
{
#ifdef DEBUG
	printf ("%s\n", mess);
#endif
}

