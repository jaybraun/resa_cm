      program main
      CALL INITR4
      CALL RAT4
      CALL DMPHSH
      CALL ENDR4
      END
      SUBROUTINE GETDEF(TOKEN,TOKSIZ,DEFN,DEFSIZ,FD)
      LOGICAL*1 GTOK,NGETCH
      INTEGER DEFSIZ,FD,I,NLPAR,TOKSIZ
      LOGICAL*1 C,DEFN(10000),TOKEN(100),T,PTOKEN(100)
      CALL SKPBLK(FD)
      C =GTOK(PTOKEN,100,FD)
      IF (C.EQ.40) THEN
        T =40
        ELSE
            T =32
            CALL PBSTR(PTOKEN)
        END  IF
      CALL SKPBLK(FD)
      IF (GTOK(TOKEN,TOKSIZ,FD).NE.-9) THEN
        CALL BADERR('non-alphanumeric name.')
        END IF
      CALL SKPBLK(FD)
      C =GTOK(PTOKEN,100,FD)
      IF (T.EQ.32) THEN
          CALL PBSTR(PTOKEN)
          I =1
23006     CONTINUE
              C =NGETCH(C,FD)
              IF (I.GT.DEFSIZ) THEN
                CALL BADERR('definition too long.')
                END IF
              DEFN(I) =C
              I =I +1
23007       IF(.NOT.(C.EQ.35.OR.C.EQ.10.OR.C.EQ.-1))GOTO 23006
23008       CONTINUE
          IF (C.EQ.35) THEN
            CALL PUTBAK(C)
            END IF
        ELSE IF (T.EQ.40) THEN
          IF (C.NE.44) THEN
            CALL BADERR('missing comma in define.')
            END IF
          NLPAR =0
          I =1
          DO WHILE (NLPAR.GE.0)
            IF (I.GT.DEFSIZ) THEN
              CALL BADERR('definition too long.')
              ELSE IF (NGETCH(DEFN(I),FD).EQ.-1) THEN
              CALL BADERR('missing right paren.')
              ELSE IF (DEFN(I).EQ.40) THEN
              NLPAR =NLPAR +1
              ELSE IF (DEFN(I).EQ.41) THEN
              NLPAR =NLPAR -1
              END IF
23018       I=I+1
            END DO
23019       CONTINUE
        ELSE
          CALL BADERR('getdef is confused.')
        END  IF
      DEFN(I-1) =0
      RETURN
      END
      SUBROUTINE GETUDEF(TOKEN,TOKSIZ,DEFN,DEFSIZ,FD)
      LOGICAL*1 GTOK,NGETCH
      INTEGER DEFSIZ,FD,I,NLPAR,TOKSIZ
      LOGICAL*1 C,DEFN(10000),TOKEN(100),T,PTOKEN(100)
      CALL SKPBLK(FD)
      C =GTOK(PTOKEN,100,FD)
      IF (C.EQ.40) THEN
        T =40
        ELSE
            T =32
            CALL PBSTR(PTOKEN)
        END  IF
      CALL SKPBLK(FD)
      IF (GTOK(TOKEN,TOKSIZ,FD).NE.-9) THEN
        CALL SYNERR('non-alphanumeric name for undefine.')
        END IF
      CALL SKPBLK(FD)
      C =GTOK(PTOKEN,100,FD)
      IF (T.EQ.40.AND.C.EQ.41) THEN
          CONTINUE
        ELSE
            CALL PBSTR(PTOKEN)
        END  IF
      DEFN(1) =0
      RETURN
      END
      SUBROUTINE INSTAL(NAME,DEFN)
      LOGICAL*1 NAME(100),DEFN(10000)
      INTEGER NLEN,DLEN,LENGTH,C,HSHFCN
      EXTERNAL SCOPY
      COMMON /CLOOK/ LASTP,LASTT,HSHPTR(4007),TABPTR(2,22000),TABLE(8000
     *00)
      INTEGER LASTP
      INTEGER LASTT
      INTEGER HSHPTR
      INTEGER TABPTR
      LOGICAL*1 TABLE
      CALL FOLD (NAME)
      NLEN =LENGTH(NAME) +1
      DLEN =LENGTH(DEFN) +1
      IF (LASTT+NLEN+DLEN.GT.800000.OR.LASTP.GE.22000) THEN
          CALL PUTLIN(NAME,7)
          CALL REMARK_V(' : too many definitions.')
        ELSE
            LASTP =LASTP +1
            TABPTR(2,LASTP) =LASTT +1
            C =HSHFCN(NAME,4007)
            TABPTR(1,LASTP) =HSHPTR(C)
            HSHPTR(C) =LASTP
            CALL SCOPY(NAME,1,TABLE,LASTT +1)
            CALL SCOPY(DEFN,1,TABLE,LASTT +NLEN +1)
            LASTT =LASTT +NLEN +DLEN
        END  IF
      RETURN
      END
      SUBROUTINE DMPHSH
      INTEGER C,TOTS(0:100),LOAD(100)
      COMMON /CLOOK/ LASTP,LASTT,HSHPTR(4007),TABPTR(2,22000),TABLE(8000
     *00)
      INTEGER LASTP
      INTEGER LASTT
      INTEGER HSHPTR
      INTEGER TABPTR
      LOGICAL*1 TABLE
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      LOGICAL*1 HEAD1(32)
      DATA HEAD1(1)/10/,HEAD1(2)/10/,HEAD1(3)/32/,HEAD1(4)/32/,HEAD1(5)/
     *32/,HEAD1(6)/32/,HEAD1(7)/32/,HEAD1(8)/68/,HEAD1(9)/117/,HEAD1(10)
     */109/,HEAD1(11)/112/,HEAD1(12)/105/,HEAD1(13)/110/,HEAD1(14)/103/,
     *HEAD1(15)/32/,HEAD1(16)/72/,HEAD1(17)/97/,HEAD1(18)/115/,HEAD1(19)
     */104/,HEAD1(20)/32/,HEAD1(21)/84/,HEAD1(22)/97/,HEAD1(23)/98/,HEAD
     *1(24)/108/,HEAD1(25)/101/,HEAD1(26)/32/,HEAD1(27)/83/,HEAD1(28)/11
     *6/,HEAD1(29)/97/,HEAD1(30)/116/,HEAD1(31)/115/,HEAD1(32)/0/
      IF (.NOT.(OPTION(88).EQ.1)) THEN
        RETURN
        END IF
      CALL REMARK (HEAD1)
      MAXCNT =0
      MAXTOT =0
      C=1
      DO WHILE (C.LE.4007)
          ICNT =0
          I=HSHPTR(C)
          DO WHILE (I.GT.0)
              ICNT =ICNT +1
              K =MIN (100,ICNT)
              LOAD(K) =LOAD(K) +1
23042       I=TABPTR(1,I)
            END DO
23043       CONTINUE
          K =MIN (100,ICNT)
          MAXCNT =MAX (MAXCNT,ICNT)
          MAXTOT =MAX (MAXTOT,K)
          TOTS(K) =TOTS(K)+1
          IF (ICNT.EQ.0) THEN
            GOTO 23039 ! NEXT
            END IF
23039   C=C+1
        END DO
23040   CONTINUE
      CALL PUTCH (10,7)
      CALL PUTINT (LASTP,9,7)
      CALL REMARK_V (' Defines stored.')
      CALL PUTINT (LASTT,9,7)
      CALL REMARK_V (' Bytes of Buffer Used.')
      K =LASTT*100.0/800000+.5
      CALL PUTINT (K,9,7)
      CALL REMARK_V (' Percent of Buffer Used.')
      CALL PUTINT (TOTS(0),9,7)
      CALL REMARK_V (' Lists with no entries.')
      CALL PUTINT (MAXCNT,9,7)
      CALL REMARK_V (' was the Maximum Depth.')
      CALL PUTCH (10,7)
      K=1
      DO WHILE (K.LE.MAXTOT)
          IF (LOAD(K).EQ.0) THEN
            GOTO 23047 ! NEXT
            END IF
          CALL PUTINT (LOAD(K),7,7)
          CALL PUTLIN_V (' defines at level',7)
          CALL PUTINT (K,4,7)
          CALL PUTLIN_V ('.',7)
          IF (TOTS(K).NE.0) THEN
              CALL PUTINT (TOTS(K),15,7)
              CALL PUTLIN_V (' list',7)
              IF (TOTS(K).EQ.1) THEN
                CALL PUTLIN_V ('  ',7)
                ELSE
                  CALL PUTLIN_V ('s ',7)
                END  IF
              CALL PUTINT (K,4,7)
              CALL PUTLIN_V (' deep.',7)
            END IF
          CALL PUTCH (10,7)
23047   K=K+1
        END DO
23048   CONTINUE
      RETURN
      END
      INTEGER FUNCTION LOOKUP(NAME,DEFN)
      LOGICAL*1 NAME(100),DEFN(10000)
      INTEGER C,HSHFCN,I,J,K,EQUAL
      EXTERNAL SCOPY
      COMMON /CLOOK/ LASTP,LASTT,HSHPTR(4007),TABPTR(2,22000),TABLE(8000
     *00)
      INTEGER LASTP
      INTEGER LASTT
      INTEGER HSHPTR
      INTEGER TABPTR
      LOGICAL*1 TABLE
      LOGICAL*1 SUNDEF(10)
      DATA SUNDEF(1)/42/,SUNDEF(2)/42/,SUNDEF(3)/85/,SUNDEF(4)/78/,SUNDE
     *F(5)/68/,SUNDEF(6)/69/,SUNDEF(7)/70/,SUNDEF(8)/42/,SUNDEF(9)/42/,S
     *UNDEF(10)/0/
      C =HSHFCN(NAME,4007)
      LOOKUP =0
      I=HSHPTR(C)
      DO WHILE (I.GT.0)
          J =TABPTR(2,I)
          K=1
          DO WHILE (NAME(K).EQ.TABLE(J).AND.NAME(K).NE.0)
            J =J +1
23059       K=K+1
            END DO
23060       CONTINUE
          IF (NAME(K).EQ.TABLE(J)) THEN
              CALL SCOPY(TABLE,J+1,DEFN,1)
              IF (DEFN(1).EQ.42.AND.EQUAL(DEFN,SUNDEF).EQ.1) THEN
                LOOKUP =0
                ELSE
                  LOOKUP =1
                END  IF
              J =LENGTH(DEFN)
              DEFN(J+1) =0
              GOTO 23057 ! BREAK
            END IF
23056   I=TABPTR(1,I)
        END DO
23057   CONTINUE
      RETURN
      END
      INTEGER FUNCTION HSHFCN(STRNG,N)
      LOGICAL*1 STRNG(100)
      INTEGER N,I,LENGTH,I1,I2,ILO,IHI,K
      I1 =0
      K =1
      DO WHILE (STRNG(K).NE.0)
          IF (K) THEN
            I1 =I1 +STRNG(K)
            ELSE
              I1 =I1 +(STRNG(K) *256)
            END  IF
23066   K=K+1
        END DO
23067   CONTINUE
      HSHFCN =MOD(I1,N) +1
      RETURN
      END
      SUBROUTINE TBINIT
      COMMON /CLOOK/ LASTP,LASTT,HSHPTR(4007),TABPTR(2,22000),TABLE(8000
     *00)
      INTEGER LASTP
      INTEGER LASTT
      INTEGER HSHPTR
      INTEGER TABPTR
      LOGICAL*1 TABLE
      LASTP =0
      LASTT =0
      I=1
      DO WHILE (I.LE.4007)
        HSHPTR(I) =0
23071   I=I+1
        END DO
23072   CONTINUE
      RETURN
      END
      SUBROUTINE DOCODE(LAB)
      INTEGER LABGEN
      INTEGER LAB
      LOGICAL*1 GNBTOK
      LOGICAL*1 LEXSTR(100)
      COMMON /CGOTO/ XFER
      INTEGER XFER
      LOGICAL*1 SDO(3)
      DATA SDO(1)/100/,SDO(2)/111/,SDO(3)/0/
      XFER =0
      CALL OUTTAB
      CALL OUTSTR(SDO)
      CALL OUTCH(32)
      LAB =LABGEN(2)
      IF (GNBTOK(LEXSTR,100).EQ.2) THEN
        CALL OUTSTR(LEXSTR)
        ELSE
            CALL PBSTR(LEXSTR)
            CALL OUTNUM(LAB)
        END  IF
      CALL OUTCH(32)
      CALL EATUP
      CALL OUTDON
      RETURN
      END
      SUBROUTINE DOSTAT(LAB)
      INTEGER LAB
      CALL OUTCON(LAB)
      CALL OUTCON(LAB+1)
      RETURN
      END
      SUBROUTINE BADERR(MSG)
      character*100 MSG
      CALL SYNERR(MSG)
      CALL DMPHSH
      CALL ENDR4
      END
      SUBROUTINE SYNER2(MSG1,MSG2)
      LOGICAL*1 MSG(400),MSG1(100),MSG2(100)
      J=1
      CALL STCOPY (MSG1,1,MSG,J)
      CALL STCOPY (MSG2,1,MSG,J)
      CALL SYNERR (MSG)
      RETURN
      END
      SUBROUTINE SYNERR(MSG)
      LOGICAL*1 LC(10)
      character*100 MSG
      INTEGER ITOC
      INTEGER I,JUNK
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      LOGICAL*1 IN(5)
      LOGICAL*1 ERRMSG(15)
      DATA IN(1)/32/,IN(2)/105/,IN(3)/110/,IN(4)/32/,IN(5)/0/
      DATA ERRMSG(1)/101/,ERRMSG(2)/114/,ERRMSG(3)/114/,ERRMSG(4)/111/,E
     *RRMSG(5)/114/,ERRMSG(6)/32/,ERRMSG(7)/97/,ERRMSG(8)/116/,ERRMSG(9)
     */32/,ERRMSG(10)/108/,ERRMSG(11)/105/,ERRMSG(12)/110/,ERRMSG(13)/10
     *1/,ERRMSG(14)/32/,ERRMSG(15)/0/
      CALL PUTLIN(ERRMSG,7)
      IF (LEVEL.GE.1) THEN
        I =LEVEL
        ELSE
          I =1
        END  IF
      JUNK =ITOC (LINECT(I),LC,10)
      CALL PUTLIN(LC,7)
      I =FNAMP-1
      DO WHILE (I.GT.1)
        IF (FNAMES(I-1).EQ.0) THEN
            CALL PUTLIN(IN,7)
            CALL PUTLIN(FNAMES(I),7)
            GOTO 23079 ! BREAK
          END IF
23078   I=I-1
        END DO
