####################################################################
#
#         RGMAIN.INC
#
####################################################################

include "bbctl.inc"         
include "bbcode.inc"         

include "bbacc.inc"         
include "bbboy.inc"         
include "bbdeq.inc"         
include "bbhfd.inc"         
include "bbird.inc"         
include "bbpst.inc"         
include "bbrad.inc"         
include "bbrmt.inc"         
include "bbrvu.inc"         
include "bbsat.inc"         
include "bbsbi.inc"         
include "bbshc.inc"         
include "bbsop.inc"         
include "bbsos.inc"         
include "bbssc.inc"         
include "bbssr.inc"         
include "bbsta.inc"         
include "bbstr.inc"         
include "bbswb.inc"         
include "bbswd.inc"         
include "bbunt.inc"         
include "bbvue.inc"         

include "rgctl.inc"         
include "rgcode.inc"         
include "rgmsg.inc"         
include "rgtcr.inc"         

include "gcgam.inc"         
include "ascii.inc"         
include "ssdef.inc"         
include "macros.inc"         



define($pi, 3.14159265)
define ($$NOOUT,   0)  # No output messages                      -- clf 8/6/92
define ($$PORTOUT, 1)  # Output messages to port                 -- clf 8/6/92
define ($$FILEOUT, 2)  # Output messages to file instead of port -- clf 8/6/92
define ($RG$OutUnit, 10) # logical unit number for file output   -- clf 8/6/92

define ($MaxBuf, 500000)
define ($max_hf, 6)
define ($maxline, 400)
define ($maxname, 16)

define (max$unts,arith(1001,+,unt$slots)) # max units that can be entered in 
                                          # Init Table + 1
define (notlisted,arith(1,+,max$unts)) # to indicate units not in the 
                                        # Init Table 
define ($false$ctc,arith(1,+,notlisted)) # to indicate a false contact
define (max$pline, 75)       # used for POST 
define (max$v6line, 75)      # used for V6
define (max$postbuf, 15000)  # used for POST
define (max$v6buf, 15000)    # used for V6

define (auditcommon, 
    integer   index, icnt[max$unts], igld
    character audit[3]

    common / acom/ index, icnt, igld, audit
    )

define (servercommon,
   integer sockfd
   character server_name[$maxname]
   character server_port[$maxname]

   common /servercom/ sockfd, server_name, server_port
)

define (postcommon, 
    integer     inpnt, outpnt, msgpnt, ctcpnt, pospnt, radpnt,
                postchan, FOFileNBR 
    integer*2   post_iosb[4]                    
    character   postline[max$postbuf], bufline[max$postbuf],
                msgline[max$pline], ctcline[max$pline], 
                posline[max$pline], radline[max$pline]
    common / pcom / inpnt, outpnt, msgpnt, ctcpnt, pospnt, radpnt, 
                postchan, FOFileNBR, post_iosb,
                postline, bufline, msgline, ctcline, posline, radline
    )
        
define (v6common, 
    integer    inpnt, outpnt, v6chan, v6FileNBR, v6msgstat
    integer    rec_cnt, snd_cnt, msg_seq_no
    logical    resend
    integer*2  v6_in_iosb[4],v6_out_iosb[4]
    character  v6line[max$v6buf], bufline[max$v6buf]

    common / v6com / inpnt, outpnt, v6chan, v6FileNBR, v6msgstat, 
                rec_cnt, snd_cnt, msg_seq_no, resend, 
                v6_in_iosb, v6_out_iosb, v6line, bufline
    )
        
define (msgcommon,
    character  cline[1600]
    integer imsgcnt,itotmsgs,ilen,iact_rain,inslots,ioslots,
            iesm_rain,isos_rain,igold,ipass,isnmsg,imode,ipass_gold,
            irnd_gold,isqnumb,igdelay,jsqnumb,ioldmin

    common / mcom / cline,
              imsgcnt,itotmsgs,ilen,iact_rain,inslots,ioslots,
              iesm_rain,isos_rain,igold,ipass,isnmsg,imode,ipass_gold,
              irnd_gold,isqnumb,igdelay,jsqnumb,ioldmin
    )
        
define (tmpcommon,
    literal posit(5),this_year,this_month,mnth
    integer imnth,iday,imin,ideg,itot

    common / tcom/ posit,this_year,this_month,mnth,
                   imnth,iday,imin,ideg,itot
    )

