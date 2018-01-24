#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "transif.h"  

#include "gen_types.h"
#include "mess_struct.h"
#include "mess_format.h"
#include "alsp_parse.h"
#include "ascii_tables.h"

#define MAX_MSG_LEN 20000     /* increased from 2700 to 20000 - SMM 10/96 (E1693) */
static int wait_val = FALSE;  /* always defaults to TRUE */

void alsp_setup_emitter_list();
void alsp_setup_sonar_list();
void alsp_setup_position_list();
void alsp_setup_iffmode_list();             /* smp 2/98 (E1907) */

static void shift_attr_array(a_param, attr_array)
    ALSP_ATTRIBUTE_LIST a_param;
    ALSP_ATTRIBUTE_LIST attr_array;

{
    int i;

    a_param[0] = 0;
    for (i=0; i < N_ATTRIBUTE_VALS; i++) 
        a_param[i+1] = attr_array[i];          
}                                          

extern int my_alsp_write();

/*
 *
 * set_alsp_write_wait_val(value)
 *    int *value
 *
 *    Sets the write wait value to the passed in parameter.  The wait value
 *    will default to FALSE after each write, but a call to this function
 *    can be used to set it to TRUE for a single write.
 *
 */

void set_alsp_write_wait_val(value)
    int *value;

{
    wait_val = *value;
}

/*
 * alsp_invoke_parser(buf, buf_len)
 *      struct dsc$descriptor_s *buf;
 *      int *buf_len;
 *
 *   Used to invoke the parser (and subsequently the translator callback
 *   functions).  Call after reading a message from the mailbox
 *  
 *   5/02 JHM - Added preprocessing to strip spaces from messages
 *        and check for unbalanced parens, both of which kill 
 *        the translator, due to inability of recursive logic
 *        to handle these conditions. 
 *  12/02 JHM - E2323 now only checks for two parse killers, unbalanced parens
 *        and empty parens in messages other than ping or refresh_request.
 */
void alsp_invoke_parser(buf, buf_len)

    char *buf;
    int *buf_len;

{
        char msg_string[MAX_MSG_LEN];
        MESSAGE_STRUCT *mstruct;
        char *next_char;
        int i, count_left=0, count_right=0;
        int right_paren=41, left_paren=40, space_char=32, null_char=0;
        int dquote=34;
        int P_char=80, I_char=73, N_char=78, G_char=71;
        int R_char=82, E_char=69, F_char=70, empty_parens = 0;
        int count_bad = 0, prev_char = 0, in_dquote = 0;

    strncpy( msg_string, buf, *buf_len);
    msg_string[*buf_len]='\0';

    next_char = alsp_parse (msg_string, &mstruct);
}

static int my_alsp_write_msg(string, length)
    char string[];
    int length;

{

    int err;

    alsp_write_mbx(string, &length, &wait_val, &err);
#ifdef DEBUG
    printf("attempted to write %s...", string);
    if (err > 0)
        printf("error writing message\n");
    else if (err < 0)
        printf("error openning mailbox\n");
    else
        printf("Successful\n");
#endif

/* re-set the wait value to TRUE after each message is written */
    wait_val = FALSE;

    return (err);
}


 
static void my_set_enum_val(mtype, msubtype, ptype, eval, par)

    int mtype, msubtype, ptype, eval;
    MESS_PARAM_LIST *par;

{

    char *etab;
    int max;

    pval_table_lookup_by_type (mtype, msubtype, ptype, &etab, &max); 
    if (max == 0) {
#ifdef DEBUG
        printf("set_enum_val:  failed in table lookup for %d, %d, %d\n",mtype, msubtype, ptype);
#endif
    } else {
        set_par_enum_value (par, eval);
        set_par_enum_string (par, get_keyword (etab, eval));
    }
}

static void my_set_enum_list(mtype, msubtype, ptype, evals, num_vals, par)

    int 
        mtype, 
        msubtype, 
        ptype, 
        evals[],
        num_vals;
    MESS_PARAM_LIST *par;

{
    ENUMLIST elist;
    char *etab;
    int i, max;

    for (i=0; i < num_vals; i++) {
        elist[i].enum_value = evals[i];
        pval_table_lookup_by_type(mtype, msubtype, ptype, &etab, &max);
        if (max == 0) {
#ifdef DEBUG
            printf("my_set_enum_list : illegal enumeration!\n");
#endif
            return;
        }
        elist[i].enum_string = get_keyword(etab, evals[i]);
    }
    set_par_enum_list(par, elist, num_vals);
}

        


    
/*
 *  ALSP_Create_Request(id)
 *    int *id;
 *   Builds a create_request message for the given unit id and sends
 *   it to the acm.
 */

void alsp_create_request(id)
    int *id;

{
    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    int err;

    mess = new_message(actor_acm_mess, type_ta_create_request);
    par = new_param(par_ta_create_request_id, mess);
    set_par_int_value(par, *id);
    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF
    printf( "ALSP_Create_Request %d\n", *id);  

#endif
    
}

/*
 *  ALSP_Cancel_Lock(id, num_attr, attr_array)  
 *        int *id;
 *        int *num_attr;
 *        int attr_array[NUM_ATTR];
 *
 *    builds a Cancel_Lock msg and sends it to the acm.
 *    attr_array is an array of boolean flags for each possible
 *    attribute.  It will be indexed by the enumerated type defined
 *    for TT_ATTRIBUTES in mess_struct.h
 *
 *  Also, the parameters attr_array can be arrays of attribute enumerated 
 *  values defined for TT_ATTRIBUTES in mess_struct.h.  The option for 
 *  attr_array be either the arrays of boolean or arrays of attribute 
 *  enumerated values is depended on the implementation.  If attr_array 
 *  is implemented to be arrays of boolean, then the MACRO NOT_POSITIONAL 
 *  does not need to be defined, else defined this MACRO.
 *
 *    if num_attr <=0, it is assumed that no attributes are being passed.
 */

void alsp_cancel_lock(id, num_attr, attr_array)  
    int *id;
    int *num_attr;
    int attr_array[];
 
{


    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    ALSP_ATTRIBUTE_LIST a_param;
    int i, err;


    mess = new_message(actor_acm_mess, type_ta_cancel_lock);
    par = new_param(par_ta_cancel_lock_id, mess);
    set_par_int_value(par, *id);


    if (*num_attr > 0) {
        par = new_param(par_ta_cancel_lock_attrs, mess);
#ifdef NOT_POSITIONAL    
        for (i=0; i <= N_ATTRIBUTE_VALS; i++)
            a_param[i] = (BOOL)FALSE;
        for (i=0; i < *num_attr; i++) 
            a_param[attr_array[i]] = (BOOL)TRUE;
        set_par_attr_list(par, a_param);
#else
        shift_attr_array(a_param, attr_array);
        set_par_attr_list(par, a_param);
#endif
    }

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF
    printf( "ALSP_Cancel_Lock %d ", *id);
    for (i=0; i < *num_attr; i++)
        printf( "%d ", attr_array[i]);
    printf("\n");
#endif





}

/* alsp_join(lookahead, node_len, node, confed, starttime, name_len,
 *             actor_name)
 *
 *     float *lookahead;  
 *     int *node_len;
 *     struct dsc$descriptor_s *node;
 *     int *confed;
 *     float *starttime;
 *     int *name_len;
 *     struct dsc$descriptor_s *actor_name;
 *
 *    Builds a join messages and sends to the ACM.
 * 
 *    lookahead, starttime and are optional parameters. Pass -1 for
 *    these is no value is to be specified.  
 *
 *    For the string parameters node and actor_name, the lengths of
 *    the strings must be passed in the node_len and name_len
 *    parameters.    If these lengths are set to a value <= 0, it is
 *    assumed that no value is to be specified for these strings.
 *
 */
void alsp_join(lookahead, node_len, name_len, actor_name)
 
    float *lookahead;  
    int *name_len;
    char *actor_name;
{

    int i, err;
    char name_str[100];

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
        TA_JOIN_TIME_MODE_PVALS enum_val;
    char message_str[MAX_MSG_LEN];

    strcpy(name_str, actor_name);

    mess = new_message(actor_acm_mess, type_ta_join);

    /* option parameters */
    if (*lookahead >= 0.0) {
        par = new_param(par_ta_join_lookahead, mess);
        set_par_float_value(par, *lookahead);
    }
    par = new_param(par_ta_join_actor_name, mess);
    set_par_string_value(par, name_str);

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);
    free_message (mess);
}

/*
 *  ALSP_Resign(owned_unit_code)
 *
 *  Builds a resign message and sends it to the acm.  No parameters
 *
 *  Modified:  Carol Kropp - 1/94
 *             Additional parameters added to resign messge.  The actor
 *             can specify to fate of owned units (DELETE or DISPERSE).
 */ 

