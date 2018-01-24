#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_types.h"
#include "mess_struct.h"
#include "ascii_tables.h"
#include "mess_format.h"
#include "alsp_utils.h"

static void gen_alsp_params ();
static void gen_emitter_params ();
static void gen_sonar_params ();
static void gen_iffmode_params ();        /* smp- 2/98 (E1907) */

/* In generating an ascii ALSP message, perform processing 
 * required to handle special case syntax for interaction
 * messages.
 */
void special_case (str, m, ptable, param_len, msubtype)

    char **str;
    MESSAGE_STRUCT *m;
    KEYSTRING_TABLE ptable;
    int *param_len;
    int msubtype;
{
    MESS_PARAM_LIST *ptemp, *psave, *tempnext, *savelast, *tempprev;

  if (msubtype == type_at_interaction) {
    ptemp = m->params;
    psave = NULL;
    tempprev = NULL;
    while (ptemp != NULL) {
        tempnext = ptemp->next;
        if ((ptemp->param_type == par_tt_int_firing)
        || (ptemp->param_type == par_tt_int_victim)
        || (ptemp->param_type == par_tt_int_fire_kind)) {
            if (psave == NULL) {
                psave = ptemp;
            } else {
                savelast->next = ptemp;
            }
            if (ptemp == m->params) {
                m->params = tempnext;
            }
            if (tempprev != NULL) {
                tempprev->next = tempnext;
            }
            savelast = ptemp;
            savelast->next = NULL;
        } else {
            tempprev = ptemp;
        }
        ptemp = tempnext;
    }
    gen_alsp_params (str, psave, ptable, param_len, msubtype);
    ptemp = psave;
    while (ptemp != NULL) {
        tempnext = ptemp;
        free_param_space (ptemp);
        ptemp = ptemp->next;
        free (tempnext);
    }

    ptemp = m->params;
    psave = NULL;
    tempprev = NULL;
    while (ptemp != NULL) {
        tempnext = ptemp->next;
        if ((ptemp->param_type == par_tt_int_salvo_num)
        || (ptemp->param_type == par_tt_int_salvo_wtype)
        || (ptemp->param_type == par_tt_int_salvo_size)) {
            if (psave == NULL) {
                psave = ptemp;
            } else {
                savelast->next = ptemp;
            }
            if (ptemp == m->params) {
                m->params = tempnext;
            }
            if (tempprev != NULL) {
                tempprev->next = tempnext;
            }
            savelast = ptemp;
            savelast->next = NULL;
        } else {
            tempprev = ptemp;
        }
        ptemp = tempnext;
    }
    strncpy (*str, "PARAMETERS(", 11);
    *str += 11;
    if (psave != NULL) {
        strncpy (*str, "SALVO(", 6);
        *str += 6;
        gen_alsp_params (str, psave, ptable, param_len, msubtype);
        strncpy (*str, END_PARAM, DELIMITER_LEN);
        *str += (DELIMITER_LEN);
        *param_len += (DELIMITER_LEN + 6);
        ptemp = psave;
        while (ptemp != NULL) {
            tempnext = ptemp;
            free_param_space (ptemp);
            ptemp = ptemp->next;
            free (tempnext);
        }
    }
    *param_len += 11;

    ptemp = m->params;             /* Added for the target_ref() wrapper when  */
    psave = NULL;                  /* non-unit targets are attacked via BE and */
    tempprev = NULL;               /* BE + TIN - jhm 2/98 (E1911)              */
    while (ptemp != NULL) {
        tempnext = ptemp->next;
        if ((ptemp->param_type ==  par_tt_target_ref_ben)
        ||  (ptemp->param_type ==  par_tt_target_ref_ben_suf)
        ||  (ptemp->param_type ==  par_tt_target_ref_ttn)
        ||  (ptemp->param_type ==  par_tt_target_ref_pin)
        ||  (ptemp->param_type ==  par_tt_target_ref_loc)) {
            if (psave == NULL) {
                psave = ptemp;
            } else {
                savelast->next = ptemp;
            }
            if (ptemp == m->params) {
                m->params = tempnext;
            }
            if (tempprev != NULL) {
                tempprev->next = tempnext;
            }
            savelast = ptemp;
            savelast->next = NULL;
        } else {
            tempprev = ptemp;
        }
        ptemp = tempnext;
    }
    /* PARAMETERS( added above */
    if (psave != NULL) {
        strncpy (*str, "TARGET_REF(", 11);
        *str += 11;
        gen_alsp_params (str, psave, ptable, param_len, msubtype);
        strncpy (*str, END_PARAM, DELIMITER_LEN);
        *str += (DELIMITER_LEN);
        *param_len += (DELIMITER_LEN + 11);
        ptemp = psave;
        while (ptemp != NULL) {
            tempnext = ptemp;
            free_param_space (ptemp);
            ptemp = ptemp->next;
            free (tempnext);
        }
    }                            /* end target_ref() wrapper hanlding */

    ptemp = m->params;
    psave = NULL;
    tempprev = NULL;
    while (ptemp != NULL) {
        tempnext = ptemp->next;
        if ((ptemp->param_type == par_tt_int_supply_type)
        ||  (ptemp->param_type == par_tt_int_supply_qty)) {
            if (psave == NULL) {
                psave = ptemp;
            } else {
                savelast->next = ptemp;
            }
            if (ptemp == m->params) {
                m->params = tempnext;
            }
            if (tempprev != NULL) {
                tempprev->next = tempnext;
            }
            savelast = ptemp;
            savelast->next = NULL;
        } else {
            tempprev = ptemp;
        }
        ptemp = tempnext;
    }
    /* PARAMETERS( added above */
    if (psave != NULL) {
        strncpy (*str, "SUPPLY_POL(", 11);
        *str += 11;
        gen_alsp_params (str, psave, ptable, param_len, msubtype);
        strncpy (*str, END_PARAM, DELIMITER_LEN);
        *str += (DELIMITER_LEN);
        *param_len += (DELIMITER_LEN + 11);
        ptemp = psave;
        while (ptemp != NULL) {
            tempnext = ptemp;
            free_param_space (ptemp);
            ptemp = ptemp->next;
            free (tempnext);
        }
    }

  } else if (msubtype == type_at_update) {
    /* khc,3/93- Generated EMITTER(EMITTER_TYPES()EMITTER_NAMES()EMITTER_STATUS()) */
    /* SMM 9/94- Generated EMITTER(EMITTER_CATEGORY(EMITTER_TYPE() */
    /* EMITTER_STATUS()EMITTER_INDEX()EMITTER_ORIENT()EMITTER_MODE())) */
    /* smp 2/98 (E1907) - Generate IFF(MASTER()MODE1(STATUS()MODE())           */
    /* MODE2(STATUS()MODE())MODE3(STATUS()MODE())MODEC(STATUS()MODE())MODE4()) */
    ptemp = m->params;
    psave = NULL;
    tempprev = NULL;
    while (ptemp != NULL) {
        tempnext = ptemp->next;
        if (ptemp->param_type == tt_attr_emitter || 
            ptemp->param_type == tt_attr_iff) { 
            if (psave == NULL) {
                psave = ptemp;
            } else {
                savelast->next = ptemp;
            }
            if (ptemp == m->params) {
                m->params = tempnext;
            }
            if (tempprev != NULL) {
                tempprev->next = tempnext;
            }
            savelast = ptemp;
            savelast->next = NULL;
        } else {
            tempprev = ptemp;
        }
        ptemp = tempnext;
    }
    if (psave != NULL) {
        gen_alsp_params (str, psave, ptable, param_len, msubtype);
        //strncpy (*str, END_PARAM, DELIMITER_LEN);
        //*str += (DELIMITER_LEN);
        //*param_len += DELIMITER_LEN;
        ptemp = psave;
        while (ptemp != NULL) {
            tempnext = ptemp;
            free_param_space (ptemp);
            ptemp = ptemp->next;
            free (tempnext);
        }
    }
  } else {
    /* skip */
  }    /* END IF..THEN..ELSE */
}