23079   CONTINUE
      CALL PUTCH(58,7)
      CALL PUTCH(32,7)
      CALL REMARK_V (MSG)
      RETURN
      END
      SUBROUTINE FORCOD(LAB)
      LOGICAL*1 GETTOK,GNBTOK
      LOGICAL*1 T,TOKEN(100)
      INTEGER LENGTH,LABGEN
      INTEGER I,J,LAB,NLPAR
      EXTERNAL SCOPY
      COMMON /CFOR/ FORDEP,FORSTK(400)
      INTEGER FORDEP
      LOGICAL*1 FORSTK
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      LOGICAL*1 IFNOT(9)
      LOGICAL*1 SDO(10)
      LOGICAL*1 SFOREVER(9)
      DATA IFNOT(1)/105/,IFNOT(2)/102/,IFNOT(3)/40/,IFNOT(4)/46/,IFNOT(5
     *)/110/,IFNOT(6)/111/,IFNOT(7)/116/,IFNOT(8)/46/,IFNOT(9)/0/
      DATA SDO(1)/100/,SDO(2)/111/,SDO(3)/32/,SDO(4)/119/,SDO(5)/104/,SD
     *O(6)/105/,SDO(7)/108/,SDO(8)/101/,SDO(9)/32/,SDO(10)/0/
      DATA SFOREVER(1)/40/,SFOREVER(2)/46/,SFOREVER(3)/116/,SFOREVER(4)/
     *114/,SFOREVER(5)/117/,SFOREVER(6)/101/,SFOREVER(7)/46/,SFOREVER(8)
     */41/,SFOREVER(9)/0/
      LAB =LABGEN(3)
      CALL OUTCON(0)
      IF (GNBTOK(TOKEN,100).NE.40) THEN
          CALL SYNERR('missing left paren.')
          RETURN
        END IF
      IF (GNBTOK(TOKEN,100).NE.59) THEN
          CALL PBSTR(TOKEN)
          CALL OUTTAB
          CALL EATUP
          CALL OUTDON
        END IF
      IF (GNBTOK(TOKEN,100).EQ.59) THEN
          IF ((OPTION(57).EQ.1)) THEN
              CALL OUTTAB
              CALL OUTSTR(SDO)
              CALL OUTSTR(SFOREVER)
              CALL OUTDON
            ELSE
                CALL OUTCON(LAB)
            END  IF
        ELSE
            CALL PBSTR(TOKEN)
            IF ((OPTION(57).EQ.1)) THEN
                CALL OUTTAB
                CALL OUTSTR(SDO)
              ELSE
                  CALL OUTNUM(LAB)
                  CALL OUTTAB
                  CALL OUTSTR(IFNOT)
              END  IF
            CALL OUTCH(40)
            NLPAR =0
            DO WHILE (NLPAR.GE.0)
                T =GNBTOK(TOKEN,100)
                IF (T.EQ.59) THEN
                  GOTO 23093 ! BREAK
                  END IF
                IF (T.EQ.40) THEN
                  NLPAR =NLPAR +1
                  ELSE IF (T.EQ.41) THEN
                  NLPAR =NLPAR -1
                  END IF
                IF (T.EQ.-1) THEN
                   CALL PBSTR(TOKEN)
                   RETURN
                  END IF
                IF (T.NE.10.AND.T.NE.95) THEN
                  CALL OUTSTR(TOKEN)
                  END IF
23092         END DO
23093         CONTINUE
            CALL OUTCH(41)
            IF ((OPTION(57).EQ.1)) THEN
                CALL OUTDON
              ELSE
                  CALL OUTCH(41)
                  CALL OUTGO(LAB+2)
              END  IF
            IF (NLPAR.LT.0) THEN
              CALL SYNERR('invalid for clause.')
              END IF
        END  IF
      FORDEP =FORDEP +1
      J =1
      I =1
      DO WHILE (I.LT.FORDEP)
        J =J +LENGTH(FORSTK(J)) +1
23109   I=I+1
        END DO
23110   CONTINUE
      FORSTK(J) =0
      NLPAR =0
      T =GNBTOK(TOKEN,100)
      CALL PBSTR(TOKEN)
      DO WHILE (NLPAR.GE.0)
          T =GNBTOK(TOKEN,100)
          IF (T.EQ.40) THEN
            NLPAR =NLPAR +1
            ELSE IF (T.EQ.41) THEN
            NLPAR =NLPAR -1
            END IF
          IF (T.EQ.-1) THEN
              CALL PBSTR(TOKEN)
              GOTO 23112 ! BREAK
            END IF
          IF (NLPAR.GE.0.AND.T.NE.10.AND.T.NE.95) THEN
              IF (J+LENGTH(TOKEN).GE.400) THEN
                CALL BADERR('for clause too long.')
                END IF
              CALL SCOPY(TOKEN,1,FORSTK,J)
              J =J +LENGTH(TOKEN)
            END IF
23111   END DO
23112   CONTINUE
      LAB =LAB +1
      RETURN
      END
      SUBROUTINE FORS(LAB)
      INTEGER LENGTH
      INTEGER I,J,LAB
      COMMON /CFOR/ FORDEP,FORSTK(400)
      INTEGER FORDEP
      LOGICAL*1 FORSTK
      COMMON /CGOTO/ XFER
      INTEGER XFER
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      LOGICAL*1 SDO(3)
      DATA SDO(1)/100/,SDO(2)/111/,SDO(3)/0/
      XFER =0
      CALL OUTNUM(LAB)
      J =1
      I =1
      DO WHILE (I.LT.FORDEP)
        J =J +LENGTH(FORSTK(J)) +1
23124   I=I+1
        END DO
23125   CONTINUE
      IF (LENGTH(FORSTK(J)).GT.0) THEN
          CALL OUTTAB
          CALL OUTSTR(FORSTK(J))
          CALL OUTDON
        END IF
      IF ((OPTION(57).EQ.1)) THEN
          CALL OUTEND(SDO)
        ELSE
            CALL OUTGO(LAB-1)
        END  IF
      CALL OUTCON(LAB+1)
      FORDEP =FORDEP -1
      RETURN
      END
      SUBROUTINE BALPAR
      LOGICAL*1 GETTOK,GNBTOK
      LOGICAL*1 T,TOKEN(100)
      INTEGER NLPAR
      IF (GNBTOK(TOKEN,100).NE.40) THEN
          CALL SYNERR('missing left paren.')
          RETURN
        END IF
      CALL OUTSTR(TOKEN)
      NLPAR =1
23132 CONTINUE
          T =GNBTOK(TOKEN,100)
          IF (T.EQ.59.OR.T.EQ.123.OR.T.EQ.125.OR.T.EQ.-1) THEN
              CALL PBSTR(TOKEN)
              GOTO 23134 ! BREAK
            END IF
          IF (T.EQ.10) THEN
            TOKEN(1) =0
            ELSE IF (T.EQ.40) THEN
            NLPAR =NLPAR +1
            ELSE IF (T.EQ.41) THEN
            NLPAR =NLPAR -1
            END IF
          CALL OUTSTR(TOKEN)
23133   IF(.NOT.(NLPAR.LE.0))GOTO 23132
23134   CONTINUE
      IF (NLPAR.NE.0) THEN
        CALL SYNERR('missing parenthesis in condition.')
        END IF
      RETURN
      END
      SUBROUTINE ELSEIF(LAB)
      INTEGER LAB
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      IF ((OPTION(57).EQ.1)) THEN
          CALL ELSECD(LAB)
        ELSE
            CALL OUTGO(LAB+1)
            CALL OUTCON(LAB)
        END  IF
      RETURN
      END
      SUBROUTINE IFCODE(LAB)
      INTEGER LABGEN
      INTEGER LAB
      COMMON /CGOTO/ XFER
      INTEGER XFER
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      XFER =0
      LAB =LABGEN(2)
      IF ((OPTION(57).EQ.1)) THEN
        CALL IFTHEN(LAB)
        ELSE
          CALL IFGO (LAB)
        END  IF
      RETURN
      END
      SUBROUTINE IFGO(LAB)
      INTEGER LAB
      LOGICAL*1 IFNOT(9)
      DATA IFNOT(1)/105/,IFNOT(2)/102/,IFNOT(3)/40/,IFNOT(4)/46/,IFNOT(5
     *)/110/,IFNOT(6)/111/,IFNOT(7)/116/,IFNOT(8)/46/,IFNOT(9)/0/
      CALL OUTTAB
      CALL OUTSTR(IFNOT)
      CALL BALPAR
      CALL OUTCH(41)
      CALL OUTGO(LAB)
      RETURN
      END
      SUBROUTINE IFTHEN (LAB)
      INTEGER LAB
      LOGICAL*1 SIF(4)
      LOGICAL*1 STHEN(6)
      DATA SIF(1)/105/,SIF(2)/102/,SIF(3)/32/,SIF(4)/0/
      DATA STHEN(1)/32/,STHEN(2)/116/,STHEN(3)/104/,STHEN(4)/101/,STHEN(
     *5)/110/,STHEN(6)/0/
      CALL OUTTAB
      CALL OUTSTR(SIF)
      CALL BALPAR
      CALL OUTSTR (STHEN)
      CALL OUTDON
      RETURN
      END
      SUBROUTINE ELSECD (LAB)
      INTEGER LAB
      INTEGER EQUAL
      LOGICAL*1 SIF(3)
      LOGICAL*1 SELSE(6)
      LOGICAL*1 STHEN(5)
      DATA SIF(1)/105/,SIF(2)/102/,SIF(3)/0/
      DATA SELSE(1)/101/,SELSE(2)/108/,SELSE(3)/115/,SELSE(4)/101/,SELSE
     *(5)/32/,SELSE(6)/0/
      DATA STHEN(1)/116/,STHEN(2)/104/,STHEN(3)/101/,STHEN(4)/110/,STHEN
     *(5)/0/
      LOGICAL*1 STR(100)
      LOGICAL*1 NEXTTOKEN,LEX
      CALL OUTTAB
      CALL OUTSTR(SELSE)
      NEXTTOKEN =LEX(STR)
      CALL PBSTR(STR)
      IF (EQUAL(STR,SIF).NE.1) THEN
          CALL OUTDON
        END IF
      RETURN
      END
      LOGICAL*1 FUNCTION GETTOK(TOKEN,TOKSIZ)
      INTEGER EQUAL,LENGTH
      INTEGER I,TOKSIZ,F,LEN
      LOGICAL*1 T
      LOGICAL*1 DEFTOK,NGETCH
      LOGICAL*1 GETCH
      LOGICAL*1 NAME(81),TOKEN(100)
      LOGICAL*1 LFNAME(81)
      EXTERNAL SCOPY
      INTEGER LABVAL(400),LEXTYP(400),SP
      COMMON /STACKC/ SP,LABVAL,LEXTYP
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      COMMON /CFNAME/ FCNAME(81)
      LOGICAL*1 FCNAME
      LOGICAL*1 PROG(8)
      LOGICAL*1 SUBR(11)
      LOGICAL*1 FNCN(9)
      LOGICAL*1 INCL(8)
      DATA PROG(1)/112/,PROG(2)/114/,PROG(3)/111/,PROG(4)/103/,PROG(5)/1
     *14/,PROG(6)/97/,PROG(7)/109/,PROG(8)/0/
      DATA SUBR(1)/115/,SUBR(2)/117/,SUBR(3)/98/,SUBR(4)/114/,SUBR(5)/11
     *1/,SUBR(6)/117/,SUBR(7)/116/,SUBR(8)/105/,SUBR(9)/110/,SUBR(10)/10
     *1/,SUBR(11)/0/
      DATA FNCN(1)/102/,FNCN(2)/117/,FNCN(3)/110/,FNCN(4)/99/,FNCN(5)/11
     *6/,FNCN(6)/105/,FNCN(7)/111/,FNCN(8)/110/,FNCN(9)/0/
      DATA INCL(1)/105/,INCL(2)/110/,INCL(3)/99/,INCL(4)/108/,INCL(5)/11
     *7/,INCL(6)/100/,INCL(7)/101/,INCL(8)/0/
      DO WHILE (LEVEL.GT.0)
          F =INFILE(LEVEL)
          GETTOK =DEFTOK(TOKEN,TOKSIZ,F)
          DO WHILE (GETTOK.NE.-1)
              IF (EQUAL(TOKEN,FNCN).EQ.1.OR.EQUAL(TOKEN,PROG).EQ.1.OR.EQ
     *UAL(TOKEN,SUBR).EQ.1) THEN
                  CALL SKPBLK(INFILE(LEVEL))
                  T =DEFTOK(FCNAME,81,F)
                  CALL PBSTR(FCNAME)
                  CALL PUTBAK(32)
                  IF (SP.NE.1) THEN
                    CALL SYNERR('stack reset in parser.')
                    SP =1
                   END IF
                  SW_COUNT =0
                  IF ((OPTION(86).EQ.1).AND.EQUAL(FCNAME,LFNAME).EQ.0) T
     *HEN
                    CALL SCOPY (FCNAME,1,LFNAME,1)
                    CALL PUTLIN(FCNAME,7)
                    CALL PUTCH(10,7)
                   END IF
                  IF (T.NE.-9) THEN
                   CALL SYNERR('missing sub-program name.')
                   END IF
                  RETURN
                ELSE IF (EQUAL(TOKEN,INCL).EQ.0) THEN
                RETURN
                END IF
              CALL SKPBLK(INFILE(LEVEL))
              T =DEFTOK(NAME,81,INFILE(LEVEL))
              IF (T.EQ.39.OR.T.EQ.34) THEN
                  LEN =LENGTH(NAME) -1
                  I=1
                  DO WHILE (I.LT.LEN)
                   NAME(I) =NAME(I+1)
23170              I=I+1
                   END DO
23171              CONTINUE
                  NAME(I) =0
                END IF
              I =LENGTH(NAME) +1
              IF (LEVEL.GE.12) THEN
                CALL SYNERR('includes nested too deeply.')
                ELSE
                   CALL OPNINC(NAME,I)
                END  IF
23155       GETTOK=DEFTOK(TOKEN,TOKSIZ,F)
            END DO
23156       CONTINUE
          IF (LEVEL.GT.1) THEN
              IF (INFILE(LEVEL).EQ.99999) THEN
                CALL CLSLIB
                ELSE
                   CALL CLOSE_(INFILE(LEVEL))
                   FNAMP =FNAMP -1
                   DO WHILE (FNAMP.GT.1)
                    IF (FNAMES(FNAMP-1).EQ.0) THEN
                     GOTO 23180 ! BREAK
                     END IF
23179               FNAMP=FNAMP-1
                    END DO
23180               CONTINUE
                END  IF
            END IF
23152   LEVEL=LEVEL-1
        END DO