void alsp_resign (owned_unit_code)
    int *owned_unit_code;
{

    MESSAGE_STRUCT  *mess;
    MESS_PARAM_LIST *par;
    int             param_index, msg_len, err;
    char            message_str[MAX_MSG_LEN];

    mess = new_message(actor_acm_mess, type_ta_resign);

    par = new_param (par_ta_resign_owned_units, mess);
    param_index = 1;
    my_set_enum_val (actor_acm_mess, type_ta_resign,
                     param_index, *owned_unit_code, par);

    gen_alsp_message (mess, &msg_len, message_str);

    err = my_alsp_write_msg (message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF
    printf( "ALSP_Resign\n");
#endif
}

/*
 *    alsp_lock_request(id, num_attr, attributes, service, not_len, 
 *                     notation, com_len, comment) 
 *      int *id;
 *      int *num_attr;
 *      int attributes[];
 *      int *service; 
 *      int *not_len;
 *      struct dsc$descriptor_s *notation;
 *      int *com_len;
 *      struct dsc$descriptor_s *comment;
 *
 *    Builds a lock request message and sends to the acm.
 *
 *    The attributes paramter is an array of boolean flags for each
 *    possible attribute.  It will be indexed by the enumerated type defined
 *    for TT_ATTRIBUTES in mess_struct.h.  If an attribute is not to
 *    be specified, pass FALSE (< 0) for its flag.
 *
 *  Also, the parameters attributes can be arrays of attribute enumerated 
 *  values defined for TT_ATTRIBUTES in mess_struct.h.  The option for 
 *  attributes be either the arrays of boolean or arrays of attribute 
 *  enumerated values is depended on the implementation.  If attributes 
 *  is implemented to be arrays of boolean, then the MACRO NOT_POSITIONAL 
 *  does not need to be defined, else defined this MACRO.
 *
 *    if num_attr <= 0, it is assumed that no attributes are to be sent.
 *
 *    For the string parameters notation and comment, the lengths of
 *    the strings must be passed in the not_len and com_len
 *    parameters.    If these lengths are set to a value <= 0, it is
 *    assumed that no value is to be specified for these strings.
 */
void alsp_lock_request(id, num_attr, attributes, service, not_len, 
                       notation, com_len, comment) 
 
    int *id;
    int *num_attr;
    int attributes[];
    int *service; /* an enumeration */
    int *not_len;
    char *notation;
    int *com_len;
    char *comment;

{

    int i, err;
    char not_str[100];
    char com_str[100];

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    ALSP_ATTRIBUTE_LIST a_param; 

    if (*not_len > 0) 
        strcpy(not_str, notation);
    if (*com_len > 0)
        strcpy(com_str, comment);

    mess = new_message(actor_acm_mess, type_ta_lock_req);
    par = new_param(par_ta_lock_req_id, mess);
    set_par_int_value(par, *id);

    if (*num_attr > 0) {
        par = new_param(par_ta_lock_req_attrs, mess);
#ifdef NOT_POSITIONAL    
        for (i=0; i <= N_ATTRIBUTE_VALS; i++)
            a_param[i] = (BOOL)FALSE;
        for (i=0; i < *num_attr; i++) 
            a_param[attributes[i]] = (BOOL)TRUE;
        set_par_attr_list(par, a_param);
#else
        shift_attr_array(a_param, attributes);
        set_par_attr_list(par, a_param);
#endif

    }


    par = new_param(par_ta_lock_req_service, mess);
    my_set_enum_val(actor_acm_mess, type_ta_lock_req,
                    par_ta_lock_req_service, *service, par);

    if (*not_len > 0) {
        par = new_param(par_ta_lock_req_notation, mess);
        set_par_string_value(par, not_str);
    }
    if (*com_len > 0) {
        par = new_param(par_ta_lock_req_comment, mess);
        set_par_string_value(par, com_str);
    }

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf( "ALSP_Lock_Request %d ", *id);
    
    for (i=0; i < *num_attr; i++)
        printf( "%d ", attributes[i]);
    
    printf( "%d %s %s\n", service, not_str, com_str);
    
#endif
}

/*
 *  ALSP_Unlock(id, num_attr, attributes)
 *     int *id;
 *     int *num_attr;
 *     int attributes[];
 *
 *  Builds a unlock message and sends it to the acm.
 *
 *    The attributes paramter is an array of boolean flags for each
 *    possible attribute.  It will be indexed by the enumerated type defined
 *    for TT_ATTRIBUTES in mess_struct.h.  If an attribute is not to
 *    be specified, pass FALSE (< 0) for its flag.
 *
 *  Also, the parameters attributes can be arrays of attribute enumerated 
 *  values defined for TT_ATTRIBUTES in mess_struct.h.  The option for 
 *  attributes be either the arrays of boolean or arrays of attribute 
 *  enumerated values is depended on the implementation.  If attributes 
 *  is implemented to be arrays of boolean, then the MACRO NOT_POSITIONAL 
 *  does not need to be defined, else defined this MACRO.
 *
 *    if num_attr <= 0, it is assumed that no attributes are to be sent.
 *
 */
void alsp_unlock(id, num_attr, attributes)
    int *id;
    int *num_attr;
    int attributes[];

{
    int i, err;

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    ALSP_ATTRIBUTE_LIST a_param;

    mess = new_message(actor_acm_mess, type_ta_unlock);
    par= new_param(par_ta_unlock_id, mess);
    set_par_int_value(par, *id);


    if (*num_attr > 0) {
        par = new_param(par_ta_unlock_attrs, mess);
#ifdef NOT_POSITIONAL    
        for (i=0; i <= N_ATTRIBUTE_VALS; i++)
            a_param[i] = (BOOL)0;
        for (i=0; i < *num_attr; i++) 
            a_param[attributes[i]] = (BOOL)TRUE;
        set_par_attr_list(par, a_param);
#else
        shift_attr_array(a_param, attributes);
        set_par_attr_list(par, a_param);
#endif

    }


    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf( "ALSP_Unlock %d ", *id);
    for  (i=0; i< *num_attr; i++)
        printf( "%d ", attributes[i]);
    printf( "\n");
#endif
}

/*
 *  ALSP_Register_Attributes(class, num_create_attr, create_attr,
 *                           num_intr_attr, intr_attr,
 *                           num_update_attr, update_attr) 
 *   
 *        int *class;
 *        int *num_create_attr;
 *        int create_attr[];
 *        int *num_intr_attr;
 *        int intr_attr;
 *        int *num_update_attr;
 *        int update_attr;
 *
 *  Builds a register attribute message and sends it to the acm.
 * 
 *  All parameters are optional.  Pass -1 for class, num_create_attr
 *  and num_intr_attr if these values are not to be specified.
 *
 *  The paramters create_attr, intr_attr and update_attr are arrays of boolean
 *  flags for each possible attribute.  It will be indexed by the
 *  enumerated type defined for TT_ATTRIBUTES in mess_struct.h.  If
 *  an attribute is not to be specified, pass FALSE (< 0) for its flag.
 *
 *  Also, the parameters create_attr, intr_attr and update_attr can be arrays of
 *  attribute enumerated values defined for TT_ATTRIBUTES in mess_struct.h.  The
 *  option for create_attr, intr_attr, and update_attr to be either the arrays 
 *  of boolean or arrays of attribute enumerated values is depended on the 
 *  implementation.  If create_attr, intr_attr and update_attr are implemented 
 *  to be arrays of boolean, then the MACRO NOT_POSITIONAL does not need to be
 *  defined, else define this MACRO.
 *
 *  If num_create_attr, num_inter_attr or num_update_attr are set to <=0, it is
 *  assumed no attributes are being passed.
 */
void alsp_reg_attr(class, num_create_attr, create_attr,
                          num_intr_attr, intr_attr,
                          num_update_attr, update_attr)
    
    int *class;  /* an enumeration */
    int *num_create_attr;
    int create_attr[];
    int *num_intr_attr;
    int intr_attr[];
    int *num_update_attr;
    int update_attr[];

{

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    ALSP_ATTRIBUTE_LIST a_param;
    int i, err;

    mess = new_message(actor_acm_mess, type_ta_reg_attrs);

    if (*class > 0) {
        par = new_param(par_ta_reg_attrs_class, mess);
        my_set_enum_val(actor_acm_mess, type_ta_reg_attrs,
                        par_ta_reg_attrs_class, *class, par); 
    }

    if (*num_create_attr > 0) {
        par = new_param(par_ta_reg_attrs_create, mess);
#ifdef NOT_POSITIONAL    
        for (i=0; i <= N_ATTRIBUTE_VALS; i++)
            a_param[i] = (BOOL)FALSE; 
        for (i=0; i < *num_create_attr; i++) {
        if (create_attr[i] == tt_attr_radar  ||  
            create_attr[i] == tt_attr_jammer ||  
            create_attr[i] == tt_attr_index  ||
            create_attr[i] == tt_attr_orient ||
            create_attr[i] == tt_attr_mode   ||
            create_attr[i] == tt_attr_type   ||
            create_attr[i] == tt_attr_emitter_status ||
            create_attr[i] == tt_attr_unit   ||      /* clk 10/94 - Don't want the c2w_factors */
            create_attr[i] == tt_attr_emtr   ||      /* sub-keyword attributes                 */
            create_attr[i] == tt_attr_ew_c3  ||
            create_attr[i] == tt_attr_ew_acq ||
            create_attr[i] == tt_attr_ew_launch ||
            create_attr[i] == tt_attr_ew_guide  ||
            create_attr[i] == tt_attr_ew_power  ||
            create_attr[i] == tt_attr_ew_surf   ||
            create_attr[i] == tt_attr_iffmode_master ||     /* smp 2/98 (E1907) */
            create_attr[i] == tt_attr_iffmode1 ||           /* smp 2/98 (E1907) */
            create_attr[i] == tt_attr_iffmode2 ||           /* smp 2/98 (E1907) */
            create_attr[i] == tt_attr_iffmode3 ||           /* smp 2/98 (E1907) */
            create_attr[i] == tt_attr_iffmode4 ||           /* smp 2/98 (E1907) */
            create_attr[i] == tt_attr_iffmodeC ||           /* smp 2/98 (E1907) */
            create_attr[i] == tt_attr_iffmode_value ||      /* smp 2/98 (E1907) */
            create_attr[i] == tt_attr_iffmode_status)       /* smp 2/98 (E1907) */

            continue;     
            a_param[create_attr[i]] = (BOOL)TRUE;   /* khc,3/93- For CREATE parameter, use value */
    }                                           /* emitter types to be EMITTER */
        set_par_attr_list(par, a_param);
#else
        shift_attr_array(a_param, create_attr);
        set_par_attr_list(par, a_param);
#endif

    }


    if (*num_intr_attr > 0) {
        par = new_param(par_ta_reg_attrs_intr, mess);
#ifdef NOT_POSITIONAL    
    for (i=0; i <= N_ATTRIBUTE_VALS; i++) 
            a_param[i] = (BOOL)FALSE;
        for (i=0; i < *num_intr_attr; i++) {
        if (intr_attr[i] == tt_attr_radar  ||      /* khc,3/93- Don't want the emitter sub-keyword */
            intr_attr[i] == tt_attr_jammer ||      /* attributes because the real keyword is EMITTER */
            intr_attr[i] == tt_attr_index  ||
            intr_attr[i] == tt_attr_orient ||
            intr_attr[i] == tt_attr_mode   ||
            intr_attr[i] == tt_attr_type   ||
            intr_attr[i] == tt_attr_emitter_status ||
            intr_attr[i] == tt_attr_unit   ||      /* clk 10/94 - Don't want the c2w_factors */
            intr_attr[i] == tt_attr_emtr   ||      /* sub-keyword attributes                 */
            intr_attr[i] == tt_attr_ew_c3  ||
            intr_attr[i] == tt_attr_ew_acq ||
            intr_attr[i] == tt_attr_ew_launch ||
            intr_attr[i] == tt_attr_ew_guide  ||
            intr_attr[i] == tt_attr_ew_power  ||
            intr_attr[i] == tt_attr_ew_surf   ||
            intr_attr[i] == tt_attr_iffmode_master ||      /* smp 2/98 (E1907) */
            intr_attr[i] == tt_attr_iffmode1 ||            /* smp 2/98 (E1907) */
            intr_attr[i] == tt_attr_iffmode2 ||            /* smp 2/98 (E1907) */
            intr_attr[i] == tt_attr_iffmode3 ||            /* smp 2/98 (E1907) */
            intr_attr[i] == tt_attr_iffmode4 ||            /* smp 2/98 (E1907) */
            intr_attr[i] == tt_attr_iffmodeC ||            /* smp 2/98 (E1907) */
            intr_attr[i] == tt_attr_iffmode_value ||       /* smp 2/98 (E1907) */
            intr_attr[i] == tt_attr_iffmode_status)        /* smp 2/98 (E1907) */
            continue;
            a_param[intr_attr[i]] = (BOOL)TRUE;
    }
        set_par_attr_list(par, a_param);
#else
        shift_attr_array(a_param, intr_attr);
        set_par_attr_list(par, a_param);
#endif

    }

    if (*num_update_attr > 0) {
        par = new_param(par_ta_reg_attrs_update, mess);
#ifdef NOT_POSITIONAL    
        for (i=0; i <= N_ATTRIBUTE_VALS; i++) 
            a_param[i] = (BOOL)FALSE;
        for (i=0; i < *num_update_attr; i++) {
        if (update_attr[i] == tt_attr_radar  ||      /* khc,3/93- Don't want the emitter sub-keyword */
            update_attr[i] == tt_attr_jammer ||      /* attributes because the real keyword is EMITTER */
            update_attr[i] == tt_attr_index  ||
            update_attr[i] == tt_attr_orient ||
            update_attr[i] == tt_attr_mode   ||
            update_attr[i] == tt_attr_type   ||
            update_attr[i] == tt_attr_emitter_status ||
            update_attr[i] == tt_attr_unit   ||      /* clk 10/94 - Don't want the c2w_factors */
            update_attr[i] == tt_attr_emtr   ||      /* sub-keyword attributes                 */
            update_attr[i] == tt_attr_ew_c3  ||
            update_attr[i] == tt_attr_ew_acq ||
            update_attr[i] == tt_attr_ew_launch ||
            update_attr[i] == tt_attr_ew_guide  ||
            update_attr[i] == tt_attr_ew_power  ||
            update_attr[i] == tt_attr_ew_surf   ||
            update_attr[i] == tt_attr_iffmode_master ||    /* smp 2/98 (E1907) */
            update_attr[i] == tt_attr_iffmode1 ||          /* smp 2/98 (E1907) */
            update_attr[i] == tt_attr_iffmode2 ||          /* smp 2/98 (E1907) */
            update_attr[i] == tt_attr_iffmode3 ||          /* smp 2/98 (E1907) */
            update_attr[i] == tt_attr_iffmode4 ||          /* smp 2/98 (E1907) */
            update_attr[i] == tt_attr_iffmodeC ||          /* smp 2/98 (E1907) */
            update_attr[i] == tt_attr_iffmode_value ||     /* smp 2/98 (E1907) */
            update_attr[i] == tt_attr_iffmode_status)      /* smp 2/98 (E1907) */
              continue;
              a_param[update_attr[i]] = (BOOL)TRUE;
        }
        set_par_attr_list(par, a_param);
#else
        shift_attr_array(a_param, update_attr);
        set_par_attr_list(par, a_param);
#endif
    }

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf( "ALSP_Register_Attributes %d \(", *class);
    for (i =0; i < *num_create_attr; i++)
        printf( "%d ", create_attr[i]);

    printf( "\)  \(");

    for (i =0; i < *num_intr_attr; i++)
        printf( "%d ", intr_attr[i]);
 
    printf( "\)\n");
#endif
}

/*
 *  ALSP_Register_Object(id, class, num_attr, attributes)
 *            int *id;
 *            int *class;
 *            int *num_attr;
 *            int attributes[];
 *
 *  Builds a register_object message and sends it to the acm
 *
 *  id = object id, class = object class (i.e. ground, ground.maneuver...)
 *
 *    The attributes paramter is an array of boolean flags for each
 *    possible attribute.  It will be indexed by the enumerated type defined
 *    for TT_ATTRIBUTES in mess_struct.h.  If an attribute is not to
 *    be specified, pass FALSE (< 0) for its flag.
 *
 *  Also, the parameters attributes can be arrays of attribute enumerated 
 *  values defined for TT_ATTRIBUTES in mess_struct.h.  The option for 
 *  attributes be either the arrays of boolean or arrays of attribute 
 *  enumerated values is depended on the implementation.  If attributes 
 *  is implemented to be arrays of boolean, then the MACRO NOT_POSITIONAL 
 *  does not need to be defined, else defined this MACRO.
 *
 *    if num_attr <= 0, it is assumed that no attributes are to be sent.
 *
 */

void alsp_register_object(id, class, num_attr, attributes)
    int *id;
    int *class;  /* an enumeration */
    int *num_attr;
    int attributes[];
 
{

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    ALSP_ATTRIBUTE_LIST a_param;
    int i, err;


    mess = new_message(actor_acm_mess, type_ta_reg_object);

    par = new_param(par_ta_reg_object_id, mess);
    set_par_int_value(par, *id);

    par = new_param(par_ta_reg_object_class, mess);
    my_set_enum_val(actor_acm_mess, type_ta_reg_object,
                    par_ta_reg_object_class, *class, par); 

    if (*num_attr > 0) {
        par = new_param(par_ta_reg_object_unlock, mess);
#ifdef NOT_POSITIONAL    
        for (i=0; i <= N_ATTRIBUTE_VALS; i++)
            a_param[i] = (BOOL)FALSE;
        for (i=0; i < *num_attr; i++) 
            a_param[attributes[i]] = (BOOL)TRUE;
        set_par_attr_list(par, a_param);
#else
        shift_attr_array(a_param, attributes);
        set_par_attr_list(par, a_param);
#endif
    }

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf( "ALSP_Register_Object %d %d ", *id, *class);
    for (i=0; i < *num_attr; i++)
        printf( "%d ", attributes[i]);

    printf( "\n");

#endif
}

/*
 *  alsp_filter_attr(param_flag, class, num_id, id, lat, lon, alt,
 *                    num_brnch, brnch, head, name_len, name, pct,
 *                    num_side, side, num_size, size, spd, num_stat,
 *                    stat)    
 *
 *    int param_flag[N_FILTER_ATTR_PAR];
 *    int *class; 
 *    int *num_id;
 *    int id[MAX_LIST];
 *    float lat[2], lon[2];
 *    int alt[2];
 *    int *num_brnch; 
 *    int brnch[MAX_LIST]; 
 *    int head[2];
 *    int *name_len;
 *    struct dsc$descriptor_s *name;
 *    int pct[2];
 *    int *num_side; 
 *    int side[MAX_LIST];  
 *    int *num_size; 
 *    int size[MAX_LIST];  
 *    int spd[2];
 *    int *num_stat;
 *    int stat[MAX_LIST]; 
 *
 *  This ugly function builds a filter_attr message and sends it to
 *  the acm.  The parameter list mirrors those of the message as
 *  specified in the Mitre document, so consulting with that should
 *  make this more clear.
 *
 *  param_flag indicates which parameters are being passed.  It is an
 *  array of booledan flags indexed by TA_FILTER_ATTR_PARAMS in
 *  mess_struct.h.  If a flag is set to TRUE, it is assumed that its
 *  corresponding paramter is passed.  For instance, if the first
 *  element of the param flag is set, then the class parameter is
 *  passed on in the message (CLASS = 1 in the TA_FILTER_ATTR_PARAMS
 *  enumeration).   However, if the second element is not set, the id
 *  parameter is ignored (ID = 2 in the TA_FILTER_ATTR_PARAMS
 *  enumeration).   
 *
 *  For the string parameter name, the length ofthe string must be
 *  passed in the name_len parameters.  
 *
 *  id is an array of ids, so num_id must be provided also
 *
 *  brnch, side, size and stat are all lists of enumerations. 
 *  
 */
void alsp_filter_attr(param_flag, class, num_id, id, lat, lon, alt,
                      num_brnch, brnch, head, name_len, name, pct,
                      num_side, side, num_size, size, spd, num_stat,
                      stat)    

    int param_flag[N_FILTER_ATTR_PAR];
    int *class;  /* an enumeration */
    int *num_id;
    int id[MAX_LIST];
    float lat[2], lon[2];
    int alt[2];
    int *num_brnch; 
    int brnch[MAX_LIST];  /* an enumeration */
    int head[2];
    int *name_len;

    char *name;

    int pct[2];
    int *num_side; 
    int side[MAX_LIST];  /* an enumeration */
    int *num_size; 
    int size[MAX_LIST];  /* an enumeration */
    int spd[2];
    int *num_stat;
    int stat[MAX_LIST]; /* an enumeration */

{

    int i;
    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    char name_str[ARRAY_STR_LENS];
    char message_str[MAX_MSG_LEN];
    int err, msg_len;


    mess = new_message(actor_acm_mess, type_ta_filter_attr);

    if (param_flag[par_ta_filter_class-1] > 0) {
        par = new_param(par_ta_filter_class, mess);
        my_set_enum_val(actor_acm_mess, type_ta_filter_attr,
                        par_ta_filter_class, *class, par);
    }
    if (param_flag[par_ta_filter_attr_id-1] > 0) {
        par = new_param(par_ta_filter_attr_id, mess);
        set_par_int_list(par, id, *num_id);
    }
    if (param_flag[par_ta_filter_attr_lat-1] > 0) {
        par = new_param(par_ta_filter_attr_lat, mess);
        set_par_float_range(par, lat[0], lat[1]);
    }
    if (param_flag[par_ta_filter_attr_long-1] > 0) {
        par = new_param(par_ta_filter_attr_long, mess);
        set_par_float_range(par, lon[0], lon[1]);
    }
    if (param_flag[par_ta_filter_alt-1] > 0) {
        par = new_param(par_ta_filter_alt, mess);
        set_par_int_range(par, alt[0], alt[1]);
    }
    if (param_flag[par_ta_filter_heading-1] > 0) {
        par = new_param(par_ta_filter_heading, mess);
        set_par_int_range(par, head[0], head[1]);
    }
    if (param_flag[par_ta_filter_name-1] > 0) {
        par = new_param(par_ta_filter_name, mess);
        strcpy(name_str, name);
        set_par_string_value(par, name_str);
    }
    if (param_flag[par_ta_filter_percent-1] > 0) {
        par = new_param(par_ta_filter_percent, mess);
        set_par_int_range(par, pct[0], pct[1]);
    }
    if (param_flag[par_ta_filter_side-1] > 0) {
        par = new_param(par_ta_filter_side, mess);
        my_set_enum_list(actor_acm_mess, type_ta_filter_attr,
                         par_ta_filter_side, side, *num_side, par);
    }
    if (param_flag[par_ta_filter_size-1] > 0) {
        par = new_param(par_ta_filter_size, mess);
        my_set_enum_list(actor_acm_mess, type_ta_filter_attr,
                         par_ta_filter_size, size, *num_size, par);
    }
    if (param_flag[par_ta_filter_speed-1] > 0) {
        par = new_param(par_ta_filter_speed, mess);
        set_par_int_range(par, spd[0], spd[1]);
    }
    if (param_flag[par_ta_filter_stat-1] > 0) {
        par = new_param(par_ta_filter_stat, mess);
        my_set_enum_list(actor_acm_mess, type_ta_filter_attr,
                         par_ta_filter_stat, stat, *num_stat, par);
    }
    if (param_flag[par_ta_filter_unit_type-1] > 0) {
        par = new_param(par_ta_filter_unit_type, mess);
        my_set_enum_list(actor_acm_mess, type_ta_filter_attr,
                         par_ta_filter_unit_type, brnch, *num_brnch, par);
    }

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

}

/*
 *  ALSP_Filter_Class(class, condition, send_dr_flag)  
 *        int *class;
 *        int *condition;
 *        int *send_dr_flag; 
 *
 *  Builds a filter_class message and sends to the acm.
 *
 *  Modified:  C. Kropp (NRaD)  4/26/93
 *             Added the send_dr_flag.
 */

void alsp_filter_class(class, condition, send_dr_flag)  
    int *class;  /* an enumeration */
    int *condition;  /* an enumeration */
    int *send_dr_flag;   /* TRUE | FALSE enumeration */

{

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    int i, err;

    mess = new_message(actor_acm_mess, type_ta_filter_class);

    par = new_param(par_ta_filter_class_class, mess);
    my_set_enum_val(actor_acm_mess, type_ta_filter_class,
                    par_ta_filter_class_class, *class, par); 

    par = new_param(par_ta_filter_class_condition, mess);
    my_set_enum_val(actor_acm_mess, type_ta_filter_class,
                    par_ta_filter_class_condition, *condition, par); 

    par = new_param (par_ta_filter_class_send_dr, mess);
    my_set_enum_val (actor_acm_mess, type_ta_filter_class,
                     par_ta_filter_class_send_dr, *send_dr_flag, par); 


    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);



#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf( "ALSP_Filter_Class %d %d\n", *class, *condition);

#endif

}

