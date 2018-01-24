/*  File:  AT_MESS_HANDLER.C
    Author:  Kristi Carlson, Los Alamos National Labs
*/ 

/* 26-AUG-1992 modified fill_params, fill_inter_params, handle_at_update,
   handle_at_create, and handle_at_interaction to use dsc$descriptor_a
   instead of dsc$descriptor_s to fix a problem passing an array of character
   strings from C to RATFOR.  Wayne Anderson and Dave Perme.              */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_types.h"
#include "mess_struct.h"
#include "mess_format.h"
#include "tt_mess_handler.h"
#include "alsp_parse.h"
#include "transif.h"

#define MAX_STR_LEN 100
#define MAX_ARRAY_SIZE N_ATTRIBUTE_VALS

static void shift_attr_left(a_param, attr_array)
    ALSP_ATTRIBUTE_LIST a_param;
    ALSP_ATTRIBUTE_LIST attr_array;
{

    int i;
    a_param[0] = 0;
    for (i=1; i <= N_ATTRIBUTE_VALS; i++)
    a_param[i] = attr_array[i-1];
}

void fill_inter_params(mess, n_params, params, int_params, f_params, strlens,
            string_params, num_list, tlist) 
    MESSAGE_STRUCT *mess;
    int *n_params;
    int params[], int_params[];
    float f_params[];
    int strlens[];
    char string_params[N_INTERACTION_PAR][ARRAY_STR_LENS];
    int *num_list;
    int tlist[];  /* enumeration list */
{

    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this message */
    int max_params, i;
    ENUMLIST tmp_list;
    char temp_string[ARRAY_STR_LENS];

    int msubtype;

    msubtype = get_message_subtype(mess);  /* should be interaction */
    *n_params = N_INTERACTION_PAR;

    for (i=0; i < *n_params; i++) 
    {
    strcpy(string_params[i], ""); 
    params[i] = 0;
    strlens[i] = 0;
    int_params[i] = 0;
    f_params[i] = 0.0;
    }

    list = get_message_parameters (mess);
    while (list != NULL) {
    ptype = get_parameter_type (list);
    params[ptype-1] = ptype;
    switch (get_alsp_param_data_type(acm_actor_mess, msubtype, ptype)) {
        case value_int:
            int_params[ptype-1] = get_par_int_value(list);
        break;
        case value_float:
            f_params[ptype-1] = get_par_float_value(list);
        break;
        case value_string:
            strcpy(string_params[ptype-1], get_par_string_value(list));
            strlens[ptype-1] = strlen(string_params[ptype-1]);
        break;
        case value_enum:
            int_params[ptype-1] = get_par_enum_value(list);
        break;
        case value_enum_list:
            get_par_enum_list(list, tmp_list, num_list); 
            for  (i=0; i < *num_list; i++)
            tlist[i] = tmp_list[i].enum_value;
           break;
        default:
#ifdef DEBUG
        printf("fill inter params:  unknown parameter type\n");
#endif
        break;
        }
    list = next_param (list);
    }
}

/* Carol Kropp - 10/94                                                      */
/* Added the unit_ew_xxxx arrays for holding data received in create/update */
/* messages for implementation of the JECEWSI ICD.                          */
/*                                                                          */
/* Modified:  Carol Kropp - 1/97 (E1693)                                    */
/*            When processing attributes with string values, the strcpy was */
/*            changed to a strncpy with max number of characters equal to   */
/*            ARRAY_STR_LENS-1 (last space for end of string terminator).   */
/*            During the 1997 AAI unit NAMEs were sent longer than this     */
/*            value, causing a crash.                                       */
/*                                                                          */
/*            Stephanie Paden - 2/98 (E1907)                                */
/*            Added IFF related arguemnts to the function interface.  JTC   */
/*            1998 implemented the IFF concept to share IFF mode info.      */