/* Given a MESSAGE_STRUCT, return the corresponding ascii ALSP
 * message text and its corresponding length (not including null
 * character) in alsp_message and len, respectively.
 */
void gen_alsp_message (mess, len, alsp_message) 

    MESSAGE_STRUCT *mess;
    int *len;
    char *alsp_message;
{
    char *current_data = alsp_message;
    int current_len = 0;
    int param_len = 0;
    MESS_HEADER header;
    MESSAGE_DATA_TYPE mtype;
    int msubtype;
    char *keystring;

    current_data = alsp_message;
    header = mess->header;
    mtype = header.alsp_mess_type.mess_type;
    switch (mtype) {
        case actor_acm_mess:
            msubtype = (int) header.alsp_mess_type.mess_subtype.ta_type;
            keystring = get_keyword (ta_mess_keytab, msubtype);
            break;
        case acm_actor_mess:
            msubtype = (int) header.alsp_mess_type.mess_subtype.at_type;
            keystring = get_keyword (at_mess_keytab, msubtype);
            break;
        case actor_actor_mess:
            msubtype = (int) header.alsp_mess_type.mess_subtype.tt_type;
            keystring = get_keyword (tt_mess_keytab, msubtype);
            break;
        default:
            break;
    }
        
    strncpy (current_data, keystring, strlen (keystring));
    current_data += strlen (keystring);
    current_len += strlen (keystring);

    strncpy (current_data, START_PARAM, DELIMITER_LEN);
    current_data += DELIMITER_LEN;
    current_len += DELIMITER_LEN;

    if ((mtype == acm_actor_mess)
        && ((msubtype == type_at_interaction) ||    /* khc,3/93- Generate EMITTER() inside UPDATE() */
            (msubtype == type_at_update))) {        /* message. */
        special_case (&current_data, mess, ptable_lookup_by_mess_type (
            mtype, msubtype), &param_len, msubtype);
    }
    gen_alsp_params (&current_data, mess->params, 
        ptable_lookup_by_mess_type (mtype, msubtype), &param_len, msubtype);
    current_len += param_len;

    if ((mtype == acm_actor_mess)
        && (msubtype == type_at_interaction)) {
        strncpy (current_data, END_PARAM, DELIMITER_LEN);
        current_data += DELIMITER_LEN;
        current_len += DELIMITER_LEN;
    }
    strncpy (current_data, END_PARAM, DELIMITER_LEN);
    current_data += DELIMITER_LEN;
    current_len += DELIMITER_LEN;

    current_data [0] = '\0';

    *len = current_len;
}

