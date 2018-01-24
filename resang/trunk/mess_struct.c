#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_types.h"
#include "ascii_tables.h"
#include "mess_format.h"
#include "mess_struct.h"
#include "alsp_utils.h"

/* Allocate a structure for a new message, and set its type to mtype */
MESSAGE_STRUCT *new_message (mtype, msubtype)

    MESSAGE_DATA_TYPE mtype;
    int msubtype;
{
    MESSAGE_STRUCT *new;

    new = (MESSAGE_STRUCT *) malloc (sizeof (*new));
    if (new == NULL) {
        alsp_console_message ("Malloc failure in new_message.");
        exit (0);
    } else {
        new->header.alsp_mess_type.mess_type = mtype;
        switch (mtype) {
            case acm_actor_mess:
                new->header.alsp_mess_type.mess_subtype.at_type
                    = (AT_MESSAGE_TYPE) msubtype;
                break;
            case actor_acm_mess:
                new->header.alsp_mess_type.mess_subtype.ta_type
                    = (TA_MESSAGE_TYPE) msubtype;
                break;
            case actor_actor_mess:
                new->header.alsp_mess_type.mess_subtype.tt_type
                    = (TT_MESSAGE_TYPE) msubtype;
                break;
            default:
                alsp_console_message ("Illegal message type in new_message");
                exit (0);
        }   
        new->params = NULL;
    }
    return (new);
}

/* Make a copy of orig */  
MESSAGE_STRUCT *copy_message (orig)
/* Modified:  Stephanie Paden - 2/98 (E1907)                   */
/*            Added case to handle the IFF structure list, for */
/*            sending/receiving IFF data over ALSP.            */
    
    MESSAGE_STRUCT *orig;
{
    MESSAGE_STRUCT *copy;
    MESS_HEADER header;
    MESS_PARAM_LIST *params, *par_copy;
    int msubtype;

    header = orig->header;
    switch (header.alsp_mess_type.mess_type) {
        case actor_actor_mess:
            msubtype = (int) header.alsp_mess_type.mess_subtype.tt_type;
            break;
        case actor_acm_mess:
            msubtype = (int) header.alsp_mess_type.mess_subtype.ta_type;
            break;
        case acm_actor_mess:
            msubtype = (int) header.alsp_mess_type.mess_subtype.at_type;
            break;
        default:
            alsp_console_message ("Unknown message type in copy_message");
    }

    copy = new_message (header.alsp_mess_type.mess_type, msubtype);
    params = orig->params;

    while (params != NULL) {
        par_copy = new_param (params->param_type, copy);
        set_par_attr_qual (par_copy, get_par_attr_qual (params));
    
        switch (params->param_data.val_type) {
            case value_int:
                set_par_int_value (par_copy, get_par_int_value (params));
                break;
            case value_int_range:
                {
                    int lower, upper;

                    get_par_int_range (params, &lower, &upper);
                    set_par_int_range (par_copy, lower, upper);
                }
                break;
            case value_int_list:
                {
                    INTLIST list;
                    int nvals;

                    get_par_int_list (params, list, &nvals);
                    set_par_int_list (par_copy, list, nvals);
                }
                break;
            case value_enum:
                set_par_enum_value
                    (par_copy, get_par_enum_value (params));
                set_par_enum_string
                    (par_copy, get_par_enum_string (params));
                break;
            case value_loc_struct:
                {
                    float lat, lon;

                    get_par_loc_value (params, &lat, &lon);
                    set_par_loc_value (par_copy, lat, lon);
                }
                break;
            case value_loc_struct_list:
                {
                    LOCLIST list;
                    int nvals;

                    get_par_loc_list (params, list, &nvals);
                    set_par_loc_list (par_copy, list, nvals);
                }
                break;
            case value_emit_struct_list:
                {
                    EMITTERLIST list;
                    int nvals;

                    /* send the address of number of values */
                    get_par_emitter_list (params, list, &nvals);
                    set_par_emitter_list (par_copy, list, &nvals);

                }
                break;
            case value_sonar_struct_list:
                {
                    SONARLIST list;
                    int nvals;

                    /* send the address of number of values */
                    get_par_sonar_list (params, list, &nvals);
                    set_par_sonar_list (par_copy, list, &nvals);

                }
                break;
            case value_iff_struct_list:    /* smp 2/98 (E1907) */
                {
                    /*IFFMODELIST listIFF;*/
                    IFFMODESTR iffmode_array[N_IFF_PARAMS]; 
                    int nvals;

                    /* send the address of number of values */
                    get_par_iffmode_list (params, iffmode_array);
                    set_par_iffmode_list (par_copy, iffmode_array);
                }
                break;
            case value_enum_list:
                {
                    ENUMLIST list;
                    int nvals;

                    get_par_enum_list (params, list, &nvals);
                    set_par_enum_list (par_copy, list, nvals);
                }
                break;
            case value_float:
                set_par_float_value (par_copy, 
                    get_par_float_value (params));
                break;
            case value_float_range:
                {
                    float lower, upper;

                    get_par_float_range (params, &lower, &upper);
                    set_par_float_range (par_copy, lower, upper);
                }
                break;
            case value_string:
                set_par_string_value
                    (par_copy, get_par_string_value (params));
                break;
            case value_message:
                set_par_mess_value (par_copy, 
                    copy_message (get_par_mess_value (params)));
                break;
            case value_attribute_list:
                {
                    ALSP_ATTRIBUTE_LIST a;

                    get_par_attr_list (params, a);
                    set_par_attr_list (par_copy, a);
                    break;
                }
            default:
                alsp_console_message ("Unknown value type in copy_message");
            }
        params = params->next;
    }
    
    return (copy);
}