/*
 *  ALSP_Filter_Interaction (kind, condition)
 *        int *kind;
 *        int *condition;
 *
 *  Builds a filter_interaction message and sends to the acm.
 *
 *  Author:    Carol Kropp - 12/93
 */

void alsp_filter_interaction  (kind, condition)  
    int *kind;       /* interaction KIND enumeration */
    int *condition;  /* CONDITION enumeration */
{
    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    int i, err;

    mess = new_message (actor_acm_mess, type_ta_filter_interaction);

    par  = new_param (par_ta_filter_interaction_kind, mess);
    my_set_enum_val  (actor_acm_mess, type_ta_filter_interaction,
                      par_ta_filter_interaction_kind, *kind, par); 

    par = new_param (par_ta_filter_class_condition, mess);
    my_set_enum_val (actor_acm_mess, type_ta_filter_class,
                     par_ta_filter_class_condition, *condition, par); 

    gen_alsp_message (mess, &msg_len, message_str);
    err = my_alsp_write_msg (message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF
    printf( "ALSP_Filter_Interaction %d %d\n", *kind, *condition);
#endif
}

/*
 *  ALSP_Gen_Unique_ID(num_ids)  
 *      int *num_ids; 
 *
 *   Will send an id request to the acm for the number of ids requested
 */
void alsp_gen_unique_id(num_ids)  
    int *num_ids;  /* number of ids requested */

{

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    int err;

    mess = new_message(actor_acm_mess, type_ta_id_req);

    par = new_param(par_ta_id_req_number, mess);
    set_par_int_value(par, *num_ids);

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);


#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf( "ALSP_Gen_Unique_ID %d\n", *num_ids);

#endif
}


