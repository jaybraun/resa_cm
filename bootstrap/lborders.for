      SUBROUTINE UNIT_NOTFOR_OFFSET (KPOINT_UNT,ISTAT)
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
      INTEGER ISTAT
      INTEGER*4  KPOINT_UNT
      INTEGER*4  I_UNT_TYPE
      INTEGER*4  I_UNT_SUPERTYPE
      INTEGER*4  I_UNT_LASTMANEUVERORDERCODE
      ISTAT =-1
      I_UNT_SUPERTYPE=(IAND(ISHFT(IBB(KPOINT_UNT+16),-(17)),'07FF'X))
      I_UNT_TYPE=(IAND(ISHFT(IBB(KPOINT_UNT+16),-(20)),'00FF'X))
      I_UNT_LASTMANEUVERORDERCODE=(IAND(ISHFT(IBB(KPOINT_UNT+74),-(22)),
     * '03FF'X))
      IF(.NOT.(I_UNT_LASTMANEUVERORDERCODE.EQ.173.OR.I_UNT_LASTMANEUVERO
     *RDERCODE.EQ.104.OR.I_UNT_LASTMANEUVERORDERCODE.EQ.192.OR.I_UNT_LAS
     *TMANEUVERORDERCODE.EQ.163.OR.I_UNT_LASTMANEUVERORDERCODE.EQ.158.OR
     *.I_UNT_LASTMANEUVERORDERCODE.EQ.154.OR.I_UNT_LASTMANEUVERORDERCODE
     *.EQ.159.OR.I_UNT_LASTMANEUVERORDERCODE.EQ.127))GOTO 23000
      IF(.NOT.(I_UNT_SUPERTYPE.EQ.(ISHFT(2,3)+1)))GOTO 23002
      ISTAT =21
      GOTO 23003
23002 CONTINUE
      IF(.NOT.(I_UNT_TYPE.EQ.1))GOTO 23004
      ISTAT =9
      GOTO 23005
23004 CONTINUE
      IF(.NOT.(I_UNT_TYPE.EQ.2))GOTO 23006
      ISTAT =20
      GOTO 23007
23006 CONTINUE
      IF(.NOT.(I_UNT_TYPE.EQ.5))GOTO 23008
      ISTAT =10
      GOTO 23009
23008 CONTINUE
      IF(.NOT.(I_UNT_SUPERTYPE.EQ.(ISHFT(3,3))))GOTO 23010
      ISTAT =18
      GOTO 23011
23010 CONTINUE
      IF(.NOT.(I_UNT_SUPERTYPE.EQ.(ISHFT(3,3)+1)))GOTO 23012
      ISTAT =11
23012 CONTINUE
23011 CONTINUE
23009 CONTINUE
23007 CONTINUE
23005 CONTINUE
23003 CONTINUE
      GOTO 23001
23000 CONTINUE
      IF(.NOT.(I_UNT_TYPE.EQ.7))GOTO 23014
      ISTAT =3
      GOTO 23015
23014 CONTINUE
      IF(.NOT.(I_UNT_SUPERTYPE.EQ.(ISHFT(2,3)+1)))GOTO 23016
      ISTAT =19
      GOTO 23017
23016 CONTINUE
      IF(.NOT.(I_UNT_TYPE.EQ.1))GOTO 23018
      ISTAT =0
      GOTO 23019
23018 CONTINUE
      IF(.NOT.(I_UNT_TYPE.EQ.2))GOTO 23020
      ISTAT =2
      GOTO 23021
23020 CONTINUE
      IF(.NOT.(I_UNT_TYPE.EQ.5))GOTO 23022
      ISTAT =25
      GOTO 23023
23022 CONTINUE
      IF(.NOT.(I_UNT_SUPERTYPE.EQ.(ISHFT(3,3))))GOTO 23024
      ISTAT =1
      GOTO 23025
23024 CONTINUE
      IF(.NOT.(I_UNT_SUPERTYPE.EQ.(ISHFT(3,3)+1)))GOTO 23026
      ISTAT =7
23026 CONTINUE
23025 CONTINUE
23023 CONTINUE
23021 CONTINUE
23019 CONTINUE
23017 CONTINUE
23015 CONTINUE
      IF(.NOT.((IAND(ISHFT(IBB(KPOINT_UNT+25),-(30)),'0001'X)).EQ.1))GOT
     *O 23028
      ISTAT =15
23028 CONTINUE
23001 CONTINUE
      RETURN
      END
      INTEGER FUNCTION ISPLOTTABLEORDER(ISEQ,NORD,NARGS,ARGS)
      IMPLICIT NONE
      INTEGER ISEQ,NORD,NARGS,ISOK
      REAL*8 ARGS(100)
      ISOK =0
      IF(.NOT.(NORD.EQ.165))GOTO 23030
      IF(.NOT.(ARGS(5).EQ.-20444))GOTO 23032
      ISOK =1