/* Free any dynamically memory space used by par.  */
void free_param_space (par)

    MESS_PARAM_LIST *par;
{
    if (par->param_data.val_type == value_string) {
        if (par->param_data.val.val_string != NULL) {
            free (par->param_data.val.val_string);
            par->param_data.val.val_string = NULL;
        }
    }
    if (par->param_data.val_type == value_message) {
        if (par->param_data.val.mess != NULL) {
            free_message (par->param_data.val.mess);
            par->param_data.val.mess = NULL;
        }
    }
}

/* Free the memory occupied by the parameter list for the current message. */
void free_params (m)

    MESSAGE_STRUCT *m;
{
    MESS_PARAM_LIST *temp, *list;

    list = m->params;
    while (list != NULL) {
        temp = list;
        free_param_space (temp);
        list = list->next;
        free (temp);
    }
}

/* Free up the memory occupied by m. */
void free_message (m)

    MESSAGE_STRUCT *m;
{
    free_params (m);
    free (m);
}

/* Add a new parameter to the parameter list for the specified message */
MESS_PARAM_LIST *new_param (ptype, mess)

    PARAM_TYPE ptype;
    MESSAGE_STRUCT *mess;
{
    MESS_PARAM_LIST *new, *list, *prev, *next;

    new = (MESS_PARAM_LIST *) malloc (sizeof (*new));
    if (new == NULL) {
        alsp_console_message ("Malloc failure in new_param.");
        exit (0);
    } else {
        new->param_type = ptype;
        new->param_data.qual = qual_none;
        new->param_data.val_type = value_int;
        list = mess->params;
        prev = NULL; next = NULL;
        while (list != NULL) {
            next = list->next;
            prev = list;
            list = list->next;
        }
        if (prev == NULL) {
            mess->params = new;
        } else {
            prev->next = new;
        }
        new->next = next;
    }
    return (new);
}

/* Return the next parameter on the parameter list for the current message */
MESS_PARAM_LIST *next_param (list)

    MESS_PARAM_LIST *list;
{
    return (list->next);
}

/* Set parameter par's value to m */
void set_par_mess_value (par, m)

    MESS_PARAM_LIST *par;
    MESSAGE_STRUCT *m;
{
    free_param_space (par);
    par->param_data.val_type = value_message;
    par->param_data.val.mess = m;
}

/* Set parameter par's attribute qualifier to q */
void set_par_attr_qual (par, q)

    MESS_PARAM_LIST *par;
    ATTRIBUTE_QUALIFIER q;
{
    par->param_data.qual = q;
}

/* Set parameter par's value to i */
void set_par_int_value (par, i)

    MESS_PARAM_LIST *par;
    int i;
{
    free_param_space (par);
    par->param_data.val_type = value_int;
    par->param_data.val.val_integer = i;
}

/* Set parameter par's integer range limits. */
void set_par_int_range (par, lower, upper)

    MESS_PARAM_LIST *par;
    int lower;
    int upper;
{
    free_param_space (par);
    par->param_data.val_type = value_int_range;
    par->param_data.val.integer_range.lower = lower;
    par->param_data.val.integer_range.upper = upper;
}

/* Set parameter par's integer list values. */
void set_par_int_list (par, list, nvals)

    MESS_PARAM_LIST *par;
    INTLIST list;
    int nvals;
{
    int i;

    if (nvals >= MAX_LIST) {
        alsp_console_message
            ("Too many list values, increase MAX_LIST");
        exit (0);
    }
    free_param_space (par);
    par->param_data.val_type = value_int_list;
    for (i = 0; i < nvals; i++) {   /* nvals replaces MAX_LIST - 6/25/93 clk */
        par->param_data.val.integer_list.list [i] = list [i];
    }
    par->param_data.val.integer_list.nvals = nvals;
}

/* Carol Kropp - 10/94                                   */
/* set_par_float_list is modeled after set_par_int_list  */
/* implementation of the JECEWSI ICD required attributes */
/* which were a list of eight floating point values.     */
/* Set parameter par's float list values. */

void set_par_float_list (par, list, nvals)

    MESS_PARAM_LIST *par;
    FLOATLIST list;
    int nvals;
{
    int i;

    if (nvals >= MAX_LIST) {
        alsp_console_message
            ("Too many list values, increase MAX_LIST");
        exit (0);
    }
    free_param_space (par);
    par->param_data.val_type = value_float_list;
    for (i = 0; i < nvals; i++) {   /* nvals replaces MAX_LIST - 6/25/93 clk */
        par->param_data.val.float_list.list [i] = list [i];
    }
    par->param_data.val.float_list.nvals = nvals;
}

