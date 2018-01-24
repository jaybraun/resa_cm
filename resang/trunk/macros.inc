#
# $$EXTRACT expects 3 arguments
#   1)  32-Bit Word to operate on
#   2)  Bit Position (0..31) 
#   3)  Bit Mask of whatever width
#
# $$EXTRACT_X   does the same thing with a sign extension
# $$EXTRACT_S   will span word boundaries (expensive)
# $$EXTRACT_S_X spans word boundaries with a sign extension (expensive)
#
#
# $$INSERT expects 4 arguments
#   1)  Value to be inserted
#   2)  32-Bit Word to operate on
#   3)  Bit Position (0..31) 
#   4)  Bit Mask of whatever width
#

define(istrip   ,iand(ishft(@1,-(@2)),@3)) # This is the best
define($$extract,iand(ishft(@1,-(@2)),@3)) # This is the best

define($$extract_x,ior($$sign_X(@1,@2,@3),$$extract(@1,@2,@3)))#w/sign extend
define($$sign_x,iand($$sign_m(@1,@2,@3)!=0,not(@3)))
define($$sign_m,iand($$extract(@1,@2,@3),not(ishft(@3,-1))))

define($$Extract_s  ,iextract_s(@1,@2,@3,NO)) 	# Span word boundaries
define($$Extract_s_x,iextract_s(@1,@2,@3,YES)) 	# Span word (w/sign extension)

define(call_idress,@2=ior(iand(@2,%NOT%(ishft(@4,@3))),ishft(iand(@1,@4),@3)))
define($$Insert   ,@2=ior(iand(@2,%NOT%(ishft(@4,@3))),ishft(iand(@1,@4),@3)))
define($$Insert_s ,call insert_s(@1,@2,@3,@4))	# Span word boundaries

#############################################################################
##
##  These versions of $$Extract & $$Insert are sensitive to a LBIT value
##  of zero.  When zero, the ISHFT function is not included.
##
##  However... The compiler does the same thing.  So why bother.
##
## define($$extract,iand($$ishft2(@1,-(@2)),@3)) # This is the best
## define($$ishft2,ifelse(@2,-0,@1,ishft(@1,@2)))
##
## define($$Insert   ,@2=ior(iand(@2,%NOT%($$ishft(@4,@3))),$$ishft(iand(@1,@4),@3)))
## define($$ishft,ifelse(@2,0,@1,ishft(@1,@2)))
##
#############################################################################

# define($$ExtLit,cstrip(@1,@2,@3))             # previous version
# define($$InsLit,call cdress(%%ref(@1),@2,@3,@4))
define($$ExtLit,ifelse(@2@3,18,cstrip8(@1),cstrip(@1,@2,@3)))
define($$InsLit,ifelse(@3@4,18,call cdress8(%%ref(@1),@2),call cdress(%%ref(@1),@2,@3,@4)))

define($$MaxLit,8)      # this is the size of a Literal (in bytes)
## define($$MaxName,5) # this is the number of bytes allowed in a name
define($$MaxName,8) # jb 2141/ Allow 8 bytes in a name 12/9/99
define($$MaxVName,24) # Max characters in V type BB string

#############################################################################
#
# The following macros used by DDL to support FLAG_CHANGES capability
# The concept is to go one step beyond FLAG_UPDATES.  
# i.e. Don't set the modified field unless the new value is different
# This code is compatible with Alpha OpenVMS.
#                           (these macros and DDL mods by D Wills, July-1999)
#
# An example of DDL generated code follows (from [RESA64]):
#
# ex1)  define(putUNT$Status$I,{ $$SnapShot(IBB,UNT$Pointer+5,1,iSnap_Before)
# ex2)      $$insert((ifelse(@1,,UNT$Status$I,@1)),IBB[UNT$Pointer+5],16,M5bits)
# ex3)      $$SnapShot(IBB,UNT$Pointer+5,1,iSnap_After)
# ex4)      $$SnapDiff(iSnap_Before,iSnap_After,1)UNT$FlagField(2) })
#
#----
# $$SNAPSHOT expects 4 arguments
#   1)  Blackboard to look at (IBB)
#   2)  32-Bit Word to operate on (UNT$Pointer+n)
#   3)  Number of 32-bit Words effected (1 or 2 or ?)
#   4)  Local Variable to save in (32-bit integer array)
#
#   Typically used as: $$SNAPSHOT (IBB, UNT$Pointer+5, 1, iSnap_Before)
#                  or: $$SNAPSHOT (IBB, UNT$Pointer+5, 1, iSnap_After)
#       (See ex1 and ex3 above)
#
#----
# $$SNAPDIFF expects 3 arguments 
#   1)  First Local Variable saved (32-bit integer array)
#   2)  Second Local Variable saved (32-bit integer array)
#   3)  Number of 32-bit Words effected (1 or 2 or ?)
#
#   Typically used as: 
#       $$SNAPDIFF (iSnap_Before, iSnap_After, 1) do_this
#       (See ex4 above)
#
#----
# SnapDeclare requires no arguments, but can take 2
#
# This macro will generate the required declaration statements for 
# the $$SNAP macros to work in an IMPLICIT NONE situation.
#
# If any of the following variables are identified in a compile:
#     iSnap_d, iSnap_Before or iSnap_After
#   Include "SnapDeclare" in the routine
#
# It will not cause a problem if not needed, but included anyway.
#############################################################################

