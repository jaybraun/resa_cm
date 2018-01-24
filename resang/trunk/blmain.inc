define ($unit$tty, -2)
define ($work_size, 7000)
undefine read
undefine write
undefine err

define ($MaxRcd, 2000)

ifnotdef($no_blmain,
##############################################################################
    literal     work[$work_size],out[$work_size],oldtyp,bname
    integer     lfc,reccnt,recsz,recpnt,cmd,wrkopt,oldpos,oldlen,handle
    logical     typed,rtype,anyinp,found

    common /cdat/   work,out,bname,                     # jb (LITERAL) 3/27/00
                    lfc,reccnt,recsz,recpnt,cmd,wrkopt  # jb (INTEGER) 3/27/00
    common /rdat/   oldtyp,                             # jb (LITERAL) 3/27/00
                    oldpos,oldlen,                      # jb (INTEGER) 3/27/00
                    typed,rtype,anyinp,found            # jb (LOGICAL) 3/27/00
    common /rcal/   handle

    VMSchar*ARB     r_line, u_line
    common /rawin/  r_line, u_line

##############################################################################
)

###########################################################################
#            Macros to initialize build characteristic tables

# Object
define (Data$Object_name, DATA (ObjectC [m],m=1,4))
define (Data$Object, Data$object_name / @1, @2, @3, @4 /)

# Com(mon)
define (Data$com_name, DATA defcomC [1,$use(Data_Count)])
define (Data$com_data, DATA defcom [2,$use(Data_Count)])
define (Data$com,
     $incr(Data_Count)
     Data$com_name / @1 / 
     Data$com_data / @2 / 
)

# Typ(e)
define (Data$typ_name, DATA deftypC [$use(Data_Count)])
define (Data$typ_data, DATA deftyp [$use(Data_Count)])
define (Data$typ,
     $incr(Data_Count)
     Data$typ_name / @1 / 
     $incr(Data_Count)
     Data$typ_data / @2 / 
)

# Fld
define (Data$fld_name,  DATA deffldC [$use(Data_Count)])
define (Data$fld_dataN, DATA deffld  [$use(Data_Count)])
define (Data$fld_dataC, DATA deffldC [$use(Data_Count)])
define (Data$fld_desc,  DATA (deffldC [m],m=$use(Data_Count),
                      arith($use(Data_count),+,3)))
define (Data$fld1,
     $incr(Data_Count)
     Data$fld_name / @1 / 
     $incr(Data_Count)
     Data$fld_dataN / @2 / 
     $incr(Data_Count)
     Data$fld_dataN / @3 / 
     $incr(Data_Count)
     Data$fld_dataC / @4 / 
     $incr(Data_Count)
     Data$fld_dataN / @5 / 
     $incr(Data_Count)
     Data$fld_desc / @6, @7, @8, @9 / 
     $incr(Data_Count)
     $incr(Data_Count)
     $incr(Data_Count)
)

define (Data$fld2,
     $incr(Data_Count)
     Data$fld_name / @1 / 
     $incr(Data_Count)
     Data$fld_dataN / @2 / 
     $incr(Data_Count)
     Data$fld_dataN / @3 / 
     $incr(Data_Count)
     Data$fld_dataC / @4 / 
     $incr(Data_Count)
     Data$fld_dataN / @5 / 
     $incr(Data_Count)
     Data$fld_dataC / @6 / 
     $incr(Data_Count)
     Data$fld_dataN / @7 / 
     $incr(Data_Count)
     Data$fld_desc / @8, @9, @10, @11 / 
     $incr(Data_Count)
     $incr(Data_Count)
     $incr(Data_Count)
)

