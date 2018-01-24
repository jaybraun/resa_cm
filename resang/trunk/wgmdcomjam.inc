#    MAXIMUM_FREQUENCIES$$ is computed as the value CJC$Slots * CJC$FQ$slots 
#    (up to CJC$Slots Communications Jammers may be defined in the CJC 
#    table with each capable of operating on up to CJC$FQ$slots frequencies).

    DEFINE ( maximum_frequencies$$, arith(cjc$slots,*,cjc$fq$slots) )

#    MAXIMUM_JCT_ENTRIES$$ is the maximum number of jammer frequencies 
#    allowed during a game.  This number is an arbitrary value which
#    was agreed upon to reduce the otherwise unacceptably large value 
#    of UNT$Slots * CJC$Slots * CJC$FQ$slots (currently 40000).

    DEFINE ( maximum_jct_entries$$, 800 )

#    ADD_STACK$$ contains a stack of UNT_INDEXes which must be added to 
#    JCT prior to communications jammer processing during the current
#    game minute.  A UNT_INDEX is pushed onto ADD_STACK$$ by invoking 
#    the ADD_POTENTIAL_JAMMER subroutine each time that a unit is added 
#    to the UNT table (i.e., launched or cloned).  ADD_POTENTIAL_JAMMER 
#    must be invoked regardless of whether or not the unit is actually
#    carrying an onboard communications jammer.  Later processing of 
#    ADD_STACK$$ is responsible for insuring that a unit is carrying a 
#    jammer prior to adding any entry to the JCT.
#
#    The length of ADD_STACK$$ is defined as UNT$Slots.
#
#    ADD_STACK$$ is defined as COMMON so that programs which invoke 
#    ADD_POTENTIAL_JAMMER can have the actual implementing data 
#    structure hidden from them.  Thus, the only data required to be
#    known by the invoker is the UNT_index which will be added to 
#    ADD_STACK$$.

    DEFINE ( $$add_stack$$,

            INTEGER*2    add_stack$$_top
            INTEGER*2    add_stack$$_UNT_index [ UNT$Slots ]

        COMMON / add_stack$$ /_
            add_stack$$_top,
            add_stack$$_UNT_index

        )

#
#    CT_TYPE defines the contents of the JCT table.  It is defined as
#    a union of two data structures: the first defines the actual 
#    communications jammer computational data; the second defines a
#    single sort field of type INTEGER*2.  As a result, those fields 
#    of the CT_TYPE structure which will be used in any sort operation 
#    must also be declared as type INTEGER*2.  See the SHELL_SORT
#    subroutine for use of these fields.


#
#    DELETE_STACK$$ contains a stack of UNT_INDEXes which must be deleted
#    from JCT prior to communications jammer processing during the current
#    game minute.  A UNT_INDEX is pushed onto DELETE_STACK$$ by invoking 
#    the DELETE_POTENTIAL_JAMMER subroutine each time that a unit is deleted
#    from the UNT table (i.e., bingoed or sunk).  DELETE_POTENTIAL_JAMMER 
#    must be invoked regardless of whether or not the unit is actually
#    carrying an onboard communications jammer.  Later processing of 
#    DELETE_STACK$$ is responsible for insuring that the unit already 
#    exists in JCT prior to deleting its entries from JCT.
#
#    The length of DELETE_STACK$$ is defined as UNT$Slots.
#
#    DELETE_STACK$$ is defined as COMMON so that programs which invoke 
#    DELETE_POTENTIAL_JAMMER can have the actual implementing data 
#    structure hidden from them.  Thus, the only data required to be
#    known by the invoker is the UNT_index which will be added to 
#    DELETE_STACK$$.

    DEFINE ( $$delete_stack$$,

        INTEGER*2    delete_stack$$_top
        INTEGER*2    delete_stack$$_UNT_index [ UNT$Slots ]

        COMMON / delete_stack$$ /_
              delete_stack$$_top,
              delete_stack$$_UNT_index
        )

#
#    The Index to Jammer Frequencies (IJF$$) is a pointer array which, 
#    when ordered by SHELL_SORT, provides access in nondescending order 
#    to frequencies in JCT$$.  

    DEFINE ( $$ijf$$,

        INTEGER*2    ijf$$ ( maximum_jct_entries$$ ) # frequency pointer array
        COMMON / ijf$$ / ijf$$

        )
 
#
#    The Index to Jammer Unit indices (IJU$$) is a pointer array which, 
#    when ordered by SHELL_SORT, provides access in nondescending order 
#    to unit indices in JCT$$.  

    DEFINE ( $$iju$$,

        INTEGER*2    iju$$ ( maximum_jct_entries$$ ) # frequency pointer array
        COMMON / iju$$ / iju$$
 
        )