23153   CONTINUE
      TOKEN(1) =-1
      TOKEN(2) =0
      GETTOK =-1
      RETURN
      END
      LOGICAL*1 FUNCTION GNBTOK(TOKEN,TOKSIZ)
      INTEGER TOKSIZ
      LOGICAL*1 TOKEN(100),GETTOK
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      CALL SKPBLK (INFILE(LEVEL))
      GNBTOK =GETTOK(TOKEN,TOKSIZ)
      RETURN
      END
      LOGICAL*1 FUNCTION GTOK (LEXSTR,TOKSIZ,FD)
      LOGICAL*1 NGETCH,TYPE
      INTEGER FD,I,N,TOKSIZ,ITOC
      REAL*8 B
      LOGICAL*1 C,LEXSTR(100)
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      C =NGETCH(LEXSTR(1),FD)
      IF (C.EQ.32.OR.C.EQ.9.OR.C.EQ.12) THEN
          LEXSTR(1) =32
          DO WHILE (C.EQ.32.OR.C.EQ.9.OR.C.EQ.12)
            C =NGETCH(C,FD)
23185       END DO
23186       CONTINUE
          IF (C.EQ.35) THEN
            DO WHILE (NGETCH(C,FD).NE.10)
23189         END DO
23190         CONTINUE
            END IF
          IF (C.NE.10) THEN
            CALL PUTBAK(C)
            ELSE
              LEXSTR(1) =10
            END  IF
          LEXSTR(2) =0
          GTOK =LEXSTR(1)
          RETURN
        END IF
      I =1
      GTOK =TYPE(C)
      IF (GTOK.EQ.1.OR.GTOK.EQ.36) THEN
          I =1
          DO WHILE (I.LT.TOKSIZ-2)
              GTOK =TYPE(NGETCH(LEXSTR(I+1),FD))
              IF (GTOK.NE.1.AND.GTOK.NE.2.AND.GTOK.NE.95.AND.GTOK.NE.36)
     * THEN
                GOTO 23197 ! BREAK
                END IF
23196       I=I+1
            END DO
23197       CONTINUE
          CALL PUTBAK(LEXSTR(I+1))
          GTOK =-9
        ELSE IF (GTOK.EQ.2) THEN
          B =C -48
          I =1
          DO WHILE (I.LT.TOKSIZ-2)
              IF (TYPE(NGETCH(LEXSTR(I+1),FD)).NE.2) THEN
                GOTO 23204 ! BREAK
                END IF
              B =10*B +LEXSTR(I+1) -48
23203       I=I+1
            END DO
23204       CONTINUE
          IF (LEXSTR(I+1).EQ.37.AND.B.GE.2.AND.B.LE.36) THEN
              N =0
              DO WHILE (.TRUE.)
                  C =NGETCH(LEXSTR(1),FD)
                  IF (C.GE.97.AND.C.LE.122) THEN
                   C =C -97+57+1
                   ELSE IF (C.GE.65.AND.C.LE.90) THEN
                   C =C -65+57+1
                   END IF
                  IF (C.LT.48.OR.C.GE.48+B) THEN
                   GOTO 23211 ! BREAK
                   END IF
23210           N=B*N+C-48
                END DO
23211           CONTINUE
              CALL PUTBAK(LEXSTR(1))
              I =ITOC(N,LEXSTR,TOKSIZ)
            ELSE
              CALL PUTBAK(LEXSTR(I+1))
            END  IF
          GTOK =2
        ELSE IF (C.EQ.91) THEN
          LEXSTR(1) =40
          GTOK =40
        ELSE IF (C.EQ.93) THEN
          LEXSTR(1) =41
          GTOK =41
        ELSE IF (C.EQ.64) THEN
          IF (NGETCH(LEXSTR(2),FD).EQ.40) THEN
              LEXSTR(1) =-10
              GTOK =-10
            ELSE IF (LEXSTR(2).EQ.41) THEN
              LEXSTR(1) =-11
              GTOK =-11
            ELSE
              CALL PUTBAK(LEXSTR(2))
            END  IF
        ELSE IF (C.EQ.39.OR.C.EQ.34.OR.C.EQ.37) THEN
          I =2
          DO WHILE (NGETCH(LEXSTR(I),FD).NE.LEXSTR(1))
              IF (LEXSTR(I).EQ.95.AND.C.NE.37) THEN
                IF (NGETCH(C,FD).EQ.10) THEN
                   DO WHILE (C.EQ.10.OR.C.EQ.32.OR.C.EQ.9.OR.C.EQ.12)
                    C =NGETCH(C,FD)
23237               END DO
23238               CONTINUE
                   LEXSTR(I) =C
                  ELSE
                   CALL PUTBAK(C)
                  END  IF
                END IF
              IF (LEXSTR(I).EQ.10.OR.I.GE.TOKSIZ-1) THEN
                  IF (C.NE.37) THEN
                   CALL SYNERR('missing quote.')
                   END IF
                  LEXSTR(I) =LEXSTR(1)
                  CALL PUTBAK(10)
                  GOTO 23232 ! BREAK
                END IF
23231       I=I+1
            END DO
23232       CONTINUE
        ELSE IF (C.EQ.35) THEN
          DO WHILE (NGETCH(LEXSTR(1),FD).NE.10)
23245       END DO
23246       CONTINUE
          GTOK =10
        ELSE IF (C.EQ.62.OR.C.EQ.60.OR.C.EQ.33.OR.C.EQ.33.OR.C.EQ.94.OR.
     *C.EQ.61.OR.C.EQ.38.OR.C.EQ.124) THEN
          CALL RELATE(LEXSTR,I,FD)
        END IF
      IF (I.GE.TOKSIZ-1) THEN
        CALL SYNERR('token too long.')
        END IF
      LEXSTR(I+1) =0
      RETURN
      END
      INTEGER FUNCTION LEX(LEXSTR)
      LOGICAL*1 GNBTOK,DEFTOK
      LOGICAL*1 LEXSTR(100)
      INTEGER EQUAL
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      LOGICAL*1 SIF(3)
      LOGICAL*1 SELSE(5)
      LOGICAL*1 SWHILE(6)
      LOGICAL*1 SDO(3)
      LOGICAL*1 SBREAK(6)
      LOGICAL*1 SNEXT(5)
      LOGICAL*1 SFOR(4)
      LOGICAL*1 SREPT(7)
      LOGICAL*1 SUNTIL(6)
      LOGICAL*1 SRET(7)
      LOGICAL*1 SSTR(7)
      LOGICAL*1 SSWTCH(7)
      LOGICAL*1 SCASE(5)
      LOGICAL*1 SDEFLT(8)
      DATA SIF(1)/105/,SIF(2)/102/,SIF(3)/0/
      DATA SELSE(1)/101/,SELSE(2)/108/,SELSE(3)/115/,SELSE(4)/101/,SELSE
     *(5)/0/
      DATA SWHILE(1)/119/,SWHILE(2)/104/,SWHILE(3)/105/,SWHILE(4)/108/,S
     *WHILE(5)/101/,SWHILE(6)/0/
      DATA SDO(1)/100/,SDO(2)/111/,SDO(3)/0/
      DATA SBREAK(1)/98/,SBREAK(2)/114/,SBREAK(3)/101/,SBREAK(4)/97/,SBR
     *EAK(5)/107/,SBREAK(6)/0/
      DATA SNEXT(1)/110/,SNEXT(2)/101/,SNEXT(3)/120/,SNEXT(4)/116/,SNEXT
     *(5)/0/
      DATA SFOR(1)/102/,SFOR(2)/111/,SFOR(3)/114/,SFOR(4)/0/
      DATA SREPT(1)/114/,SREPT(2)/101/,SREPT(3)/112/,SREPT(4)/101/,SREPT
     *(5)/97/,SREPT(6)/116/,SREPT(7)/0/
      DATA SUNTIL(1)/117/,SUNTIL(2)/110/,SUNTIL(3)/116/,SUNTIL(4)/105/,S
     *UNTIL(5)/108/,SUNTIL(6)/0/
      DATA SRET(1)/114/,SRET(2)/101/,SRET(3)/116/,SRET(4)/117/,SRET(5)/1
     *14/,SRET(6)/110/,SRET(7)/0/
      DATA SSTR(1)/115/,SSTR(2)/116/,SSTR(3)/114/,SSTR(4)/105/,SSTR(5)/1
     *10/,SSTR(6)/103/,SSTR(7)/0/
      DATA SSWTCH(1)/115/,SSWTCH(2)/119/,SSWTCH(3)/105/,SSWTCH(4)/116/,S
     *SWTCH(5)/99/,SSWTCH(6)/104/,SSWTCH(7)/0/
      DATA SCASE(1)/99/,SCASE(2)/97/,SCASE(3)/115/,SCASE(4)/101/,SCASE(5
     *)/0/
      DATA SDEFLT(1)/100/,SDEFLT(2)/101/,SDEFLT(3)/102/,SDEFLT(4)/97/,SD
     *EFLT(5)/117/,SDEFLT(6)/108/,SDEFLT(7)/116/,SDEFLT(8)/0/
      LEX =GNBTOK(LEXSTR,100)
      DO WHILE (LEX.EQ.10)
23252   LEX=GNBTOK(LEXSTR,100)
        END DO
23253   CONTINUE
      IF (LEX.EQ.-1.OR.LEX.EQ.59) THEN
        RETURN
        END IF
      IF ((LEX.EQ.123.OR.LEX.EQ.125)) THEN
        RETURN
        END IF
      IF (LEX.EQ.37) THEN
        RETURN
        END IF
      IF ((OPTION(77).EQ.1)) THEN
        LEX =-14
        ELSE IF (LEX.EQ.2) THEN
        LEX =-9
        ELSE IF (LEX.EQ.-37) THEN
        LEX =-27
        ELSE IF (EQUAL(LEXSTR,SIF).EQ.1) THEN
        LEX =-19
        ELSE IF (EQUAL(LEXSTR,SELSE).EQ.1) THEN
        LEX =-11
        ELSE IF (EQUAL(LEXSTR,SWHILE).EQ.1) THEN
        LEX =-15
        ELSE IF (EQUAL(LEXSTR,SDO).EQ.1) THEN
        LEX =-10
        ELSE IF (EQUAL(LEXSTR,SBREAK).EQ.1) THEN
        LEX =-8
        ELSE IF (EQUAL(LEXSTR,SNEXT).EQ.1) THEN
        LEX =-13
        ELSE IF (EQUAL(LEXSTR,SFOR).EQ.1) THEN
        LEX =-16
        ELSE IF (EQUAL(LEXSTR,SREPT).EQ.1) THEN
        LEX =-17
        ELSE IF (EQUAL(LEXSTR,SUNTIL).EQ.1) THEN
        LEX =-18
        ELSE IF (EQUAL(LEXSTR,SRET).EQ.1) THEN
        LEX =-20
        ELSE IF (EQUAL(LEXSTR,SSTR).EQ.1) THEN
        LEX =-23
        ELSE IF (EQUAL(LEXSTR,SSWTCH).EQ.1) THEN
        LEX =-24
        ELSE IF (EQUAL(LEXSTR,SCASE).EQ.1) THEN
        LEX =-25
        ELSE IF (EQUAL(LEXSTR,SDEFLT).EQ.1) THEN
        LEX =-26
        ELSE
          LEX =-14
        END  IF
      RETURN
      END
      LOGICAL*1 FUNCTION NGETCH(C,FD)
      LOGICAL*1 NGETLN
      LOGICAL*1 C
      INTEGER FD
      COMMON /CDEFIO/ BP,BUF(20000),LP,LNBUF(400)
      INTEGER BP
      LOGICAL*1 BUF
      INTEGER LP
      LOGICAL*1 LNBUF
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      IF (BP.GT.0) THEN
          C =BUF(BP)
          BP =BP -1
        ELSE
            C =LNBUF(LP)
            LP =LP +1
            IF (C.EQ.0) THEN
                C =NGETLN(C,FD)
                LINECT(LEVEL) =LINECT(LEVEL) +1
              END IF
        END  IF
      NGETCH =C
      RETURN
      END
      SUBROUTINE PBSTR(IN)
      LOGICAL*1 IN(100)
      INTEGER LENGTH
      INTEGER I
      I =LENGTH(IN)
      DO WHILE (I.GT.0)
        CALL PUTBAK(IN(I))
23299   I=I-1
        END DO
23300   CONTINUE
      RETURN
      END
      SUBROUTINE PUTBAK(C)
      LOGICAL*1 C
      COMMON /CDEFIO/ BP,BUF(20000),LP,LNBUF(400)
      INTEGER BP
      LOGICAL*1 BUF
      INTEGER LP
      LOGICAL*1 LNBUF
      BP =BP +1
      IF (BP.GT.20000) THEN
        CALL BADERR('too many characters pushed back.')
        END IF
      BUF(BP) =C
      RETURN
      END
      SUBROUTINE RELATE(TOKEN,LAST,FD)
      LOGICAL*1 NGETCH
      LOGICAL*1 TOKEN(100)
      INTEGER LENGTH
      INTEGER FD,LAST
      IF (NGETCH(TOKEN(2),FD).NE.61) THEN
          CALL PUTBAK(TOKEN(2))
          TOKEN(3) =116
        ELSE
          TOKEN(3) =101
        END  IF
      TOKEN(4) =46
      TOKEN(5) =0
      TOKEN(6) =0
      IF (TOKEN(1).EQ.62) THEN
        IF (TOKEN(2).EQ.60) THEN
            token(2) = NGETCH(TOKEN(2),FD)
            TOKEN(2) =110
            TOKEN(3) =101
            TOKEN(4) =46
          ELSE
              TOKEN(2) =103
          END  IF
        ELSE IF (TOKEN(1).EQ.60) THEN
        IF (TOKEN(2).EQ.62) THEN
            token(2) = NGETCH(TOKEN(2),FD)
            TOKEN(2) =110
            TOKEN(3) =101
            TOKEN(4) =46
          ELSE
              TOKEN(2) =108
          END  IF
        ELSE IF (TOKEN(1).EQ.33.OR.TOKEN(1).EQ.33.OR.TOKEN(1).EQ.94) THE
     *N
          IF (TOKEN(2).NE.61) THEN
              TOKEN(3) =111
              TOKEN(4) =116
              TOKEN(5) =46
            END IF
          TOKEN(2) =110
        ELSE IF (TOKEN(1).EQ.61) THEN
          IF (TOKEN(2).NE.61) THEN
              TOKEN(2) =0
              LAST =1
              RETURN
            END IF
          TOKEN(2) =101
          TOKEN(3) =113
        ELSE IF (TOKEN(1).EQ.38) THEN
          TOKEN(2) =97
          TOKEN(3) =110
          TOKEN(4) =100
          TOKEN(5) =46
        ELSE IF (TOKEN(1).EQ.124) THEN
          TOKEN(2) =111
          TOKEN(3) =114
        ELSE
          TOKEN(2) =0
        END  IF
      TOKEN(1) =46
      LAST =LENGTH(TOKEN)
      RETURN
      END
      SUBROUTINE RAT4
      INTEGER GETARG3,OPEN_
      LOGICAL*1 BUF(81)
      INTEGER I
      integer ftype
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      LOGICAL*1 DEFNS(8)
      DATA DEFNS(1)/115/,DEFNS(2)/121/,DEFNS(3)/109/,DEFNS(4)/98/,DEFNS(
     *5)/119/,DEFNS(6)/101/,DEFNS(7)/115/,DEFNS(8)/0/
      CALL INITKW
      call setchtype(1, 6)
      call setchfd(6,6)
      call setchtype(1, 0)
      call setchfd(0, 0)
      I=1
      DO WHILE (GETARG3(I,BUF,81).NE.-1)
          IF (BUF(1).EQ.45) THEN
              J=2
              DO WHILE (BUF(J).NE.0)
                OPTION(BUF(J)) =1