define (Data$fld3,
     $incr(Data_Count)
     Data$fld_name / @1 / 
     $incr(Data_Count)
     Data$fld_dataN / @2 / 
     $incr(Data_Count)
     Data$fld_dataN / @3 / 
     $incr(Data_Count)
     Data$fld_dataC / @4 / 
     $incr(Data_Count)
     Data$fld_dataN / @5 / 
     $incr(Data_Count)
     Data$fld_dataC / @6 / 
     $incr(Data_Count)
     Data$fld_dataN / @7 / 
     $incr(Data_Count)
     Data$fld_dataC / @8 / 
     $incr(Data_Count)
     Data$fld_dataN / @9 / 
     $incr(Data_Count)
     Data$fld_desc / @10, @11, @12, @13 / 
     $incr(Data_Count)
     $incr(Data_Count)
     $incr(Data_Count)
)
define (Data$fld4,
     $incr(Data_Count)
     Data$fld_name / @1 / 
     $incr(Data_Count)
     Data$fld_dataN / @2 / 
     $incr(Data_Count)
     Data$fld_dataN / @3 / 
     $incr(Data_Count)
     Data$fld_dataC / @4 / 
     $incr(Data_Count)
     Data$fld_dataN / @5 / 
     $incr(Data_Count)
     Data$fld_dataC / @6 / 
     $incr(Data_Count)
     Data$fld_dataN / @7 / 
     $incr(Data_Count)
     Data$fld_dataC / @8 / 
     $incr(Data_Count)
     Data$fld_dataN / @9 / 
     $incr(Data_Count)
     Data$fld_dataC / @10 / 
     $incr(Data_Count)
     Data$fld_dataN / @11 / 
     $incr(Data_Count)
     Data$fld_desc / @12, @13, @14, @15 / 
     $incr(Data_Count)
     $incr(Data_Count)
     $incr(Data_Count)
)

define (Data$fld7,
     $incr(Data_Count)
     Data$fld_name / @1 / 
     $incr(Data_Count)
     Data$fld_dataN / @2 / 
     $incr(Data_Count)
     Data$fld_dataN / @3 / 
     $incr(Data_Count)
     Data$fld_dataC / @4 / 
     $incr(Data_Count)
     Data$fld_dataN / @5 / 
     $incr(Data_Count)
     Data$fld_dataC / @6 / 
     $incr(Data_Count)
     Data$fld_dataN / @7 / 
     $incr(Data_Count)
     Data$fld_dataC / @8 / 
     $incr(Data_Count)
     Data$fld_dataN / @9 / 
     $incr(Data_Count)
     Data$fld_dataC / @10 / 
     $incr(Data_Count)
     Data$fld_dataN / @11 / 
     $incr(Data_Count)
     Data$fld_dataC / @12 / 
     $incr(Data_Count)
     Data$fld_dataN / @13 / 
     $incr(Data_Count)
     Data$fld_dataC / @14 / 
     $incr(Data_Count)
     Data$fld_dataN / @15 / 
     $incr(Data_Count)
     Data$fld_dataC / @16 / 
     $incr(Data_Count)
     Data$fld_dataN / @17 / 
     $incr(Data_Count)
     Data$fld_desc / @18, @19, @20, @21 / 
     $incr(Data_Count)
     $incr(Data_Count)
     $incr(Data_Count)
)

define (Data$fld10,
     $incr(Data_Count)
     Data$fld_name / @1 / 
     $incr(Data_Count)
     Data$fld_dataN / @2 / 
     $incr(Data_Count)
     Data$fld_dataN / @3 / 
     $incr(Data_Count)
     Data$fld_dataC / @4 / 
     $incr(Data_Count)
     Data$fld_dataN / @5 / 
     $incr(Data_Count)
     Data$fld_dataC / @6 / 
     $incr(Data_Count)
     Data$fld_dataN / @7 / 
     $incr(Data_Count)
     Data$fld_dataC / @8 / 
     $incr(Data_Count)
     Data$fld_dataN / @9 / 
     $incr(Data_Count)
     Data$fld_dataC / @10 / 
     $incr(Data_Count)
     Data$fld_dataN / @11 / 
     $incr(Data_Count)
     Data$fld_dataC / @12 / 
     $incr(Data_Count)
     Data$fld_dataN / @13 / 
     $incr(Data_Count)
     Data$fld_dataC / @14 / 
     $incr(Data_Count)
     Data$fld_dataN / @15 / 
     $incr(Data_Count)
     Data$fld_dataC / @16 / 
     $incr(Data_Count)
     Data$fld_dataN / @17 / 
     $incr(Data_Count)
     Data$fld_dataC / @18 / 
     $incr(Data_Count)
     Data$fld_dataN / @19 / 
     $incr(Data_Count)
     Data$fld_dataC / @20 / 
     $incr(Data_Count)
     Data$fld_dataN / @21 / 
     $incr(Data_Count)
     Data$fld_dataC / @22 / 
     $incr(Data_Count)
     Data$fld_dataN / @23 / 
     $incr(Data_Count)
     Data$fld_desc / @24, @25, @26, @27 / 
     $incr(Data_Count)
     $incr(Data_Count)
     $incr(Data_Count)
)