/*
 * ALSP_Advance_Time(time) 
 *      float *time;
 *
 *      Request for an advancement in time to a certain simulation
 *      time.  
 *
 *      Before the request is granted, the ACM passes up to the
 *      Translator all events from other simulations with earlier time
 *      stamps.  
 */

void alsp_advance_time(time) 
    float *time;

{

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    int err;

    mess = new_message(actor_acm_mess, type_ta_advance_request);
    par = new_param(par_ta_adv_req_time, mess);

    set_par_float_value(par, *time);

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);


#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf("alsp_advance_time %f\n", *time);

#endif
}

/*
 *   ALSP_Request_Next_Event() 
 *
 *   Builds an event_request message and sends it to the acm.  No
 *   parameters.  
 */
void alsp_req_next() 

{

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len, err;
    char message_str[MAX_MSG_LEN];


    mess = new_message(actor_acm_mess, type_ta_event_request);

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);



#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf( "ALSP_Request_Next_Event\n");

#endif
}

/* dispatch_flag > 0, send it at a dispatch, otherwise it will be sent */
/* as an event */
void alsp_send_del(dispatch_flag, id)
    int *dispatch_flag;
    int *id;

{
    MESSAGE_STRUCT *mess, *sub_mess;
    MESS_PARAM_LIST *par;
    int msg_len, err;
    char message_str[MAX_MSG_LEN];

    if (*dispatch_flag > 0)
        mess = new_message(actor_acm_mess, type_ta_dispatch);
    else
        mess = new_message(actor_acm_mess, type_ta_event);

    sub_mess = new_message(acm_actor_mess, type_at_delete);


    par = new_param(par_at_delete_id, sub_mess);
    set_par_int_value(par, *id);

    if (*dispatch_flag > 0)
        par = new_param(par_ta_disp_disp, mess);
    else
        par = new_param(par_ta_event_event, mess);
    set_par_mess_value(par, sub_mess);

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
}

/*
 * ALSP_Send_Delete(id)
 *   int *id;
 *
 * Builds a delete message and sends it to the acm
 */
void alsp_send_delete(id)
    int *id;
 
{

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len, err;
    char message_str[MAX_MSG_LEN];

    mess = new_message(actor_acm_mess, type_ta_delete);

    par = new_param(par_ta_delete_id, mess);
    set_par_int_value(par, *id);

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF

    printf( "ALSP_Send_Delete %d\n", *id);

#endif
}

/*
 * ALSP_Send_Update(dispatch_flag, n_args, param_flag, int_params, 
 *                  f_params, num_emitter, emitcategory_list, emittype_list, 
 *                  emitstatus_list, emitindex_list, 
 *                  emitorient_list, emitmode_list,
 *                  num_sonar, sonartype_list, sonarstatus_list,
 *                  sonarmode_list, sonardepth_list,
 *                  num_position, sonarlat_list, sonarlon_list,
 *                  strlens, string_params, qualifiers) 
 *
 *         int *dispatch_flag; 
 *         int *n_args;  
 *         int param_flag[]; 
 *         int int_params[];
 *         float f_params[];
 *         int *num_emitter;
 *         int emitcategory__list[];
 *         int emittype_list[];
 *         int emitstatus_list[];
 *         int emitindex_list[]; 
 *         float emitorient_list[];
 *         int emitmode_list[];
 *         int strlens[];  
 *         char string_params[][MAX_STR_LEN];
 *         int qualifiers[];  
 *
 *  Builds an update message and sends to the acm.
 *
 *  If dispatch_flag > 0, this message will be a dispatch, otherwise
 *  it will be an event.
 *
 *  param_flag indicates which parameters are being passed.  It is an
 *  array of boolean flags indexed by TT_ATTRIBUTES in
 *  mess_struct.h.  If a flag is set to TRUE, it is assumed that its
 *  corresponding paramter is passed. 
 *
 *  The array int_params contains the integer-type parameters being
 *  passed,  f_params contains the float-type parameters being passed
 *  and strlens and string_params combine to pass string parameters
 *  and their lenghts.  Note that the MAX_STR_LEN must be the same
 *  between the fortran and c.  It is defined in transif.h.
 *
 *  The num_emitter and the seven arrays emitcategory_list, emittype_list, 
 *  emitstatus_list, emitindex_list, emit_mode_len, emitorient_list, 
 *  emitmode_list combine to pass seven lists of enumerator-type as the
 *  elements of the emitter parameters for a message, the num_emitter contains 
 *  a number value to indicate number of elements for all seven lists.
 *
 *  All arrays are positional.  This function works by examining each
 *  position in param_flag, and if it is TRUE it looks into the
 *  corresponding position in either the int_params, f_params or
 *  string_params array for the value of the parameter.    For
 *  instance, if param_flags[ID] were TRUE, the value would be
 *  obtained from int_params[ID].
 *  
 *  qualifiers is an array for each parameter (again, positional
 *  boolean flags) that indicate if the parameter is dead-reconable.
 *  Currenlty this can only be true for ID, so that is the only one
 *  that is checked.
 *
 *  The qualifiers array is no longer be used for ID only, it is also
 *  used for EMITTER.   
 *
 *  Modified:  Stephanie Paden - 2/98 (E1907)
 *             Added IFF related arguments to function interface for
 *             new IFF() wrapper attribute.
 */
void alsp_send_update(dispatch_flag, n_args, param_flag, int_params,
                      f_params, num_emitter, emitcategory_list, emittype_list,
                      emitstatus_list, emitindex_list, 
                      emitorient_list, emitmode_list,                      
                      num_sonar, sonartype_list, sonarstatus_list,
                      sonarmode_list, sonardepth_list,
                      num_position, positionlat_list, positionlon_list,
                      iffmaster, iffmode_status_array, iffmode_value_array,
                      strlens, string_params, qualifiers) 

    int *dispatch_flag;     /* > 0 means send as dispatch; <= 0  means */
                                        /* send as an event */ 
    int *n_args;            /* number of parameters passed this message */
    int param_flag[];       /* each element of this array will be an */
                            /* integer corresponding to the enumeration for */
                            /* the parameter provided */
    int int_params[];
    float f_params[];
    int *num_emitter;        /* num of emitters */
    int emitcategory_list[];/* category, JAMMER or RADAR */
    int emittype_list[];    /* enumerated list */
    int emitstatus_list[];  /* on or off status */
    int emitindex_list[];   /* index num of emitter on unit */
    int emitmode_list[];    /* mode enumeration type*/
    float emitorient_list[];/* orientation of jammer */
    int *num_sonar;         /* num of sonars */
    int sonartype_list[];    /* enumerated list of sonars */
    int sonarstatus_list[];  /* on or off status */
    int sonarmode_list[];    /* sonar mode enumeration type*/
    int sonardepth_list[];   /* sonar depth (ft) */
    int *num_position;       /* num of positions */
    float positionlat_list[];  /* list of position latitudes */
    float positionlon_list[];  /* list of position longitudes */
    int strlens[];          /* lengths of the strings in string_params array */
    char string_params[][ARRAY_STR_LENS];
    int qualifiers[];       /* each parameter may have a qualifier */
    int *iffmaster;
    int iffmode_status_array[];
    int iffmode_value_array[];