#
#    The JAMMER_CONTROL contains all the definitions of the 
#    Communication Jammer Model control tables collected into one 
#    INCLUDE file.  Although this file could be included within any
#    Communications Jammer subroutine, it is intended to be included
#    only within the WGMDCOMJAM modules to instantiate the control 
#    structures.  Other subprograms which require one or more of these
#    structures should INCLUDE them individually.

    DEFINE ( $$jammer_control,

        $$JCT$$

        $$JFQ$$

        $$JUN$$

        $$IJF$$

        $$IJU$$

        $$ADD_STACK$$

        $$DELETE_STACK$$

        )
 
#
#    The Jammer Control Table (JCT$$) contains data which controls the 
#    execution of the Communication Jamming Model.  Entries are added to
#    JCT$$ through the invocation of ADD_POTENTIAL_JAMMER; entries are 
#    deleted from JCT$$ through the invocation of DELETE_POTENTIAL_JAMMER.  
#    Both of these subroutines are invoked with the unit index (UNT$Index) 
#    which points to an entry in the UNT Table.  Neither of these routines 
#    actually affect JCT$$ but rather add the supplied unit index to the
#    appropiate stack (ADD_STACK$$ and DELETE_STACK$$, respectively).  
#    Later, during pre-jammer processing, these stacks are emptied, 
#    thereby affecting the contents of JCT$$.
#
#    Prior to accessing data in JCT$$, SHELL_SORT must be invoked to provide
#    the ordered indices (IJF$$ and IJU$$) into JCT$$.  These two pointer 
#    arrays are used to generate the JFQ$$ and JUN$$ values which are searched 
#    (using a binary search) to determine the presence or absence of a jammer 
#    frequency or unit index of interest in JCT$$.
#
#    The CT_TYPE definition is decoupled from the JCT$$ definition to allow
#    the use of an assumed size array in the SHELL_SORT subroutine.

    DEFINE ( $$jct$$,

    INTEGER*4    jct$$_jct_bandwidth[ maximum_jct_entries$$ ]           # jammer bandwidth (Hz)
    REAL*4       jct$$_jct_performance_factor[ maximum_jct_entries$$ ]  # precalculated (dB)
    REAL*4       jct$$_jct_time_delay_factor[ maximum_jct_entries$$ ]   # for follower jammer
    INTEGER*4    jct$$_jct_CJC_pointer[ maximum_jct_entries$$ ]         # to jammer characteristics
    INTEGER*4    jct$$_jct_DYN_pointer[ maximum_jct_entries$$ ]         # dynamics table pointer
    INTEGER*2    jct$$_jct_count               # jammer Count
    INTEGER*2    jct$$_jct_frequency[ maximum_jct_entries$$ ]           # jammer frequency (MHz)
    INTEGER*2    jct$$_jct_UNT_index[ maximum_jct_entries$$ ]           # jamming unit index
    INTEGER*2    jct$$_jct_DYN_EQ_index[ maximum_jct_entries$$ ]        # dynamics equipment index
    INTEGER*2    jct$$_jct_DYN_EQ_jammer_index[ maximum_jct_entries$$ ] # DYN_EQ jammer index
    INTEGER*2    jct$$_jct_range[ maximum_jct_entries$$ ]               # effective range (nm)
    INTEGER*2    jct$$_jct_power[ maximum_jct_entries$$ ]               # jammer power (dB/watt)
    BYTE         jct$$_jct_gain[ maximum_jct_entries$$ ]                # jammer gain (dB)
    BYTE         jct$$_jct_percent_band[ maximum_jct_entries$$ ]        # coverage for tone jammer
    LOGICAL*1    jct$$_jct_tone[ maximum_jct_entries$$ ]                # set if tone jammer
    LOGICAL*1    jct$$_jct_active[ maximum_jct_entries$$ ]              # set each game cycle

        COMMON / jct$$ /_
            jct$$_jct_bandwidth,           # jammer bandwidth (Hz)
            jct$$_jct_performance_factor,  # precalculated (dB)
            jct$$_jct_time_delay_factor,   # for follower jammer
            jct$$_jct_CJC_pointer,         # to jammer characteristics
            jct$$_jct_DYN_pointer,         # dynamics table pointer
            jct$$_jct_count,
            jct$$_jct_frequency,           # jammer frequency (MHz)
            jct$$_jct_UNT_index,           # jamming unit index
            jct$$_jct_DYN_EQ_index,        # dynamics equipment index
            jct$$_jct_DYN_EQ_jammer_index, # DYN_EQ jammer index
            jct$$_jct_range,               # effective range (nm)
            jct$$_jct_power,               # jammer power (dB/watt)
            jct$$_jct_gain,                # jammer gain (dB)
            jct$$_jct_percent_band,        # coverage for tone jammer
            jct$$_jct_tone,                # set if tone jammer
            jct$$_jct_active               # set each game cycle
        ) 
