# WIMAIN_EQ.INC

define (EqErr$Common,
    literal         cname               # Name of the parent item 
    integer         cflg                # YES if Error Message to be output
    character       cfile[11]           # Name of the file being read
    vmschar*11      v_cfile
    equivalence (cfile, v_cfile)
    common /eqerr/  cname,cflg,cfile
    )