{
    MESSAGE_STRUCT *mess, *sub_mess;
    MESS_PARAM_LIST *par, *class_p;
    int msg_len, err, i, num_pts;
    char message_str[MAX_MSG_LEN], temp_str[ARRAY_STR_LENS];
    LOCLIST orbit_pts;
    int class;
    EMITTERLIST emitter_list;
    SONARLIST sonar_list;
    IFFMODESTR iffmode_array[N_IFF_PARAMS];
    POSITIONLIST position_list;

#ifdef NODEF
    if (*n_args != N_ATTRIBUTE_VALS) {
        printf("alsp_send_update:  incorrect number of parameters\n");
        return;
    }
#endif

    if (*dispatch_flag > 0)
        mess = new_message(actor_acm_mess, type_ta_dispatch);
    else
        mess = new_message(actor_acm_mess, type_ta_event);

    sub_mess = new_message(acm_actor_mess, type_at_update);
    
    if (param_flag[tt_attr_class-1] < 0) {   /* class is required in all update messages */
#ifdef DEBUG
        printf("alsp_send_update:  required class param missing \n");
#endif
        return;
    }
    
    class = int_params[tt_attr_class-1];

    /* First, the special case of orbit points.  I'm treating this */
    /* loosely - either the first set and/or the second set can be */
    /* specified, but you have to have a complete set (lat and lon) */
    num_pts = 0;
    if ((param_flag[tt_attr_orb_lat1-1] > 0) &&
        (param_flag[tt_attr_orb_lon1-1] > 0)) {
        orbit_pts[num_pts].lat = f_params[tt_attr_orb_lat1-1];
        orbit_pts[num_pts].lon = f_params[tt_attr_orb_lon1-1];
        num_pts++;
    }
    if ((param_flag[tt_attr_orb_lat2-1] > 0) &&
        (param_flag[tt_attr_orb_lon2-1] > 0)) {
        orbit_pts[num_pts].lat = f_params[tt_attr_orb_lat2-1];
        orbit_pts[num_pts].lon = f_params[tt_attr_orb_lon2-1];
        num_pts++;
    }
    if (num_pts > 0) {
        par = new_param(tt_attr_orb_lat1, sub_mess);
        set_par_loc_list(par, orbit_pts, num_pts);
    }

    /* now for the special case of target loc...  We've defined a */
    /* target_lat and a target_lon to try to make this more general */
    /* purpose from the fortran side */
    if ((param_flag[tt_attr_target_lat-1] > 0) &&
        (param_flag[tt_attr_target_long-1] > 0)) {
        par = new_param(tt_attr_target_lat, sub_mess);
        set_par_loc_value(par, f_params[tt_attr_target_lat-1],
                          f_params[tt_attr_target_long-1]); 
    }
    
    /* emitter part of message */
    if (param_flag[tt_attr_emitter-1] > 0) {

        alsp_setup_emitter_list(emitter_list, num_emitter, 
                                emitcategory_list, emittype_list,
                                emitstatus_list, emitindex_list, 
                                emitorient_list, emitmode_list); 

        par = new_param(tt_attr_emitter, sub_mess);
        par->param_data.val_type = value_emit_struct_list;
        set_par_emitter_list(par, emitter_list, num_emitter);
    }

    /* sonar part of message */
    if (param_flag[tt_attr_sonar-1] > 0) {

        alsp_setup_sonar_list(sonar_list, num_sonar, 
                              sonartype_list,
                              sonarstatus_list,
                              sonarmode_list, 
                              sonardepth_list); 

        par = new_param(tt_attr_sonar, sub_mess);
        par->param_data.val_type = value_sonar_struct_list;
        set_par_sonar_list(par, sonar_list, num_sonar);
    }

    /* mine_location part of message */
    if (param_flag[tt_attr_position-1] > 0) {

        alsp_setup_position_list(position_list, num_position, 
                                 positionlat_list,
                                 positionlon_list);

        par = new_param(tt_attr_position, sub_mess);
        par->param_data.val_type = value_loc_struct_list;
        set_par_position_list(par, position_list, num_position);
    }

    /* sweeparea_location part of message */
    if (param_flag[tt_attr_sweeparea_location-1] > 0) {

        alsp_setup_position_list(position_list, num_position, 
                                 positionlat_list,
                                 positionlon_list);

        par = new_param(tt_attr_sweeparea_location, sub_mess);
        par->param_data.val_type = value_loc_struct_list;
        set_par_position_list(par, position_list, num_position);
    }

    if (param_flag[tt_attr_iff-1] > 0) {    /* IFF - smp 2/98 (E1907) */
        alsp_setup_iffmode_list(param_flag,
                                iffmaster, 
                                iffmode_status_array,
                                iffmode_value_array,
                                iffmode_array);         
        par = new_param(tt_attr_iff, sub_mess);
        par->param_data.val_type = value_iff_struct_list;
        set_par_iffmode_list(par, iffmode_array);
    }

    for (i=0; i < N_ATTRIBUTE_VALS; i++) {
        if (param_flag[i] > 0) {
            /* this is ugly, but we have to skip the special case of */
            /* orbit points and do them separately */
            if (((i+1) >= tt_attr_orb_lat1) && ((i+1) <= tt_attr_orb_lon2)) 
                continue;
            /* same for target_loc */
            if (((i+1) >= tt_attr_target_lat) && ((i+1) <= tt_attr_target_long))
                continue;

            /* skip over all the emitter stuff since it was handled above */
            if (((i+1) >= tt_attr_emitter) && ((i+1) <= tt_attr_emitter_status))
                continue;

            /* skip over all the sonar stuff since it was handled above */
            if (((i+1) >= tt_attr_sonar) && ((i+1) <= tt_attr_sonar_depth))
                continue;

            if (((i+1) >= tt_attr_iff) && ((i+1) <= tt_attr_iffmode_status))   
                continue;          /* smp 2/98 (E1907) */

            if (((i+1) == tt_attr_position))   
                continue;         

            if (((i+1) == tt_attr_sweeparea_location))   
                continue;         

            par = new_param(i+1, sub_mess);
            switch (get_alsp_param_data_type(acm_actor_mess,
                                             type_at_update, i+1)) {  
                case value_int: 
                set_par_int_value(par, int_params[i]);             
                if (((i+1) == tt_attr_id) && (qualifiers[i] > 0))
                        set_par_attr_qual(par, qualifiers[i]);
                break;
                
                case value_float:
                set_par_float_value(par, f_params[i]);
                break;

                case value_enum:
                if (((i+1) == tt_attr_unit_size) &&
                    ((class >= pval_tt_class_air) &&
                     ((class <= pval_tt_class_air_cruise_missile) ||
                      (class <= pval_tt_class_air_tbm)) ))
                    set_par_int_value(par, int_params[i]);
                else
                    my_set_enum_val(acm_actor_mess, type_at_update,
                                    i+1, int_params[i], par);  
                break;

                case value_string:
                strncpy(temp_str, string_params[i], strlens[i]);
                temp_str[strlens[i]] = '\0';
                set_par_string_value(par, temp_str);
                break;
            } /* End of switch */
        }   /* End of if (param_flag[i] > 0 */
    }   /* End of the for loop */

    if (*dispatch_flag > 0)
        par = new_param(par_ta_disp_disp, mess);
    else
        par = new_param(par_ta_event_event, mess);
    set_par_mess_value(par, sub_mess);

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF
    {
    int i,j;
    char string_array[100][N_ATTRIBUTE_VALS];

    printf ( "ALSP_Send_Update %d %d ", *dispatch_flag, *n_args);
    printf("\(");
    for (i=0;  i < *n_args; i++)
        printf("%d ", param_flag[i]);
    printf("\) ");
    printf("\(");
    for (i=0;  i < *n_args; i++)
        printf("%d ", int_params[i]);
    printf("\) ");
    
    printf("\(");
    for (i=0;  i < *n_args; i++)
        printf("%f ", f_params[i]);
    printf("\) ");

    /* make sure strings are terminated with \0 */
    for (i=0; i < *n_args; i++) {
        for (j=0; j < strlens[i]; i++)
           string_array[i][j] = string_params[i][j];
        string_array[i][strlens[i]] = '\0';
    }

    printf("\(");
    for (i=0;  i < *n_args; i++)
        printf("%s ", string_array[i]);
    printf("\) \n");
    }
#endif
}

/*
 * alsp_send_refresh(dispatch_flag,id, num_attr, attr_array)  
 *      int *dispatch_flag;
 *      int *id;
 *      int *num_attr;
 *      int attr_array[];
 *
 *  builds a refresh_request and sends it to the acm.
 *
 *   if dispatch flag is > 0, this is sent as a dispatch message,
 *   otherwise it's sent as an event message.
 *
 *  id is the object id the refresh request is for. 
 *    -1 => refresh request for all objects is generated
 *    -2 => refresh request for a class of objects (in num_attr)
 *          (Note:  This setup removes the option of asking for an
 *                  update of specific attributes for an entire class;
 *                  however, I cannot think when you would want to do
 *                  that.  clk 2/94)
 *
 *  The attr_array paramter is an array of boolean flags for each
 *  possible attribute.  It will be indexed by the enumerated type defined
 *  for TT_ATTRIBUTES in mess_struct.h.  If an attribute is not to
 *  be refreshed,  pass FALSE (< 0) for its flag.
 *
 *  Also, the parameters attr_array can be arrays of attribute enumerated 
 *  values defined for TT_ATTRIBUTES in mess_struct.h.  The option for 
 *  attr_array be either the arrays of boolean or arrays of attribute 
 *  enumerated values is depended on the implementation.  If attr_array 
 *  is implemented to be arrays of boolean, then the MACRO NOT_POSITIONAL 
 *  does not need to be defined, else defined this MACRO.
 *
 *    if num_attr <= 0, it is assumed that no attributes are to be sent.
 */
void alsp_send_refresh(dispatch_flag,id, num_attr, attr_array)  
    int *dispatch_flag;
    int *id;
    int *num_attr;
    int attr_array[];
 
{
    MESSAGE_STRUCT *mess, *sub_mess;
    MESS_PARAM_LIST *par;
    int msg_len, err, i, num_pts;
    char message_str[MAX_MSG_LEN], temp_str[MAX_STR_LEN];
    ALSP_ATTRIBUTE_LIST a_param;

    int  num_attributes;   /* num_attr could be a constant - clk 2/94 */

    num_attributes = *num_attr;

    if (*dispatch_flag > 0)
        mess = new_message(actor_acm_mess, type_ta_dispatch);
    else
        mess = new_message(actor_acm_mess, type_ta_event);

    sub_mess = new_message(acm_actor_mess,type_at_refresh);

/*  code change 24 may 92, used to be *id <= 0 -- Kdz */
    if (*id > 0) {
       par = new_param(par_tt_refresh_id, sub_mess);
       set_par_int_value(par, *id);
    }
    else if (*id < -1) { /* clk 2/94 - making the ability to send a REFRESH_REQUEST(CLASS(...)) */
       par = new_param(par_tt_refresh_class, sub_mess); /* use the class enumerations from update   */
       my_set_enum_val(acm_actor_mess, type_at_update,  /* messages, rather than duplicating them   */
                       tt_attr_class, *num_attr, par);
       num_attributes = 0; /* reset, so that ATTRS(...) will not be in the message */
    }

    if (num_attributes > 0) {
        par = new_param(par_tt_refresh_attrs, sub_mess);
#ifdef NOT_POSITIONAL    
        for (i=0; i <= N_ATTRIBUTE_VALS; i++)   
            a_param[i] = (BOOL)FALSE;           
                                            
        for (i=0; i < num_attributes; i++) {
        if (attr_array[i] == tt_attr_radar  ||  
            attr_array[i] == tt_attr_jammer ||  
            attr_array[i] == tt_attr_index  ||
            attr_array[i] == tt_attr_orient ||
            attr_array[i] == tt_attr_mode   ||
            attr_array[i] == tt_attr_type   ||
            attr_array[i] == tt_attr_emitter_status ||
            attr_array[i] == tt_attr_unit   ||      /* clk 10/94 - Don't want the c2w_factors */
            attr_array[i] == tt_attr_emtr   ||      /* sub-keyword attributes                 */
            attr_array[i] == tt_attr_ew_c3  ||
            attr_array[i] == tt_attr_ew_acq ||
            attr_array[i] == tt_attr_ew_launch ||
            attr_array[i] == tt_attr_ew_guide  ||
            attr_array[i] == tt_attr_ew_power  ||
            attr_array[i] == tt_attr_ew_surf   ||
            attr_array[i] == tt_attr_iffmode_master ||   /* smp 2/98 (E1907) */
            attr_array[i] == tt_attr_iffmode1 ||         /* smp 2/98 (E1907) */
            attr_array[i] == tt_attr_iffmode2 ||         /* smp 2/98 (E1907) */
            attr_array[i] == tt_attr_iffmode3 ||         /* smp 2/98 (E1907) */
            attr_array[i] == tt_attr_iffmodeC ||         /* smp 2/98 (E1907) */
            attr_array[i] == tt_attr_iffmode4 ||         /* smp 2/98 (E1907) */
            attr_array[i] == tt_attr_iffmode_value ||    /* smp 2/98 (E1907) */
            attr_array[i] == tt_attr_iffmode_status ||     /* smp 2/98 (E1907) */
            attr_array[i] == tt_attr_sonar ||  
            attr_array[i] == tt_attr_sonar_type  ||
            attr_array[i] == tt_attr_sonar_status ||
            attr_array[i] == tt_attr_sonar_mode   ||
            attr_array[i] == tt_attr_sonar_depth);
            continue;
            a_param[attr_array[i]] = (BOOL)TRUE;
    }
        set_par_attr_list(par, a_param);
#else
        shift_attr_array(a_param, attr_array);
        set_par_attr_list(par, a_param);
#endif

    }
    
    if (*dispatch_flag > 0)
        par = new_param(par_ta_disp_disp, mess);
    else
        par = new_param(par_ta_event_event, mess);
    set_par_mess_value(par, sub_mess);
    
    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
}