23331           J=J+1
                END DO
23332           CONTINUE
              GOTO 23326 ! NEXT
            END IF
23326   I=I+1
        END DO
23327   CONTINUE
      IF ((OPTION(68).NE.1).AND.(DEFNS(1).NE.0)) THEN
          CALL GETDIR(1,0,BUF)
          CALL CONCAT(BUF,DEFNS,BUF)
          ftype = 1
          INFILE(1) =OPEN_(BUF,ftype)
          IF (INFILE(1).EQ.-3) THEN
              LEVEL =0
              CALL OPNLIB (DEFNS)
            END IF
          CALL PARSE
          IF (INFILE(1).EQ.99999) THEN
              LEVEL =1
              CALL CLSLIB
            ELSE
              CALL CLOSE_ (INFILE(1))
            END  IF
        ELSE
          CALL REMARK_V ('Not using Standard Definitions File.')
        END  IF
      IFLG =0
      I=1
      DO WHILE (GETARG3(I,BUF,81).NE.-1)
          IF (BUF(1).EQ.63.AND.BUF(2).EQ.0) THEN
              CALL REMARK_V(
     *'usage:  rat4a [-lisdmv] [file ...] >outfile.')
              CALL REMARK_V(
     *'   where:   l = generate Lowercase FORTRAN')
              CALL REMARK_V('            i = Indent generate FORTRAN')
              CALL REMARK_V('            d = bypass stanDard macros')
              CALL REMARK_V('            m = translate Macros only')
              CALL REMARK_V(
     *'            v = list modules/files Verbosely')
              CALL ERROR_V ('            9 = generate FORTRAN-90 code')
            ELSE IF (BUF(1).EQ.45) THEN
              IF (BUF(2).EQ.0) THEN
                INFILE(1) =5
                ELSE
                  GOTO 23340 ! NEXT
                END  IF
            ELSE
                INFILE(1) =OPEN_(BUF,1)
                IF (INFILE(1).EQ.-3) THEN
                   LEVEL =0
                   CALL OPNLIB (BUF)
                  END IF
            END  IF
          IFLG =1
          CALL PARSE
          IF (INFILE(1).EQ.99999) THEN
              LEVEL =1
              CALL CLSLIB
            ELSE IF (INFILE(1).NE.1) THEN
            CALL CLOSE_(INFILE(1))
            END IF
23340   I=I+1
        END DO
23341   CONTINUE
      IF (IFLG.EQ.0) THEN
          INFILE(1) =5
          CALL PARSE
        END IF
      RETURN
      END
      SUBROUTINE EATUP
      LOGICAL*1 GETTOK,GNBTOK
      LOGICAL*1 PTOKEN(100),T,TOKEN(100)
      INTEGER NLPAR
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      NLPAR =0
23356 CONTINUE
          T =GETTOK(TOKEN,100)
          IF (T.EQ.10) THEN
            GOTO 23358 ! BREAK
            END IF
          IF ((OPTION(77).NE.1).AND.T.EQ.59) THEN
            GOTO 23358 ! BREAK
            END IF
          IF (T.EQ.125.OR.T.EQ.123) THEN
              CALL PBSTR(TOKEN)
              GOTO 23358 ! BREAK
            END IF
          IF (T.EQ.-1) THEN
              CALL SYNERR('unexpected EOF.')
              CALL PBSTR(TOKEN)
              GOTO 23358 ! BREAK
            END IF
          IF (T.EQ.44.OR.T.EQ.43.OR.T.EQ.45.OR.T.EQ.42.OR.T.EQ.40.OR.T.E
     *Q.38.OR.T.EQ.124.OR.T.EQ.33.OR.T.EQ.61.OR.T.EQ.95) THEN
              DO WHILE (GNBTOK(PTOKEN,100).EQ.10)
23369           END DO
23370           CONTINUE
              CALL PBSTR(PTOKEN)
              IF (T.EQ.95) THEN
                TOKEN(1) =0
                END IF
            END IF
          IF (T.EQ.40) THEN
            NLPAR =NLPAR +1
            ELSE IF (T.EQ.41.AND.NLPAR.GT.0) THEN
            NLPAR =NLPAR -1
            ELSE IF (T.EQ.41) THEN
              CALL SYNERR(
     *'unbalanced parentheses, right paren dropped.')
              GOTO 23357 ! NEXT
            END IF
          CALL OUTSTR(TOKEN)
23357   IF(.NOT.(NLPAR.LT.0))GOTO 23356
23358   CONTINUE
      IF (NLPAR.NE.0) THEN
          CALL SYNERR('unbalanced parentheses, right paren(s) added.')
          DO WHILE (NLPAR.GT.0)
              CALL OUTCH (41)
              NLPAR =NLPAR -1
23381       END DO
23382       CONTINUE
        END IF
      RETURN
      END
      SUBROUTINE LABELC(LEXSTR)
      LOGICAL*1 LEXSTR(100)
      INTEGER LENGTH
      COMMON /CGOTO/ XFER
      INTEGER XFER
      XFER =0
      IF (LENGTH(LEXSTR).EQ.5) THEN
        IF (LEXSTR(1).EQ.50.AND.LEXSTR(2).EQ.51) THEN
          CALL SYNERR('warning: possible label conflict.')
          END IF
        END IF
      CALL OUTSTR(LEXSTR)
      CALL OUTTAB
      RETURN
      END
      SUBROUTINE OTHERC(LEXSTR)
      LOGICAL*1 LEXSTR(100)
      COMMON /CGOTO/ XFER
      INTEGER XFER
      XFER =0
      CALL OUTTAB
      CALL OUTSTR(LEXSTR)
      CALL EATUP
      CALL OUTDON
      RETURN
      END
      SUBROUTINE OUTCH(C)
      LOGICAL*1 C
      INTEGER I
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      IF (OUTP.GE.72) THEN
          CALL OUTDON
          I =1
          DO WHILE (I.LT.6)
            OUTBUF(I) =32
23390       I=I+1
            END DO
23391       CONTINUE
          OUTBUF(6) =42
          OUTP =6
        END IF
      OUTP =OUTP +1
      OUTBUF(OUTP) =C
      RETURN
      END
      SUBROUTINE OUTCON(N)
      INTEGER N
      COMMON /CGOTO/ XFER
      INTEGER XFER
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      LOGICAL*1 CONTIN(9)
      DATA CONTIN(1)/99/,CONTIN(2)/111/,CONTIN(3)/110/,CONTIN(4)/116/,CO
     *NTIN(5)/105/,CONTIN(6)/110/,CONTIN(7)/117/,CONTIN(8)/101/,CONTIN(9
     *)/0/
      XFER =0
      IF (N.LE.0.AND.OUTP.EQ.0) THEN
        RETURN
        END IF
      IF (N.GT.0) THEN
        CALL OUTNUM(N)
        END IF
      CALL OUTTAB
      CALL OUTSTR(CONTIN)
      CALL OUTDON
      RETURN
      END
      SUBROUTINE OUTDON
      INTEGER ALLBLK
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      OUTBUF(OUTP+1) =10
      OUTBUF(OUTP+2) =0
      IF (ALLBLK(OUTBUF).EQ.0) THEN
        CALL PUTLIN(OUTBUF,6)
        END IF
      OUTP =0
      RETURN
      END
      SUBROUTINE OUTGO(N)
      INTEGER N
      COMMON /CGOTO/ XFER
      INTEGER XFER
      LOGICAL*1 GOTO(6)
      DATA GOTO(1)/103/,GOTO(2)/111/,GOTO(3)/116/,GOTO(4)/111/,GOTO(5)/3
     *2/,GOTO(6)/0/
      IF (XFER.EQ.1) THEN
        RETURN
        END IF
      CALL OUTTAB
      CALL OUTSTR(GOTO)
      CALL OUTNUM(N)
      CALL OUTDON
      RETURN
      END
      SUBROUTINE OUTNUM(N)
      LOGICAL*1 CHARS(10)
      INTEGER I,M
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      M =IABS(N)
      I =0
23400 CONTINUE
          I =I +1
          CHARS(I) =MOD(M,10) +48
          M =M / 10
23401   IF(.NOT.(M.EQ.0.OR.I.GE.10))GOTO 23400
23402   CONTINUE
      IF (N.LT.0) THEN
        CALL OUTCH(45)
        END IF
      DO WHILE (I.GT.0)
        CALL OUTCH(CHARS(I))
23406   I=I-1
        END DO
23407   CONTINUE
      RETURN
      END
      SUBROUTINE OUTSTR(STR)
      LOGICAL*1 C,STR(100),TYPE
      INTEGER I,J
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      I =1
      DO WHILE (STR(I).NE.0)
          C =STR(I)
          IF (C.NE.39.AND.C.NE.34.AND.C.NE.37) THEN
              IF ((OPTION(76).NE.1).AND.C.GE.97.AND.C.LE.122) THEN
                C =C -97+65
                END IF
              CALL OUTCH(C)
            ELSE IF (C.EQ.37) THEN
              I =I +1
              J =I
              DO WHILE (STR(J).NE.C)
23418           J=J+1
                END DO
23419           CONTINUE
              K =J-I
              DO WHILE (I.LT.J)
                  CALL OUTCH(STR(I))
23421           I=I+1
                END DO
23422           CONTINUE
              IF (K.EQ.0) THEN
                CALL OUTCH (37)
                END IF
            ELSE
                I =I +1
                J =I
                DO WHILE (STR(J).NE.C)
23426             J=J+1
                  END DO
23427             CONTINUE
                K =J-I
                IF ((J-I).LE.62.AND.(OUTP+3+J-I).GE.72) THEN
23430              CONTINUE
                     CALL OUTCH(32)
23431               IF(.NOT.(OUTP.LE.8.OR.OUTP.GE.72))GOTO 23430
23432               CONTINUE
                  END IF
                IF ( (OPTION(72).EQ.1)) THEN
                   CALL OUTNUM(J-I)
                   CALL OUTCH(72)
                  ELSE
                    CALL OUTCH (39)
                  END  IF
                DO WHILE (I.LT.J)
                   CALL OUTCH(STR(I))
                   IF (.NOT. (OPTION(72).EQ.1).AND.STR(I).EQ.39) THEN
                    CALL OUTCH (39)
                    END IF
23436             I=I+1
                  END DO
23437             CONTINUE
                IF (.NOT. (OPTION(72).EQ.1)) THEN
                   CALL OUTCH (39)
                  END IF
            END  IF
23409   I=I+1
        END DO
23410   CONTINUE
      RETURN
      END
      SUBROUTINE OUTTAB
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      DO WHILE (OUTP.LT.6)
        CALL OUTCH(32)
23442   END DO
23443   CONTINUE
      IF ((OPTION(73).EQ.1)) THEN
        CALL OUTLEV
        END IF
      RETURN
      END
      SUBROUTINE OUTLEV
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      INTEGER LABVAL(400),LEXTYP(400),SP
      COMMON /STACKC/ SP,LABVAL,LEXTYP
      IF (SP.LT.7) THEN
        I =4+SP*2
        ELSE
            I =4+7+SP
            IF (I.GT.40) THEN
              I =41
              END IF
        END  IF
      DO WHILE (OUTP.LT.I)
        CALL OUTCH(32)
23450   END DO
23451   CONTINUE
      RETURN
      END
      SUBROUTINE OUTGOC(N,STR)
      INTEGER N
      LOGICAL*1 STR
      COMMON /CGOTO/ XFER
      INTEGER XFER
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      LOGICAL*1 GOTO(6)
      LOGICAL*1 COM(4)
      DATA GOTO(1)/103/,GOTO(2)/111/,GOTO(3)/116/,GOTO(4)/111/,GOTO(5)/3
     *2/,GOTO(6)/0/
      DATA COM(1)/32/,COM(2)/33/,COM(3)/32/,COM(4)/0/
      IF (XFER.EQ.1) THEN
        RETURN
        END IF
      CALL OUTTAB
      CALL OUTSTR(GOTO)
      CALL OUTNUM(N)
      IF ((OPTION(57).EQ.1)) THEN
          CALL OUTSTR(COM)
          CALL OUTSTR(STR)
        END IF
      CALL OUTDON
      RETURN
      END
      SUBROUTINE OUTEND(STR)
      LOGICAL*1 STR
      LOGICAL*1 SEND(5)
      DATA SEND(1)/101/,SEND(2)/110/,SEND(3)/100/,SEND(4)/32/,SEND(5)/0/
      COMMON /CGOTO/ XFER
      INTEGER XFER
      CALL OUTTAB
      CALL OUTSTR(SEND)
      CALL OUTSTR(STR)
      CALL OUTDON
      XFER =0
      RETURN
      END
      INTEGER FUNCTION ALLBLK(BUF)
      LOGICAL*1 BUF(100)
      INTEGER I
      ALLBLK =1
      I=1
      DO WHILE (BUF(I).NE.10.AND.BUF(I).NE.0)
        IF (BUF(I).NE.32) THEN
            ALLBLK =0
            GOTO 23458 ! BREAK
          END IF
23457   I=I+1
        END DO
