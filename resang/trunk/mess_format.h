

#define MAX_MESSAGE_LEN  20000    /* msg len increased from 2700 to 20000 - SMM 10/96 (E1693) */

extern void gen_alsp_message ();
  /* gen_alsp_message (mess, len, message); 
   *   mess:  in parameter, Address of MESSAGE_STRUCT   
   *   len:   out parameter, Address for length in bytes of message data
   *   alsp_message:  out parameter, Address of binary_message_data
   */
		 
