# These are the 3 common blocks for ALSP I/O 
# they should be in a .RIN file, or in a macro, but.....        DEW 10/99

integer         max_msg, capacity, quota
common /mailbx/ max_msg, capacity, quota
#-------------------- 
integer             actor, confederation
integer             name_len, node_len
VMScharacter        alsp_actor_name*20, alsp_node_name*30

common /alsp_ids/   actor, confederation, 
                    name_len, node_len,
                    alsp_actor_name, alsp_node_name
#-------------------- 
integer         read_unit, write_unit
logical         read_open, write_open,
                msg_log_flush, msg_log_open, msg_log_read, msg_log_write
integer         msg_log_unit
integer			alsp_input_port, alsp_input_socket
integer         alsp_output_port, alsp_output_socket
logical         msg_log_console
  
common /mbx_io/ read_unit, write_unit, read_open, write_open,
                msg_log_flush, msg_log_open, msg_log_read, msg_log_write,
                msg_log_unit, msg_log_console,
				alsp_input_port, alsp_input_socket,
				alsp_output_port, alsp_output_socket
