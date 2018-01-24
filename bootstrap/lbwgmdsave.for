      SUBROUTINE WRITEM
      IMPLICIT NONE
      INTEGER*4 IBB(10240000)
      INTEGER*2 IBBW(2,10240000)
      BYTE IBBB(4,10240000)
      REAL*8 CBB(5120000)
      REAL FBB(10240000)
      INTEGER IBBP(6,170)
      EQUIVALENCE (IBB,FBB,CBB,IBBW,IBBB)
      EQUIVALENCE (IBB(1025),IBBP)
      COMMON /BBOARD/ IBB
cpar$ private / BBoard / ! keep FORTRAN from assigning a global section
      INTEGER ISNAP_D , ISNAP_BEFORE_1 , ISNAP_BEFORE_2 , ISNAP_AFTER_1 
     *, ISNAP_AFTER_2
      COMMON / SNAP_COMMON / ISNAP_D , ISNAP_BEFORE_1 , ISNAP_BEFORE_2 ,
     * ISNAP_AFTER_1 , ISNAP_AFTER_2
cpar$ private / snap_common / ! no FORTRAN global section
      INTEGER WORK(1024)
      COMMON /SCRPAD/ WORK
      INTEGER N_SAVES
      CHARACTER *64 SFILE,RFILE,XFILE,FNAMES(12)
      COMMON /SAVE_COMMON/ N_SAVES,SFILE,RFILE,XFILE,FNAMES
      INTEGER ISTEP,IFRST,ILAST,NRECS,IREC,IWORD
      ISTEP =128
      IF(.NOT.(IBB(295).EQ.0.OR.IBB(294).EQ.0))GOTO 23000
      IBB(295)=IBB(178)
      IBB(294)=IBB(183)
23000 CONTINUE
      IFRST =1
      ILAST =ISTEP
      NRECS =80000/ (ISTEP / 128)
      IREC =1
23002 IF(.NOT.(IREC.LE.NRECS))GOTO 23004
      WRITE(50,REC=IREC) (IBB(IWORD),IWORD=IFRST,ILAST)
      IFRST =IFRST +ISTEP
      ILAST =ILAST +ISTEP
23003 IREC=IREC+1
      GOTO 23002
23004 CONTINUE
      RETURN
      END