define (whocommon,
    literal sostarget
    integer sosregion, jcnt[max$unts,max$unts], intable[max$unts]
    integer rain_view, swbregion, idctor, idctee, ibase
    character filename[$maxname]
    common / wcom/ sostarget, sosregion, jcnt, intable,
        rain_view, swbregion, idctor, idctee, ibase,
        filename
    )

define (jntcommon,
    integer isubcnt,inavcnt,iaircnt,ifshcnt,imercnt
    common / jcom/isubcnt,inavcnt,iaircnt,ifshcnt,imercnt
    )

define (ptr1common,
    integer acc$pointer,shc$pointer,sos$pointer,
            sat$pointer,ssr$pointer,hfd$pointer,
            vue$pointer,swd$pointer,swb$pointer
    common/block1/acc$pointer,shc$pointer,sos$pointer,
                  sat$pointer,ssr$pointer,hfd$pointer,
                  vue$pointer,swd$pointer,swb$pointer
    )

define (ptr2common,
    integer unt$pointer, 
            rad$pointer,rmt$pointer,msg$pointer,
            pst$pointer,IRD$Pointer
    common/block2/ unt$pointer, # pos$pointer,dyn$pointer,
                   rad$pointer,rmt$pointer,msg$pointer,
                   pst$pointer,IRD$Pointer
    )

define (bufcommon,
    integer     bufpnt,ifd_out,nbaudot,nwatch,ntty,
                chan1,chan2,chan3,chan4,chan5
    integer*4   old_char[2]
    character   pbuff[$MaxBuf]
    common      /bcom/  bufpnt,ifd_out,nbaudot,nwatch,ntty,
                        chan1,chan2,chan3,chan4,chan5,
                        old_char, pbuff
    )

define (RFcommon,
    literal     UNIT_Id[max$unts],
                RADAR_Id[RAD$slots]
    integer     UNIT_Status[max$unts],
                UNIT_count,
                RADAR_count,
                HFDSTA_count,
                rainstart,
                redstart,
                esmstart,
                pcnt_garble,
                pcnt_gblmsgs,
                msg_minute,
                msgstatcnt,
                msgstat,
                ignd_truth,
                itruepos,
                iform,
                igamemin,
                case_number[max$unts]
    character   ACTADD_txt[ARB],
                RFORIG_txt[ARB],
                BFORIG_txt[ARB],
                STSORIG_txt[ARB],
                GLDORIG_txt[ARB],
                GESMORIG_txt[ARB],
                OPNAME_txt[ARB],
                RMKS_txt[ARB],
                ALERT_code[ARB],
                HFDSTA_code[32,$max_hf],
                HFDSTA_txt[32,$max_hf],
                RADAR_Name[16,RAD$slots],
                UNIT_Class[32,max$unts],
                UNIT_Name[32,max$unts],
                UNIT_Type[8,max$unts],
                UNIT_Numb[8,max$unts],
                UNIT_Call[8,max$unts],
                UNIT_nosic[16,max$unts],
                UNIT_flag[4,max$unts],
                MSG_reporters[16,MAX$MsgTypes]

    common /rf_com/ UNIT_Id, RADAR_Id,
                UNIT_Status, UNIT_count, RADAR_count, HFDSTA_count,
                rainstart, redstart, esmstart, pcnt_garble, pcnt_gblmsgs,
                msg_minute, msgstatcnt, msgstat, ignd_truth, itruepos,
                iform, igamemin, case_number,
                ACTADD_txt, RFORIG_txt, BFORIG_txt, STSORIG_txt, GLDORIG_txt,
                GESMORIG_txt, OPNAME_txt, RMKS_txt, ALERT_code, HFDSTA_code, 
                HFDSTA_txt, RADAR_Name, UNIT_Class, UNIT_Name, UNIT_Type, 
                UNIT_Numb, UNIT_Call, UNIT_nosic, UNIT_flag, MSG_reporters
    )

define (ID_Common,
    integer  otg_msg_gen_number
    Common /idcommon/ otg_msg_gen_number
    )

define (initcommon,
	integer seqctcnbr, rfm_count, special_count, di_count,
		redround, whiteround
	common/init_com/seqctcnbr, rfm_count, special_count, di_count,
		redround, whiteround)

define (astabcommon,
	integer    iastab1flag, iastab2flag, irainflag
	common/astab_com/iastab1flag, iastab2flag, irainflag)