/*
 * ALSP_Send_Interact (dispatch_flag, n_args, param_flag, int_params, f_params,
 *                     strlens, string_params, num_targets, target_list)
 *
 *         int *dispatch_flag;  
 *         int *n_args; 
 *         int param_flag[]; 
 *         int int_params[];
 *         float f_params[];
 *         int strlens[];  
 *         char string_params[][MAX_STR_LEN];
 *         int *num_targets;
 *         int target_list[];
 *
 *  If dispatch_flag > 0, this message will be a dispatch, otherwise
 *  it will be an event.
 *
 *  param_flag indicates which parameters are being passed.  It is an
 *  array of booledan flags indexed by TT_INTERACTION_PARAMS in
 *  mess_struct.h.  If a flag is set to TRUE, it is assumed that its
 *  corresponding paramter is passed. 
 *
 *  The array int_params contains the integer-type parameters being
 *  passed,  f_params contains the float-type parameters being passed
 *  and strlens and string_params combine to pass string parameters
 *  and their lenghts.  Note that the ARRAY_STR_LENS must be the same
 *  between the fortran and c.  It is defined in transif.h.
 *
 *  All arrays are positional.  This function works by examining each
 *  position in param_flag, and if it is TRUE it looks into the
 *  corresponding position in either the int_params, f_params or
 *  string_params array for the value of the parameter.    For
 *  instance, if param_flags[ID] were TRUE, the value would be
 *  obtained from int_params[ID].
 *  
 */

void alsp_send_interact (dispatch_flag, n_args, param_flag, int_params,
                         f_params, strlens, string_params, 
                         num_targets, target_list)

 
    int *dispatch_flag;  /* > 0 means send as dispatch; <= 0 means */
                         /* send as an event */ 
    int *n_args;         /* number of parameters passed this message */
    int param_flag[];    /* each element of this array will be an */
                         /* integer corresponding to the enumeration for */
                         /* the parameter provided */
    int int_params[];
    float f_params[];
    int strlens[];       /* lengths of the strings in string_params array */
    char string_params[][ARRAY_STR_LENS];
    int *num_targets;    /* number of targets for TARGETS(...) */
    int target_list[];  /* enumeration list of targets */
{
    MESSAGE_STRUCT *mess, *sub_mess;
    MESS_PARAM_LIST *par;
    int msg_len, err, i;
    char message_str[MAX_MSG_LEN], temp_str[MAX_STR_LEN];

#ifdef NODEF
    if (*n_args != N_INTERACTION_PAR) {
        printf("alsp_send_update:  incorrect number of parameters\n");
        return;
    }
#endif
    if (*dispatch_flag > 0)
        mess = new_message(actor_acm_mess, type_ta_dispatch);
    else
        mess = new_message(actor_acm_mess, type_ta_event);

    sub_mess = new_message(acm_actor_mess,type_at_interaction);

    for (i=0; i < N_INTERACTION_PAR; i++) {
        if (param_flag[i] > 0) {
            par = new_param(i+1, sub_mess);
            switch (get_alsp_param_data_type(acm_actor_mess,
                                             type_at_interaction, i+1)) { 
                case value_int: 
                set_par_int_value(par, int_params[i]);             
                break;
                
                case value_float:
                set_par_float_value(par, f_params[i]);
                break;

                case value_enum:
                my_set_enum_val(acm_actor_mess, type_at_interaction,
                                i+1, int_params[i], par);  
                break;

                case value_string:
                strncpy(temp_str, string_params[i], strlens[i]);
                temp_str[strlens[i]] = '\0';
                set_par_string_value(par, temp_str);
                break;

                case value_enum_list:
                /* only parameter this can be is target list */
                my_set_enum_list(acm_actor_mess, type_at_interaction, i+1, 
                                target_list, *num_targets, par);
                break;

                default:
#ifdef DEBUG
                printf("send_interact:  got unkown parameter type\n:");          
#endif
                break;
            }

        }
    }

    if (*dispatch_flag > 0)
        par = new_param(par_ta_disp_disp, mess);
    else
        par = new_param(par_ta_event_event, mess);
    set_par_mess_value(par, sub_mess);

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
#ifdef NODEF
    {
    int j;
    char string_array[100][N_ATTRIBUTE_VALS];

    printf ( "ALSP_Send_Interact %d %d ", *dispatch_flag, *n_args);
    printf("\(");
    for (i=0;  i < *n_args; i++)
        printf("%d ", int_params[i]);
    printf("\) ");
    
    printf("\(");
    for (i=0;  i < *n_args; i++)
        printf("%f ", f_params[i]);
    printf("\) ");

    /* make sure strings are terminated with \0 */
    for (i=0; i < *n_args; i++) {
        for (j=0; j < strlens[i]; i++)
           string_array[i][j] = string_params[i][j];
        string_array[i][strlens[i]] = '\0';
    }

    printf("\(");
    for (i=0;  i < *n_args; i++)
        printf("%s ", string_array[i]);
    printf("\) ");
    }
   
#endif

}

/*  alsp_save_request(save_time,label_len,label)
 *      float *save_time;
 *      int *label_len;
 *      struct dsc$descriptor_s *label;
 *
 *  Builds a save request message and sends it to the ACM
 *
 *  save_time is a required parameter and specifies the time of
 *  the save.
 *
 *  The label for the save is optional.  The length of the label must
 *  be passed in label_len.  A label_len <= 0 indicates no label.
 */
void alsp_save_request(save_time,label_len,label)
    float *save_time;
    int *label_len;
    char *label;
{
    int i, err;
    char label_str[100];

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];

    mess = new_message(actor_acm_mess, type_ta_save_request);
    par = new_param(par_ta_save_req_time, mess);
    set_par_float_value(par, *save_time);

/*  Check for optional save label                           */

    if (*label_len > 0) {
      strcpy(label_str, label);

    par = new_param(par_ta_save_req_label, mess);
    set_par_string_value(par, label_str);

    }

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF

    if (*label_len > 0)
        printf( "ALSP_Save_Request %f %s\n",*save_time,label_str);
    else
        printf( "ALSP_Save_Request %f\n",*save_time);
    
    printf( "\n");
    
#endif
}

/*  alsp_save_ack (sim_time, real_time);
 *      float *save_time;
 *      char   real_time [8];
 *
 *  Builds an acknowledge message to a start_save request and sends 
 *  it to the ACM
 *
 *  sim_time is a required parameter that specifies the time of the
 *  the simulation save in decimal hours.
 *  real_time is a required parameter that specifies the computer clock
 *  time in the following format:  hh:mm:ss.
 */