/* Set parameter par's value to a */
void set_par_attr_list (par, a)

    MESS_PARAM_LIST *par;
    ALSP_ATTRIBUTE_LIST a;
{
    int i;

    free_param_space (par);
    par->param_data.val_type = value_attribute_list;
/* initialize attr_list to falseval SMM 10/99 (E2161) */
    for (i = 0; i <= N_ATTRIBUTE_VALS; i++) {
        par->param_data.val.attr_list [i] = FALSEVAL;
    }

    for (i = 0; i <= N_ATTRIBUTE_VALS; i++) {
        if (i == tt_attr_type || 
            i == tt_attr_index ||
            i == tt_attr_orient ||
            i == tt_attr_radar ||
            i == tt_attr_jammer ||
            i == tt_attr_mode ||
            i == tt_attr_emitter_status ||
            i == tt_attr_iffmode_master ||        /* smp 2/98 (E1907) */
            i == tt_attr_iffmode1 ||              /* smp 2/98 (E1907) */
            i == tt_attr_iffmode2 ||              /* smp 2/98 (E1907) */
            i == tt_attr_iffmode3 ||              /* smp 2/98 (E1907) */
            i == tt_attr_iffmodeC ||              /* smp 2/98 (E1907) */
            i == tt_attr_iffmode4 ||              /* smp 2/98 (E1907) */
            i == tt_attr_iffmode_value ||         /* smp 2/98 (E1907) */
            i == tt_attr_iffmode_status ||         /* smp 2/98 (E1907) */
            i == tt_attr_sonar_status  ||
            i == tt_attr_sonar_mode  ||
            i == tt_attr_sonar_depth)


            continue;   /* KHC,3/93- Don't want, because EMITTER is the real */
                        /* keyword, not TYPE, INDEX, ORIENT, RADAR, JAMMER   */
                        /* MODE or EMITTER_STATUS */
        par->param_data.val.attr_list [i] = a [i];
        #ifdef DEBUG
            printf("param_data_val_attr_list[%d] = %d\n",i,par->param_data.val.attr_list[i]);
        #endif
    }                                              
}

/* Set parameter par's value to e */
void set_par_enum_value (par, e)

    MESS_PARAM_LIST *par;
    int e;
{
    free_param_space (par);
    par->param_data.val_type = value_enum;
    par->param_data.val.val_enum.enum_value = e;
}

/* Set parameter par's value to s, where s is a pointer into a
 * static table containing the string value for this enumerated value.
 * See ascii_tables.c & ascii_tables.h
 */
void set_par_enum_string (par, s)

    MESS_PARAM_LIST *par;
    char *s;
{
    free_param_space (par);
    par->param_data.val_type = value_enum;
    par->param_data.val.val_enum.enum_string = s;
}
/* Set parameter par's location list. */
void set_par_loc_list (par, list, nvals)

    MESS_PARAM_LIST *par;
    LOCLIST list;
    int nvals;
{
    int i;

    if (nvals >= MAX_LIST) {
        alsp_console_message
            ("Too many list values, increase MAX_LIST");
        exit (0);
    }
    free_param_space (par);
    par->param_data.val_type = value_loc_struct_list;
    for (i = 0; i < nvals; i++) {   /* nvals replaces MAX_LIST - 6/25/93 clk */
        par->param_data.val.location_list.list [i].lat
            = list [i].lat;
        par->param_data.val.location_list.list [i].lon
            = list [i].lon;
    }
    par->param_data.val.location_list.nvals = nvals;
}

/* Set parameter par's emitter list. */
void set_par_emitter_list(par, list, nvals)

    MESS_PARAM_LIST *par;
    EMITTERLIST list;
    int *nvals;
{
    int i, param_index;

    if (*nvals >= MAX_EMITTERS) {
        alsp_console_message
            ("Too many emitter values in set_par_emitter_list, increase MAX_EMITTERS");
        exit (0);
    }

    free_param_space (par);
    par->param_data.val_type = value_emit_struct_list;

    for (i = 0; i < *nvals; i++) {  

        for (param_index = 0; param_index < N_EMIT_PARAMS; param_index++) {
            par->param_data.val.list_of_emitters.list[i].emitter_params[param_index] =
                list[i].emitter_params[param_index];
        }

        /* always load the required information - category, index, status */
        par->param_data.val.list_of_emitters.list[i].emitter_cat =
            list [i].emitter_cat; 

        par->param_data.val.list_of_emitters.list[i].emitter_index =
            list[i].emitter_index;

        par->param_data.val.list_of_emitters.list[i].emitter_status.enum_value =
            list[i].emitter_status.enum_value;
        par->param_data.val.list_of_emitters.list[i].emitter_status.enum_string =
            list[i].emitter_status.enum_string;

        /* check to see if non-required parameter is there */
        if (list[i].emitter_params[1] == TRUEVAL) {
            par->param_data.val.list_of_emitters.list[i].emitter_type.enum_value =
                list[i].emitter_type.enum_value; 
            par->param_data.val.list_of_emitters.list[i].emitter_type.enum_string =
                list[i].emitter_type.enum_string; 
        }

        if (list[i].emitter_params[4] == TRUEVAL) {
            par->param_data.val.list_of_emitters.list[i].emitter_orientation =
                list[i].emitter_orientation; 
        }

        if (list[i].emitter_params[5] == TRUEVAL) {
            par->param_data.val.list_of_emitters.list[i].emitter_mode.enum_value =
                list[i].emitter_mode.enum_value;
            par->param_data.val.list_of_emitters.list[i].emitter_mode.enum_string =
                list[i].emitter_mode.enum_string;
        }
    }

    par->param_data.val.list_of_emitters.nvals = *nvals;
}