void fill_params (mess, n_params, params, int_params, f_params, strlens,
                  string_params, qualifiers, num_of_emitters, emitcategory_list,
                  emittype_list, emitstatus_list, emitindex_list, 
                  emitorient_list, emitmode_list,
                  num_of_sonars, sonartype_list, sonarstatus_list,
                  sonarmode_list,sonardepth_list,
                  num_of_positions, positionlat_list, positionlon_list,
                  unit_ew_acq, unit_ew_launch, unit_ew_guide, 
                  unit_ew_power, unit_ew_surf,
                  iffmode_params_list, iffmode_status_list, iffmode_value_list)

    MESSAGE_STRUCT *mess;
    int *n_params;
    int params[], int_params[];
    float f_params[];
    int strlens[];
    char string_params[N_ATTRIBUTE_VALS][ARRAY_STR_LENS];
    int qualifiers[];
    int *num_of_emitters, emitcategory_list[], emittype_list[];
    int emitstatus_list[], emitindex_list[], emitmode_list[];
    float emitorient_list[];
    int *num_of_sonars, sonartype_list[];
    int sonarstatus_list[], sonarmode_list[], sonardepth_list[];
    int *num_of_positions;
    float positionlat_list[], positionlon_list[];
    float unit_ew_acq[];
    float unit_ew_launch[];
    float unit_ew_guide[];
    float unit_ew_power[];
    float unit_ew_surf[];
    int iffmode_status_list[];
    int iffmode_value_list[];
    int iffmode_params_list[];
{

    MESS_PARAM_LIST *list, *class_p; 
    int ptype;  /* an enumeration of parameters for this message */
    int max_params, i, num_list;
    int *tmp_list;
    char temp_string[ARRAY_STR_LENS];
    ATTRIBUTE_QUALIFIER qual;
    int msubtype;
    int class,nvals;
    LOCLIST orbit_pts;
    FLOATLIST tmp_float_list;
    ENUMLIST tmp_enum_list; /* khc,3/93- Added for case of val_enum_list value type */
    int      tmp_num;
    EMITTERLIST tmp_emitter_list;  /* SMM 10/94 */
    SONARLIST tmp_sonar_list;
    POSITIONLIST tmp_position_list;
    IFFMODESTR tmp_iffmode_array[N_IFF_PARAMS];   /* smp 2/98 (E1907) */

    /* have to pull class off because of the special case of int. */
    /* sizes on air update/create */
    class_p = lookup(tt_attr_class, mess);
    class = get_par_enum_value(class_p);

    msubtype = get_message_subtype(mess);   
    switch (msubtype) {
       case type_at_create:
         *n_params = N_ATTRIBUTE_VALS;
         break;
       case type_at_update:
         *n_params = N_ATTRIBUTE_VALS;
         break;
       case type_at_refresh:
         *n_params = N_ATTRIBUTE_VALS;
         break;
       default:
#ifdef DEBUG
         printf("fill_params: received unexpected message subtype\n");
#endif
       return;
    }   /* end of switch (msubtype) */
   
    for (i=0; i < *n_params; i++) {
       strcpy(string_params[i], "");
       params[i] = 0;
       strlens[i] = 0;
       int_params[i] = 0;
       f_params[i] = 0.0;
       qualifiers[i] = 0;
    }  /* end of loop to initialize array values */

    /* initialize emitter values to zero or null */
    for (i = 0; i < MAX_EMITTERS; i++) {
        emitcategory_list[i] = 0;
        emittype_list[i] = 0;
        emitindex_list[i] = 0;
        emitstatus_list[i] = 0;
        emitorient_list[i] = 0.0;
        emitmode_list[i] = 0;
    }

    /* initialize sonar values */
    for (i = 0; i < MAX_SONARS; i++) {
        sonartype_list[i] = 0;
        sonarstatus_list[i] = 0;
        sonarmode_list[i] = 0;
        sonardepth_list[i] = -1;
    }

    /* initialize position values */
    for (i = 0; i < MAX_POSITIONS; i++) {
        positionlat_list[i] = 0.0;
        positionlon_list[i] = 0.0;
    }

    for (i = 0;  i < N_EW_SECTOR_VALS; i++) {
        /* Initialize the ew sector data fields */
        unit_ew_acq[i] = 1.0;
        unit_ew_launch[i] = 1.0;
        unit_ew_guide[i] = 1.0;
        unit_ew_power[i] = 1.0;
        unit_ew_surf[i] = 1.0;
    }

    for (i = 0; i < N_IFF_PARAMS; i++) {    /* initial the IFF data to */
        iffmode_status_list[i] = 0;         /* zero/null - smp 2/98 (E1907) */
        iffmode_value_list[i] = 0;
        iffmode_params_list[i] = 0;
    } 

    list = get_message_parameters (mess);
    while (list != NULL) {
       ptype = get_parameter_type (list);
       params[ptype-1] = ptype;
       switch (get_alsp_param_data_type(acm_actor_mess, msubtype, ptype)) {
          case value_int:
             /* special case for qualifier on id */
             if (ptype == tt_attr_id) {
                qual = get_par_attr_qual(list);
             if (qual > 1)
                qualifiers[i] = qual;
             }

             int_params[ptype-1] = get_par_int_value(list);
             break;
          case value_float:
             f_params[ptype-1] = get_par_float_value(list);
             break;
          case value_float_list:
             get_par_float_list (list, tmp_float_list, &tmp_num);
             if (ptype == tt_attr_ew_acq) {
                for (i=0; i < tmp_num; i++)
                    unit_ew_acq[i] = tmp_float_list[i];
             }
             else if (ptype == tt_attr_ew_launch) {
                for (i=0; i < tmp_num; i++)
                    unit_ew_launch[i] = tmp_float_list[i];
             }
             else if (ptype == tt_attr_ew_guide) {
                for (i=0; i < tmp_num; i++)
                    unit_ew_guide[i] = tmp_float_list[i];
             }
             else if (ptype == tt_attr_ew_power) {
                for (i=0; i < tmp_num; i++)
                    unit_ew_power[i] = tmp_float_list[i];
             }
             else if (ptype == tt_attr_ew_surf) {
                for (i=0; i < tmp_num; i++)
                    unit_ew_surf[i] = tmp_float_list[i];
             }
             break;
          case value_string:
             strncpy (string_params[ptype-1],
                      get_par_string_value(list), ARRAY_STR_LENS-1);
             *(string_params[ptype-1] + (ARRAY_STR_LENS-1)) = '\0';
             strlens[ptype-1] = strlen(string_params[ptype-1]); 
             break;
          case value_enum:
             /* special case for size attribute */
             if ((ptype == tt_attr_unit_size) && 
                 ((class >= pval_tt_class_air) && 
                  ((class <= pval_tt_class_air_cruise_missile) ||
                   (class <= pval_tt_class_air_tbm)) ))
                int_params[ptype-1] = get_par_int_value(list);
             else
                int_params[ptype-1] = get_par_enum_value(list);
             break;
          case value_emit_struct_list: 
             get_par_emitter_list (list, tmp_emitter_list, num_of_emitters);
             for (i=0; i < *num_of_emitters; i++) {
                emitcategory_list[i] = tmp_emitter_list[i].emitter_cat;
                emittype_list[i] = tmp_emitter_list[i].emitter_type.enum_value;
                emitindex_list[i] = tmp_emitter_list[i].emitter_index;
                emitstatus_list[i] = tmp_emitter_list[i].emitter_status.enum_value;
                if (emitcategory_list[i] == tt_attr_jammer) {
                    emitorient_list[i] = tmp_emitter_list[i].emitter_orientation; 
                    emitmode_list[i] = tmp_emitter_list[i].emitter_mode.enum_value;
                }
             }             
             break;
          case value_sonar_struct_list: 
             get_par_sonar_list (list, tmp_sonar_list, num_of_sonars);
             for (i=0; i < *num_of_sonars; i++) {
                sonartype_list[i] = tmp_sonar_list[i].sonar_type.enum_value;
                sonarstatus_list[i] = tmp_sonar_list[i].sonar_status.enum_value;
                sonarmode_list[i] = tmp_sonar_list[i].sonar_mode.enum_value;
                sonardepth_list[i] = tmp_sonar_list[i].sonar_depth; 
             }             
             break;

          case value_iff_struct_list:        /* smp 2/98 (E1907) */
             get_par_iffmode_list (list, tmp_iffmode_array);
             for (i = 0; i < N_IFF_PARAMS; i ++) {
                 if (tmp_iffmode_array[i].iffmode_params == TRUEVAL)
                     iffmode_params_list[i] = 1;
                 iffmode_status_list[i] = tmp_iffmode_array[i].iffmode_status.enum_value;
                 iffmode_value_list[i] = tmp_iffmode_array[i].iffmode_value;
             }   /*  end for i = 0 to N_IFF_PARAMS  */
 
             break;
          case value_loc_struct:
             if (ptype == tt_attr_target_lat)   /* get target structure */
                get_par_loc_value(list, &(f_params[tt_attr_target_lat-1]),
                                  &(f_params[tt_attr_target_long-1]));
             else if (ptype == tt_attr_target_long) {
                /* skip */
             }
             else
                printf("fill params: unknown loc struct parameter type\n");
             break;
          case value_loc_struct_list:
             if (ptype == tt_attr_orb_lat1) {
                /* get the whole orbit list */
                get_par_loc_list(list, orbit_pts, &nvals);
                f_params[tt_attr_orb_lat1-1] = orbit_pts[0].lat;
                f_params[tt_attr_orb_lon1-1] = orbit_pts[0].lon;
                f_params[tt_attr_orb_lat2-1] = orbit_pts[1].lat;
                f_params[tt_attr_orb_lon2-1] = orbit_pts[1].lon;
             }
             else if (ptype == tt_attr_position) {         /* minefield */
                 get_par_loc_list(list, tmp_position_list, num_of_positions);
                 for (i=0; i < *num_of_positions; i++) {
                    positionlat_list[i] = tmp_position_list[i].latitude;
                    positionlon_list[i] = tmp_position_list[i].longitude;
                 }             
             }
             else if (ptype == tt_attr_sweeparea_location) {   /* sweeparea */
                 get_par_loc_list(list, tmp_position_list, num_of_positions);
                 for (i=0; i < *num_of_positions; i++) {
                    positionlat_list[i] = tmp_position_list[i].latitude;
                    positionlon_list[i] = tmp_position_list[i].longitude;
                 }             
             }
             else if ((ptype == tt_attr_orb_lon1) ||
                      (ptype == tt_attr_orb_lat2) ||
                      (ptype == tt_attr_orb_lon2)) {
                /* skip */
             }
             else {
                printf("fill params:  unknown parameter type\n");
             }
             break;
          case value_enum_list: /* khc,3/93- Added value_enum_list param. type */
             get_par_enum_list(list, tmp_enum_list, &tmp_num);
             break;
          default:
             break;
       }  /* end of switch (get_alsp_param_data_type(...)) */

       list = next_param (list);
    }    /*  end of while (list != NULL) */
}