define($$MaxSnap,2)     # max number of words that can be snap-shot'd

define($$SS1,$concat(@4,_1) = @1[@2+0];ifelse(@3,1,,$$SS2(@1,@2,@3,@4)))
define($$SS2,$concat(@4,_2) = @1[@2+1];ifelse(@3,2,,$$SS3(@1,@2,@3,@4)))
define($$SS3,$concat(@4,_3) = @1[@2+2];ifelse(@3,3,,$$SS4(@1,@2,@3,@4)))
define($$SS4,$concat(@4,_4) = @1[@2+3];)

# Macros for 4 words are defined, but 2 is the typical max (a literal)  DEW 7/99
# Attempts to match more than 4 words will not error, but they
# will not happen, until additional lines are defined here

define($$SnapShot,{$$SS1(@1,@2,@3,@4)})

#---

define($$SD1,%      if (%$concat(@1,_1) != $concat(@2,_1)%)% iSnap_D = 1;ifelse(@3,1,,$$SD2(@1,@2,@3)))
define($$SD2,%      if (%$concat(@1,_2) != $concat(@2,_2)%)% iSnap_D = 1;ifelse(@3,2,,$$SD3(@1,@2,@3)))
define($$SD3,%      if (%$concat(@1,_3) != $concat(@2,_3)%)% iSnap_D = 1;ifelse(@3,3,,$$SD4(@1,@2,@3)))
define($$SD4,%      if (%$concat(@1,_4) != $concat(@2,_4)%)% iSnap_D = 1;)

define($$SnapDiff,
    iSnap_d = 0
    $$SD1(@1,@2,@3)
    if (iSnap_d != 0)
    )

#---

define($$SCc1,%,% $concat(@1,_1) ifelse(@2,1,,$$SCc2(@1,@2)))
define($$SCc2,%,% $concat(@1,_2) ifelse(@2,2,,$$SCc3(@1,@2)))
define($$SCc3,%,% $concat(@1,_3) ifelse(@2,3,,$$SCc4(@1,@2)))
define($$SCc4,%,% $concat(@1,_4) )

define(SnapDeclare,
    Integer iSnap_d _
                    $$SCc1(ifelse(@1,,iSnap_Before,@1),$$MaxSnap) _
                    $$SCc1(ifelse(@2,,iSnap_After,@2),$$MaxSnap)
    )

define(SnapCommon,
    Common / @1 / iSnap_d _
                    $$SCc1(ifelse(@2,,iSnap_Before,@2),$$MaxSnap) _
                    $$SCc1(ifelse(@3,,iSnap_After,@3),$$MaxSnap)
    )
                                
#############################################################################

define($system_size, arith(arith($$MaxName,*,2),+,1) )  # platform/system
define($platform_size, arith($$MaxName,*,1))
define($device_size, arith($$MaxName,*,1))

define($$ExtVAXc,vstrip(@1,@2,@3))
define($$InsVAXc,call vdress(@1,@2,@3,@4))
define($$MaxVAXc,250)

define(call_jdress,@2=iand(ior(@2,ishft(@4,@3)),ishft(iand(@1,@4),@3)))

define ($clearbit,call_idress(0,@1,@2,m1bits))
define ($setbit,call_idress(1,@1,@2,m1bits))
define ($modbit,call_idress(@3,@1,@2,m1bits))
define ($bitset,istrip(@1,@2,m1bits))