/* Set parameter par's sonar list. */
void set_par_sonar_list(par, list, nvals)

    MESS_PARAM_LIST *par;
    SONARLIST list;
    int *nvals;
{
    int i, param_index;

    if (*nvals >= MAX_SONARS) {
        alsp_console_message
            ("Too many sonar values in set_par_sonar_list, increase MAX_SONARS");
        exit (0);
    }

    free_param_space (par);
    par->param_data.val_type = value_sonar_struct_list;

    for (i = 0; i < *nvals; i++) {  

        for (param_index = 0; param_index < N_SONAR_PARAMS; param_index++) {
         par->param_data.val.list_of_sonars.list[i].sonar_params[param_index] =
          list[i].sonar_params[param_index];
        }

        par->param_data.val.list_of_sonars.list[i].sonar_type.enum_value =
            list[i].sonar_type.enum_value; 
        par->param_data.val.list_of_sonars.list[i].sonar_type.enum_string =
            list[i].sonar_type.enum_string; 

        par->param_data.val.list_of_sonars.list[i].sonar_status.enum_value =
            list[i].sonar_status.enum_value;
        par->param_data.val.list_of_sonars.list[i].sonar_status.enum_string =
            list[i].sonar_status.enum_string;

        if (list[i].sonar_params[2] == TRUEVAL) {
            par->param_data.val.list_of_sonars.list[i].sonar_mode.enum_value =
                list[i].sonar_mode.enum_value;
            par->param_data.val.list_of_sonars.list[i].sonar_mode.enum_string =
                list[i].sonar_mode.enum_string;
        }

        if (list[i].sonar_params[3] == TRUEVAL) {
            par->param_data.val.list_of_sonars.list[i].sonar_depth =
                list[i].sonar_depth; 
        }
    }

    par->param_data.val.list_of_sonars.nvals = *nvals;
}
/* Set parameter par's position list. */
void set_par_position_list(par, list, nvals)

    MESS_PARAM_LIST *par;
    POSITIONLIST list;
    int *nvals;
{
    int i, param_index;

    if (*nvals >= MAX_POSITIONS) {
        alsp_console_message
            ("Too many positions in set_par_position_list, increase MAX_POSITIONS");
        exit (0);
    }

    free_param_space (par);
    par->param_data.val_type = value_loc_struct_list;

    for (i = 0; i < *nvals; i++) {  

        par->param_data.val.list_of_positions.list[i].latitude =
            list[i].latitude; 
        par->param_data.val.list_of_positions.list[i].longitude =
            list[i].longitude; 

    }

    par->param_data.val.list_of_positions.nvals = *nvals;
}

/* Set parameter par's enumeration list. */
void set_par_enum_list (par, elist, nvals)

    MESS_PARAM_LIST *par;
    ENUMLIST elist;
    int nvals;
{
    int i;

    if (nvals >= MAX_LIST) {
        alsp_console_message
            ("Too many list values, increase MAX_LIST");
        exit (0);
    }
    free_param_space (par);
    par->param_data.val_type = value_enum_list;
    for (i = 0; i < nvals; i++) {   /* nvals replaces MAX_LIST - 6/25/93 clk */
        par->param_data.val.en_list.list [i].enum_value
            = elist [i].enum_value;
        par->param_data.val.en_list.list [i].enum_string
            = elist [i].enum_string;
    }
    par->param_data.val.en_list.nvals = nvals;
}

/* Set parameter par's location value to {lat, lon}. */
void set_par_loc_value (par, lat, lon)

    MESS_PARAM_LIST *par;
    float lat, lon;
{
    free_param_space (par);
    par->param_data.val_type = value_loc_struct;
    par->param_data.val.val_loc.lat = lat;
    par->param_data.val.val_loc.lon = lon;
}

/* Set parameter par's value to f */
void set_par_float_value (par, f)

    MESS_PARAM_LIST *par;
    float f;
{
    free_param_space (par);
    par->param_data.val_type = value_float;
    par->param_data.val.val_float = f;
}

/* Set parameter par's float range limits. */
void set_par_float_range (par, lower, upper)

    MESS_PARAM_LIST *par;
    float lower;
    float upper;
{
    free_param_space (par);
    par->param_data.val_type = value_float_range;
    par->param_data.val.float_pt_range.lower = lower;
    par->param_data.val.float_pt_range.upper = upper;
}

/* Set parameter par's value to s */
void set_par_string_value (par, s)

    MESS_PARAM_LIST *par;
    char *s;
{
    free_param_space (par);
    par->param_data.val_type = value_string;
    par->param_data.val.val_string = (char *) malloc (strlen (s) + 1);
    if (par->param_data.val.val_string == NULL) {
        alsp_console_message ("Malloc failure in set_string_param.");
        exit (0);
    } else {
        strncpy (par->param_data.val.val_string, s, strlen (s));
        *(par->param_data.val.val_string + strlen (s)) = '\0';
    }

}


/*  Return the message type of mess, acm-acm, trans-acm, trans-trans */
MESSAGE_DATA_TYPE get_message_type (mess)

    MESSAGE_STRUCT *mess;
{
    return (mess->header.alsp_mess_type.mess_type);
}

/*  Return the message subtype of mess (type_ta_join, type_at_join_ack, etc.) */
int get_message_subtype (mess)

    MESSAGE_STRUCT *mess;
{
    int result;

    switch (mess->header.alsp_mess_type.mess_type) {
        case actor_acm_mess:
            result = (int)
                mess->header.alsp_mess_type.mess_subtype.ta_type;
            break;
        case acm_actor_mess:
            result = (int)
                mess->header.alsp_mess_type.mess_subtype.at_type;
            break;
        case actor_actor_mess:
            result = (int)
                mess->header.alsp_mess_type.mess_subtype.tt_type;
            break;
        default:
            alsp_console_message ("Bad message type in get_message_subtype");
            exit (0);
    }
    return (result);
}