/* Process a mess_struct's parameter list, placing the 
 * corresponding ALSP0.r ascii_text into *mess_string.
 */
/* Modified:  Susan Miller 3/98 (E1956) */
/*            Changed open and close parens to periods when processing */
/*            a register enumeration message containing emitter information */
static void gen_alsp_params (mess_string, params, keytab, plen, msubtype)
    
    char **mess_string;
    MESS_PARAM_LIST *params;
    KEYSTRING_TABLE keytab;
    int *plen;
    int msubtype;
{
    MESS_PARAM_LIST *par, *save_par;
    char *tmp, *ttmp, *keystring;
    char buff [200];
    int tlen, i, lentmp;
    int nvals;
    BOOL need_end_dlim = FALSEVAL;
    BOOL need_list_dlim;
    ALSP_ATTRIBUTE_LIST a;
    LOCLIST locs;
    EMITTERLIST emit_list;
    SONARLIST sonar_list;
    int num_of_emitters;
    int num_of_sonars;
    IFFMODESTR iffmode_array[N_IFF_PARAMS];  /* smp - 2/98 (E1907) */
    int num_of_iffmodes;                     /* smp - 2/98 (E1907) */
    INTLIST list;
    ENUMLIST elist;
    float lowerf, upperf, lat, lon;
    int loweri, upperi;
    ATTRIBUTE_QUALIFIER q;
    int previous_param_type = 0;             /*  SMM  3/98  CT98  E1943  */

    buff[0] = 0;
    par = params;
    tmp = *mess_string;

    while (par != NULL) {
        need_list_dlim = FALSEVAL;
        keystring = get_keyword (keytab, par->param_type);
        tlen = strlen (keystring);
        if (strncmp (keystring, "", tlen) != 0) {
            strncpy (tmp, keystring, tlen);
            tmp += tlen;
            strncpy (tmp, START_PARAM, DELIMITER_LEN);
            tmp += DELIMITER_LEN;
            *plen += tlen + DELIMITER_LEN;
            need_end_dlim = TRUEVAL;
        }

        switch (par->param_data.val_type) {
            case value_int:
                sprintf (buff, "%d", get_par_int_value (par));
                tlen = strlen (buff);
                strncpy (tmp, buff, tlen);
                break;
            case value_enum:
                tlen = strlen (get_par_enum_string (par));
                strncpy (tmp, get_par_enum_string (par), tlen);
                break;
            case value_float:
                sprintf (buff, "%f", get_par_float_value (par));
                tlen = strlen (buff);
                strncpy (tmp, buff, tlen);
                break;
            case value_loc_struct:
                get_par_loc_value (par, &lat, &lon);
                sprintf (buff, "LAT(%f)LONG(%f)", lat, lon);
                tlen = strlen (buff);
                strncpy (tmp, buff, tlen);
                break;
            case value_string:
                ttmp = tmp;
                tlen = strlen (get_par_string_value (par));
                strncpy (buff, get_par_string_value (par), tlen);
                for (i = 0; i < tlen; i++) {
                    if ((buff [i] == ')') || (buff [i] == '(')) {
                        ttmp [0] = '"';
                        ttmp [1] = buff [i];
                        ttmp [2] = '"';
                        ttmp += 2;
                        tlen += 2;
                    } else {
                        ttmp [0] = buff [i];
                    }
                    ttmp++;
                }
                ttmp[0] = '\0';
                break;
            case value_message:
                 gen_alsp_message (
                    get_par_mess_value (par), &tlen, tmp);
                break;
            case value_attribute_list:
                get_par_attr_list (par, a);
                tlen = 0;
                ttmp = tmp;
                for (i = 0; i <= N_ATTRIBUTE_VALS; i++) {
                    if (a [i] == TRUEVAL) {
                        if (need_list_dlim == TRUEVAL) {
                            ttmp [0] = LIST_DELIM [0];
                            tlen += DELIMITER_LEN;
                            ttmp += DELIMITER_LEN;
                        }

                        keystring = get_keyword (tt_attribute_keytab,i);/* special case */
                        lentmp = strlen (keystring);
                        strncpy (ttmp, keystring, lentmp);
                        need_list_dlim = TRUEVAL;
                        tlen += lentmp;
                        ttmp += lentmp;
                    }
                    #ifdef DEBUG
                        if (i == 71 || i == 72) {
                             keystring = get_keyword (tt_attribute_keytab,i);/* special case */
                             printf("CREATE(%s)(%d)\n", keystring, i);
                        }
                    #endif
                }
                break;
            case value_int_list:
                get_par_int_list (par, list, &nvals);
                tlen = 0;
                ttmp = tmp;
                for (i = 0; i < nvals; i++) {
                    if (need_list_dlim == TRUEVAL) {
                        ttmp [0] = LIST_DELIM [0];
                        tlen += DELIMITER_LEN;
                        ttmp += DELIMITER_LEN;
                    }
                    sprintf (buff, "%d", list [i]);
                    lentmp = strlen (buff);
                    strncpy (ttmp, buff, lentmp);
                    need_list_dlim = TRUEVAL;
                    tlen += lentmp;
                    ttmp += lentmp;
                }
                break;
            case value_loc_struct_list:
                get_par_loc_list (par, locs, &nvals);
                tlen = 0;
                ttmp = tmp;
                for (i = 0; i < nvals; i++) {
                    sprintf (buff, "LAT(%f)LONG(%f)",
                        locs [i].lat, locs [i].lon);
                    lentmp = strlen (buff);
                    strncpy (ttmp, buff, lentmp);
                    need_list_dlim = TRUEVAL;
                    tlen += lentmp;
                    ttmp += lentmp;
                }
                break;
            case value_emit_struct_list:    /* all values in emit_list */
            /*  Assign emitter a previous param type to avoid mismatched parens */
            /*  if the next parameter is the IFF attribute    SMP  3/98  CT98  E1943  */
                previous_param_type = tt_attr_emitter;    /*  SMM  3/98  CT98  E1943  */
                get_par_emitter_list(par,emit_list,&num_of_emitters); 
                tlen = 0;
                ttmp = tmp;
                for (i = 0; i < num_of_emitters; i++) {
                    if (emit_list[i].emitter_cat == tt_attr_jammer) 
                        sprintf (buff, "JAMMER(");
                    else if (emit_list[i].emitter_cat == tt_attr_radar)
                        sprintf (buff, "RADAR(");

                    lentmp = strlen (buff);
                    strncpy (ttmp, buff, lentmp);
                    need_end_dlim = TRUEVAL;
                    tlen += lentmp;
                    ttmp += lentmp;
                    gen_emitter_params(i,emit_list,&lentmp,ttmp);
                    tlen += lentmp;
                    ttmp += lentmp;
                    if (need_end_dlim) {
                        strncpy (ttmp, END_PARAM, DELIMITER_LEN); 
                        tlen += DELIMITER_LEN;
                        ttmp += DELIMITER_LEN;
                        need_end_dlim = FALSEVAL;
                    }
                }
                need_end_dlim = TRUEVAL;
                break;
            case value_sonar_struct_list:    /* all values in sonar_list */
                previous_param_type = tt_attr_sonar;
                get_par_sonar_list(par,sonar_list,&num_of_sonars); 
                tlen = 0;
                ttmp = tmp;
                for (i = 0; i < num_of_sonars; i++) {
                    lentmp = strlen (buff);
                    strncpy (ttmp, buff, lentmp);
                    need_end_dlim = FALSEVAL;
                    tlen += lentmp;
                    ttmp += lentmp;
                    gen_sonar_params(i,sonar_list,&lentmp,ttmp);
                    tlen += lentmp;
                    ttmp += lentmp;
                    if (need_end_dlim) {
                        strncpy (ttmp, END_PARAM, DELIMITER_LEN); 
                        tlen += DELIMITER_LEN;
                        ttmp += DELIMITER_LEN;
                        need_end_dlim = FALSEVAL;
                    }
                }
                need_end_dlim = TRUEVAL;
                break;
            case value_iff_struct_list:     /* all values in iff_list - smp 2/98 (E1907) */
                get_par_iffmode_list(par,iffmode_array); 
                tlen = 0;
                ttmp = tmp;
          
                gen_iffmode_params(iffmode_array,&lentmp,ttmp);
                need_end_dlim = TRUEVAL;
                tlen += lentmp;
                ttmp += lentmp;
                break; 
            case value_enum_list:
                get_par_enum_list (par, elist, &nvals);
                tlen = 0;
                ttmp = tmp;
                for (i = 0; i < nvals; i++) {
                    if (need_list_dlim == TRUEVAL) {
                        ttmp [0] = LIST_DELIM [0];
                        tlen += DELIMITER_LEN;
                        ttmp += DELIMITER_LEN;
                    }
                    lentmp = strlen (elist [i].enum_string);
                    strncpy (ttmp, elist [i].enum_string, lentmp);
                    need_list_dlim = TRUEVAL;
                    tlen += lentmp;
                    ttmp += lentmp;
                }
                break;
            case value_int_range:
                get_par_int_range (par, &loweri, &upperi);
                sprintf (buff, "%d,%d", loweri,upperi);
                tlen = strlen (buff);
                strncpy (tmp, buff, tlen);
                break;
            case value_float_range:
                get_par_float_range (par, &lowerf, &upperf);
                sprintf (buff, "%f,%f", lowerf, upperf);
                tlen = strlen (buff);
                strncpy (tmp, buff, tlen);
                break;
            default:
                alsp_console_message
                    ("Unrecognized parameter in gen_alsp_params.");
        }   /* end switch () */

        tmp += tlen;
        *plen += (tlen);

        q = get_par_attr_qual (par);
        tlen = 0;

/* SMM 1/14/97 E1694) */
        if ((msubtype == type_ta_reg_enum) &&
            (par->param_type == par_ta_reg_enum_attr) &&
            (par->param_data.val.val_enum.enum_value == tt_attr_emitter)) {

           /* get the RADAR or JAMMER part before closing the parans */
           par = par->next; 

           strncpy (tmp, PERIOD_SYMBOL, DELIMITER_LEN); /* SMM 3/98 (E1956) */
           tmp += DELIMITER_LEN;
           *plen += tlen + DELIMITER_LEN;

           keystring = get_keyword (keytab, par->param_type);
           tlen = strlen (keystring);
           if (strncmp (keystring, "", tlen) != 0) {
               strncpy (tmp, keystring, tlen);
               tmp += tlen;
               strncpy (tmp, PERIOD_SYMBOL, DELIMITER_LEN);/* SMM 3/98 (E1956)*/
               tmp += DELIMITER_LEN;
               *plen += tlen + DELIMITER_LEN;
           }
           tlen = 0;
           ttmp = tmp;

           tlen = strlen (get_par_enum_string (par));
           strncpy (tmp, get_par_enum_string (par), tlen);
           tmp += tlen;
           *plen += (tlen);
           tlen = 0;

           lentmp = strlen (buff);
           strncpy (ttmp, buff, lentmp);
           tlen += lentmp;
           ttmp += lentmp;
        } /* end if */

/* khc, 3/93 - The qualifier string of EMITTER is placed like this: */
/* EMITTER(RADAR(type()index()status())
           JAMMER(type()index()status()mode()) 
           JAMMER(type()index()status()orient())@ALL) */ 
/* IFF(MASTER()MODE1((STATUS()MODE())MODE2((STATUS()MODE())
            MODE3((STATUS()MODE())MODEC((STATUS()MODE())MODE4()))) - smp 2/98 (E1907) */


        if (par->param_type == tt_attr_emitter ||
            par->param_type == tt_attr_iff) {
            if (q != qual_none) {
                sprintf (buff, "%s", get_keyword (attr_qual_keytab, q));
                tlen = strlen (buff);
                strncpy (tmp, buff, tlen);
            }
            *plen += (tlen);
            tmp += tlen;
            if (need_end_dlim) {
                strncpy (tmp, END_PARAM, DELIMITER_LEN);
                tmp += DELIMITER_LEN;
                *plen += DELIMITER_LEN;
                need_end_dlim = FALSEVAL;
            }
        } else {
            if (q != qual_none) {
                sprintf (buff, "%s", get_keyword (attr_qual_keytab, q));
                tlen = strlen (buff);
                strncpy (tmp, buff, tlen);
            }
            *plen += (tlen);
            tmp += tlen;
            if (need_end_dlim) {
                strncpy (tmp, END_PARAM, DELIMITER_LEN);
                tmp += DELIMITER_LEN;
                *plen += DELIMITER_LEN;
                need_end_dlim = FALSEVAL;
            }
        }       /* end if..then..else */

// Removed 7/15 because adds extra right paren to IFF attribute
//        if ((par->param_type == tt_attr_iff) &&
//            (previous_param_type == tt_attr_emitter)) {     /*  SMM  3/98  CT98  E1943  */
//            strncpy (tmp, END_PARAM, DELIMITER_LEN);
//            tmp += DELIMITER_LEN;
//            *plen += DELIMITER_LEN;
//        }
        par = par->next;

    }
    *mess_string = tmp;
}