23458   CONTINUE
      RETURN
      END
      SUBROUTINE INITKW
      LOGICAL*1 DEFT(2),INCT(2),SUBT(2),IFT(2),ART(2),IFDFT(2),IFNDT(2)
      LOGICAL*1 UNDEFT(2)
      COMMON /CLABEL/ LABEL
      INTEGER LABEL
      LOGICAL*1 DEFNAM(7)
      LOGICAL*1 UDEFNM(9)
      LOGICAL*1 INCNAM(5)
      LOGICAL*1 SUBNAM(7)
      LOGICAL*1 IFNAM(7)
      LOGICAL*1 ARNAM(6)
      LOGICAL*1 IFDFNM(6)
      LOGICAL*1 IFNDNM(9)
      DATA DEFNAM(1)/100/,DEFNAM(2)/101/,DEFNAM(3)/102/,DEFNAM(4)/105/,D
     *EFNAM(5)/110/,DEFNAM(6)/101/,DEFNAM(7)/0/
      DATA UDEFNM(1)/117/,UDEFNM(2)/110/,UDEFNM(3)/100/,UDEFNM(4)/101/,U
     *DEFNM(5)/102/,UDEFNM(6)/105/,UDEFNM(7)/110/,UDEFNM(8)/101/,UDEFNM(
     *9)/0/
      DATA INCNAM(1)/105/,INCNAM(2)/110/,INCNAM(3)/99/,INCNAM(4)/114/,IN
     *CNAM(5)/0/
      DATA SUBNAM(1)/115/,SUBNAM(2)/117/,SUBNAM(3)/98/,SUBNAM(4)/115/,SU
     *BNAM(5)/116/,SUBNAM(6)/114/,SUBNAM(7)/0/
      DATA IFNAM(1)/105/,IFNAM(2)/102/,IFNAM(3)/101/,IFNAM(4)/108/,IFNAM
     *(5)/115/,IFNAM(6)/101/,IFNAM(7)/0/
      DATA ARNAM(1)/97/,ARNAM(2)/114/,ARNAM(3)/105/,ARNAM(4)/116/,ARNAM(
     *5)/104/,ARNAM(6)/0/
      DATA IFDFNM(1)/105/,IFDFNM(2)/102/,IFDFNM(3)/100/,IFDFNM(4)/101/,I
     *FDFNM(5)/102/,IFDFNM(6)/0/
      DATA IFNDNM(1)/105/,IFNDNM(2)/102/,IFNDNM(3)/110/,IFNDNM(4)/111/,I
     *FNDNM(5)/116/,IFNDNM(6)/100/,IFNDNM(7)/101/,IFNDNM(8)/102/,IFNDNM(
     *9)/0/
      DATA DEFT(1),DEFT(2) /-4,0/
      DATA UNDEFT(1),UNDEFT(2) /-29,0/
      DATA INCT(1),INCT(2) /-12,0/
      DATA SUBT(1),SUBT(2) /-13,0/
      DATA IFT(1),IFT(2) /-11,0/
      DATA ART(1),ART(2) /-14,0/
      DATA IFDFT(1),IFDFT(2) /-15,0/
      DATA IFNDT(1),IFNDT(2) /-16,0/
      CALL TBINIT
      CALL ULSTAL(DEFNAM,DEFT)
      CALL ULSTAL(UDEFNM,UNDEFT)
      CALL ULSTAL(INCNAM,INCT)
      CALL ULSTAL(SUBNAM,SUBT)
      CALL ULSTAL(IFNAM,IFT)
      CALL ULSTAL(ARNAM,ART)
      CALL ULSTAL(IFDFNM,IFDFT)
      CALL ULSTAL(IFNDNM,IFNDT)
      LABEL =23000
      RETURN
      END
      SUBROUTINE INIT
      INTEGER I
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      COMMON /CDEFIO/ BP,BUF(20000),LP,LNBUF(400)
      INTEGER BP
      LOGICAL*1 BUF
      INTEGER LP
      LOGICAL*1 LNBUF
      COMMON /CFOR/ FORDEP,FORSTK(400)
      INTEGER FORDEP
      LOGICAL*1 FORSTK
      COMMON /CLOOK/ LASTP,LASTT,HSHPTR(4007),TABPTR(2,22000),TABLE(8000
     *00)
      INTEGER LASTP
      INTEGER LASTT
      INTEGER HSHPTR
      INTEGER TABPTR
      LOGICAL*1 TABLE
      COMMON /CFNAME/ FCNAME(81)
      LOGICAL*1 FCNAME
      COMMON /CLABEL/ LABEL
      INTEGER LABEL
      COMMON /CSBUF/ SBP,SBUF(10000)
      INTEGER SBP
      LOGICAL*1 SBUF
      COMMON /CSWTCH/ SWTOP,SWLAST,SWSTAK(1000)
      INTEGER SWTOP
      INTEGER SWLAST
      INTEGER SWSTAK
      OUTP =0
      LEVEL =1
      LINECT(1) =1
      SBP =1
      FNAMP =2
      FNAMES(1) =0
      BP =0
      FORDEP =0
      FCNAME(1) =0
      SWTOP =0
      SWLAST =1
      RETURN
      END
      SUBROUTINE PARSE
      LOGICAL*1 LEXSTR(100)
      INTEGER LEX
      INTEGER LABVAL(400),LEXTYP(400),SP
      COMMON /STACKC/ SP,LABVAL,LEXTYP
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      INTEGER LAB,TOKEN,I
      logical*1 GNBTOK
      COMMON /CGOTO/ XFER
      INTEGER XFER
      COMMON /CFOR/ FORDEP,FORSTK(400)
      INTEGER FORDEP
      LOGICAL*1 FORSTK
      COMMON /CFNAME/ FCNAME(81)
      LOGICAL*1 FCNAME
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      COMMON /CSBUF/ SBP,SBUF(10000)
      INTEGER SBP
      LOGICAL*1 SBUF
      COMMON /CLABEL/ LABEL
      INTEGER LABEL
      COMMON /CLOOK/ LASTP,LASTT,HSHPTR(4007),TABPTR(2,22000),TABLE(8000
     *00)
      INTEGER LASTP
      INTEGER LASTT
      INTEGER HSHPTR
      INTEGER TABPTR
      LOGICAL*1 TABLE
      COMMON /CDEFIO/ BP,BUF(20000),LP,LNBUF(400)
      INTEGER BP
      LOGICAL*1 BUF
      INTEGER LP
      LOGICAL*1 LNBUF
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      CALL INIT
      SP =1
      LEXTYP(1) =-1
      TOKEN =LEX(LEXSTR)
      DO WHILE (TOKEN.NE.-1)
          IF (TOKEN.EQ.-19) THEN
            CALL IFCODE(LAB)
            ELSE IF (TOKEN.EQ.-10) THEN
            CALL DOCODE(LAB)
            ELSE IF (TOKEN.EQ.-15) THEN
            CALL WHILEC(LAB)
            ELSE IF (TOKEN.EQ.-16) THEN
            CALL FORCOD(LAB)
            ELSE IF (TOKEN.EQ.-17) THEN
            CALL REPCOD(LAB)
            ELSE IF (TOKEN.EQ.-24) THEN
            CALL SWCODE(LAB)
            ELSE IF (TOKEN.EQ.-25.OR.TOKEN.EQ.-26) THEN
              I =SP
              DO WHILE (I.GT.0)
                IF (LEXTYP(I).EQ.-24) THEN
                  GOTO 23480 ! BREAK
                  END IF
23479           I=I-1
                END DO
23480           CONTINUE
              IF (I.EQ.0) THEN
                CALL SYNERR('illegal case or default.')
                ELSE
                  CALL CASCOD(LABVAL(I),TOKEN)
                END  IF
            ELSE IF (TOKEN.EQ.-9) THEN
            CALL LABELC(LEXSTR)
            ELSE IF (TOKEN.EQ.-11) THEN
              IF (LEXTYP(SP).EQ.-19) THEN
                CALL ELSEIF(LABVAL(SP))
                ELSE
                  CALL SYNERR('illegal else.')
                END  IF
            ELSE IF (TOKEN.EQ.-27) THEN
            CALL LITRAL
            END IF
          IF (TOKEN.EQ.-19.OR.TOKEN.EQ.-11.OR.TOKEN.EQ.-15.OR.TOKEN.EQ.-
     *16.OR.TOKEN.EQ.-17.OR.TOKEN.EQ.-24.OR.TOKEN.EQ.-10.OR.TOKEN.EQ.-9.
     *OR.TOKEN.EQ.123) THEN
              SP =SP +1
              IF (SP.GT.400) THEN
                CALL BADERR('stack overflow in parser.')
                END IF
              LEXTYP(SP) =TOKEN
              LABVAL(SP) =LAB
              IF ((OPTION(57).EQ.1).AND.SP.GT.1) THEN
                  IF (LEXTYP(SP).EQ.-19.AND.LEXTYP(SP-1).EQ.-11) THEN
                   SP =SP -2
                   END IF
                END IF
            ELSE IF (TOKEN.EQ.37) THEN
              CALL OUTSTR(LEXSTR)
              TOKEN =GNBTOK(LEXSTR,100)
              CALL PBSTR(LEXSTR)
              IF (TOKEN.EQ.10) THEN
                CALL OUTDON
                END IF
            ELSE IF (TOKEN.NE.-25.AND.TOKEN.NE.-26) THEN
              IF (TOKEN.EQ.125) THEN
                  IF (LEXTYP(SP).EQ.123) THEN
                   SP =SP -1
                   ELSE IF (LEXTYP(SP).EQ.-24) THEN
                    CALL SWEND(LABVAL(SP))
                    SP =SP -1
                   ELSE
                    CALL SYNERR('illegal right brace.')
                   END  IF
                ELSE IF (TOKEN.EQ.-14) THEN
                CALL OTHERC(LEXSTR)
                ELSE IF (TOKEN.EQ.-8.OR.TOKEN.EQ.-13) THEN
                CALL BRKNXT(SP,LEXTYP,LABVAL,TOKEN)
                ELSE IF (TOKEN.EQ.-20) THEN
                CALL RETCOD
                ELSE IF (TOKEN.EQ.-23) THEN
                CALL STRDCL
                END IF
              TOKEN =LEX(LEXSTR)
              CALL PBSTR(LEXSTR)
              CALL UNSTAK(SP,LEXTYP,LABVAL,TOKEN)
            END IF
23462   TOKEN=LEX(LEXSTR)
        END DO
23463   CONTINUE
      IF (SP.NE.1) THEN
        CALL SYNERR('unexpected EOF.')
        END IF
      RETURN
      END
      SUBROUTINE UNSTAK(SP,LEXTYP,LABVAL,TOKEN)
      INTEGER LABVAL(400),LEXTYP(400),SP,TOKEN
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      LOGICAL*1 SIF(3)
      LOGICAL*1 SELSE(4)
      DATA SIF(1)/73/,SIF(2)/70/,SIF(3)/0/
      DATA SELSE(1)/32/,SELSE(2)/73/,SELSE(3)/70/,SELSE(4)/0/
      DO WHILE (SP.GT.1)
          IF (LEXTYP(SP).EQ.123.OR.LEXTYP(SP).EQ.-24) THEN
            GOTO 23525 ! BREAK
            END IF
          IF (LEXTYP(SP).EQ.-19.AND.TOKEN.EQ.-11) THEN
            GOTO 23525 ! BREAK
            END IF
          IF (LEXTYP(SP).EQ.-19) THEN
              IF ((OPTION(57).EQ.1)) THEN
                CALL OUTEND (SIF)
                ELSE
                  CALL OUTCON(LABVAL(SP))
                END  IF
            ELSE IF (LEXTYP(SP).EQ.-11) THEN
              IF (SP.GT.2) THEN
                SP =SP -1
                END IF
              IF ((OPTION(57).EQ.1)) THEN
                CALL OUTEND (SELSE)
                ELSE
                  CALL OUTCON(LABVAL(SP)+1)
                END  IF
            ELSE IF (LEXTYP(SP).EQ.-10) THEN
            CALL DOSTAT(LABVAL(SP))
            ELSE IF (LEXTYP(SP).EQ.-15) THEN
            CALL WHILES(LABVAL(SP))
            ELSE IF (LEXTYP(SP).EQ.-16) THEN
            CALL FORS(LABVAL(SP))
            ELSE IF (LEXTYP(SP).EQ.-17) THEN
            CALL UNTILS(LABVAL(SP),TOKEN)
            END IF
23524   SP=SP-1
        END DO
23525   CONTINUE
      RETURN
      END
      SUBROUTINE ULSTAL(NAME,DEFN)
      LOGICAL*1 NAME(100),DEFN(100)
      CALL INSTAL(NAME,DEFN)
      RETURN
      END
      SUBROUTINE REPCOD(LAB)
      INTEGER LABGEN
      INTEGER LAB
      CALL OUTCON(0)
      LAB =LABGEN(3)
      CALL OUTCON(LAB)
      LAB =LAB +1
      RETURN
      END
      SUBROUTINE UNTILS(LAB,TOKEN)
      LOGICAL*1 PTOKEN(100)
      INTEGER LEX
      INTEGER JUNK,LAB,TOKEN
      COMMON /CGOTO/ XFER
      INTEGER XFER
      XFER =0
      CALL OUTNUM(LAB)
      IF (TOKEN.EQ.-18) THEN
          JUNK =LEX(PTOKEN)
          CALL IFGO(LAB-1)
        ELSE
          CALL OUTGO(LAB-1)
        END  IF
      CALL OUTCON(LAB+1)
      RETURN
      END
      SUBROUTINE RETCOD
      LOGICAL*1 TOKEN(100),GNBTOK,T
      COMMON /CFNAME/ FCNAME(81)
      LOGICAL*1 FCNAME
      COMMON /CGOTO/ XFER
      INTEGER XFER
      LOGICAL*1 SRET(7)
      DATA SRET(1)/114/,SRET(2)/101/,SRET(3)/116/,SRET(4)/117/,SRET(5)/1
     *14/,SRET(6)/110/,SRET(7)/0/
      T =GNBTOK(TOKEN,100)
      IF (T.NE.10.AND.T.NE.59.AND.T.NE.125) THEN
          CALL PBSTR(TOKEN)
          CALL OUTTAB
          CALL OUTSTR(FCNAME)
          CALL OUTCH(61)
          CALL EATUP
          CALL OUTDON
        ELSE IF (T.EQ.125) THEN
        CALL PBSTR(TOKEN)
        END IF
      CALL OUTTAB
      CALL OUTSTR(SRET)
      CALL OUTDON
      XFER =1
      RETURN
      END
      SUBROUTINE STRDCL
      LOGICAL*1 T,TOKEN(100),GNBTOK,ESC
      INTEGER I,J,K,N,LEN
      INTEGER LENGTH,CTOI,LEX,ELENTH
      LOGICAL*1 DCHAR(100)
      COMMON /CSBUF/ SBP,SBUF(10000)
      INTEGER SBP
      LOGICAL*1 SBUF
      LOGICAL*1 CHAR(11)
      LOGICAL*1 DAT(6)
      LOGICAL*1 EOSS(5)
      DATA CHAR(1)/99/,CHAR(2)/104/,CHAR(3)/97/,CHAR(4)/114/,CHAR(5)/97/
     *,CHAR(6)/99/,CHAR(7)/116/,CHAR(8)/101/,CHAR(9)/114/,CHAR(10)/47/,C
     *HAR(11)/0/
      DATA DAT(1)/100/,DAT(2)/97/,DAT(3)/116/,DAT(4)/97/,DAT(5)/32/,DAT(
     *6)/0/
      DATA EOSS(1)/69/,EOSS(2)/79/,EOSS(3)/83/,EOSS(4)/47/,EOSS(5)/0/
      T =GNBTOK(TOKEN,100)
      IF (T.NE.-9) THEN
        CALL SYNERR('missing string token.')
        END IF
      CALL OUTTAB
      CALL PBSTR(CHAR)
23556 CONTINUE
          T =GNBTOK(DCHAR,100)
          IF (T.EQ.47) THEN
            GOTO 23558 ! BREAK
            END IF
          CALL OUTSTR (DCHAR)