/* Return the parameter list associated with message mess.  */
MESS_PARAM_LIST *get_message_parameters (mess)
    
    MESSAGE_STRUCT *mess;
{
    return (mess->params);
}


/* Return the parameter type associated with par.  */
int get_parameter_type (par)

    MESS_PARAM_LIST *par;
{
    return (par->param_type);
}

/* Return the attribute qualifier value associated with parameter par.*/
ATTRIBUTE_QUALIFIER  get_par_attr_qual (par)

    MESS_PARAM_LIST *par;
{
    return (par->param_data.qual);
}

/* Return the integer value associated with parameter par.  */
int get_par_int_value (par)

    MESS_PARAM_LIST *par;
{
    if (par->param_data.val_type != value_int) {
        alsp_console_message 
            ("Bad parameter type in get_par_int_value.");
        exit (0);
    } 
    return (par->param_data.val.val_integer);
}

/* Return the integer range associated with parameter par.  */
void get_par_int_range (par, lower, upper)

    MESS_PARAM_LIST *par;
    int *lower;
    int *upper;
{
    if (par->param_data.val_type != value_int_range) {
        alsp_console_message 
            ("Bad parameter type in get_par_int_range.");
        exit (0);
    } 
    *lower = par->param_data.val.integer_range.lower;
    *upper = par->param_data.val.integer_range.upper;
}

/* Return the integer list value associated with parameter par.  */
void get_par_int_list (par, list, nvals)

    MESS_PARAM_LIST *par;
    INTLIST list;
    int *nvals;
{
    int i;

    if (par->param_data.val_type != value_int_list) {
        alsp_console_message
            ("Bad parameter type in get_par_int_list.");
        exit (0);
    } 
    *nvals = par->param_data.val.integer_list.nvals;  /* moved above the for loop */
    for (i = 0; i < *nvals; i++) {                    /* *nvals replaces MAX_LIST - clk 6/25/93 */
        list [i] = par->param_data.val.integer_list.list [i];
    }
}

/* Carol Kropp - 10/94                                   */
/* get_par_float_list is modeled after get_par_int_list  */
/* implementation of the JECEWSI ICD required attributes */
/* which were a list of eight floating point values.     */
/* Return the float list value associated with par.      */
void get_par_float_list (par, list, nvals)

    MESS_PARAM_LIST *par;
    FLOATLIST list;
    int *nvals;
{
    int i;

    if (par->param_data.val_type != value_float_list) {
        alsp_console_message
            ("Bad parameter type in get_par_float_list.");
        exit (0);
    } 
    *nvals = par->param_data.val.float_list.nvals;
    for (i = 0; i < *nvals; i++) {
        list [i] = par->param_data.val.float_list.list [i];
    }
}

/* Return the location list value associated with parameter par.  */
void get_par_loc_list (par, list, nvals)

    MESS_PARAM_LIST *par;
    LOCLIST list;
    int *nvals;
{
    int i;

    if (par->param_data.val_type != value_loc_struct_list) {
        alsp_console_message
            ("Bad parameter type in get_par_loc_list.");
        exit (0);
    } 
    *nvals = par->param_data.val.location_list.nvals;   /* moved above the for loop */
    for (i = 0; i < *nvals; i++) {                      /* *nvals replaces MAX_LIST - clk 6/25/93 */
        list [i].lat = 
            par->param_data.val.location_list.list [i].lat;
        list [i].lon = 
            par->param_data.val.location_list.list [i].lon;
    }
}

/* Return the emitter list value associated with parameter par. */
void get_par_emitter_list(par, list, nvals)

    MESS_PARAM_LIST *par;
    EMITTERLIST list;
    int *nvals;
{
    int i, param_index;

    if (par->param_data.val_type != value_emit_struct_list) {
        alsp_console_message
            ("Bad parameter type in get_par_emitter_list.");
        exit (0);
    } 
    *nvals = par->param_data.val.list_of_emitters.nvals;  
    for (i = 0; i < *nvals; i++) {                      

        for (param_index = 0; param_index < N_EMIT_PARAMS; param_index++) {
            list[i].emitter_params[param_index] = 
            par->param_data.val.list_of_emitters.list[i].emitter_params[param_index];
        }

        /* always load the required information - category, index, status */
        list [i].emitter_cat = 
            par->param_data.val.list_of_emitters.list[i].emitter_cat;

        list[i].emitter_index =
            par->param_data.val.list_of_emitters.list[i].emitter_index;

        list[i].emitter_status.enum_value =
            par->param_data.val.list_of_emitters.list[i].emitter_status.enum_value;
        list[i].emitter_status.enum_string = 
            par->param_data.val.list_of_emitters.list[i].emitter_status.enum_string;


        if (par->param_data.val.list_of_emitters.list[i].emitter_params[1] == TRUEVAL) {
            list[i].emitter_type.enum_value = 
                par->param_data.val.list_of_emitters.list[i].emitter_type.enum_value;
            list[i].emitter_type.enum_string = 
                par->param_data.val.list_of_emitters.list[i].emitter_type.enum_string;
        }

        if (par->param_data.val.list_of_emitters.list[i].emitter_params[4] == TRUEVAL) {
            list[i].emitter_orientation = 
                par->param_data.val.list_of_emitters.list[i].emitter_orientation;
        }

        if (par->param_data.val.list_of_emitters.list[i].emitter_params[5] == TRUEVAL) {
            list[i].emitter_mode.enum_value =
                par->param_data.val.list_of_emitters.list[i].emitter_mode.enum_value;
            list[i].emitter_mode.enum_string = 
                par->param_data.val.list_of_emitters.list[i].emitter_mode.enum_string;
        }
    }
}