# Arg(ument)

# 4 arguments

define (Data$arg4N_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg4N_data, DATA (defarg [m,$use(Data_Count)],m=2,4))
define (Data$arg4N,
     $incr(Data_Count)
     Data$arg4N_name / @1 / 
     Data$arg4n_data / @2,
                       ifelse(@3,,0,@3), 
                       ifelse(@4,,0,@4)/ 
)

define (Data$arg4C_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg4C_data2, DATA defarg [2,$use(Data_Count)])
define (Data$arg4C_data, DATA (defargC [m,$use(Data_Count)],m=3,4))
define (Data$arg4C,
     $incr(Data_Count)
     Data$arg4C_name / @1 / 
     Data$arg4C_data2 / @2/
     Data$arg4C_data / @3,
                       ifelse(@4,,%""%,@4)/ 
)

# 5 arguments

define (Data$arg5N_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg5N_data, DATA (defarg [m,$use(Data_Count)],m=2,5))
define (Data$arg5N,
     $incr(Data_Count)
     Data$arg5N_name / @1 / 
     Data$arg5n_data / @2,
                       ifelse(@3,,0,@3), 
                       ifelse(@4,,0,@4), 
                       ifelse(@5,,0,@5)/ 
)

define (Data$arg5C_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg5C_data2, DATA defarg [2,$use(Data_Count)])
define (Data$arg5C_data, DATA (defargC [m,$use(Data_Count)],m=3,5))
define (Data$arg5C,
     $incr(Data_Count)
     Data$arg5C_name / @1 / 
     Data$arg5C_data2 / @2/
     Data$arg5C_data / @3,
                       ifelse(@4,,%""%,@4), 
                       ifelse(@5,,%""%,@5)/ 
)

# 6 arguments

define (Data$arg6N_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg6N_data, DATA (defarg [m,$use(Data_Count)],m=2,6))
define (Data$arg6N,
     $incr(Data_Count)
     Data$arg6N_name / @1 / 
     Data$arg6n_data / @2,
                       ifelse(@3,,0,@3), 
                       ifelse(@4,,0,@4), 
                       ifelse(@5,,0,@5), 
                       ifelse(@6,,0,@6)/ 
)

define (Data$arg6C_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg6C_data2, DATA defarg [2,$use(Data_Count)])
define (Data$arg6C_data, DATA (defargC [m,$use(Data_Count)],m=3,6))
define (Data$arg6C,
     $incr(Data_Count)
     Data$arg6C_name / @1 / 
     Data$arg6C_data2 / @2/
     Data$arg6C_data / @3,
                       ifelse(@4,,%""%,@4), 
                       ifelse(@5,,%""%,@5), 
                       ifelse(@6,,%""%,@6)/ 
)

# 8 arguments

define (Data$arg8N_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg8N_data, DATA (defarg [m,$use(Data_Count)],m=2,8))
define (Data$arg8N,
     $incr(Data_Count)
     Data$arg8N_name / @1 / 
     Data$arg8n_data / @2,
                       ifelse(@3,,0,@3), 
                       ifelse(@4,,0,@4), 
                       ifelse(@5,,0,@5), 
                       ifelse(@6,,0,@6), 
                       ifelse(@7,,0,@7), 
                       ifelse(@8,,0,@8)/ 
)