23032 CONTINUE
23030 CONTINUE
      IF(.NOT.(NORD.EQ.194))GOTO 23034
      IF(.NOT.(ARGS(2).EQ.-20444))GOTO 23036
      ISOK =1
23036 CONTINUE
23034 CONTINUE
      IF(.NOT.(NORD.EQ.123))GOTO 23038
      ISOK =1
23038 CONTINUE
      IF(.NOT.(NORD.EQ.103))GOTO 23040
      IF(.NOT.(ARGS(1).EQ.-20052))GOTO 23042
      ISOK =1
23042 CONTINUE
23040 CONTINUE
      IF(.NOT.(NORD.EQ.133))GOTO 23044
      IF(.NOT.(ARGS(2).EQ.-20053))GOTO 23046
      ISOK =1
23046 CONTINUE
23044 CONTINUE
      IF(.NOT.(NORD.EQ.149))GOTO 23048
      ISOK =1
23048 CONTINUE
      IF(.NOT.(ISEQ.EQ.1))GOTO 23050
      IF(.NOT.(NORD.EQ.123))GOTO 23052
      IF(.NOT.(ARGS(2).EQ.-20130))GOTO 23054
      ISOK =0
23054 CONTINUE
23052 CONTINUE
23050 CONTINUE
      ISPLOTTABLEORDER=(ISOK)
      RETURN
      END
      SUBROUTINE GET_POSITION_FROM_ORDER(NORD,NARGS,ARGS,LAT,LON)
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
      INTEGER NORD,NARGS
      REAL*8 ARGS(100)
      REAL LAT,LON,DMS2RAD,BRG,FRANGE,FCOS
      REAL*8 BASE_NAME
      INTEGER*4  KPOINT_UNT
      INTEGER*4  I_UNT_VIEW
      IF(.NOT.(NORD.EQ.103.AND.ARGS(1).EQ.-20052))GOTO 23056
      LAT =DMS2RAD(ARGS(3))
      LON =DMS2RAD(ARGS(4))
      GOTO 23057
23056 CONTINUE
      IF(.NOT.(NORD.EQ.194))GOTO 23058
      LAT =DMS2RAD(ARGS(3))
      LON =DMS2RAD(ARGS(4))
      GOTO 23059
23058 CONTINUE
      IF(.NOT.(NORD.EQ.165))GOTO 23060
      IF(.NOT.(ARGS(5).EQ.-20444))GOTO 23062
      LAT =DMS2RAD(ARGS(6))
      LON =DMS2RAD(ARGS(7))
23062 CONTINUE
      GOTO 23061
23060 CONTINUE
      IF(.NOT.(NORD.EQ.123))GOTO 23064
      IF(.NOT.(ARGS(2).EQ.-20130))GOTO 23066
      BRG =ARGS(3) *(3.141592654/180.)
      FRANGE =ARGS(4)
      FCOS =COS(LAT)
      CALL RRB2LL (LAT,LON,FRANGE,BRG,0.0,FCOS)
      GOTO 23067
23066 CONTINUE
      LAT =DMS2RAD(ARGS(3))
      LON =DMS2RAD(ARGS(4))
23067 CONTINUE
      GOTO 23065
23064 CONTINUE
      IF(.NOT.(NORD.EQ.133.AND.ARGS(2).EQ.-20053))GOTO 23068
      BASE_NAME =ARGS(3)
      I_UNT_VIEW=IBB(128)
      CALL UNTFND(I_UNT_VIEW,BASE_NAME,KPOINT_UNT)
      IF(.NOT.((KPOINT_UNT.GE.IBBP(1,01).AND.KPOINT_UNT.LE.(IBBP(1,01)+I
     *BBP(3,01)-1))))GOTO 23070
      LAT =FBB(KPOINT_UNT+19)
      LON =FBB(KPOINT_UNT+20)
23070 CONTINUE
23068 CONTINUE
23065 CONTINUE
23061 CONTINUE
23059 CONTINUE
23057 CONTINUE
      RETURN
      END
      INTEGER FUNCTION FNDRTE (RTENAME)
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
      INTEGER*4  KPOINT_RTE
      REAL*8 RTENAME,CSTRIP8
      INTEGER NAMSLOT,LITCMP
      NAMSLOT =0
      KPOINT_RTE=IBBP(1,88)
23072 IF(.NOT.((KPOINT_RTE.GE.IBBP(1,88).AND.KPOINT_RTE.LE.(IBBP(1,88)+I
     *BBP(3,88)-1))))GOTO 23074
      IF(.NOT.(LITCMP(CSTRIP8(IBB(KPOINT_RTE)),RTENAME).EQ.0))GOTO 23075
      NAMSLOT =((KPOINT_RTE-IBBP(1,88))/4+1)
      GOTO 23074
23075 CONTINUE
23073 KPOINT_RTE=KPOINT_RTE+4
      GOTO 23072
23074 CONTINUE
      FNDRTE=(NAMSLOT)
      RETURN
      END