/* Return the sonar list value associated with parameter par. */
void get_par_sonar_list(par, list, nvals)

    MESS_PARAM_LIST *par;
    SONARLIST list;
    int *nvals;
{
    int i, param_index;

    if (par->param_data.val_type != value_sonar_struct_list) {
        alsp_console_message
            ("Bad parameter type in get_par_sonar_list.");
        exit (0);
    } 
    *nvals = par->param_data.val.list_of_sonars.nvals;  
    for (i = 0; i < *nvals; i++) {                      

        for (param_index = 0; param_index < N_SONAR_PARAMS; param_index++) {
            list[i].sonar_params[param_index] = 
            par->param_data.val.list_of_sonars.list[i].sonar_params[param_index];
        }

        /* always load the required information - type, status */
        list[i].sonar_type.enum_value = 
            par->param_data.val.list_of_sonars.list[i].sonar_type.enum_value;
        list[i].sonar_type.enum_string = 
            par->param_data.val.list_of_sonars.list[i].sonar_type.enum_string;

        list[i].sonar_status.enum_value =
            par->param_data.val.list_of_sonars.list[i].sonar_status.enum_value;
        list[i].sonar_status.enum_string = 
            par->param_data.val.list_of_sonars.list[i].sonar_status.enum_string;


        if (par->param_data.val.list_of_sonars.list[i].sonar_params[2] == TRUEVAL) {
            list[i].sonar_mode.enum_value =
                par->param_data.val.list_of_sonars.list[i].sonar_mode.enum_value;
            list[i].sonar_mode.enum_string = 
                par->param_data.val.list_of_sonars.list[i].sonar_mode.enum_string;
        }

        if (par->param_data.val.list_of_sonars.list[i].sonar_params[3] == TRUEVAL) {
            list[i].sonar_depth = 
                par->param_data.val.list_of_sonars.list[i].sonar_depth;
        }
        else {
            list[i].sonar_depth = -1;
        }

    }
}

/* Return the enum list value associated with parameter par.  */
void get_par_enum_list (par, list, nvals)

    MESS_PARAM_LIST *par;
    ENUMLIST list;
    int *nvals;
{
    int i;

    if (par->param_data.val_type != value_enum_list) {
        alsp_console_message
            ("Bad parameter type in get_par_enum_list.");
        exit (0);
    } 
    *nvals = par->param_data.val.en_list.nvals; /* moved above the for loop */
    for (i = 0; i < *nvals; i++) {              /* *nvals replaces MAX_LIST - clk 6/25/93 */
        list [i].enum_value = 
            par->param_data.val.en_list.list [i].enum_value;
        list [i].enum_string = 
            par->param_data.val.en_list.list [i].enum_string;
    }
}

/* Return the attribute list value associated with parameter par.  */
void get_par_attr_list (par, a)

    MESS_PARAM_LIST *par;
    ALSP_ATTRIBUTE_LIST a;
{
    int i;

    if (par->param_data.val_type != value_attribute_list) {
        alsp_console_message
            ("Bad parameter type in get_par_attr_list.");
        exit (0);
    } 
    for (i = 0; i <= N_ATTRIBUTE_VALS; i++) {
        a [i] = par->param_data.val.attr_list [i];
        #ifdef DEBUG
            printf("a[%d] = %d\n",i,a[i]);
        #endif
    }                                               
}

/* Return the integer enumeration value associated with parameter par.  */
int get_par_enum_value (par)

    MESS_PARAM_LIST *par;
{
    if (par->param_data.val_type != value_enum) {
        alsp_console_message ("Bad parameter type in get_par_enum_value.");
        exit (0);
    } 
    return (par->param_data.val.val_enum.enum_value);
}

/* Return the string enumeration value associated with parameter par.  */
char *get_par_enum_string (par)

    MESS_PARAM_LIST *par;
{
    if (par->param_data.val_type != value_enum) {
        alsp_console_message
            ("Bad parameter type in get_par_enum_string.");
        exit (0);
    } 
    return (par->param_data.val.val_enum.enum_string);
}

/* Return the location value associated with parameter par.  */
void get_par_loc_value (par, lat, lon)

    MESS_PARAM_LIST *par;
    float *lat, *lon;
{
    if (par->param_data.val_type != value_loc_struct) {
        alsp_console_message 
            ("Bad parameter type in get_par_loc_value.");
        exit (0);
    } 
    *lat = par->param_data.val.val_loc.lat;
    *lon = par->param_data.val.val_loc.lon;
}

/* Return the float value associated with parameter par.  */
float get_par_float_value (par)

    MESS_PARAM_LIST *par;
{
    if (par->param_data.val_type != value_float) {
        alsp_console_message 
            ("Bad parameter type in get_par_float_value.");
        exit (0);
    } 
    return (par->param_data.val.val_float);
}

/* Return the float range associated with parameter par.  */
void get_par_float_range (par, lower, upper)

    MESS_PARAM_LIST *par;
    float *lower;
    float *upper;
{
    if (par->param_data.val_type != value_float_range) {
        alsp_console_message 
            ("Bad parameter type in get_par_float_range.");
        exit (0);
    } 
    *lower = par->param_data.val.float_pt_range.lower;
    *upper = par->param_data.val.float_pt_range.upper;
}