# shift jtc record entry from @2 to @1
define ($Shift_jct,
   {jct$$_jct_bandwidth[@2]          = jct$$_jct_bandwidth[@1]
    jct$$_jct_performance_factor[@2] = jct$$_jct_performance_factor[@1]
    jct$$_jct_time_delay_factor[@2]  = jct$$_jct_time_delay_factor[@1]
    jct$$_jct_CJC_pointer[@2]        = jct$$_jct_CJC_pointer[@1]
    jct$$_jct_DYN_pointer[@2]        = jct$$_jct_DYN_pointer[@1]
    jct$$_jct_frequency[@2]          = jct$$_jct_frequency[@1]
    jct$$_jct_UNT_index[@2]          = jct$$_jct_UNT_index[@1]
    jct$$_jct_DYN_EQ_index[@2]       = jct$$_jct_DYN_EQ_index[@1]
    jct$$_jct_DYN_EQ_jammer_index[@2]= jct$$_jct_DYN_EQ_jammer_index[@1]
    jct$$_jct_range[@2]              = jct$$_jct_range[@1]
    jct$$_jct_power[@2]              = jct$$_jct_power[@1]
    jct$$_jct_gain[@2]               = jct$$_jct_gain[@1]
    jct$$_jct_percent_band[@2]       = jct$$_jct_percent_band[@1]
    jct$$_jct_tone[@2]               = jct$$_jct_tone[@1]
    jct$$_jct_active[@2]             = jct$$_jct_active[@1]}
)

 
#
#    JFQ$$ contains the lower and upper indicies of each unique jammer 
#    FREQUENCY which appears in JCT$$.  JFQ$$ is rebuilt at the beginning 
#    of each game cycle if a unit with an onboard communications jammer 
#    is either added to or deleted from the WARGAME.

    DEFINE ( $$jfq$$,

            INTEGER*2    jfq$$_jfq_count
            INTEGER*2    jfq$$_jfq_frequency[ maximum_frequencies$$ ]
            INTEGER*2    jfq$$_jfq_UNT_index[ maximum_frequencies$$ ]
            INTEGER*2    jfq$$_jfq_lower_index[ maximum_frequencies$$ ]
            INTEGER*2    jfq$$_jfq_upper_index[ maximum_frequencies$$ ]
            equivalence (jfq$$_jfq_frequency, jfq$$_jfq_UNT_index)

        COMMON / jfq$$ /_
            jfq$$_jfq_frequency,
            jfq$$_jfq_lower_index,
            jfq$$_jfq_upper_index,
            jfq$$_jfq_count
        ) 

#
#    JFQ_JUN_TYPE contains the lower and upper indicies of each unique 
#    jammer FREQUENCY or each unique jammer UNT_INDEX appearing in JCT$$.  
#
#    JFQ_JUN_TYPE is defined as a union of three data structures: the first 
#    defines the actual communications jammer frequencies; the second 
#    defines the UNT_index of units which carry a communications jammer;
#    the last is used as a comparisom datum.  The union was chosen to 
#    facilitate a common defintion for JFQ$$ and JUN$$ data structures and 
#    the development of the generalized subroutines RETURN_INDICES and 
#    BUILD_INDICES.
#
#    The JFQ$$ and JUN$$ are rebuilt at the beginning of each game cycle 
#    if a unit with an onboard communications jammer is either added to or 
#    deleted from the WARGAME.

 
#
#    JUN$$ contains the lower and upper indicies of each unique jammer 
#    carrying UNT_INDEX which appears in JCT$$.  JUN$$ is rebuilt at the 
#    beginning of each game cycle if a unit with an onboard communications 
#    jammer is either added to or deleted from the WARGAME.

    DEFINE ( $$jun$$,

            INTEGER*2    jun$$_jun_count

            INTEGER*2    jun$$_jun_frequency[ UNT$Slots ]
            INTEGER*2    jun$$_jun_UNT_index[ UNT$Slots ]

            INTEGER*2    jun$$_jun_lower_index[ UNT$Slots ]
            INTEGER*2    jun$$_jun_upper_index[ UNT$Slots ]
            equivalence (jun$$_jun_frequency, jun$$_jun_UNT_index)

        COMMON / jun$$ /_
            jun$$_jun_frequency,
            jun$$_jun_lower_index,
            jun$$_jun_upper_index,
            jun$$_jun_count
 
        )