static void gen_emitter_params (index, list, tlen, ttmp)
    
    int index;
    EMITTERLIST list;
    int *tlen;
    char *ttmp;
{
    char buff [80];
    int i;
    int str_len;
    BOOL category_flag, index_flag, status_flag;  /* all required */
    BOOL type_flag, orient_flag, mode_flag;
    int temp_tlen = 0;
        
    category_flag = list[index].emitter_params[0];
    type_flag = list[index].emitter_params[1]; 
    index_flag = list[index].emitter_params[2]; 
    status_flag = list[index].emitter_params[3];
    orient_flag = list[index].emitter_params[4];
    mode_flag = list[index].emitter_params[5];

    /* int emitter_index, required parameter */
    sprintf (buff, "INDEX(%d)",list[index].emitter_index);
    str_len = strlen (buff);
    strncpy (ttmp, buff, str_len); 
    temp_tlen += str_len;
    ttmp += str_len;

    /* ENUM_VALUE emitter_status, required parameter */
    sprintf (buff, "STATUS(");
    str_len = strlen(buff);
    strncpy (ttmp, buff, str_len);
    temp_tlen += str_len;
    ttmp += str_len;

    strcpy (buff, list[index].emitter_status.enum_string);
    str_len = strlen (buff);
    strncpy (ttmp, buff, str_len);
    temp_tlen += str_len;
    ttmp += str_len;

    strncpy (buff, END_PARAM, DELIMITER_LEN);
    strncpy (ttmp, buff, DELIMITER_LEN);
    temp_tlen += DELIMITER_LEN;
    ttmp += DELIMITER_LEN;

    if (type_flag) {
        /* ENUM_VALUE emitter_type */
        sprintf (buff, "TYPE(");
        str_len = strlen (buff);
        strncpy (ttmp, buff, str_len);
        temp_tlen += str_len;
        ttmp += str_len;

        str_len = strlen (list[index].emitter_type.enum_string);
        strncpy (buff, list[index].emitter_type.enum_string, str_len);
        strncpy (ttmp, buff, str_len);
        temp_tlen += str_len;
        ttmp += str_len;
    
        strncpy (buff, END_PARAM, DELIMITER_LEN);
        strncpy (ttmp, buff, DELIMITER_LEN);
        temp_tlen += DELIMITER_LEN;
        ttmp += DELIMITER_LEN;
    }

    if (list[index].emitter_cat == tt_attr_jammer) {

        if (orient_flag) {
            /* float emitter_orientation */
            sprintf (buff, "ORIENT(%.1f)",list[index].emitter_orientation);
            str_len = strlen (buff);
            strncpy (ttmp, buff, str_len);
            temp_tlen += str_len;
            ttmp += str_len;
        }

        if (mode_flag) {
            /* char *emitter_mode */
            strncpy (buff, "MODE(", 10);
            str_len = strlen(buff);
            strncpy (ttmp, buff, str_len);
            temp_tlen += str_len;
            ttmp += str_len;

            strcpy (buff, list[index].emitter_mode.enum_string);
            str_len = strlen (buff);
            strncpy (ttmp, buff, str_len);
            temp_tlen += str_len;
            ttmp += str_len;

            strncpy (buff, END_PARAM, DELIMITER_LEN);
            strncpy (ttmp, buff, DELIMITER_LEN);
            temp_tlen += DELIMITER_LEN;
            ttmp += DELIMITER_LEN;
        }
    }
    *tlen = temp_tlen;
}