define (Data$arg8C_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg8C_data2, DATA defarg [2,$use(Data_Count)])
define (Data$arg8C_data, DATA (defargC [m,$use(Data_Count)],m=3,8))
define (Data$arg8C,
     $incr(Data_Count)
     Data$arg8C_name / @1 / 
     Data$arg8C_data2 / @2/
     Data$arg8C_data / @3,
                       ifelse(@4,,%""%,@4), 
                       ifelse(@5,,%""%,@5), 
                       ifelse(@6,,%""%,@6), 
                       ifelse(@7,,%""%,@7), 
                       ifelse(@8,,%""%,@8)/ 
)

# 10 arguments

define (Data$arg10N_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg10N_data, DATA (defarg [m,$use(Data_Count)],m=2,10))
define (Data$arg10N,
     $incr(Data_Count)
     Data$arg10N_name / @1 / 
     Data$arg10n_data / @2,
                       ifelse(@3,,0,@3), 
                       ifelse(@4,,0,@4), 
                       ifelse(@5,,0,@5), 
                       ifelse(@6,,0,@6), 
                       ifelse(@7,,0,@7), 
                       ifelse(@8,,0,@8), 
                       ifelse(@9,,0,@9), 
                       ifelse(@10,,0,@10)/ 
)

define (Data$arg10C_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg10C_data2, DATA defarg [2,$use(Data_Count)])
define (Data$arg10C_data, DATA (defargC [m,$use(Data_Count)],m=3,10))
define (Data$arg10C,
     $incr(Data_Count)
     Data$arg10C_name / @1 / 
     Data$arg10C_data2 / @2/
     Data$arg10C_data /ifelse(@3,,%""%,@3),
                       ifelse(@4,,%""%,@4), 
                       ifelse(@5,,%""%,@5), 
                       ifelse(@6,,%""%,@6), 
                       ifelse(@7,,%""%,@7), 
                       ifelse(@8,,%""%,@8), 
                       ifelse(@9,,%""%,@9), 
                       ifelse(@10,,%""%,@10)/ 
)

# 12 arguments

define (Data$arg12N_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg12N_data, DATA (defarg [m,$use(Data_Count)],m=2,12))
define (Data$arg12N,
     $incr(Data_Count)
     Data$arg12N_name / @1 / 
     Data$arg12n_data / @2,
                       ifelse(@3,,0,@3), 
                       ifelse(@4,,0,@4), 
                       ifelse(@5,,0,@5), 
                       ifelse(@6,,0,@6), 
                       ifelse(@7,,0,@7), 
                       ifelse(@8,,0,@8), 
                       ifelse(@9,,0,@9), 
                       ifelse(@10,,0,@10), 
                       ifelse(@11,,0,@11), 
                       ifelse(@12,,0,@12)/ 
)

define (Data$arg12C_name, DATA defargC [1,$use(Data_Count)])
define (Data$arg12C_data2, DATA defarg [2,$use(Data_Count)])
define (Data$arg12C_data, DATA (defargC [m,$use(Data_Count)],m=3,12))
define (Data$arg12C,
     $incr(Data_Count)
     Data$arg12C_name / @1 / 
     Data$arg12C_data2 / @2/
     Data$arg12C_data /ifelse(@3,,%""%,@3),
                       ifelse(@4,,%""%,@4), 
                       ifelse(@5,,%""%,@5), 
                       ifelse(@6,,%""%,@6), 
                       ifelse(@7,,%""%,@7), 
                       ifelse(@8,,%""%,@8), 
                       ifelse(@9,,%""%,@9), 
                       ifelse(@10,,%""%,@10), 
                       ifelse(@11,,%""%,@11), 
                       ifelse(@12,,%""%,@12)/ 
)

# Fmt
define (Data$fmt_name, DATA (deffmtC [m,$use(Data_Count)],m=2,5))
define (Data$fmt_data, DATA deffmt [1,$use(Data_Count)])
define (Data$fmt,
     $incr(Data_Count)
     Data$fmt_data / @1 / 
     Data$fmt_name / @2, @3, @4, @5 / 
)