define ($isnumeric, (@1 == Aint(@1) & isnumber(@1) == YES))
define ($iswhole  , (@1 == Aint(@1) ) )

define ($alphabet ,abcdefghijklmnopqrstuvwxyz)

define ($Control  ,arith($alphabet,?,@1))
define ($UpperCase,arith(arith($alphabet,?,@1),+,64))
define ($LowerCase,arith(arith($alphabet,?,@1),+,96))

define ($nnl,arith(@1,\,@2))                ## Remove Newlines....
define ($concat,arith(@1,:,@2))             ## Remove embedded spaces/tabs...
define ($modulo,arith(@1,-,arith(arith(@1,/,@2),*,@2)))

define ($$hexmask,137FFFFFFFFFFFFFFFFF)

define ($gen_FFF,substr($$hexmask,4,arith(@1,/,4)))
define ($gen_1hex,substr($$hexmask,$modulo(@1,4),1))
define ($gen_hex,$concat($gen_1hex(@1),$gen_FFF(@1)))

define ($bitmask,ifelse(arith(@1,+,0),0,'0'X,%'%$gen_hex(@1)%'X%))

define m1bits  '0001'x
define m2bits  '0003'x
define m3bits  '0007'x
define m4bits  '000F'x
define m5bits  '001F'x
define m6bits  '003F'x
define m7bits  '007F'x
define m8bits  '00FF'x
define m9bits  '01FF'x

define m01bits '0001'x
define m02bits '0003'x
define m03bits '0007'x
define m04bits '000F'x
define m05bits '001F'x
define m06bits '003F'x
define m07bits '007F'x
define m08bits '00FF'x
define m09bits '01FF'x
define m10bits '03FF'x
define m11bits '07FF'x
define m12bits '0FFF'x
define m13bits '1FFF'x
define m14bits '3FFF'x 
define m15bits '7FFF'x 
define m16bits 'FFFF'x  

define m17bits '01FFFF'x
define m18bits '03FFFF'x
define m19bits '07FFFF'x
define m20bits '0FFFFF'x
define m21bits '1FFFFF'x
define m22bits '3FFFFF'x
define m23bits '7FFFFF'x
define m24bits 'FFFFFF'x

define m25bits '01FFFFFF'x
define m26bits '03FFFFFF'x
define m27bits '07FFFFFF'x
define m28bits '0FFFFFFF'x
define m29bits '1FFFFFFF'x
define m30bits '3FFFFFFF'x
define m31bits '7FFFFFFF'x
define m32bits 'FFFFFFFF'x

define $bit0  '00000001'X
define $bit1  '00000002'X
define $bit2  '00000004'X
define $bit3  '00000008'X
define $bit4  '00000010'X
define $bit5  '00000020'X
define $bit6  '00000040'X
define $bit7  '00000080'X
define $bit8  '00000100'X
define $bit9  '00000200'X

define $bit00 '00000001'X
define $bit01 '00000002'X
define $bit02 '00000004'X
define $bit03 '00000008'X
define $bit04 '00000010'X
define $bit05 '00000020'X
define $bit06 '00000040'X
define $bit07 '00000080'X
define $bit08 '00000100'X
define $bit09 '00000200'X
define $bit10 '00000400'X
define $bit11 '00000800'X
define $bit12 '00001000'X
define $bit13 '00002000'X
define $bit14 '00004000'X
define $bit15 '00008000'X
define $bit16 '00010000'X
define $bit17 '00020000'X
define $bit18 '00040000'X
define $bit19 '00080000'X
define $bit20 '00100000'X
define $bit21 '00200000'X
define $bit22 '00400000'X
define $bit23 '00800000'X
define $bit24 '01000000'X
define $bit25 '02000000'X
define $bit26 '04000000'X
define $bit27 '08000000'X
define $bit28 '10000000'X
define $bit29 '20000000'X
define $bit30 '40000000'X
define $bit31 '80000000'X