23557   GOTO 23556
23558   CONTINUE
      CALL OUTCH(32)
      CALL OUTSTR(TOKEN)
      CALL ADDSTR(TOKEN,SBUF,SBP,10000)
      CALL ADDCHR(0,SBUF,SBP,10000)
      IF (GNBTOK(TOKEN,100).NE.40) THEN
          LEN =ELENTH(TOKEN) +1
          IF (TOKEN(1).EQ.39.OR.TOKEN(1).EQ.34.OR.TOKEN(1).EQ.37) THEN
            LEN =LEN -2
            END IF
        ELSE
            T =GNBTOK(TOKEN,100)
            I =1
            LEN =CTOI(TOKEN,I)
            IF (TOKEN(I).NE.0) THEN
              CALL SYNERR('invalid string size.')
              END IF
            IF (GNBTOK(TOKEN,100).NE.41) THEN
              CALL SYNERR('missing right paren.')
              ELSE
                T =GNBTOK(TOKEN,100)
              END  IF
        END  IF
      CALL OUTCH(40)
      CALL OUTNUM(LEN)
      CALL OUTCH(41)
      CALL OUTDON
      IF (TOKEN(1).EQ.39.OR.TOKEN(1).EQ.34.OR.TOKEN(1).EQ.37) THEN
          LEN =LENGTH(TOKEN)
          TOKEN(LEN) =0
          CALL ADDSTR(TOKEN(2),SBUF,SBP,10000)
        ELSE
          CALL ADDSTR(TOKEN,SBUF,SBP,10000)
        END  IF
      CALL ADDCHR(0,SBUF,SBP,10000)
      T =LEX(TOKEN)
      CALL PBSTR(TOKEN)
      IF (T.NE.-23) THEN
          I =1
          DO WHILE (I.LT.SBP)
              CALL OUTTAB
              CALL OUTSTR(DAT)
              K =1
              J =I +LENGTH(SBUF(I)) +1
              DO WHILE (.TRUE.)
                  IF (K.GT.1) THEN
                   CALL OUTCH(44)
                   END IF
                  CALL OUTSTR(SBUF(I))
                  CALL OUTCH(40)
                  CALL OUTNUM(K)
                  CALL OUTCH(41)
                  CALL OUTCH(47)
                  IF (SBUF(J).EQ.0) THEN
                   GOTO 23578 ! BREAK
                   END IF
                  N =ESC(SBUF,J)
                  CALL OUTNUM (N)
                  CALL OUTCH(47)
                  K =K +1
23577           J=J+1
                END DO
23578           CONTINUE
              CALL PBSTR(EOSS)
23583         CONTINUE
                  T =GNBTOK(TOKEN,100)
                  CALL OUTSTR(TOKEN)
23584           IF(.NOT.(T.EQ.47))GOTO 23583
23585           CONTINUE
              CALL OUTDON
23574       I=J+1
            END DO
23575       CONTINUE
          SBP =1
        END IF
      RETURN
      END
      SUBROUTINE ADDCHR(C,BUF,BP,MAXSIZ)
      INTEGER BP,MAXSIZ
      LOGICAL*1 C,BUF(100)
      IF (BP.GT.MAXSIZ) THEN
        CALL BADERR('buffer overflow.')
        END IF
      BUF(BP) =C
      BP =BP +1
      RETURN
      END
      SUBROUTINE ADDSTR(S,BUF,BP,MAXSIZ)
      LOGICAL*1 S(100),BUF(100)
      INTEGER BP,MAXSIZ
      INTEGER I
      I =1
      DO WHILE (S(I).NE.0)
        CALL ADDCHR(S(I),BUF,BP,MAXSIZ)
23589   I=I+1
        END DO
23590   CONTINUE
      RETURN
      END
      INTEGER FUNCTION ALLDIG(STR)
      LOGICAL*1 TYPE
      LOGICAL*1 STR(100)
      INTEGER I
      ALLDIG =0
      IF (STR(1).EQ.0) THEN
        RETURN
        END IF
      I =1
      DO WHILE (STR(I).NE.0)
        IF (TYPE(STR(I)).NE.2) THEN
          RETURN
          END IF
23594   I=I+1
        END DO
23595   CONTINUE
      ALLDIG =1
      RETURN
      END
      INTEGER FUNCTION LABGEN(N)
      INTEGER N
      COMMON /CLABEL/ LABEL
      INTEGER LABEL
      LABGEN =LABEL
      LABEL =LABEL +N
      RETURN
      END
      SUBROUTINE SKPBLK(FD)
      INTEGER FD
      LOGICAL*1 C,NGETCH
      C =NGETCH(C,FD)
      DO WHILE (C.EQ.32.OR.C.EQ.9.OR.C.EQ.12)
23599   C=NGETCH(C,FD)
        END DO
23600   CONTINUE
      CALL PUTBAK(C)
      RETURN
      END
      SUBROUTINE CASCOD(LAB,TOKEN)
      INTEGER LAB,TOKEN
      INTEGER T,L,LB,UB,I,J
      LOGICAL*1 TOK(100)
      INTEGER CASLAB,LABGEN
      logical*1 GNBTOK
      COMMON /CSWTCH/ SWTOP,SWLAST,SWSTAK(1000)
      INTEGER SWTOP
      INTEGER SWLAST
      INTEGER SWSTAK
      COMMON /CGOTO/ XFER
      INTEGER XFER
      IF (SWTOP.LE.0) THEN
          CALL SYNERR('illegal case or default.')
          RETURN
        END IF
      CALL OUTGO(LAB+1)
      XFER =1
      L =LABGEN(1)
      IF (TOKEN.EQ.-25) THEN
          DO WHILE (CASLAB(LB,T).NE.-1)
              UB =LB
              IF (T.EQ.45) THEN
                JUNK =CASLAB(UB,T)
                END IF
              IF (LB.GT.UB) THEN
                  CALL SYNERR('illegal range in case label.')
                  UB =LB
                END IF
              IF (SWLAST+3.GT.1000) THEN
                CALL BADERR('switch table overflow.')
                END IF
              ISDUP =0
              I =SWTOP +3
              DO WHILE (I.LT.SWLAST)
                IF (LB.LE.SWSTAK(I)) THEN
                  GOTO 23615 ! BREAK
                  ELSE IF (LB.LE.SWSTAK(I+1)) THEN
                   CALL SYNERR('duplicate case label!.')
                   ISDUP =1
                  END IF
23614           I=I+3
                END DO
23615           CONTINUE
              IF (I.LT.SWLAST.AND.UB.GE.SWSTAK(I)) THEN
                  CALL SYNERR('duplicate case label !.')
                  ISDUP =1
                END IF
              IF (ISDUP.EQ.0) THEN
                  J =SWLAST
                  DO WHILE (J.GT.I)
                   SWSTAK(J+2) =SWSTAK(J-1)
23625              J=J-1
                   END DO
23626              CONTINUE
                  SWSTAK(I) =LB
                  SWSTAK(I+1) =UB
                  SWSTAK(I+2) =L
                  SWSTAK(SWTOP+1) =SWSTAK(SWTOP+1) +1
                  SWLAST =SWLAST +3
                END IF
              IF (T.EQ.58) THEN
                GOTO 23606 ! BREAK
                ELSE IF (T.NE.44) THEN
                CALL SYNERR('illegal case syntax.')
                END IF
23605       END DO
23606       CONTINUE
        ELSE
            T =GNBTOK(TOK,100)
            IF (SWSTAK(SWTOP+2).NE.0) THEN
              CALL SYNERR('multiple defaults in switch statement.')
              ELSE
                SWSTAK(SWTOP+2) =L
              END  IF
        END  IF
      IF (T.EQ.-1) THEN
        CALL SYNERR('unexpected EOF.')
        ELSE IF (T.NE.58) THEN
        CALL SYNERR('missing colon in case or default label.')
        END IF
      XFER =0
      CALL OUTCON(L)
      RETURN
      END
      INTEGER FUNCTION CASLAB(N,T)
      INTEGER N,T
      LOGICAL*1 TOK(100)
      INTEGER I,S
      INTEGER CTOI
      logical*1 GNBTOK
      T =GNBTOK(TOK,100)
      DO WHILE (T.EQ.10)
        T =GNBTOK(TOK,100)
23637   END DO
23638   CONTINUE
      IF (T.EQ.-1) THEN
        CASLAB=(T)
        RETURN
        END IF
      IF (T.EQ.45) THEN
        S =-1
        ELSE
          S =+1
        END  IF
      IF (T.EQ.45.OR.T.EQ.43) THEN
        T =GNBTOK(TOK,100)
        END IF
      IF (T.NE.2) THEN
          CALL SYNERR('invalid case label.')
          N =0
        ELSE
            I =1
            N =S*CTOI(TOK,I)
        END  IF
      T =GNBTOK(TOK,100)
      DO WHILE (T.EQ.10)
        T =GNBTOK(TOK,100)
23647   END DO
23648   CONTINUE
      CASLAB=(T)
      RETURN
      END
      SUBROUTINE SWCODE(LAB)
      INTEGER LAB
      INTEGER SW_COUNT
      COMMON /SWCOM/ SW_COUNT
      LOGICAL*1 TOK(100)
      INTEGER LABGEN
      logical*1 GNBTOK
      COMMON /CSWTCH/ SWTOP,SWLAST,SWSTAK(1000)
      INTEGER SWTOP
      INTEGER SWLAST
      INTEGER SWSTAK
      COMMON /CGOTO/ XFER
      INTEGER XFER
      LAB =LABGEN(2)
      IF (SWLAST+3.GT.1000) THEN
        CALL BADERR('switch table overflow.')
        END IF
      SWSTAK(SWLAST) =SWTOP
      SWSTAK(SWLAST+1) =0
      SWSTAK(SWLAST+2) =0
      SWTOP =SWLAST
      SWLAST =SWLAST +3
      XFER =0
      SW_COUNT =SW_COUNT +1
      CALL OUTTAB
      CALL SWVAR(LAB)
      CALL OUTCH(61)
      CALL BALPAR
      CALL OUTDON
      CALL OUTGO(LAB)
      XFER =1
      DO WHILE (GNBTOK(TOK,100).EQ.10)
23651   END DO
23652   CONTINUE
      IF (TOK(1).NE.123) THEN
          CALL SYNERR('missing left brace in switch statement.')
          CALL PBSTR(TOK)
        END IF
      RETURN
      END
      SUBROUTINE SWEND(LAB)
      INTEGER SW_COUNT
      COMMON /SWCOM/ SW_COUNT
      INTEGER LAB
      INTEGER LB,UB,N,I,J
      COMMON /CSWTCH/ SWTOP,SWLAST,SWSTAK(1000)
      INTEGER SWTOP
      INTEGER SWLAST
      INTEGER SWSTAK
      COMMON /CGOTO/ XFER
      INTEGER XFER
      LOGICAL*1 SIF(4)
      LOGICAL*1 SLT(10)
      LOGICAL*1 SGT(5)
      LOGICAL*1 SGOTO(6)
      LOGICAL*1 SEQ(5)
      LOGICAL*1 SGE(5)
      LOGICAL*1 SLE(5)
      LOGICAL*1 SAND(6)
      DATA SIF(1)/105/,SIF(2)/102/,SIF(3)/40/,SIF(4)/0/
      DATA SLT(1)/46/,SLT(2)/108/,SLT(3)/116/,SLT(4)/46/,SLT(5)/49/,SLT(
     *6)/46/,SLT(7)/111/,SLT(8)/114/,SLT(9)/46/,SLT(10)/0/
      DATA SGT(1)/46/,SGT(2)/103/,SGT(3)/116/,SGT(4)/46/,SGT(5)/0/
      DATA SGOTO(1)/103/,SGOTO(2)/111/,SGOTO(3)/116/,SGOTO(4)/111/,SGOTO
     *(5)/40/,SGOTO(6)/0/
      DATA SEQ(1)/46/,SEQ(2)/101/,SEQ(3)/113/,SEQ(4)/46/,SEQ(5)/0/
      DATA SGE(1)/46/,SGE(2)/103/,SGE(3)/101/,SGE(4)/46/,SGE(5)/0/
      DATA SLE(1)/46/,SLE(2)/108/,SLE(3)/101/,SLE(4)/46/,SLE(5)/0/
      DATA SAND(1)/46/,SAND(2)/97/,SAND(3)/110/,SAND(4)/100/,SAND(5)/46/
     *,SAND(6)/0/
      LB =SWSTAK(SWTOP+3)
      UB =SWSTAK(SWLAST-2)
      N =SWSTAK(SWTOP+1)
      CALL OUTGO(LAB+1)
      IF (SWSTAK(SWTOP+2).EQ.0) THEN
        SWSTAK(SWTOP+2) =LAB +1
        END IF
      XFER =0
      CALL OUTCON(LAB)
      IF (N.GE.3.AND.UB-LB+1.LT.2*N) THEN
          IF (LB.NE.1) THEN
              CALL OUTTAB
              CALL SWVAR(LAB)
              CALL OUTCH(61)
              CALL SWVAR(LAB)
              IF (LB.LT.1) THEN
                CALL OUTCH(43)
                END IF
              CALL OUTNUM(-LB +1)
              CALL OUTDON
            END IF
          CALL OUTTAB
          CALL OUTSTR(SIF)
          CALL SWVAR(LAB)
          CALL OUTSTR(SLT)
          CALL SWVAR(LAB)
          CALL OUTSTR(SGT)
          CALL OUTNUM(UB -LB +1)
          CALL OUTCH(41)
          CALL OUTGO(SWSTAK(SWTOP+2))
          CALL OUTTAB
          CALL OUTSTR(SGOTO)
          J =LB
          I =SWTOP +3
          DO WHILE (I.LT.SWLAST)
              DO WHILE (J.LT.SWSTAK(I))
                  CALL OUTNUM(SWSTAK(SWTOP+2))
                  CALL OUTCH(44)
23667           J=J+1
                END DO
23668           CONTINUE
              J =SWSTAK(I+1) -SWSTAK(I)
              DO WHILE (J.GE.0)
                  CALL OUTNUM(SWSTAK(I+2))
                  IF (J.GT.0) THEN
                   CALL OUTCH(44)
                   END IF
23670           J=J-1
                END DO
23671           CONTINUE
              J =SWSTAK(I+1) +1
              IF (I.LT.SWLAST-3) THEN
                CALL OUTCH(44)
                END IF
23664       I=I+3
            END DO
