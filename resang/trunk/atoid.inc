#-------------------------------------------------------------------------
# ATOID history table definition (AI)

define (AI$Slots, 20)
define (AI$Pointer, ai_indx)
define (AI$Pointer$First, ai_indx = 1)
define (AI$Pointer$OK, ai_indx <= XAI$Used$i)
define (AI$Pointer$Valid, ai_indx <= AI$Slots)
define (AI$Pointer$Next, ai_indx = ai_indx + 1)

define (XAI$Used$I, ai_used)
define (AI$Used$I, i_ai_used)
define (PUTAI$Used$I,
    ai_used = ifelse(@1,,AW$used$i,@1)
)

define (XAI$Name$C,
    {kk_ai = 0
    call smovh(ai_name[1,ai_indx], ARB, ai$name$c, kk_ai)
    call smov (EOS, ai$name$c, kk_ai)}
)

define (AI$Name$C, c_ai_name)
define (PUTAI$Name$C,
    {kk_ai = 0
    call smovh(ifelse(@1,,AI$name$c,@1), ARB, ai_name[1,ai_indx], kk_ai)
    call smov (EOS, ai_name[1,ai_indx], kk_ai)}
)

define (XAI$UniqueChar$C,
    {kk_ai = 0
    call smovh(ai_UniqueChar[1,ai_indx], ARB, ai$UniqueChar$c, kk_ai)
    call smov (EOS, ai$UniqueChar$c, kk_ai)}
)

define (AI$UniqueChar$C, c_ai_UniqueChar)
define (PUTAI$UniqueChar$C,
    kk_ai = 0
    call smovh(ifelse(@1,,ai$UniqueChar$c,@1), ARB, ai_UniqueChar[1,ai_indx],
               kk_ai)
    call smov (EOS, ai_UniqueChar[1,ai_indx], kk_ai)
)

define (ATOIDCommon,
    character ai_name[80,AI$Slots], c_ai_name[80]
    character ai_UniqueChar[80,AI$Slots], c_ai_UniqueChar[80]
    integer ai_used, kk_ai
    common/AICommon/ai_used, kk_ai, ai_Name, c_ai_name, 
                    ai_UniqueChar, c_ai_UniqueChar
)