#------------------------------------------------------------
# This group of macros define short cut code to do the following:
#
#     FOR (OWN$POINTER$FIRST ; OWN$POINTER$OK ; OWN$POINTER$NEXT)
# becomes:
#     FOR_ALL_OK(own)
#
#     FOR (UNT$POINTER$FIRST ; UNT$POINTER$VALID ; UNT$POINTER$NEXT)
# becomes:
#     FOR_ALL_VALID(unt)
#
#     UNT$POINTER$OK   and   UNT$POINTER$VALID
# can be replaced with:
#     STILL_OK(UNT)    and   STILL_VALID(UNT)
#
# 
# Special Note:
#    These macros will work on all current table that have 3 character 
#    mnemonic names.  If the table name is 2 characters, or more than 3,
#    then the code generated will not work.  This is because of the 
#    requirement to remove the numeric suffix for UNT2, UNT3, DUP2, etc.
#
#

define(Still_ok,$concat(@1,$pointer) <= $concat(substr(@1,1,3),$Last) )
define(Still_valid,$concat(@1,$pointer)<($concat(substr(@1,1,3),$Base)+$concat(substr(@1,1,3),$Size)))

define (for_all_OK,for ($concat(@1,$pointer$first) ; 
                        still_ok(@1) ;
                        $concat(@1,$pointer$next))
    )

define (for_all_VALID,for ($concat(@1,$pointer$first) ;
                           still_valid(@1) ;
                           $concat(@1,$pointer$next))
    )

#----------------------------------------------------------------------
# The following Macros help generate the declaration section for BB
# fields.  For example:
#
# RAT Source                    Comparable Source
# ==========                    =================
# bbdeclare (UNT$LATITUDE$F)    REAL*4          UNT$LATITUDE$F
# bbdeclare (UNT$STATUS$I)      INTEGER*4       UNT$STATUS$I
# bbdeclare (UNT$NAME$C)        LITERAL         UNT$NAME$C
# bbdeclare (UNT$NAME$V)        VAXCHARACTER    UNT$NAME$V * $SIZE_UNT$NAME$V
#
# Note:
#   Non-BB fields will be defined as Integer type
#   Up to 18 fields may be specified at once,
#       any beyond the 18th will cause a compile error.....
#
# D. Wills 14-Sep-1994
#----------------------------------------------------------------------

define (Inst_I,Integer*4 % %$nnl(@1)
        )
define (Inst_F,Real*4 %    %$nnl(@1)
        )
define (Inst_C,Literal %    %$nnl(@1)
        )
define (Inst_V,vaxcharacter % %$nnl(@1)*$concat($size_,$nnl(@1)) 
        )

define (declare_i,ifelse(substr($concat($nnl(@1)),1,2),I_,Inst_I(@1),@2))
define (declare_f,ifelse(substr($concat($nnl(@1)),1,2),F_,Inst_F(@1),@2))
define (declare_c,ifelse(substr($concat($nnl(@1)),1,2),C_,Inst_C(@1),@2))
define (declare_v,ifelse(substr($concat($nnl(@1)),1,2),V_,Inst_V(@1),@2))

Define (Declare1,Declare_I(@1,Declare_F(@1,Declare_C(@1,Declare_V(@1,Inst_I(@1))))))
Define (Declare3,Ifelse(@1,,,Declare1(@1)) Ifelse(@2,,,Declare1(@2)) Ifelse(@3,,,Declare1(@3)) )
Define (Declare6,Ifelse(@1@2@3,,,Declare3(@1,@2,@3)) Ifelse(@4@5@6,,,Declare3(@4,@5,@6)) )

Define (BBDeclare,Ifelse(@1@2@3@4@5@6,,,Declare6(@1,@2,@3,@4,@5,@6)) 
                  Ifelse(@7@8@9@10@11@12,,,Declare6(@7,@8,@9,@10,@11,@12)) 
                  Ifelse(@13@14@15@16@17@18,,,Declare6(@13,@14,@15,@16,@17,@18))
                  Ifelse(@19,,,$too_many_arguments_to_BBdeclare ) )

#----------------------------------------------------------------------

#----------------------------------------------------------------------
# This macro is a logical function that returns true if the first 
# argument is equal to the 2nd, 3rd, 4th, or Nth argument.
#
#   Sample RAT Source
#   =================
#
#   unt$type$i = Xunt$type$i
#   if (code$matches (unt$type$i, $Air$Code, $Sub$Code, $Surface$Code) )
#       {
#       # Matched...
#       }
#
#   Comparable Source
#   =================
#
#   unt$type$i = Xunt$type$i
#   if (unt$type$i == $Air$Code |
#       unt$type$i == $Sub$Code |
#       unt$type$i == $Surface$Code) 
#       {
#       # Matched...
#       }
#
#
# Note:
#   Up to 15 codes may be specified at once,
#       any beyond the 15th are ignored, dropped, lost.....
#
# D. Wills 17-Sep-1996
#----------------------------------------------------------------------