23665       CONTINUE
          CALL OUTCH(41)
          CALL OUTCH(44)
          CALL SWVAR(LAB)
          CALL OUTDON
        ELSE IF (N.GT.0) THEN
          I =SWTOP +3
          DO WHILE (I.LT.SWLAST)
              CALL OUTTAB
              CALL OUTSTR(SIF)
              CALL SWVAR(LAB)
              IF (SWSTAK(I).EQ.SWSTAK(I+1)) THEN
                  CALL OUTSTR(SEQ)
                  CALL OUTNUM(SWSTAK(I))
                ELSE
                   CALL OUTSTR(SGE)
                   CALL OUTNUM(SWSTAK(I))
                   CALL OUTSTR(SAND)
                   CALL SWVAR(LAB)
                   CALL OUTSTR(SLE)
                   CALL OUTNUM(SWSTAK(I+1))
                END  IF
              CALL OUTCH(41)
              CALL OUTGO(SWSTAK(I+2))
23679       I=I+3
            END DO
23680       CONTINUE
          IF (LAB+1.NE.SWSTAK(SWTOP+2)) THEN
            CALL OUTGO(SWSTAK(SWTOP+2))
            END IF
        END IF
      CALL OUTCON(LAB+1)
      SWLAST =SWTOP
      SWTOP =SWSTAK(SWTOP)
      SW_COUNT =SW_COUNT -1
      RETURN
      END
      SUBROUTINE SWVAR(LAB)
      INTEGER LAB
      INTEGER SW_COUNT
      COMMON /SWCOM/ SW_COUNT
      COMMON /CSWTCH/ SWTOP,SWLAST,SWSTAK(1000)
      INTEGER SWTOP
      INTEGER SWLAST
      INTEGER SWSTAK
      LOGICAL*1 SNAME(10)
      DATA SNAME(1)/73/,SNAME(2)/95/,SNAME(3)/83/,SNAME(4)/87/,SNAME(5)/
     *73/,SNAME(6)/84/,SNAME(7)/67/,SNAME(8)/72/,SNAME(9)/95/,SNAME(10)/
     *0/
      CALL OUTSTR(SNAME)
      CALL OUTNUM(SW_COUNT)
      RETURN
      END
      SUBROUTINE WHILEC(LAB)
      INTEGER LABGEN
      INTEGER LAB
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      CALL OUTCON(0)
      LAB =LABGEN(2)
      IF ((OPTION(57).EQ.1)) THEN
          CALL DOWHILE (LAB)
        ELSE
            CALL OUTNUM(LAB)
            CALL IFGO(LAB+1)
        END  IF
      RETURN
      END
      SUBROUTINE WHILES(LAB)
      INTEGER LAB
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      LOGICAL*1 SDO(3)
      DATA SDO(1)/100/,SDO(2)/111/,SDO(3)/0/
      IF ((OPTION(57).EQ.1)) THEN
          CALL OUTCON(0)
          CALL OUTNUM(LAB)
          CALL OUTEND(SDO)
        ELSE
            CALL OUTGO(LAB)
        END  IF
      CALL OUTCON(LAB+1)
      RETURN
      END
      SUBROUTINE DOWHILE (LAB)
      INTEGER LAB
      LOGICAL*1 SDO(10)
      DATA SDO(1)/100/,SDO(2)/111/,SDO(3)/32/,SDO(4)/119/,SDO(5)/104/,SD
     *O(6)/105/,SDO(7)/108/,SDO(8)/101/,SDO(9)/32/,SDO(10)/0/
      CALL OUTTAB
      CALL OUTSTR(SDO)
      CALL BALPAR
      CALL OUTDON
      RETURN
      END
      SUBROUTINE LITRAL
      INTEGER GETLIN,INDEXX
      INTEGER I
      COMMON /COUTLN/ OUTP,OUTBUF(400)
      INTEGER OUTP
      LOGICAL*1 OUTBUF
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      IF (OUTP.GT.0) THEN
        CALL OUTDON
        END IF
      I =GETLIN (OUTBUF,INFILE(LEVEL))
      DO WHILE (GETLIN(OUTBUF,INFILE(LEVEL)).NE.-1)
          I =1
          CALL SKIPBL (OUTBUF,I)
          IF (OUTBUF(I).EQ.-37) THEN
            GOTO 23692 ! BREAK
            END IF
          CALL PUTLIN (OUTBUF,6)
          LINECT(LEVEL) =LINECT(LEVEL) +1
23691   END DO
23692   CONTINUE
      OUTP =0
      RETURN
      END
      SUBROUTINE BRKNXT(SP,LEXTYP,LABVAL,TOKEN)
      INTEGER LABVAL(400),LEXTYP(400),SP,TOKEN
      INTEGER I,N,ALLDIG,CTOI
      LOGICAL*1 T,PTOKEN(100),GNBTOK
      COMMON /CGOTO/ XFER
      INTEGER XFER
      LOGICAL*1 SBREAK(8)
      LOGICAL*1 SNEXT(7)
      DATA SBREAK(1)/98/,SBREAK(2)/114/,SBREAK(3)/101/,SBREAK(4)/97/,SBR
     *EAK(5)/107/,SBREAK(6)/32/,SBREAK(7)/110/,SBREAK(8)/0/
      DATA SNEXT(1)/110/,SNEXT(2)/101/,SNEXT(3)/120/,SNEXT(4)/116/,SNEXT
     *(5)/32/,SNEXT(6)/110/,SNEXT(7)/0/
      SNEXT(6) =0
      SBREAK(7) =0
      N =0
      T =GNBTOK(PTOKEN,100)
      IF (ALLDIG(PTOKEN).EQ.1) THEN
          I =1
          N =CTOI(PTOKEN,I) -1
          SNEXT(6) =N +49
          SBREAK(7) =N +49
        ELSE IF (T.NE.59) THEN
        CALL PBSTR(PTOKEN)
        END IF
      I =SP
      DO WHILE (I.GT.0)
        IF (LEXTYP(I).EQ.-15.OR.LEXTYP(I).EQ.-10.OR.LEXTYP(I).EQ.-16.OR.
     *LEXTYP(I).EQ.-17) THEN
            IF (N.GT.0) THEN
                N =N -1
                GOTO 23700 ! NEXT
              ELSE IF (TOKEN.EQ.-8) THEN
                XFER =0
                CALL OUTGOC(LABVAL(I)+1,SBREAK)
              ELSE
                  XFER =0
                  CALL OUTGOC(LABVAL(I),SNEXT)
              END  IF
            XFER =1
            RETURN
          END IF
23700   I=I-1
        END DO
23701   CONTINUE
      IF (TOKEN.EQ.-8) THEN
        CALL SYNERR('illegal break.')
        ELSE
          CALL SYNERR('illegal next.')
        END  IF
      RETURN
      END
      LOGICAL*1 FUNCTION DEFTOK(TOKEN,TOKSIZ,FD)
      LOGICAL*1 TOKEN(100)
      INTEGER TOKSIZ,FD
      LOGICAL*1 GTOK
      INTEGER LOOKUP,PUSH,IFPARM
      INTEGER AP,ARGSTK(100),CALLST(50),NLB,PLEV(50),IFL
      LOGICAL*1 T,C,DEFN(10001),BALP(3),MDEFN(10001)
      COMMON /CMACRO/ CP,EP,EVALST(10000)
      INTEGER CP
      INTEGER EP
      LOGICAL*1 EVALST
      LOGICAL*1 UNDEFSTR(10)
      DATA UNDEFSTR(1)/42/,UNDEFSTR(2)/42/,UNDEFSTR(3)/85/,UNDEFSTR(4)/7
     *8/,UNDEFSTR(5)/68/,UNDEFSTR(6)/69/,UNDEFSTR(7)/70/,UNDEFSTR(8)/42/
     *,UNDEFSTR(9)/42/,UNDEFSTR(10)/0/
      DATA BALP/40,41,0/
      CP =0
      AP =1
      EP =1
      T=GTOK(TOKEN,TOKSIZ,FD)
      DO WHILE (T.NE.-1)
          IF (T.EQ.-9) THEN
              CALL FOLD (TOKEN)
              IF (LOOKUP(TOKEN,DEFN).EQ.0) THEN
                IF (CP.EQ.0) THEN
                  GOTO 23712 ! BREAK
                  ELSE
                   CALL PUTTOK(TOKEN)
                  END  IF
                ELSE IF (DEFN(1).EQ.-4) THEN
                  CALL GETDEF(TOKEN,TOKSIZ,DEFN,10000,FD)
                  CALL INSTAL(TOKEN,DEFN)
                  CALL CHECK_RAT_OPTIONS (TOKEN,.TRUE.)
                ELSE IF (DEFN(1).EQ.-29) THEN
                  CALL GETUDEF(TOKEN,TOKSIZ,DEFN,10000,FD)
                  CALL FOLD (TOKEN)
                  IF (LOOKUP(TOKEN,MDEFN).EQ.1) THEN
                    CALL INSTAL(TOKEN,undefstr)
                    CALL CHECK_RAT_OPTIONS (TOKEN,.FALSE.)
                   END IF
                ELSE IF (DEFN(1).EQ.-15.OR.DEFN(1).EQ.-16) THEN
                  C =DEFN(1)
                  CALL GETDEF(TOKEN,TOKSIZ,DEFN,10000,FD)
                  CALL FOLD (TOKEN)
                  IFL =LOOKUP(TOKEN,MDEFN)
                  IF ((IFL.EQ.1.AND.C.EQ.-15).OR.(IFL.EQ.0.AND.C.EQ.-16)
     *) THEN
                   CALL PBSTR(DEFN)
                   END IF
                ELSE
                   CP =CP +1
                   IF (CP.GT.50) THEN
                    CALL BADERR('call stack overflow.')
                    END IF
                   CALLST(CP) =AP
                   AP =PUSH(EP,ARGSTK,AP)
                   CALL PUTTOK(DEFN)
                   CALL PUTCHR(0)
                   AP =PUSH(EP,ARGSTK,AP)
                   CALL PUTTOK(TOKEN)
                   CALL PUTCHR(0)
                   AP =PUSH(EP,ARGSTK,AP)
                   T =GTOK(TOKEN,TOKSIZ,FD)
                   IF (T.EQ.32) THEN
                    T =GTOK(TOKEN,TOKSIZ,FD)
                    END IF
                   CALL PBSTR(TOKEN)
                   IF (T.NE.40) THEN
                    CALL PBSTR(BALP)
                    ELSE IF (IFPARM(DEFN).EQ.0) THEN
                    CALL PBSTR(BALP)
                    END IF
                   PLEV(CP) =0
                END  IF
            ELSE IF (T.EQ.-10) THEN
              NLB =1
23739         CONTINUE
                  T =GTOK(TOKEN,TOKSIZ,FD)
                  IF (T.EQ.-10) THEN
                   NLB =NLB +1
                   ELSE IF (T.EQ.-11) THEN
                    NLB =NLB -1
                    IF (NLB.EQ.0) THEN
                     GOTO 23741 ! BREAK
                     END IF
                   ELSE IF (T.EQ.-1) THEN
                   CALL BADERR('EOF in string.')
                   END IF
                  CALL PUTTOK(TOKEN)
23740           GOTO 23739
23741           CONTINUE
            ELSE IF (CP.EQ.0) THEN
            GOTO 23712 ! BREAK
            ELSE IF (T.EQ.40) THEN
              IF (PLEV(CP).GT.0) THEN
                CALL PUTTOK(TOKEN)
                END IF
              PLEV(CP) =PLEV(CP) +1
            ELSE IF (T.EQ.41) THEN
              PLEV(CP) =PLEV(CP) -1
              IF (PLEV(CP).GT.0) THEN
                CALL PUTTOK(TOKEN)
                ELSE
                   CALL PUTCHR(0)
                   CALL EVALR(ARGSTK,CALLST(CP),AP-1)
                   AP =CALLST(CP)
                   EP =ARGSTK(AP)
                   CP =CP -1
                END  IF
            ELSE IF (T.EQ.44.AND.PLEV(CP).EQ.1) THEN
              CALL PUTCHR(0)
              AP =PUSH(EP,ARGSTK,AP)
            ELSE
              CALL PUTTOK(TOKEN)
            END  IF
23711   T=GTOK(TOKEN,TOKSIZ,FD)
        END DO
23712   CONTINUE
      DEFTOK =T
      IF (T.EQ.-9) THEN
        CALL FOLD(TOKEN)
        END IF
      RETURN
      END
      SUBROUTINE CHECK_RAT_OPTIONS (TOKEN,FLAG)
      LOGICAL*1 TOKEN(100)
      LOGICAL FLAG
      INTEGER SET_TO
      INTEGER EQUAL
      LOGICAL*1 S_INDENT(12)
      LOGICAL*1 S_MACRO(11)
      LOGICAL*1 S_VERBOSE(13)
      LOGICAL*1 S_HOLLERITH(15)
      LOGICAL*1 S_F90(9)
      DATA S_INDENT(1)/36/,S_INDENT(2)/114/,S_INDENT(3)/97/,S_INDENT(4)/
     *116/,S_INDENT(5)/36/,S_INDENT(6)/105/,S_INDENT(7)/110/,S_INDENT(8)
     */100/,S_INDENT(9)/101/,S_INDENT(10)/110/,S_INDENT(11)/116/,S_INDEN
     *T(12)/0/
      DATA S_MACRO(1)/36/,S_MACRO(2)/114/,S_MACRO(3)/97/,S_MACRO(4)/116/
     *,S_MACRO(5)/36/,S_MACRO(6)/109/,S_MACRO(7)/97/,S_MACRO(8)/99/,S_MA
     *CRO(9)/114/,S_MACRO(10)/111/,S_MACRO(11)/0/
      DATA S_VERBOSE(1)/36/,S_VERBOSE(2)/114/,S_VERBOSE(3)/97/,S_VERBOSE
     *(4)/116/,S_VERBOSE(5)/36/,S_VERBOSE(6)/118/,S_VERBOSE(7)/101/,S_VE
     *RBOSE(8)/114/,S_VERBOSE(9)/98/,S_VERBOSE(10)/111/,S_VERBOSE(11)/11
     *5/,S_VERBOSE(12)/101/,S_VERBOSE(13)/0/
      DATA S_HOLLERITH(1)/36/,S_HOLLERITH(2)/114/,S_HOLLERITH(3)/97/,S_H
     *OLLERITH(4)/116/,S_HOLLERITH(5)/36/,S_HOLLERITH(6)/104/,S_HOLLERIT
     *H(7)/111/,S_HOLLERITH(8)/108/,S_HOLLERITH(9)/108/,S_HOLLERITH(10)/
     *101/,S_HOLLERITH(11)/114/,S_HOLLERITH(12)/105/,S_HOLLERITH(13)/116
     */,S_HOLLERITH(14)/104/,S_HOLLERITH(15)/0/
      DATA S_F90(1)/36/,S_F90(2)/114/,S_F90(3)/97/,S_F90(4)/116/,S_F90(5
     *)/36/,S_F90(6)/102/,S_F90(7)/57/,S_F90(8)/48/,S_F90(9)/0/
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      IF (TOKEN(1).EQ.36) THEN
          IF (FLAG) THEN
            SET_TO =1
            ELSE
              SET_TO =.NOT. 1
            END  IF
          IF (EQUAL(TOKEN,S_INDENT).EQ.1) THEN
            OPTION(73) =SET_TO
            ELSE IF (EQUAL(TOKEN,S_MACRO).EQ.1) THEN
            OPTION(77) =SET_TO
            ELSE IF (EQUAL(TOKEN,S_VERBOSE).EQ.1) THEN
            OPTION(86) =SET_TO
            ELSE IF (EQUAL(TOKEN,S_HOLLERITH).EQ.1) THEN
            OPTION(72) =SET_TO
            ELSE IF (EQUAL(TOKEN,S_F90).EQ.1) THEN
            OPTION(57) =SET_TO
            END IF
        END IF
      RETURN
      END
      SUBROUTINE DOARTH(ARGSTK,I,J)
      INTEGER CTOI
      INTEGER ARGSTK(100),I,J,K,L,M,LEN
      LOGICAL*1 OP,CHAR
      COMMON /CMACRO/ CP,EP,EVALST(10000)
      INTEGER CP
      INTEGER EP
      LOGICAL*1 EVALST
      K =ARGSTK(I+2)
      L =ARGSTK(I+4)
      OP =EVALST(ARGSTK(I+3))
      IF (OP.EQ.43) THEN
        CALL PBNUM(CTOI(EVALST,K)+CTOI(EVALST,L))
        ELSE IF (OP.EQ.45) THEN
        CALL PBNUM(CTOI(EVALST,K)-CTOI(EVALST,L))
        ELSE IF (OP.EQ.42) THEN
        CALL PBNUM(CTOI(EVALST,K)*CTOI(EVALST,L))
        ELSE IF (OP.EQ.47) THEN
        CALL PBNUM(CTOI(EVALST,K)/CTOI(EVALST,L))
        ELSE IF (OP.EQ.63) THEN
          M =INDEXX(EVALST(K),EVALST(L))
          CALL PBNUM(M)
        ELSE IF (OP.EQ.58) THEN
          LEN =LENGTH(EVALST(L))
          DO WHILE (LEN.GT.0)
              CHAR =EVALST(L+LEN-1)
              IF (CHAR.NE.32) THEN
                CALL PUTBAK(CHAR)
                END IF