static void gen_sonar_params (index, list, tlen, ttmp)
    
    int index;
    SONARLIST list;
    int *tlen;
    char *ttmp;
{
    char buff [80];
    int i;
    int str_len;
    BOOL type_flag, status_flag;  /* all required */
    BOOL mode_flag, depth_flag;
    int temp_tlen = 0;
        
    type_flag = list[index].sonar_params[0]; 
    status_flag = list[index].sonar_params[1];
    mode_flag = list[index].sonar_params[2];
    depth_flag = list[index].sonar_params[3];

    /* ENUM_VALUE sonar_type, required parameter */
    sprintf (buff, "TYPE(");
    str_len = strlen (buff);
    strncpy (ttmp, buff, str_len);
    temp_tlen += str_len;
    ttmp += str_len;

    str_len = strlen (list[index].sonar_type.enum_string);
    strncpy (buff, list[index].sonar_type.enum_string, str_len);
    strncpy (ttmp, buff, str_len);
    temp_tlen += str_len;
    ttmp += str_len;
    
    strncpy (buff, END_PARAM, DELIMITER_LEN);
    strncpy (ttmp, buff, DELIMITER_LEN);
    temp_tlen += DELIMITER_LEN;
    ttmp += DELIMITER_LEN;

    /* ENUM_VALUE sonar_status, required parameter */
    sprintf (buff, "STATUS(");
    str_len = strlen(buff);
    strncpy (ttmp, buff, str_len);
    temp_tlen += str_len;
    ttmp += str_len;

    strcpy (buff, list[index].sonar_status.enum_string);
    str_len = strlen (buff);
    strncpy (ttmp, buff, str_len);
    temp_tlen += str_len;
    ttmp += str_len;

    strncpy (buff, END_PARAM, DELIMITER_LEN);
    strncpy (ttmp, buff, DELIMITER_LEN);
    temp_tlen += DELIMITER_LEN;
    ttmp += DELIMITER_LEN;

    if (mode_flag) {
        /* char *sonar_mode */
        strncpy (buff, "MODE(", 10);
        str_len = strlen(buff);
        strncpy (ttmp, buff, str_len);
        temp_tlen += str_len;
        ttmp += str_len;

        strcpy (buff, list[index].sonar_mode.enum_string);
        str_len = strlen (buff);
        strncpy (ttmp, buff, str_len);
        temp_tlen += str_len;
        ttmp += str_len;

        strncpy (buff, END_PARAM, DELIMITER_LEN);
        strncpy (ttmp, buff, DELIMITER_LEN);
        temp_tlen += DELIMITER_LEN;
        ttmp += DELIMITER_LEN;
    }
    if (depth_flag) {
        /* float emitter_orientation */
        sprintf (buff, "DEPTH(%d)",list[index].sonar_depth);
        str_len = strlen (buff);
        strncpy (ttmp, buff, str_len);
        temp_tlen += str_len;
        ttmp += str_len;
    }

    *tlen = temp_tlen;
}