define(Code$Matches,(@1 == @2 |
                     @1 == @3 |
                     ifelse(@4,,,@1 == @4 |)
                     ifelse(@5,,,@1 == @5 |)
                     ifelse(@6,,,@1 == @6 |)
                     ifelse(@7,,,@1 == @7 |)
                     ifelse(@8,,,@1 == @8 |)
                     ifelse(@9,,,@1 == @9 |)
                     ifelse(@10,,,@1 == @10 |)
                     ifelse(@11,,,@1 == @11 |)
                     ifelse(@12,,,@1 == @12 |)
                     ifelse(@13,,,@1 == @13 |)
                     ifelse(@14,,,@1 == @14 |)
                     ifelse(@15,,,@1 == @15 |)
                     ifelse(@16,,,@1 == @16 |)
                     ifelse(@17,,,$too_many_arguments |)
                     1 == 2) )

#----------------------------------------------------------------------

#------------------------------------------------------------
# For 20 years, the first record of BUILD and FORCE files has only 
# had 1, 32-bit integer value.  That value identified the number of 
# records in use for data.
#
# Now, we are going to output a little bit more.. 3 values:
# a)  Reccnt (as a 32-bit integer)
# b)  Reccnt (as a 32-bit real) in F_Float format
# c)  Reccnt (as a 64-bit real) in D_Float [VAX] or G_Float [AXP]
#
# Sample usage:
#   write (31,REC=1) $Count_W_fingerprint(reccnt) 
#
#                                                       DEW 8/18/2000

define ($Count_W_fingerprint,@1, real(@1), dfloat(@1) )

#------------------------------------------------------------

# Assigning a string to a literal is not portable to g77 or f90 Linux compilers
# Hollerith strings work on VMS and Linux Fortran compilers
# This macro, hstr, converts a string without quotes to a hollerith string.
# For example  "abcdefgh" becomes $hstr(abcdefgh)
# For an unknown reason, the rat4 preprocessor will not accept quotes in argument.
  
define ($hstr,
             ifelse(substr(@1,1,1),,0,
             ifelse(substr(@1,2,1),,arith(1,:,arith(H,:,@1)),
             ifelse(substr(@1,3,1),,arith(2,:,arith(H,:,@1)),
             ifelse(substr(@1,4,1),,arith(3,:,arith(H,:,@1)),
             ifelse(substr(@1,5,1),,arith(4,:,arith(H,:,@1)),
             ifelse(substr(@1,6,1),,arith(5,:,arith(H,:,@1)),
             ifelse(substr(@1,7,1),,arith(6,:,arith(H,:,@1)),
             ifelse(substr(@1,8,1),,arith(7,:,arith(H,:,@1)),
             ifelse(substr(@1,9,1),,arith(8,:,arith(H,:,@1)),badstr
             )))))))))
)

# Macros for data statments with string constants
# Fortran 8 character string and literal are equivalenced

define($cat,arith(@1,:,@2))                     # concatenate (removes spaces)
define($def,$cat(d e f i n e ,(@1 , @2)))       # deferred DEFINE
define($set,$def($$$$ @1,@2))                   # SET a dynamic value
define($use,$cat($$$$ @1))                      # USE a dynamic value
define($incr,$def($$$$ @1, incr($$$$@1)))       # Increment a dynamic value

# IntCharEquiv(integer_array_name[| [dimension_1[|, dimension_2]])
define(IntCharEquiv,
       Integer ifelse(@1,,,ifelse(@2,,@1,ifelse(@3,,@1[@2],@1[@2,@3])))
       vmschar*4 ifelse(@1,,,ifelse(@2,,@1C,ifelse(@3,,@1C[@2],@1C[@2,@3])))
       equivalence  (@1,@1C)
      )
# LitCharEquiv(literal_array_name[| [dimension_1[|, dimension_2]])
define(LitCharEquiv,
       literal ifelse(@1,,,ifelse(@2,,@1,ifelse(@3,,@1[@2],@1[@2,@3])))
       vmschar*8 ifelse(@1,,,ifelse(@2,,@1C,ifelse(@3,,@1C[@2],@1C[@2,@3])))
       equivalence  (@1,@1C)
      )