/* Return the string value associated with parameter par.  */
char *get_par_string_value (par)

    MESS_PARAM_LIST *par;
{
    if (par->param_data.val_type != value_string) {
        alsp_console_message ("Bad parameter type in get_par_string_value.");
        exit (0);
    } 
    return (par->param_data.val.val_string);
}

/* Return the message value associated with parameter par. */
MESSAGE_STRUCT *get_par_mess_value (par) 
    
    MESS_PARAM_LIST *par;
{
    if (par->param_data.val_type != value_message) {
        alsp_console_message ("Bad parameter type in get_par_mess_value.");
        exit (0);
    } 
    return (par->param_data.val.mess);
}

/* Return a pointer to the parameter of ptype found on the parameter list */
MESS_PARAM_LIST *lookup (ptype, mess)

    PARAM_TYPE ptype;
    MESSAGE_STRUCT *mess;
{
    MESS_PARAM_LIST *result;
    
    result = mess->params;
    while (result != NULL) {
        if (result->param_type == ptype) {
            return (result);
        }
        result = result->next;
    }
    return (result);
}

#ifdef DEBUG

void dump_mess_struct (mess) 

    MESSAGE_STRUCT *mess;
{

#define START_REC_LINE "***** MESSAGE STRUCT *****"
#define END_REC_LINE   "**************************"
#define HEADER_LINE    "  header: "
#define PARAM_LINE    "  params: "

    char buff [MAX_MESSAGE_LEN]; 
    MESSAGE_DATA_TYPE mtype;
    int msubtype;
    char *msub_string, *par_type_string;
    MESS_PARAM_LIST *par;

    debug_print (START_REC_LINE);
    debug_print (HEADER_LINE);

    mtype = mess->header.alsp_mess_type.mess_type;
    switch (mtype) {
        case actor_actor_mess:
            msubtype = (int) 
                mess->header.alsp_mess_type.mess_subtype.tt_type;
            msub_string = get_keyword (tt_mess_keytab, msubtype);
            break;
        case actor_acm_mess:
            msubtype = (int)
                mess->header.alsp_mess_type.mess_subtype.ta_type;
            msub_string = get_keyword (ta_mess_keytab, msubtype);
            break;
        case acm_actor_mess:
            msubtype = (int) 
                mess->header.alsp_mess_type.mess_subtype.at_type;
            msub_string = get_keyword (at_mess_keytab, msubtype);
            break;
        default:
            msub_string = NULL;
            break;
    }

    sprintf (buff, "MESSAGE TYPE: %s %s",
        get_keyword (mess_type_keytab, mtype), msub_string);
    debug_print (buff);

    debug_print (PARAM_LINE);
    par = mess->params;
    while (par != NULL) {
        par_type_string = get_param_keyword 
            (mtype, msubtype, par->param_type);
        sprintf (buff, "PARAM TYPE:  %s", par_type_string);
        debug_print (buff);

        sprintf (buff, "QUALIFIER:  %s",
            get_keyword (attr_qual_keytab, par->param_data.qual));
        debug_print (buff);

        switch (par->param_data.val_type) {
            case value_int:
                sprintf (buff, "PARAM VALUE:  %d",
                    par->param_data.val.val_integer);
                debug_print (buff);
                break;
            case value_int_range:
                sprintf (buff, "PARAM VALUE:  lower: %d  upper: %d",
                    par->param_data.val.integer_range.lower,
                    par->param_data.val.integer_range.upper);
                debug_print (buff);
                break;
            case value_int_list:
                {
                    int i;

                    sprintf (buff, "integer list");
                    debug_print (buff);
                    for (i = 0; i <
                        par->param_data.val.integer_list.nvals; i++) {
                        sprintf (buff, "%d",
                            par->param_data.val.integer_list.list [i]);
                        debug_print (buff);
                    }
                }
                break;
            case value_enum:
                sprintf (buff, "PARAM VALUE:  %d %s",
                    par->param_data.val.val_enum.enum_value,
                    par->param_data.val.val_enum.enum_string);
                debug_print (buff);
                break;
            case value_loc_struct_list:
                {
                    int i;

                    sprintf (buff, "location list");
                    debug_print (buff);
                    for (i = 0; i <
                        par->param_data.val.location_list.nvals; i++) {
                        sprintf (buff, "lat %f  lon %f",
                            par->param_data.val.
                            location_list.list [i].lat,
                            par->param_data.val.
                            location_list.list [i].lon);
                        debug_print (buff);
                    }
                }
                break;
            case value_enum_list:
                {
                    int i;

                    sprintf (buff, "enum list");
                    debug_print (buff);
                    for (i = 0; i <
                        par->param_data.val.en_list.nvals; i++) {
                        sprintf (buff, "%d %s", par->param_data.val.
                            en_list.list [i].enum_value,
                            par->param_data.val.
                            en_list.list [i].enum_string);
                        debug_print (buff);
                    }
                }
                break;
            case value_float:
                sprintf (buff, "PARAM VALUE:  %f",
                    par->param_data.val.val_float);
                debug_print (buff);
                break;
            case value_loc_struct:
                sprintf (buff, "PARAM VALUE:  lat: %f   long: %f",
                    par->param_data.val.val_loc.lat,
                    par->param_data.val.val_loc.lon);
                debug_print (buff);
                break;
            case value_float_range:
                sprintf (buff, "PARAM VALUE:  lower: %f  upper: %f",
                    par->param_data.val.float_pt_range.lower,
                    par->param_data.val.float_pt_range.upper);
                debug_print (buff);
                break;
            case value_string:
                sprintf (buff, "PARAM VALUE  %s",
                    par->param_data.val.val_string);
                debug_print (buff);
                break;
            case value_message:
                sprintf (buff, "message");
                debug_print (buff);
                dump_mess_struct (par->param_data.val.mess);
                break;
            case value_attribute_list:
                {
                    int i;

                    sprintf (buff, "attribute list");
                    debug_print (buff);
                    for (i = 0; i <= N_ATTRIBUTE_VALS; i++) { /* KHC,3/93- Added EMITTER */
                        if (par->param_data.val.attr_list [i]
                            == TRUEVAL) {
                            /* khc,3/93- We like to skip enumerator emitter name and status of TT_ATTRIBUTES
                             * table in file MESS_STRUCT.H, but we are using emitter type to be EMITTER which 
                             * should be parameter key-string of an UPDATE message. 
                             */
                            if (i == tt_attr_type || 
                                i == tt_attr_index ||
                                i == tt_attr_orient ||
                                i == tt_attr_radar ||
                                i == tt_attr_jammer ||
                                i == tt_attr_mode ||
                                i == tt_attr_emitter_status ||
                                i == tt_attr_iffmode_master ||       /* smp 2/98 (E1907) */
                                i == tt_attr_iffmode1   ||           /* smp 2/98 (E1907) */
                                i == tt_attr_iffmode2   ||           /* smp 2/98 (E1907) */
                                i == tt_attr_iffmode3   ||           /* smp 2/98 (E1907) */
                                i == tt_attr_iffmodeC   ||           /* smp 2/98 (E1907) */
                                i == tt_attr_iffmode4   ||           /* smp 2/98 (E1907) */
                                i == tt_attr_iffmode_status ||       /* smp 2/98 (E1907) */
                                i == tt_attr_iffmode_value  )        /* smp 2/98 (E1907) */
                                continue;
                            else
                                sprintf (buff, get_keyword (
                                            tt_attribute_keytab, i));
                            debug_print (buff);
                        }
                    }
                }
                break;
        }
        par = par->next;
    }

    debug_print (END_REC_LINE);
}
#endif