static void gen_iffmode_params (iffmode_array, tlen, ttmp)
/* Handle the creation of the IFF parameters - smp 2/98 (E1907) */
/* IFF(MASTER(v_status)MODE1(STATUS(v_status)MODE(v_mode1)) */
/*                     MODE2(STATUS(v_status)MODE(v_mode2)) */
/*                     MODE3(STATUS(v_status)MODE(v_mode3)) */
/*                     MODEC(STATUS(v_status)MODE(v_modeC)) */
/*                     MODE4(STATUS(v_status))              */

    IFFMODESTR iffmode_array[N_IFF_PARAMS];
    int *tlen;
    char *ttmp;
{
    char buff [200];
    int str_len;
    int temp_tlen = 0;
    int index;

    /* may not always have a value for MASTER status */
    if (iffmode_array[0].iffmode_params == TRUEVAL) { 
        sprintf (buff, "MASTER(");
        str_len = strlen (buff);
        strncpy (ttmp, buff, str_len);
        temp_tlen += str_len;
        ttmp += str_len;
        
        /* get the MASTER status - ON or OFF */
        strcpy (buff, iffmode_array[0].iffmode_status.enum_string);
        str_len = strlen (buff);
        strncpy (ttmp, buff, str_len);
        temp_tlen += str_len;
        ttmp += str_len;

        strncpy (buff, END_PARAM, DELIMITER_LEN);
        strncpy (ttmp, buff, DELIMITER_LEN);
        temp_tlen += DELIMITER_LEN;
        ttmp += DELIMITER_LEN;
    } /* end of dealing with status for MASTER */

    /* now let's loop through all the modes 1 to C*/
    for (index = 1; index < N_IFF_PARAMS; index++){

        if (iffmode_array[index].iffmode_params == TRUEVAL) { 

            /* index 4 is MODEC */
            if (index != 4) {
                if (index == 5)
                     sprintf (buff, "MODE%d(", index-1);
                else sprintf (buff, "MODE%d(", index);
                str_len = strlen (buff);
                strncpy (ttmp, buff, str_len);
                temp_tlen += str_len;
                ttmp += str_len;
            }
            else {
                sprintf (buff, "MODEC(");
                str_len = strlen (buff);
                strncpy (ttmp, buff, str_len);
                temp_tlen += str_len;
                ttmp += str_len;
            }

            /* every mode except MASTER & MODE4 gets a STATUS */
            if (index != 5) {
                sprintf (buff, "STATUS(");
                str_len = strlen(buff);
                strncpy (ttmp, buff, str_len);
                temp_tlen += str_len;
                ttmp += str_len;
            }

            sprintf (buff, iffmode_array[index].iffmode_status.enum_string);
            str_len = strlen (buff);
            strncpy (ttmp, buff, str_len);
            temp_tlen += str_len;
            ttmp += str_len;

            /* every mode except MASTER & MODE4 gets a STATUS */
            if (index != 5) {
                /* close the STATUS( */
                strncpy (buff, END_PARAM, DELIMITER_LEN);
                strncpy (ttmp, buff, DELIMITER_LEN);
                temp_tlen += DELIMITER_LEN;
                ttmp += DELIMITER_LEN;
            }

            /* every mode except 4 may have a mode value */
            /* added code to produce leading zeroes for iff */ 
            /* MODES 1 to 3           SMP     CT98   E1939  */
            /* every mode except 4 may have a mode value */
            if (index != 5) {
               if (index == 4) {
                 /* print MODE4(altitude) as is */
                 sprintf (buff, "MODE(%d)", iffmode_array[index].iffmode_value);
                 str_len = strlen (buff);
                 strncpy (ttmp, buff, str_len);
                 temp_tlen += str_len;
                 ttmp += str_len;
               }   
               else {
                 /* make sure 4 digits print out for MODES 2 and 3 and 
                 /* 2 digits print out for MODE1 by printing out leading 
                 /* zeroes if necessary  */
                 if (iffmode_array[index].iffmode_value < 10 && 
                     index != 1) {
                   sprintf (buff, "MODE(000%d)", iffmode_array[index].iffmode_value);
                   str_len = strlen (buff);
                   strncpy (ttmp, buff, str_len);
                   temp_tlen += str_len;
                   ttmp += str_len;
                 }
                 else if (iffmode_array[index].iffmode_value < 100 &&
                          iffmode_array[index].iffmode_value >= 10 &&
                          index != 1) {
                    sprintf (buff, "MODE(00%d)", iffmode_array[index].iffmode_value);
                    str_len = strlen (buff);
                    strncpy (ttmp, buff, str_len);
                    temp_tlen += str_len;
                    ttmp += str_len;
                 }
                 else if (iffmode_array[index].iffmode_value < 1000 &&
                          iffmode_array[index].iffmode_value >= 100 &&
                          index != 1) {
                    sprintf (buff, "MODE(0%d)", iffmode_array[index].iffmode_value);
                    str_len = strlen (buff);
                    strncpy (ttmp, buff, str_len);
                    temp_tlen += str_len;
                    ttmp += str_len;
                 }   
                 else {
                   if (index == 1 && iffmode_array[index].iffmode_value < 10) {
                     sprintf (buff, "MODE(0%d)", iffmode_array[index].iffmode_value);
                   }
                   else {
                     sprintf (buff, "MODE(%d)", iffmode_array[index].iffmode_value);
                   }
                   str_len = strlen (buff);
                   strncpy (ttmp, buff, str_len);
                   temp_tlen += str_len;
                   ttmp += str_len;
                 }
               }  /* end if index == 4 */
            }   /*   end if index != 5   */

            /* close the MODEX( */
            strncpy (buff, END_PARAM, DELIMITER_LEN);
            strncpy (ttmp, buff, DELIMITER_LEN);
            temp_tlen += DELIMITER_LEN;
            ttmp += DELIMITER_LEN;
        } /* end if mode_params is TRUEVAL */
    } /* end of loop through index==1 */

    *tlen = temp_tlen;
}
