# this file implements a number of the FORTRAN compiler directives for use 
# paralleling RESA.  Each should be the first thing on a line. But it is not
# neccessary to be in column number 1.

define ($PAR$Context_Shared,
%CPAR$ % Context_Shared )

define ($PAR$Context_Shared_All,
%CPAR$ % Context_Shared_All )

define ($PAR$Private,
%CPAR$ % Private )

define ($PAR$Private_all,
%CPAR$ % Private_All )

define ($PAR$Shared,
%CPAR$ % Shared )

define ($PAR$Shared_All,
%CPAR$ % Shared_All )

define ($PAR$Do_Parallel,
%CPAR$ % Do_Parallel )

define ($PAR$LockOn,
%CPAR$ % LockOn )

define ($PAR$LockOff,
%CPAR$ % LockOff )

#definitions from old vax.inc
define ($IF,I%F %)
define ($THEN, T%HEN% {)
define ($ELSEIF,} E%LSE IF %)
define ($ELSE,} E%LSE% {)
define ($ENDIF,} E%ND IF%)
define ($WHILE,D%O WHILE %)
define ($DO,{)
define ($ENDDO,} E%ND DO%)
define ($FOR,D%O % )

