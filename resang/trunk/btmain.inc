define (BBimplicit, IMPLICIT NONE)

define($$tbl,ctbl)
define(tbl$abase,tbl$base)

define($$BBMaxSize,$$maxbbsize)

#define($$$$LastTable,IBB[$$BB$BeginTables-1])

define($$LastName,2048)         # number of variable/field names


#if (ixmax < 1024) imax = 1024
#ixmax = Max$BB$Words

define($$maxforks,16)           # number of forks allowed
define($$maxargs,80)            # number of arguments allowed on one line
define($$maxline,80)            # number of words (char = n*8)
define($$maxtext,16)            # number of quadwords per arg (char = n*8)
define($$SuperArgs,40)
define($$maxlabels,32)          # number of labels to save

define ($$blank,0)
define ($$address,1)
define ($$table,2)
define ($$name,3)
define ($$down,4)
define ($$same,5)
define ($$up,6)
define ($$next,7)
define ($$last,8)
define ($$put,9)
define ($$octal,10)
define ($$ascii,11)
define ($$decimal,12)
define ($$exit,13)
define ($$Goct,14)
define ($$Noct,15)
define ($$Gasc,16)
define ($$Nasc,17)
define ($$Gint,18)
define ($$Nint,19)
define ($$Gflt,20)
define ($$Nflt,21)
define ($$integer,22)
define ($$find,23)
define ($$findf,24)
define ($$findr,25)
define ($$inc,26)
define ($$view,27)
define ($$help,28)
define ($$set,29)
define ($$entry,30)
define ($$position,31)
define ($$zapBB,32)
define ($$fillBB,33)
define ($$repeat,34)
define ($$show,35)
define ($$define,36)
define ($$clear,37)
define ($$compBB,38)
define ($$mcompBB,39)
define ($$tclear,40)
define ($$fork,41)
define ($$resume,42)
define ($$file,43)
define ($$sleep,44)
define ($$arg,45)
define ($$if,46)
define ($$label,47)
define ($$goto,48)
define ($$take,49)
define ($$hush,50)
define ($$Ghex,60)
define ($$Nhex,61)
define ($$degrees,62)
define ($$load,63)
define ($$snapshot,64)
define ($$say,65)
define ($$new7,66)
define ($$new8,67)
define ($$new9,68)
define ($$Gbin,69)
define ($$Nbin,70)

define ($$vars_word,1)  # was 9
define ($$vars_tbl,2)   # was 10
define ($$vars_bits,3)  # was 11
define ($$vars_lbit,4)  # was 12
define ($$vars_char,5)

define (TESTcommon,             ## Define common data

BBcommon

literal         args[$$maxText,$$maxargs],    ## up to N args of 8*Txt bytes each
                Cmndlb,                       ## Goto Label
                Sargn[$$Superargs],           ## Super Arg name
                Sargs[$$MaxText,$$Superargs]  ## Super Arg text

logical hushed
vaxcharacter*32     vars_name[$$LastName]
integer vars[5,$$LastName],     ## Variable Names & bb pointers
        vadds,                  ## ?? I haven't a clue.... DEW 3/6/97
        ctbl,                   ## Current Table id
        ctble,                  ## Current Table entry
        ctblp,                  ## Current Table position
        ctbln,                  ## Current Table name
        cadd,                   ## Absolute BB address
        cpage,                  ## Current Mapped page
        Roct,                   ## generate Octal repesentation
        Rhex,                   ## generate Hexidecimal repesentation
        Rbin,                   ## generate Binary repesentation
        Rint,                   ## generate Integer repesentation
        Rflt,                   ## generate real repesentation
        Rasc,                   ## generate ascii repesentation
        nargs,                  ## number of input arguments
        argv[$$maxargs],        ## arg value (for ascii/int/octal/text)
        argt[$$maxargs],        ## arg type flag (codes defined above)
        mask[32],               ## bit masks for all sizes
        forks[7,$$maxforks],    ## fork table
        Mfile[0:9,0:2],         ## Blackboard file JFNs & names
        Cfile,                  ## Current file index
        Cmnd,                   ## Command file status
        Cmnds[8],               ## Command file name
        Sargt[$$Superargs],     ## Super Arg type
        Seed,
        Sargv[$$Superargs],     ## Super Arg value (for ascii/int/octal/text)
        Sargl[$$Superargs]      ## Super Arg text length (in bytes)

real         argf[$$maxargs]    ## arg value (for floating point)
equivalence (argf,argv)

real         Sargf[$$Superargs] ## Super Arg value (for floating point)
equivalence (Sargf,Sargv)

common /cdata/ vars,vars_name,vadds,ctbl,ctbln,ctble,ctblp,cadd,cpage,
               roct,rhex,rint,rflt,rasc,rbin,
               nargs,argt,args,argv,mask,
               forks,Mfile,Cfile,
               Cmnd,CmndLb,CmndS,seed, 
               Sargt,Sargv,Sargl, hushed,
               Sargn, Sargs 

integer*4 handle                 ## Command Recall handle
common /btmain_recall/ handle

integer         igame
vaxcharacter    bbname*30
common /gdata/  igame,bbname

literal         F_Labels[$$maxlabels],         ## Found Labels
                M_Labels[$$maxlabels]          ## Mark for label
integer*4       C_Labels                      ## Count of Labels
                
common /labeldata/  F_Labels, M_Labels, C_Labels
    )