void alsp_save_ack (sim_time, real_time)
    float *sim_time;
    char   real_time [8];
{
    int i, err;

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str [MAX_MSG_LEN];

    mess = new_message (actor_acm_mess, type_ta_save_ack);

    par = new_param     (par_ta_save_ack_time, mess);
    set_par_float_value (par, *sim_time);

    par = new_param      (par_ta_save_ack_clock, mess);
    set_par_string_value (par, real_time);

    gen_alsp_message (mess, &msg_len, message_str);
    err = my_alsp_write_msg (message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF
    printf ("ALSP Save_ACK - sim_time=%f   real_time=%s\n", *sim_time, real_time);
#endif
}


/*  alsp_save_complete (sim_time, real_time);
 *      float *sim_time;
 *      char   real_time [8];
 *
 *  Builds the save_complete message and sends it to the ACM (to be called
 *  after a save_ack was send followed by a game state save).
 *
 *  sim_time is a required parameter that specifies the time of the
 *  the simulation save in decimal hours.
 *  real_time is a required parameter that specifies the computer clock
 *  time in the following format:  hh:mm:ss.
 */
void alsp_save_complete (sim_time, real_time)
    float *sim_time;
    char   real_time [8];
{
    int i, err;

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str [MAX_MSG_LEN];

    mess = new_message (actor_acm_mess, type_ta_save_complete);

    par = new_param     (par_ta_save_complete_time, mess);
    set_par_float_value (par, *sim_time);

    par = new_param      (par_ta_save_complete_clock, mess);
    set_par_string_value (par, real_time);

    gen_alsp_message (mess, &msg_len, message_str);
    err = my_alsp_write_msg (message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF
    printf ("ALSP Save_Complete - sim_time=%f   real_time=%s\n", *sim_time, real_time);
#endif
}


void alsp_ping (text_len, text)
/*
 * void alsp_ping (text_len, text)           clk  1/94
 *    
 *    Sets up and sends an Actor to ACM PING(...) message.
 *    The optional parameter of the PING message is a single free text
 *    string.  The ACM should in response send a PING_ACK message.
 *
 */

    int *text_len;
    char *text;
{
    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len,i;
    char text_str[ARRAY_STR_LENS];
    char message_str[MAX_MSG_LEN];
    int err;

    mess = new_message(actor_acm_mess, type_ta_ping);

    if (*text_len > 0)  /* PING([text]), [text] is optional */
    {
      par = new_param (par_ta_ping_text, mess);
      strncpy (text_str, text, *text_len);
      text_str[*text_len] = '\0';
      set_par_string_value(par, text_str);
    }

    gen_alsp_message (mess, &msg_len, message_str);
    err = my_alsp_write_msg (message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF
    printf( "ALSP_Ping: %s\n", text_str);  
#endif
}

void alsp_ping_ack (text_len,text)
/*
 * void alsp_ping_ack (text_len, text)       clk  1/94
 *    
 *    Sets up and sends an Actor to ACM PING_ACK(...) message, which
 *    is send in response to a ACM to Actor PING() message.  The
 *    optional parameter of the PING message is a single free text
 *    string.
 *
 */

    int *text_len;
    char *text;
{
    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len,i;
    char text_str[ARRAY_STR_LENS];
    char message_str[MAX_MSG_LEN];
    int err;

    mess = new_message(actor_acm_mess, type_ta_ping_ack);

    if (*text_len > 0)  /* PING_ACK([text]), [text] is optional */
    {
      par = new_param (par_ta_ping_ack_text, mess);
      strncpy (text_str, text, *text_len);
      text_str[*text_len] = '\0';
      set_par_string_value(par, text_str);
    }

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF
    printf( "ALSP_Ping_ACK: %s\n", text_str);  
#endif
}


void alsp_setup_emitter_list(emitter_list, num_emitter, 
                             emitcategory_list, emittype_list,
                             emitstatus_list, emitindex_list, 
                             emitorient_list, emitmode_list)

    EMITTERLIST emitter_list;
    int *num_emitter;       /* num of emitters */
    int emitcategory_list[];/* category, JAMMER or RADAR */
    int emittype_list[];    /* enumerated list */
    int emitstatus_list[];  /* on or off status */
    int emitindex_list[];   /* index num of emitter on unit */
    float emitorient_list[];/* orientation of jammer */
    int emitmode_list[];    /* mode enumeration type */
{
    int index;
    char temp_str[ARRAY_STR_LENS];
    char *etab;
    int max;
    BOOL category_flag, type_flag, index_flag;
    BOOL status_flag, orient_flag, mode_flag;

    if (*num_emitter >= MAX_EMITTERS) {
        printf ("Too many emitter values in alsp_setup_emitter_list, increase MAX_EMITTERS\n");
        return;
    }

    for (index = 0; index < *num_emitter; index++) {
        category_flag = (BOOL)FALSE;
        type_flag = (BOOL)FALSE;
        index_flag = (BOOL)FALSE; 
        status_flag = (BOOL)FALSE;
        orient_flag = (BOOL)FALSE;
        mode_flag = (BOOL)FALSE;
        
        /* CATEGORY - RADAR or JAMMER, required */
        emitter_list[index].emitter_cat = emitcategory_list[index];
        category_flag = (BOOL)TRUE;

        /* TYPE ,not required*/
        pval_table_lookup_by_type(acm_actor_mess, type_at_update,
                                  tt_attr_type, &etab, &max);
        if (max == 0) {
#ifdef DEBUG
            printf("alsp_setup_emitter_list - type:  failed in table lookup for %d, %d, %d\n", mtype, msubtype, ptype);
#endif
            return;
        }
        if (emittype_list[index] != 0) {
            emitter_list[index].emitter_type.enum_value = emittype_list[index];
            emitter_list[index].emitter_type.enum_string = 
                                get_keyword(etab, emittype_list[index]); 
            type_flag = (BOOL)TRUE;
        } else {
            type_flag = (BOOL)FALSE;
        }
    
        /* INDEX, required */
        emitter_list[index].emitter_index = emitindex_list[index];
        index_flag = (BOOL)TRUE;

        /* EMITTER_STATUS, required */
        pval_table_lookup_by_type(acm_actor_mess, type_at_update,
                                  tt_attr_emitter_status, &etab, &max);
        if (max == 0) {
#ifdef DEBUG
            printf("alsp_setup_emitter_list - type:  failed in table lookup for %d, %d, %d\n", mtype, msubtype, ptype);
#endif
            return;
        }
        emitter_list[index].emitter_status.enum_value = 
                                emitstatus_list[index];
        emitter_list[index].emitter_status.enum_string = 
                                get_keyword(etab, emitstatus_list[index]); 
        status_flag = (BOOL)TRUE;

        /* ORIENT, not required and only needed for jammer */
        if (emitcategory_list[index] == tt_attr_jammer) {
            if (emitorient_list[index] > -1.0) {
                emitter_list[index].emitter_orientation = 
                                                    emitorient_list[index]; 
                orient_flag = (BOOL)TRUE;
            } else {
                orient_flag = (BOOL)FALSE;
        }

            /* MODE, not required and only needed for comm jammer */
            pval_table_lookup_by_type(acm_actor_mess, type_at_update,
                                  tt_attr_mode, &etab, &max);
            if (max == 0) {
#ifdef DEBUG
                printf("alsp_setup_emitter_list - mode:  failed in table lookup for %d, %d, %d\n", mtype, msubtype, ptype);
#endif
                return;
            }
            if (emitmode_list[index] > 0) {
                emitter_list[index].emitter_mode.enum_value = 
                                    emitmode_list[index];
                emitter_list[index].emitter_mode.enum_string = 
                                    get_keyword(etab, emitmode_list[index]); 
                mode_flag = (BOOL)TRUE;
            } else {
                mode_flag = (BOOL)FALSE;
            } 
        } /* end of if category is a jammer */

        emitter_list[index].emitter_params[0] = category_flag; 
        emitter_list[index].emitter_params[1] = type_flag; 
        emitter_list[index].emitter_params[2] = index_flag; 
        emitter_list[index].emitter_params[3] = status_flag;
        emitter_list[index].emitter_params[4] = orient_flag;
        emitter_list[index].emitter_params[5] = mode_flag;
    } /* end of for loop thru number of emitters */
}


void alsp_setup_sonar_list(sonar_list, num_sonar, 
                           sonartype_list,
                           sonarstatus_list,
                           sonarmode_list,
                           sonardepth_list)

    SONARLIST sonar_list;
    int *num_sonar;          /* num of sonars */
    int sonartype_list[];    /* enumerated list */
    int sonarstatus_list[];  /* on or off status */
    int sonarmode_list[];    /* mode enumeration type */
    int sonardepth_list[];   /* sonar depth */
{
    int index;
    char temp_str[ARRAY_STR_LENS];
    char *etab;
    int max;
    BOOL type_flag;
    BOOL status_flag, mode_flag, depth_flag;

    if (*num_sonar >= MAX_SONARS) {
        printf ("Too many sonar values in alsp_setup_sonar_list, increase MAX_SONARS\n");
        return;
    }

    for (index = 0; index < *num_sonar; index++) {
        type_flag   = (BOOL)FALSE;
        status_flag = (BOOL)FALSE;
        mode_flag   = (BOOL)FALSE;
        depth_flag  = (BOOL)FALSE;
        
        /* TYPE ,not required*/
        pval_table_lookup_by_type(acm_actor_mess, type_at_update,
                                  tt_attr_sonar_type, &etab, &max);
        if (max == 0) {
#ifdef DEBUG
            printf("alsp_setup_sonar_list - type:  failed in table lookup for %d, %d, %d\n", acm_actor_mess, type_at_update, tt_attr_type);
#endif
            return;
        }
        if (sonartype_list[index] != 0) {
            sonar_list[index].sonar_type.enum_value = sonartype_list[index];
            sonar_list[index].sonar_type.enum_string = 
                                get_keyword(etab, sonartype_list[index]); 
            type_flag = (BOOL)TRUE;
        } else {
            type_flag = (BOOL)FALSE;
        }
    
        /* SONAR_STATUS, required */
        pval_table_lookup_by_type(acm_actor_mess, type_at_update,
                                  tt_attr_sonar_status, &etab, &max);
        if (max == 0) {
#ifdef DEBUG
            printf("alsp_setup_sonar_list - type:  failed in table lookup for %d, %d, %d\n", acm_actor_mess, type_at_update, tt_attr_sonar_status);
#endif
            return;
        }
        sonar_list[index].sonar_status.enum_value = 
                                sonarstatus_list[index];
        sonar_list[index].sonar_status.enum_string = 
                                get_keyword(etab, sonarstatus_list[index]); 
        status_flag = (BOOL)TRUE;

        /* MODE, not required and only needed for hull mounted active sonar */
        pval_table_lookup_by_type(acm_actor_mess, type_at_update,
                                  tt_attr_sonar_mode, &etab, &max);
        if (max == 0) {
#ifdef DEBUG
            printf("alsp_setup_sonar_list - mode:  failed in table lookup for %d, %d, %d\n", acm_actor_mess, type_at_update, tt_attr_mode);
#endif
            return;
        }
        if (sonarmode_list[index] > 0) {
            sonar_list[index].sonar_mode.enum_value = 
                              sonarmode_list[index];
            sonar_list[index].sonar_mode.enum_string = 
                              get_keyword(etab, sonarmode_list[index]); 
            mode_flag = (BOOL)TRUE;
        } else {
            mode_flag = (BOOL)FALSE;
        } 

        /* DEPTH, not required and only needed for variable depth sonar */
        if (sonardepth_list[index] > -1) {
                sonar_list[index].sonar_depth = sonardepth_list[index]; 
            depth_flag = (BOOL)TRUE;
        } else {
            depth_flag = (BOOL)FALSE;
        }

        sonar_list[index].sonar_params[0] = type_flag; 
        sonar_list[index].sonar_params[1] = status_flag;
        sonar_list[index].sonar_params[2] = mode_flag; 
        sonar_list[index].sonar_params[3] = depth_flag;
    } /* end of for loop thru number of sonars */
}


void alsp_setup_position_list(position_list, num_position, 
                              positionlat_list,
                              positionlon_list)

    POSITIONLIST position_list;
    int *num_position;          /* num of positions */
    float positionlat_list[];   /* latitude list */
    float positionlon_list[];   /* longitude list */
{
    int index;
    char temp_str[ARRAY_STR_LENS];
    char *etab;
    int max;

    if (*num_position >= MAX_POSITIONS) {
        printf ("Too many positions in alsp_setup_position_list, increase MAX_POSITIONS\n");
        return;
    }

    for (index = 0; index < *num_position; index++) {
        
        position_list[index].latitude = 
                                positionlat_list[index];
        position_list[index].longitude = 
                                positionlon_list[index]; 

    } /* end of for loop thru number of positions */
}

/*
 *  alsp_filter_parameters (kind, to, from)
 *        int *kind;
 *        int *to;
 *        int *from;
 *
 *  Builds a filter_parameters message and sends to the acm.
 *
 *  Author:    Susan Miller - 9/96  (E1691)
 *
 *  Modified:  Carol Kropp - 1/97 (E1691)
 *             Allow the to and from to be optional by passing in
 *             the pval_ta_fp_restrictions_dummy value.
 */

void alsp_filter_parameters  (kind, to, from)  
    int *kind;  /* interaction KIND enumeration */
    int *to;    /* interaction TO enumeration */
    int *from;  /* interaction FROM enumeration */

{
    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];
    int i, err;

    mess = new_message (actor_acm_mess, type_ta_filter_parameters);

    par  = new_param (par_ta_filter_parameters_kind, mess);
    my_set_enum_val  (actor_acm_mess, type_ta_filter_parameters,
                      par_ta_filter_interaction_kind, *kind, par); 

    if (*to > pval_ta_fp_restrictions_dummy)
      {
       par = new_param (par_ta_filter_parameters_to, mess);
       my_set_enum_val (actor_acm_mess, type_ta_filter_parameters,
                        par_ta_filter_parameters_to, *to, par); 
      }   /* end of if the TO() is to be included */
                     
    if (*from > pval_ta_fp_restrictions_dummy)
      {
       par = new_param (par_ta_filter_parameters_from, mess);
       my_set_enum_val (actor_acm_mess, type_ta_filter_parameters,
                        par_ta_filter_parameters_from, *from, par); 
      }   /* end of if the FROM() is to be included */

    gen_alsp_message (mess, &msg_len, message_str);
    err = my_alsp_write_msg (message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF
    printf( "ALSP_Filter_Parameters %d %d %d\n", *kind, *to, *from);
#endif
}

/*
 * void alsp_send_error (text_len, text)
 *    
 *    Purpose:  Sets up and sends an Actor to ACM ERROR(...) message
 *              when a unit is not created due to either a full blackboard, 
 *              non-enumerated class value, enumerated class that is not 
 *              present in blackboard or some other error in the create 
 *              message received from the ACM.
 *              The text parameter of the ERROR message is a single free text
 *              string.
 *
 *    Date:     9/16/96 (E1692)
 *    Author:   Susan Miller
 */
void alsp_send_error (text_len, text)

    int *text_len;
    char *text;
{
    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len,i;
    char text_str[ARRAY_STR_LENS];
    char message_str[MAX_MSG_LEN];
    int err;

    mess = new_message(actor_acm_mess, type_ta_error);

    if (*text_len > 0)  /* ERROR([text]) */
    {
      par = new_param (par_ta_error_text, mess);
      strncpy (text_str, text, *text_len);
      text_str[*text_len] = '\0';
      set_par_string_value(par, text_str);
    }

    gen_alsp_message(mess, &msg_len, message_str);
    err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);

#ifdef NODEF
    printf( "ALSP_Error: %s\n", text_str);  
#endif
}

/*
 * ALSP_Send_Enum(register_type,attribute_type,enum_index,
 *                RESA_num_of_names,RESA_name,ALSP_class,
 *                default_flag,default_RESA,RESA_name_len) 
 *
 *         int *register_type; 
 *         int *attribute_type;  
 *         int *enum_index; 
 *         int *RESA_num_of_names;
 *         char RESA_name[][MAX_STR_LEN];
 *         int *ALSP_class;
 *         int *default_flag;
 *         char default_RESA[];
 *         int RESA_name_len[MAX_STR_LEN];
 *
 *  Builds a register enumeration message and sends to the acm.
 *
 *  If register_type = 3, this message will be a register_enumeration KIND
 *  type of message.  If register_type = 2, this message will be a 
 *  register_enumeration CLASS type of message.
 *
 *  attribute_type indicates the type of attribute:  UNIT_TYPE, EMITTER,
 *  WEAPON, etc.
 *
 *  enum_index indicates the integer number of the related enumeration
 *
 *  RESA_num_of_names indicates the total number of RESA database names
 *  associated with the enum_index.
 *
 *  RESA_name is an array holding all of the RESA names associated with 
 *  the enum_index.
 *
 *  ALSP_class 
 *  
 *  default_flag is $yes when there are more than one RESA names associated
 *  with the enum_index indicating that an additional parameter needs to 
 *  be sent in the register_enumeration message - default
 *
 *  default_RESA is the first RESA name associated with a particular 
 *  enum_index
 *  
 *  Susan Miller - E1694 Dec 1996
 */
void alsp_send_enum(register_type,attribute_type,enum_index,
                    RESA_num_of_names, RESA_name, RESA_name_len,
                    ALSP_class,
                    default_flag, default_RESA, default_RESA_len)

int *register_type;
int *attribute_type;
int *enum_index;
int *RESA_num_of_names; 
char RESA_name[][5];    /* current max RESA name length = 5 */
int *ALSP_class;
int *default_flag; 
char default_RESA[5];
int RESA_name_len[];
int *default_RESA_len;

{
 MESSAGE_STRUCT *mess, *sub_mess;
 MESS_PARAM_LIST *par;
 int msg_len;
 char message_str[MAX_MSG_LEN];
 int err;
 char text_str[ARRAY_STR_LENS] = "";
 int names_to_move = 0;
 int text_len = 0, loop_counter = 0;
 int RESA_len = 0;
 char *blank_pntr;

 /* REGISTER_ENUMERATION */
 mess = new_message(actor_acm_mess, type_ta_reg_enum);
 sub_mess = new_message(actor_acm_mess, type_ta_reg_enum);
 
 if (*register_type == tt_attr_class)   /* class type message */
    {
     /* CLASS */
     par = new_param(par_ta_reg_enum_class, mess);
     /* AIR.FIXEDWING or AIR.HELICOPTER or etc. */
     my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                     par_ta_reg_enum_class, *ALSP_class, par); 

     /* ATTRIBUTE */
     if ((*attribute_type == tt_attr_radar) ||
         (*attribute_type == tt_attr_jammer))
        {
         par = new_param(par_ta_reg_enum_attr, mess);
         my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                     par_ta_reg_enum_attr, tt_attr_emitter, par); 

         /* EMITTER - actually store RADAR or JAMMER */
         if (*attribute_type == tt_attr_radar) {
             par = new_param(par_ta_reg_enum_radar, mess);
             my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                         par_ta_reg_enum_radar, tt_attr_type, par); 
         }
         else if (*attribute_type == tt_attr_jammer) {
             par = new_param(par_ta_reg_enum_jammer, mess);
             my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                         par_ta_reg_enum_jammer, tt_attr_type, par); 
         }
         /* ALSP_VALUE */
         par = new_param(par_ta_reg_enum_alsp_val, mess);
         /* an enumerated name */ 
         my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                         par_ta_reg_enum_type, *enum_index, par); 
        }
     else 
        {
         /* ATTRIBUTE */
         par = new_param(par_ta_reg_enum_attr, mess);
         my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                     par_ta_reg_enum_attr, *attribute_type, par); 

         /* UNIT_TYPE or etc. */
         my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                         par_ta_reg_enum_attr, *attribute_type, par); 

         if (*attribute_type == tt_attr_unit_type)
            {
             /* ALSP_VALUE */
             par = new_param(par_ta_reg_enum_alsp_val, mess);
             /* an enumerated name */ 
             my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                            par_ta_reg_enum_unit_type, *enum_index, par); 
            }         
        }
    }    
 else                                   /* kind type message */
    {
     /* KIND */
     par = new_param(par_ta_reg_enum_kind, mess);
     /* ENGAGEMENT.AIR_TO_AIR or etc. */
     my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                     par_ta_reg_enum_kind, *ALSP_class, par); 

     /* ATTRIBUTE */
     par = new_param(par_ta_reg_enum_attr, mess);
     my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                     par_ta_reg_enum_attr, *attribute_type, par); 

     /* ALSP_VALUE */
     par = new_param(par_ta_reg_enum_alsp_val, mess);
     my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                     par_ta_reg_enum_weapon, *enum_index, par); 
    }

 /* ACTOR_VALUE */
 par = new_param(par_ta_reg_enum_actor_val, mess);
 names_to_move = *RESA_num_of_names;
 while (names_to_move > 0)
    {
     strncat (text_str,RESA_name[loop_counter],RESA_name_len[loop_counter]);
     names_to_move--;
     loop_counter++;
     blank_pntr = strchr (text_str, ' ');
     if (blank_pntr != 0) *blank_pntr = '\0';
     if (names_to_move > 0) 
       {
        text_len = strlen(text_str);
        text_str[text_len] = ',';
       }
    }
 text_len = strlen (text_str);
 text_str[text_len] = '\0';
 set_par_string_value(par, text_str);

 if (*default_flag > 0)
    {
     /* DEFAULT */
     par = new_param(par_ta_reg_enum_default, mess);
     my_set_enum_val(actor_acm_mess, type_ta_reg_enum,
                 par_ta_reg_enum_alsp_val, default_RESA, par); 

     strncpy(text_str, default_RESA, *default_RESA_len);
     text_str[*default_RESA_len] = '\0';
     set_par_string_value(par, text_str);
    }    

 gen_alsp_message(mess, &msg_len, message_str);
 err = my_alsp_write_msg(message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
 free_message (mess);
 }