void fill_attr_array(num_attr, dest, src)

    int *num_attr; 
    ALSP_ATTRIBUTE_LIST dest, src;

{
    int i, dest_index;
    
    *num_attr = 0;
    dest_index = 0;
    for (i=0; i <= N_ATTRIBUTE_VALS; i++) 
    if (src[i] == TRUEVAL) {
        dest[dest_index++] = i;
        (*num_attr)++;
    }

}

void handle_at_available (mess)
              
    MESSAGE_STRUCT *mess;
{

    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this message */
    int id;
    ALSP_ATTRIBUTE_LIST attrs, attr_send;
    int num_attr;

    list = get_message_parameters (mess);
             
    while (list != NULL) {
    ptype = get_parameter_type (list);
    switch(ptype) {
        case (int)par_at_avail_id:
            id = get_par_int_value (list);
        break;
        case (int)par_at_avail_attrs:
#ifdef NOT_POSITIONAL
        get_par_attr_list(list, attrs);
        fill_attr_array(&num_attr, attr_send, attrs);
#else
        get_par_attr_list(list, attrs);
        shift_attr_left(attr_send, attrs);
        num_attr = N_ATTRIBUTE_VALS;
#endif
        break;
        default:
#ifdef DEBUG
        printf(stderr, 
               "received illegal parameter for available message\n");
#endif
          break;
        }
    list = next_param (list);
    }


    alsp_receive_available(&id, &num_attr, attr_send);
#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_create (mess)
/* Modified:  Stephanie Paden - 2/98 (E1907)                */
/*            Added IFF data to the attributes handled by a */
/*            create message.                               */
    MESSAGE_STRUCT *mess;
{

    int num_p = 0;
    int
    params[N_ATTRIBUTE_VALS], int_params[N_ATTRIBUTE_VALS],
    strlens[N_ATTRIBUTE_VALS], qualifier[N_ATTRIBUTE_VALS]; 
    int num_of_emitters, emitcategory_list[MAX_EMITTERS];
    int emittype_list[MAX_EMITTERS], emitstatus_list[MAX_EMITTERS];
    int emitindex_list[MAX_EMITTERS], emitmode_list[MAX_EMITTERS]; 
    float emitorient_list[MAX_EMITTERS];
    int num_of_sonars;
    int sonartype_list[MAX_SONARS], sonarstatus_list[MAX_SONARS];
    int sonarmode_list[MAX_SONARS], sonardepth_list[MAX_SONARS]; 
    int num_of_positions;
    float positionlat_list[MAX_POSITIONS], positionlon_list[MAX_POSITIONS];
    float unit_ew_acq[N_EW_SECTOR_VALS];
    float unit_ew_launch[N_EW_SECTOR_VALS];
    float unit_ew_guide[N_EW_SECTOR_VALS];
    float unit_ew_power[N_EW_SECTOR_VALS];
    float unit_ew_surf[N_EW_SECTOR_VALS];
    int iffmode_params_list[N_IFF_PARAMS];
    int iffmode_status_list[N_IFF_PARAMS];
    int iffmode_value_list[N_IFF_PARAMS];

    char string_params[N_ATTRIBUTE_VALS][ARRAY_STR_LENS];
    float f_params[N_ATTRIBUTE_VALS];

    fill_params(mess, &num_p, params, int_params, f_params, strlens,
                string_params, qualifier, &num_of_emitters, emitcategory_list,
                emittype_list, emitstatus_list, emitindex_list,
                emitorient_list, emitmode_list,
                &num_of_sonars, sonartype_list, sonarstatus_list,
                sonarmode_list, sonardepth_list,
                &num_of_positions, positionlat_list, positionlon_list,
                unit_ew_acq, unit_ew_launch, unit_ew_guide, unit_ew_power,
                unit_ew_surf, iffmode_params_list, iffmode_status_list,
                iffmode_value_list);

    alsp_receive_create(&num_p,
                        params, 
                        int_params,
                        f_params,
                        &num_of_emitters, 
                        emitcategory_list,
                        emittype_list, 
                        emitstatus_list, 
                        emitindex_list,
                        emitorient_list, 
                        emitmode_list,
                        &num_of_sonars,
                        sonartype_list,
                        sonarstatus_list,
                        sonarmode_list,
                        sonardepth_list,
                        &num_of_positions,
                        positionlat_list,
                        positionlon_list,
                        unit_ew_acq,
                        unit_ew_launch,
                        unit_ew_guide,
                        unit_ew_power,
                        unit_ew_surf,
                        iffmode_params_list, 
                        iffmode_status_list, 
                        iffmode_value_list, 
                        strlens,
                        string_params
                        ); 


#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_delete (mess)

    MESSAGE_STRUCT *mess;
{

    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this */
                  /* message */ 
    int id;

    list = get_message_parameters (mess);
             
    if (list == NULL) {
#ifdef DEBUG
    printf(stderr, "received delete message with no parameters\n");
#endif
    } else {
    ptype = get_parameter_type (list);
    if (ptype != (int)par_at_delete_id) {
#ifdef DEBUG
        printf(stderr, 
           "received delete message with unknown parameter\n");
#endif
    } else {
        id = get_par_int_value (list);
        alsp_receive_delete(&id);
    }
    }
#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_grant_advance (mess)

    MESSAGE_STRUCT *mess;
{

    float time;
    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this message */

    list = get_message_parameters (mess);
             
    if (list == NULL)  {
#ifdef DEBUG
    printf(stderr, "received advance message with no parameters\n");
#endif
    }
    else {
    ptype = get_parameter_type (list);
    if (ptype != (int)par_at_grant_adv_time) {
#ifdef DEBUG
        printf(stderr, 
           "received advance message with unknown parameter\n");
#endif
    } else {
        time = get_par_float_value (list);
        alsp_receive_advance(&time);
        }
    }

#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_interaction (mess)

    MESSAGE_STRUCT *mess;
{

    int num_p = 0;
/*****************************************************************************
changed by dave perme 28 may 92 since n_attribute_vals = 32, and this
is an interaction, N_INTERACTION_PAR = 18, this is a mismatch, which may
cause problems occaissionally between the c and fortran  
    int 
    params[N_ATTRIBUTE_VALS], int_params[N_ATTRIBUTE_VALS],
        strlens[N_ATTRIBUTE_VALS]; 
    float f_params[N_ATTRIBUTE_VALS];
    char string_params[N_ATTRIBUTE_VALS][ARRAY_STR_LENS];

    char string_array[N_ATTRIBUTE_VALS][ARRAY_STR_LENS];
    int num_targets = 0;

    int target_list[N_ATTRIBUTE_VALS]; 
******************************************************************************/
    int params[N_INTERACTION_PAR], int_params[N_INTERACTION_PAR],
        strlens[N_INTERACTION_PAR]; 
    float f_params[N_INTERACTION_PAR];
    char string_params[N_INTERACTION_PAR][ARRAY_STR_LENS];
    int num_targets = 0;
    int target_list[N_INTERACTION_PAR];

    fill_inter_params(mess, &num_p, params, int_params,
              f_params, strlens, string_params,
              &num_targets, target_list); 

    alsp_receive_interact(&num_p, params, int_params, f_params, strlens,
            string_params, &num_targets, target_list);   

#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_join_ack (mess)

    MESSAGE_STRUCT *mess;
{
    float time;
    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this message */

    list = get_message_parameters (mess);
             
    if (list == NULL)  {
#ifdef DEBUG
    printf(stderr, "received join ack message with no parameters\n");
#endif
    } else {
    ptype = get_parameter_type (list);
    if (ptype != (int) par_at_join_ack_time) {
#ifdef DEBUG
        printf(stderr, 
           "received join ack message with unknown parameter\n");
#endif
    } else {
        time = get_par_float_value (list);
        alsp_receive_join_ack(&time);
        }
    }

#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_lock_notify (mess)

    MESSAGE_STRUCT *mess;
{

    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this message */
    ALSP_ATTRIBUTE_LIST attrs, attr_send;
    int id, num_attr, status, not_len, com_len;
    char notation[ARRAY_STR_LENS], comment[ARRAY_STR_LENS];


    num_attr = not_len = com_len = 0;
    status = -1;

    notation[0] = '\0';
    comment[0] = '\0';


    list = get_message_parameters (mess);
             
    while (list != NULL) {
    ptype = get_parameter_type (list);
    switch(ptype) {
        case (int)par_at_lock_notify_id:
            id = get_par_int_value (list);
        break;
        case (int)par_at_lock_notify_attrs:
#ifdef NOT_POSITIONAL
        get_par_attr_list(list, attrs);
        fill_attr_array(&num_attr, attr_send, attrs);
#else
        get_par_attr_list(list, attrs);
        shift_attr_left(attr_send, attrs);
        num_attr = N_ATTRIBUTE_VALS;
#endif
        break;
        case (int)par_at_lock_notify_status:
        status = get_par_enum_value(list);
        break;
        case (int)par_at_lock_notify_notation:

        strcpy(notation, get_par_string_value(list));
        not_len = strlen(notation);
        break;
        case (int)par_at_lock_notify_comment:
        strcpy(comment, get_par_string_value(list));
        com_len = strlen(comment);
        break;
        default:
#ifdef DEBUG
        printf(stderr, 
               "received illegal parameter for lock not message\n");
#endif
        break;
        }
    list = next_param (list);
    }

    alsp_receive_lock_not(&id, &num_attr, attr_send, &status, &not_len,
             notation, &com_len , comment);
#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_new_id (mess)

    MESSAGE_STRUCT *mess;
{
    INTLIST id_list;
    int num_ids;
    MESS_PARAM_LIST *list;
    int ptype;

    list = get_message_parameters (mess);

    if (list == NULL) {
#ifdef DEBUG
        printf("received new ids message with no parameters\n");
#endif
    } else {
        ptype = get_parameter_type (list);
        if (ptype != (int) par_at_new_id_id_list) {
#ifdef DEBUG
       printf("unrecognizable parameter type received in new ids\n");
#endif
        } else {
            get_par_int_list( list, id_list, &num_ids);
            alsp_receive_new_ids(&num_ids, id_list);
         }
     }
#ifdef DUMP
    dump_mess_struct (mess); 
#endif

}

void handle_at_refresh (mess)

    MESSAGE_STRUCT *mess;
{

    int id, num_attr;
    ALSP_ATTRIBUTE_LIST attrs, attr_send;
    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this message */

    list = get_message_parameters (mess);
             
    while (list != NULL) {
    ptype = get_parameter_type (list);
    switch(ptype) {
        case (int)par_tt_refresh_id:
            id = get_par_int_value (list);
        break;
        case (int)par_tt_refresh_attrs:
#ifdef NOT_POSITIONAL
        get_par_attr_list(list, attrs);
        fill_attr_array(&num_attr, attr_send, attrs);
#else
        get_par_attr_list(list, attrs);
        shift_attr_left(attr_send, attrs);
        num_attr = N_ATTRIBUTE_VALS;
#endif
        break;
        default:
#ifdef DEBUG
        printf(stderr, 
               "received illegal parameter for available message\n");
#endif
        break;
        }
    list = next_param (list);
    }

    alsp_receive_refresh(&id, &num_attr, attr_send);


#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_unavail (mess)

    MESSAGE_STRUCT *mess;
{
  
    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this message */
    ALSP_ATTRIBUTE_LIST attrs, attr_send;
    int num_attr, id;

    list = get_message_parameters (mess);
             
    while (list != NULL) {
    ptype = get_parameter_type (list);
    switch(ptype) {
        case (int)par_at_unavail_id:
            id = get_par_int_value (list);
        break;
        case (int)par_at_unavail_attrs:
#ifdef NOT_POSITIONAL
        get_par_attr_list(list, attrs);
        fill_attr_array(&num_attr, attr_send, attrs);
#else
        get_par_attr_list(list, attrs);
        shift_attr_left(attr_send, attrs);
        num_attr = N_ATTRIBUTE_VALS;
#endif
        break;
        default:
#ifdef DEBUG
        printf(stderr, 
               "received illegal parameter for available message\n");
#endif
        break;
        }
    list = next_param (list);
    }
    alsp_receive_unavailable(&id, &num_attr, attr_send);

#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_unlock_request (mess)

    MESSAGE_STRUCT *mess;
{

    MESS_PARAM_LIST *list; 
    int ptype;  /* an enumeration of parameters for this message */
    int id;
    ALSP_ATTRIBUTE_LIST attrs, attr_send;
    
    int num_attr, not_len;
    char notation[ARRAY_STR_LENS];

    not_len = num_attr = 0;

    list = get_message_parameters (mess);
             
    while (list != NULL) {
    ptype = get_parameter_type (list);
    switch(ptype) {
        case (int)par_at_unlock_req_id:
            id = get_par_int_value (list);
        break;
        case (int)par_at_unlock_req_attrs:
#ifdef NOT_POSITIONAL
        get_par_attr_list(list, attrs);
        fill_attr_array(&num_attr, attr_send, attrs);
#else
        get_par_attr_list(list, attrs);
        shift_attr_left(attr_send, attrs);
        num_attr = N_ATTRIBUTE_VALS;
#endif
        break;
        case (int)par_at_unlock_req_notation:
        strcpy(notation, get_par_string_value(list));
        not_len = strlen(notation);
        break;
        default:
#ifdef DEBUG
        printf(stderr, 
               "received illegal parameter for available message\n");
#endif
        break;
        }
    list = next_param (list);
    }

    alsp_receive_unlock_req(&id, &num_attr, attr_send, &not_len,
                notation); 

#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_update (mess)
/* Modified:  Stephanie Paden - 2/98 (E1907)                 */
/*            Added IFF data to the attributes handled by an */
/*            update message.                                */

    MESSAGE_STRUCT *mess;
{

    int num_p = 0;
    int 
    params[N_ATTRIBUTE_VALS], int_params[N_ATTRIBUTE_VALS],
    strlens[N_ATTRIBUTE_VALS], qualifier[N_ATTRIBUTE_VALS]; 
    int num_of_emitters = 0;
    int emitcategory_list[MAX_EMITTERS];
    int emittype_list[MAX_EMITTERS], emitstatus_list[MAX_EMITTERS];
    int emitindex_list[MAX_EMITTERS], emitmode_list[MAX_EMITTERS]; 
    float emitorient_list[MAX_EMITTERS];
    int num_of_sonars;
    int sonartype_list[MAX_SONARS], sonarstatus_list[MAX_SONARS];
    int sonarmode_list[MAX_SONARS], sonardepth_list[MAX_SONARS]; 
    int num_of_positions;
    float positionlat_list[MAX_POSITIONS], positionlon_list[MAX_POSITIONS];
    float unit_ew_acq[N_EW_SECTOR_VALS];
    float unit_ew_launch[N_EW_SECTOR_VALS];
    float unit_ew_guide[N_EW_SECTOR_VALS];
    float unit_ew_power[N_EW_SECTOR_VALS];
    float unit_ew_surf[N_EW_SECTOR_VALS];
    int iffmode_params_list[N_IFF_PARAMS];
    int iffmode_status_list[N_IFF_PARAMS];
    int iffmode_value_list[N_IFF_PARAMS];     

    float f_params[N_ATTRIBUTE_VALS];
    char string_params[N_ATTRIBUTE_VALS][ARRAY_STR_LENS];

    fill_params (mess, &num_p, params, int_params, f_params, strlens,
                string_params, qualifier, &num_of_emitters, emitcategory_list,
                emittype_list, emitstatus_list, emitindex_list,
                emitorient_list, emitmode_list,
                &num_of_sonars, sonartype_list, sonarstatus_list,
                sonarmode_list, sonardepth_list,
                &num_of_positions, positionlat_list, positionlon_list,
                unit_ew_acq, unit_ew_launch, unit_ew_guide, unit_ew_power,
                unit_ew_surf, iffmode_params_list, iffmode_status_list,
                iffmode_value_list);

    alsp_receive_update(&num_p,
                        params,
                        int_params,
                        f_params,
                        &num_of_emitters, 
                        emitcategory_list,
                        emittype_list, 
                        emitstatus_list, 
                        emitindex_list,
                        emitorient_list, 
                        emitmode_list,
                        &num_of_sonars, 
                        sonartype_list,  
                        sonarstatus_list, 
                        sonarmode_list,
                        sonardepth_list,
                        &num_of_positions, 
                        positionlat_list,  
                        positionlon_list, 
                        unit_ew_acq,
                        unit_ew_launch,
                        unit_ew_guide,
                        unit_ew_power,
                        unit_ew_surf,
                        iffmode_params_list,
                        iffmode_status_list, 
                        iffmode_value_list, 
                        strlens,
                        string_params
                        ); 

#ifdef DUMP
    dump_mess_struct (mess); 
#endif

}

void handle_at_start_save (mess)

    MESSAGE_STRUCT *mess;
{
     MESS_PARAM_LIST *list;
     int ptype;                /* an enumeration of parameters for this
                                  message     */
     float time;
     int label_len;

     char label[ARRAY_STR_LENS];

     label_len = 0;

     label[0] = '\0';
     list = get_message_parameters (mess);

     while (list != NULL) {
          ptype = get_parameter_type (list);
          switch(ptype) {
              case (int)par_at_start_save_time:
                  time = get_par_float_value(list);
                  break;
              case (int)par_at_start_save_label:
                  strcpy(label, get_par_string_value(list));
                  label_len = strlen(label);
                  break;
               default:
#ifdef DEBUG
                  printf(stderr,
                     "received illegal parameter for start_save msg\n");
#endif
                  break;
           }
        list = next_param (list);
     }

     alsp_receive_start_save(&time, &label_len, label);

#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_set_rate (mess)
/* Jay Braun - 4/2015 */

    MESSAGE_STRUCT *mess;
{
     MESS_PARAM_LIST *list;
     int ptype;                /* an enumeration of parameters for this
                                  message     */
     float rate;

     list = get_message_parameters (mess);

     while (list != NULL) {
          ptype = get_parameter_type (list);
          switch(ptype) {
              case (int)par_at_set_rate_rate:
                  rate = get_par_float_value(list);
                  break;
               default:
#ifdef DEBUG
                  printf(stderr,
                     "received illegal parameter for set_rate msg\n");
#endif
                  break;
           }
        list = next_param (list);
     }

     alsp_receive_set_rate(&rate);

#ifdef DUMP
    dump_mess_struct (mess);
#endif
}

void handle_at_ping (mess)
/*  Carol Kropp - 1/94                                          */
/*  Handle_at_ping processes the ACM to Actor PING(...) message */

    MESSAGE_STRUCT *mess;
{
     MESS_PARAM_LIST *list;
     int text_len;

     char text[MAX_MESSAGE_LEN];

     text[0] = '\0';
     list = get_message_parameters (mess);
     if (list != NULL)
         strcpy(text, get_par_string_value(list));
     text_len = strlen(text);

    alsp_receive_ping(&text_len, text);

#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}

void handle_at_ping_ack (mess)
/*  Carol Kropp - 1/94                                                  */
/*  Handle_at_ping_ack processes the ACM to Actor PING_ACK(...) message */

    MESSAGE_STRUCT *mess;
{
     MESS_PARAM_LIST *list;
     int text_len;

     char text[MAX_MESSAGE_LEN];

     text[0] = '\0';
     list = get_message_parameters (mess);
     strcpy(text, get_par_string_value(list));
     text_len = strlen(text);

    alsp_receive_ping_ack(&text_len, text);

#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}


void handle_at_signal (mess)
/*  Carol Kropp - 9/94  (E1457)                                         */
/*  Handle_at_Signal processes the ACM to Actor SIGNAL() message.       */

    MESSAGE_STRUCT *mess;
{
    MESS_PARAM_LIST *list; 
    int ptype;            /* an enumeration of parameters for this message */
    int signal_type;

    list = get_message_parameters (mess);
             
    if (list == NULL) {
      printf ("ALSP:  Received a signal message with no parameters\n");
    }
    else {
      ptype = get_parameter_type (list);
      if (ptype != (int)par_at_signal_type) {
        printf ("ALSP:  Received a signal message with unknown parameter\n");
      }
      else {
        signal_type = get_par_enum_value (list);
        alsp_receive_signal (&signal_type);
      }   /* end of else signal type matched an enumeration */
    }   /* end of else list is not null */
    
#ifdef DUMP
    dump_mess_struct (mess); 
#endif
}   /* end of function handle_at_signal */
