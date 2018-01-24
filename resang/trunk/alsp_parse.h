

/* Parse an incoming ALSP message into a Message struct
 */
extern char *alsp_parse ();

/* Return the data type for a parameter value based on the
 * ALSP message type, message subtype, and parameter type 
 */
extern PARAM_VALUE_TYPE get_alsp_param_data_type ();