void set_par_iffmode_list(par, iffmode_array)
/* Set parameter par's iffmode list - smp 2/98 (E1907) */

    MESS_PARAM_LIST *par;
    IFFMODESTR iffmode_array[N_IFF_PARAMS]; 
{
    int i, param_index;

    free_param_space (par);
    par->param_data.val_type = value_iff_struct_list;

    /* move all the data from listIFF into par */
    for (i = 0; i < N_IFF_PARAMS; i++) {                      

       if (iffmode_array[i].iffmode_params == TRUEVAL){

            par->param_data.val.list_of_iffmodes[i].iffmode_params = TRUEVAL;

            par->param_data.val.list_of_iffmodes[i].iffmode_status.enum_value =
                                   iffmode_array[i].iffmode_status.enum_value;

            par->param_data.val.list_of_iffmodes[i].iffmode_status.enum_string =
                                   iffmode_array[i].iffmode_status.enum_string;

            par->param_data.val.list_of_iffmodes[i].iffmode_value =
                                   iffmode_array[i].iffmode_value; 
           } /* end if iffmode_params == TRUEVAL */
       else {
            par->param_data.val.list_of_iffmodes[i].iffmode_params = FALSEVAL;
            par->param_data.val.list_of_iffmodes[i].iffmode_status.enum_value = 0;
            par->param_data.val.list_of_iffmodes[i].iffmode_status.enum_string = NULL;
            par->param_data.val.list_of_iffmodes[i].iffmode_value = 0;
       }
      } /*   end for i == .... */
} /*  end set_par_iffmode_list   */

void get_par_iffmode_list(par, iffmode_array)
/* Return the iffmode list value associated with parameter par. */
/* smp 2/98 (E1907) */

    MESS_PARAM_LIST *par;
    IFFMODESTR iffmode_array[N_IFF_PARAMS]; 
{
    int i, param_index;

    if (par->param_data.val_type != value_iff_struct_list) {
        alsp_console_message
            ("Bad parameter type in get_par_iff_list.");
        exit (0);
    } 

    for (param_index = 0; param_index < N_IFF_PARAMS; param_index++) {
        iffmode_array[param_index].iffmode_params = 
            par->param_data.val.list_of_iffmodes[param_index].iffmode_params;
    }  

    for (i = 0; i < N_IFF_PARAMS; i++) {                      

        /*  MASTER, MODE1, MODE2, MODE3, MODEC, MODE4  */
        if (par->param_data.val.list_of_iffmodes[i].iffmode_params == TRUEVAL) {

            iffmode_array[i].iffmode_status.enum_value =
                par->param_data.val.list_of_iffmodes[i].iffmode_status.enum_value;
            iffmode_array[i].iffmode_status.enum_string =
                par->param_data.val.list_of_iffmodes[i].iffmode_status.enum_string;

            /* MASTER and MODE4 have no values */
            if ((i != 5) || (i != 0)) {
                iffmode_array[i].iffmode_value =
                        par->param_data.val.list_of_iffmodes[i].iffmode_value;
            } /* end if not MODE4 or MASTER */
        } /* end param == TRUEVAL */
        else {
            iffmode_array[i].iffmode_status.enum_value = 0;
            iffmode_array[i].iffmode_status.enum_string = NULL;
            iffmode_array[i].iffmode_value = 0;
        }
    } /*   end for i = 0.... */
} /*   end get par_iffmode_list   */