/*  alsp_enumerations_complete ();
 *
 *  Builds the SIGNAL(ENUMERATIONS_COMPLETE) message and sends it 
 *  to the ACM to be called after the REGISTER_ENUMERATION messages
 *  have been sent. 
 *
 *  Susan Miller - E1694 Dec 1996
 */
void alsp_enumerations_complete ()
{
 MESSAGE_STRUCT *mess;
 MESS_PARAM_LIST *par;
 int msg_len;
 char message_str [MAX_MSG_LEN];
 int err;

 mess = new_message (actor_acm_mess, type_ta_signal);

 par = new_param (par_ta_signal_type, mess);
 my_set_enum_val(actor_acm_mess, type_ta_signal,
                 par_ta_signal_type, pval_ta_signal_enumerations_complete, 
                 par);

 gen_alsp_message (mess, &msg_len, message_str);
 err = my_alsp_write_msg (message_str, msg_len);

#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
 free_message (mess);
}


/* alsp_register_rate (game_rate)
 *
 *    Builds an ALSP REGISTER_RATE message and sends it to the ACM.
 * 
 *    Game_rate is the x value from x:1 - negative numbers are assumed
 *    to be paused, so a 0.0 will be sent.
 * 
 *    Date:    September 1997 (E1892)
 *    Author:  Carol Kropp
 * 
 */
void alsp_register_rate (game_rate)
 
    float *game_rate;  
{
    int err;

    MESSAGE_STRUCT *mess;
    MESS_PARAM_LIST *par;
    int msg_len;
    char message_str[MAX_MSG_LEN];

    mess = new_message (actor_acm_mess, type_ta_reg_rate);
    par = new_param (par_ta_reg_rate_gamerate, mess);
    if (*game_rate >= 0.0)
      { set_par_float_value(par, *game_rate); }
    else
      { set_par_float_value(par, 0.0); }

    gen_alsp_message (mess, &msg_len, message_str);
    err = my_alsp_write_msg (message_str, msg_len);
#ifdef DEBUG
    dump_mess_struct (mess); 
#endif
    free_message (mess);
}


/* smp 2/98 (E1907) */
void alsp_setup_iffmode_list(param_flag,
                             iffmaster, 
                             iffmode_status_array, 
                             iffmode_value_array,
                             iffmode_array)
    int param_flag[];
    int *iffmaster;
    int iffmode_status_array[];     /* iffmode status */
    int iffmode_value_array[];      /* iffmode values */
    IFFMODESTR iffmode_array[N_IFF_PARAMS];
{
    int index;
    char temp_str[ARRAY_STR_LENS];
    char *etab;
    int max;

    for (index = 0; index <= MAX_IFFMODES; index++) {
        iffmode_array[index].iffmode_params = FALSEVAL;
    }

    /* MASTER required */
    pval_table_lookup_by_type(acm_actor_mess, type_at_update,
                              tt_attr_iffmode_status, &etab, &max);
    if (max == 0) {
#ifdef DEBUG
       printf("alsp_setup_iffmode_list - status:  failed in table lookup.\n");
#endif
       return;
    }

    if (param_flag[tt_attr_iffmode_master-1] > 0)
        {
         iffmode_array[0].iffmode_params = TRUEVAL;
         iffmode_array[0].iffmode_status.enum_value = *iffmaster;
         iffmode_array[0].iffmode_status.enum_string = 
                                 get_keyword(etab, *iffmaster); 
         iffmode_array[0].iffmode_value = 0; 
    
         iffmode_array[5].iffmode_params = TRUEVAL;
         iffmode_array[5].iffmode_status.enum_value = *iffmaster;
         iffmode_array[5].iffmode_status.enum_string = 
                                 get_keyword(etab, *iffmaster); 
         iffmode_array[5].iffmode_value = 0; 
    }

    for (index = 1; index < N_IFF_PARAMS; index++) {
        if (param_flag[tt_attr_iffmode_master-1+index] > 0) {
/*         if ((iffmode_value_array[index - 1] != 0) && (index != 5)) { */

          iffmode_array[index].iffmode_params = TRUEVAL;
          iffmode_array[index].iffmode_status.enum_value = iffmode_status_array[index-1];
          iffmode_array[index].iffmode_status.enum_string = 
                                get_keyword(etab, iffmode_status_array[index-1]); 
          iffmode_array[index].iffmode_value = iffmode_value_array[index-1]; 
       }
    } /* end of for loop thru number of iffmodes */
}