23791       LEN=LEN-1
            END DO
23792       CONTINUE
          LEN =LENGTH(EVALST(K))
          DO WHILE (LEN.GT.0)
              CHAR =EVALST(K+LEN-1)
              IF (CHAR.NE.32) THEN
                CALL PUTBAK(CHAR)
                END IF
23796       LEN=LEN-1
            END DO
23797       CONTINUE
        ELSE IF (OP.EQ.92) THEN
          LEN =LENGTH(EVALST(L))
          DO WHILE (LEN.GT.0)
              CHAR =EVALST(L+LEN-1)
              IF (CHAR.NE.32) THEN
                CALL PUTBAK(CHAR)
                END IF
23803       LEN=LEN-1
            END DO
23804       CONTINUE
          LEN =LENGTH(EVALST(K))
          DO WHILE (LEN.GT.0)
              CHAR =EVALST(K+LEN-1)
              IF (CHAR.NE.32.AND.CHAR.NE.10) THEN
                  CALL PUTBAK(CHAR)
                END IF
23808       LEN=LEN-1
            END DO
23809       CONTINUE
        ELSE
            CALL REMARK_V('arith error')
        END  IF
      RETURN
      END
      SUBROUTINE DOIF(ARGSTK,I,J)
      INTEGER EQUAL
      INTEGER A2,A3,A4,A5,ARGSTK(100),I,J
      COMMON /CMACRO/ CP,EP,EVALST(10000)
      INTEGER CP
      INTEGER EP
      LOGICAL*1 EVALST
      IF (J-I.LT.5) THEN
        RETURN
        END IF
      A2 =ARGSTK(I+2)
      A3 =ARGSTK(I+3)
      A4 =ARGSTK(I+4)
      A5 =ARGSTK(I+5)
      IF (EQUAL(EVALST(A2),EVALST(A3)).EQ.1) THEN
        CALL PBSTR(EVALST(A4))
        ELSE
          CALL PBSTR(EVALST(A5))
        END  IF
      RETURN
      END
      SUBROUTINE DOINCR(ARGSTK,I,J)
      INTEGER CTOI
      INTEGER ARGSTK(100),I,J,K
      COMMON /CMACRO/ CP,EP,EVALST(10000)
      INTEGER CP
      INTEGER EP
      LOGICAL*1 EVALST
      K =ARGSTK(I+2)
      CALL PBNUM(CTOI(EVALST,K)+1)
      RETURN
      END
      SUBROUTINE DOSUB(ARGSTK,I,J)
      INTEGER CTOI,LENGTH
      INTEGER AP,ARGSTK(100),FC,I,J,K,NC
      COMMON /CMACRO/ CP,EP,EVALST(10000)
      INTEGER CP
      INTEGER EP
      LOGICAL*1 EVALST
      IF (J-I.LT.3) THEN
        RETURN
        END IF
      IF (J-I.LT.4) THEN
        NC =100
        ELSE
            K =ARGSTK(I+4)
            NC =CTOI(EVALST,K)
        END  IF
      K =ARGSTK(I+3)
      AP =ARGSTK(I+2)
      FC =AP +CTOI(EVALST,K) -1
      IF (FC.GE.AP.AND.FC.LT.AP+LENGTH(EVALST(AP))) THEN
          K =FC +MIN(NC,LENGTH(EVALST(FC))) -1
          DO WHILE (K.GE.FC)
            CALL PUTBAK(EVALST(K))
23823       K=K-1
            END DO
23824       CONTINUE
        END IF
      RETURN
      END
      SUBROUTINE EVALR(ARGSTK,I,J)
      INTEGER INDEXX,LENGTH
      INTEGER ARGNO,ARGSTK(100),I,J,K,M,N,T,TD,ARGNO2
      COMMON /CMACRO/ CP,EP,EVALST(10000)
      INTEGER CP
      INTEGER EP
      LOGICAL*1 EVALST
      LOGICAL*1 DIGITS(11)
      DATA DIGITS(1) /48/
      DATA DIGITS(2) /49/
      DATA DIGITS(3) /50/
      DATA DIGITS(4) /51/
      DATA DIGITS(5) /52/
      DATA DIGITS(6) /53/
      DATA DIGITS(7) /54/
      DATA DIGITS(8) /55/
      DATA DIGITS(9) /56/
      DATA DIGITS(10) /57/
      DATA DIGITS(11) /0/
      T =ARGSTK(I)
      TD =EVALST(T)
      IF (TD.EQ.-12) THEN
        CALL DOINCR(ARGSTK,I,J)
        ELSE IF (TD.EQ.-13) THEN
        CALL DOSUB(ARGSTK,I,J)
        ELSE IF (TD.EQ.-11) THEN
        CALL DOIF(ARGSTK,I,J)
        ELSE IF (TD.EQ.-14) THEN
        CALL DOARTH(ARGSTK,I,J)
        ELSE
            K =T+LENGTH(EVALST(T))-1
            DO WHILE (K.GT.T)
              IF (EVALST(K-1).EQ.64) THEN
                  ARGNO =EVALST(K) -48
                  IF (ARGNO.GE.0.AND.ARGNO.LT.J-I) THEN
                    N =I +ARGNO +1
                    M =ARGSTK(N)
                    CALL PBSTR(EVALST(M))
                   END IF
                  K =K -1
                ELSE IF (EVALST(K-2).EQ.64) THEN
                  ARGNO =EVALST(K-1) -48
                  ARGNO2 =EVALST(K) -48
                  IF (ARGNO2.GE.0.AND.ARGNO2.LE.9.AND.ARGNO.GE.0.AND.ARG
     *NO.LE.9) THEN
                    ARGNO =ARGNO *10 +ARGNO2
                    IF (ARGNO.GE.0.AND.ARGNO.LT.J-I) THEN
                      N =I +ARGNO +1
                      M =ARGSTK(N)
                      CALL PBSTR(EVALST(M))
                     END IF
                    K =K -2
                   ELSE
                     CALL PUTBAK(EVALST(K))
                   END  IF
                ELSE
                   CALL PUTBAK(EVALST(K))
                END  IF
23834         K=K-1
              END DO
23835         CONTINUE
            IF (K.EQ.T) THEN
              CALL PUTBAK(EVALST(K))
              END IF
        END  IF
      RETURN
      END
      INTEGER FUNCTION IFPARM(STRNG)
      LOGICAL*1 STRNG(100),C
      INTEGER I,INDEXX,TYPE
      C =STRNG(1)
      IF (C.EQ.-12.OR.C.EQ.-13.OR.C.EQ.-11.OR.C.EQ.-14.OR.C.EQ.-15) THEN
        IFPARM =1
        ELSE
            IFPARM =0
            I=1
            DO WHILE (INDEXX(STRNG(I),64).GT.0)
                I =I +INDEXX(STRNG(I),64)
                IF (TYPE(STRNG(I)).EQ.2) THEN
                  IF (TYPE(STRNG(I+1)).NE.2) THEN
                    IFPARM =1
                    GOTO 23852 ! BREAK
                   END IF
                  END IF
23851         END DO
23852         CONTINUE
        END  IF
      RETURN
      END
      SUBROUTINE PBNUM(N)
      INTEGER MOD
      INTEGER M,N,NUM
      LOGICAL*1 DIGITS(11)
      DATA DIGITS(1) /48/
      DATA DIGITS(2) /49/
      DATA DIGITS(3) /50/
      DATA DIGITS(4) /51/
      DATA DIGITS(5) /52/
      DATA DIGITS(6) /53/
      DATA DIGITS(7) /54/
      DATA DIGITS(8) /55/
      DATA DIGITS(9) /56/
      DATA DIGITS(10) /57/
      DATA DIGITS(11) /0/
      NUM =ABS(N)
23857 CONTINUE
          M =MOD(NUM,10)
          CALL PUTBAK(DIGITS(M+1))
          NUM =NUM / 10
23858   IF(.NOT.(NUM.EQ.0))GOTO 23857
23859   CONTINUE
      IF (N.LT.0) THEN
        CALL PUTBAK(45)
        END IF
      RETURN
      END
      INTEGER FUNCTION PUSH(EP,ARGSTK,AP)
      INTEGER AP,ARGSTK(100),EP
      IF (AP.GT.100) THEN
        CALL BADERR('arg stack overflow.')
        END IF
      ARGSTK(AP) =EP
      PUSH =AP +1
      RETURN
      END
      SUBROUTINE PUTCHR(C)
      LOGICAL*1 C
      COMMON /CMACRO/ CP,EP,EVALST(10000)
      INTEGER CP
      INTEGER EP
      LOGICAL*1 EVALST
      IF (EP.GT.10000) THEN
        CALL BADERR('evaluation stack overflow.')
        END IF
      EVALST(EP) =C
      EP =EP +1
      RETURN
      END
      SUBROUTINE PUTTOK(STR)
      LOGICAL*1 STR(100)
      INTEGER I
      I =1
      DO WHILE (STR(I).NE.0)
        CALL PUTCHR(STR(I))
23867   I=I+1
        END DO
23868   CONTINUE
      RETURN
      END
      INTEGER FUNCTION ELENTH(BUF)
      LOGICAL*1 BUF(100),C
      LOGICAL*1 ESC
      INTEGER I,N
      N =0
      I=1
      DO WHILE (BUF(I).NE.0)
          C =ESC(BUF,I)
          N =N +1
23870   I=I+1
        END DO
23871   CONTINUE
      ELENTH =N
      RETURN
      END
      LOGICAL*1 FUNCTION ESC (BUF,I)
      LOGICAL*1 BUF(100),C
      INTEGER I
      C =BUF(I)
      IF (C.EQ.64) THEN
          I =I +1
          C =BUF(I)
          IF (C.EQ.110.OR.C.EQ.78) THEN
            C =10
            ELSE IF (C.EQ.114.OR.C.EQ.82) THEN
            C =13
            ELSE IF (C.EQ.108.OR.C.EQ.76) THEN
            C =10
            ELSE IF (C.EQ.102.OR.C.EQ.70) THEN
            C =12
            ELSE IF (C.EQ.116.OR.C.EQ.84) THEN
            C =9
            ELSE IF (C.EQ.48) THEN
            C =0
            END IF
        END IF
      ESC=(C)
      RETURN
      END
      LOGICAL*1 FUNCTION NGETLN(C,FD)
      INTEGER GETLIN
      LOGICAL*1 C
      INTEGER FD,CNT
      COMMON /CDEFIO/ BP,BUF(20000),LP,LNBUF(400)
      INTEGER BP
      LOGICAL*1 BUF
      INTEGER LP
      LOGICAL*1 LNBUF
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      LP =2
      IF (FD.EQ.99999) THEN
        CNT =LIBSRC(LNBUF)
        ELSE
          CNT =GETLIN(LNBUF,FD)
        END  IF
      IF (CNT.EQ.-1) THEN
          LNBUF(2) =0
          C =-1
        ELSE
          C =LNBUF(1)
        END  IF
      NGETLN =C
      RETURN
      END
      SUBROUTINE OPNINC(NAME,I)
      INTEGER OPTION(127)
      COMMON /OPTCOM/ OPTION
      COMMON /CLINE/ LEVEL,LINECT(12),INFILE(12),FNAMP,FNAMES(972),LBIND
     *X(12),LNAMP,LNAMES(972)
      INTEGER LEVEL
      INTEGER LINECT
      INTEGER INFILE
      INTEGER FNAMP
      LOGICAL*1 FNAMES
      INTEGER LBINDX
      INTEGER LNAMP
      LOGICAL*1 LNAMES
      INTEGER OPEN_
      EXTERNAL SCOPY
      INFILE(LEVEL+1) =OPEN_(NAME,1)
	  call setchfd(INFILE(LEVEL+1), INFILE(LEVEL+1))
      LINECT(LEVEL+1) =1
      IF (INFILE(LEVEL+1).EQ.-3) THEN
        CALL OPNLIB(NAME)
        ELSE
            LEVEL =LEVEL +1
            IF (FNAMP+I.LE.972) THEN
                CALL SCOPY(NAME,1,FNAMES,FNAMP)
                FNAMP =FNAMP +I
              END IF
            F =INFILE(LEVEL)
            IF ((OPTION(86).EQ.1)) THEN
                CALL PUTLIN_V('  ',7)
                CALL PUTLIN(NAME,7)
                CALL PUTCH(10,7)
              END IF
        END  IF
      RETURN
      END
      SUBROUTINE REMARK_V (STR)
      CHARACTER *(*) STR
      CALL REMARK (%REF(STR))
      RETURN
      END
      SUBROUTINE ERROR_V (STR)
      CHARACTER *(*) STR
      CALL ERROR (%REF(STR))
      RETURN
      END
